#include <map>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "derivedattributes.h"
#include "ui_mainwindow.h"
#include "heesgraphicsitem.h"
#include "heesitemwidget.h"
#include "../tinyxml/tinyxml.h"
#include "../tinyxml/tinystr.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    scene = new HEESSystemScene;
    ui->setupUi(this);
    // Add items to the combo box
    ui->compTypeBox->addItem(QString("Source"));
    ui->compTypeBox->addItem(QString("Bank"));
    ui->compTypeBox->addItem(QString("Load"));
    ui->compTypeBox->addItem(QString("CTI"));
    ui->compTypeBox->addItem(QString("Converter"));
    ui->compTypeBox->addItem(QString("Manager"));
    ui->compTypeBox->setCurrentIndex(0);

    ui->graphicsView->setScene( scene );

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(selectionChangedInScene()) );
    connect(ui->itemWidget, SIGNAL(selectPort()), this, SLOT(selectPortInScene()) );
    connect(scene, SIGNAL(portSelected(HEESGraphicsItem*)), ui->itemWidget, SLOT(portSelectedFromScene(HEESGraphicsItem*)));
    connect(ui->itemWidget, SIGNAL(removeItem(HEESGraphicsItem*)), this, SLOT(removeItemInScene(HEESGraphicsItem*)));
    connect(ui->openAction, SIGNAL(triggered()), this, SLOT(openFile()) );
    connect(ui->saveAction, SIGNAL(triggered()), this, SLOT(saveFile()) );
    connect(ui->newAction, SIGNAL(triggered()), this, SLOT(newFile()) );
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::selectionChangedInScene()
{
    QList<QGraphicsItem *> itemList = scene->selectedItems();
    if( itemList.empty() )
    {
        ui->itemWidget->setModel(NULL);
    }
    else
    {
        // If multiple items are selected, only the first one is set as the model.
        // This one is not necessarily the first selected one.
        ui->itemWidget->setModel(static_cast<HEESGraphicsItem*>(itemList[0]));
    }
}

void MainWindow::removeItemInScene(HEESGraphicsItem* item)
{
    scene->removeItem(item);
    delete item;
}

void MainWindow::selectPortInScene()
{
    scene->setAddArrowMode();
}

void MainWindow::on_addButton_clicked()
{
    scene->addItem( new HEESGraphicsItem(ui->compTypeBox->currentIndex()) );
}

void MainWindow::on_cmdEditButton_clicked()
{
    // Open a dialog to modify the commands
    QDialog * dialog = new QDialog(this, Qt::WindowCloseButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    QTableView * tableView = new QTableView(dialog);
    QGridLayout *layout = new QGridLayout;
    tableView->setModel(&commands);
    layout->addWidget(tableView);
    dialog->setWindowTitle(QString("Edit Commands"));
    dialog->setLayout(layout);
    // Use a bigger fixed size to hold the tableview
    //dialog->layout()->setSizeConstraint( QLayout::SetFixedSize );
    dialog->setFixedSize(600,400);
    // Call exec() so that main window will freeze
    dialog->exec();
}

void MainWindow::on_projectEdit_editingFinished()
{
    projectName = ui->projectEdit->text();
}

void MainWindow::openFile()
{
    if( !scene->items().isEmpty() )
    {
        QMessageBox::warning(this, tr("Warning"), tr("All unsaved data will be lost!"));
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("XML Files (*.xml)"));
    if(fileName != "")
    {
        scene->clearAllItems();
        projectName.clear();
        commands.clear();

        TiXmlDocument doc;
        if(!doc.LoadFile(fileName.toStdString()))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        }
        TiXmlElement * pProjectElement = doc.FirstChildElement("project");
        TiXmlElement * pNodeElement;
        if( pProjectElement == NULL )
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not parse file: <project> not found"));
            return;
        }
        pNodeElement = pProjectElement->FirstChildElement("name");
        if( pNodeElement != NULL )
        {
            projectName = QString(pNodeElement->GetText());
			ui->projectEdit->setText(projectName);
        }
        std::map<QString, HEESGraphicsItem*> nameIndex;
        // First pass: add components
        for( pNodeElement = pProjectElement->FirstChildElement("comp"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("comp"))
        {
            HEESGraphicsItem * item;
            QString type(pNodeElement->Attribute("type"));
            if( type == QString("cti") )
            {
                item = new HEESGraphicsItem(CTI);
            }
            else if( type == QString("bank"))
            {
                item = new HEESGraphicsItem(BANK);
            }
            else if( type == QString("load") )
            {
                item = new HEESGraphicsItem(LOAD);
            }
            else if( type == QString("source") )
            {
                item = new HEESGraphicsItem(SOURCE);
            }
            else if( type == QString("converter") )
            {
                // will be added in the second pass
                continue;
            }
            else if( type == QString("manager") )
            {
                item = new HEESGraphicsItem(MANAGER);
            }
            else
            {
                continue;
            }
            QString xpos(pNodeElement->Attribute("x_pos"));
            QString ypos(pNodeElement->Attribute("y_pos"));
            if( !xpos.isEmpty() && !ypos.isEmpty() )
            {
                item->setPos( xpos.toDouble(), ypos.toDouble() );
            }
            TiXmlElement * pNameElement = pNodeElement->FirstChildElement("name");
            if( pNameElement != NULL )
            {
                item->name = QString(pNameElement->GetText());
            }
            TiXmlElement * pDerivedElement = pNodeElement->FirstChildElement("derived");
            if( pDerivedElement == NULL )
            {
                QMessageBox::critical(this, tr("Error"), tr("Could not parse file: <derived> not found for component:")+item->name);
                scene->clearAllItems();
                projectName.clear();
                commands.clear();
                return;
            }
            if( type != QString("cti") )
            {
                item->derivedType = QString(pDerivedElement->Attribute("type"));
            }
            TiXmlElement * pElement;
            for( pElement = pDerivedElement->FirstChildElement(); pElement != NULL; pElement = pElement->NextSiblingElement() )
            {
                item->myAttributes()->insertBack( pElement->Value(), pElement->GetText() );
            }
            nameIndex[item->name] = item;
            scene->addItem(item);
        }
        // Second pass: add converters
        for( pNodeElement = pProjectElement->FirstChildElement("comp"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("comp"))
        {
            HEESGraphicsItem * item;
            QString type(pNodeElement->Attribute("type"));
            if( type == QString("converter") )
            {
                // will be added in the second pass
                item = new HEESGraphicsItem(CONVERTER);
            }
            else
            {
                continue;
            }
            QString xpos(pNodeElement->Attribute("x_pos"));
            QString ypos(pNodeElement->Attribute("y_pos"));
            if( !xpos.isEmpty() && !ypos.isEmpty() )
            {
                item->setPos( xpos.toDouble(), ypos.toDouble() );
            }
            TiXmlElement * pNameElement = pNodeElement->FirstChildElement("name");
            if( pNameElement != NULL )
            {
                item->name = QString(pNameElement->GetText());
            }
            TiXmlElement * pDerivedElement = pNodeElement->FirstChildElement("derived");
            if( pDerivedElement == NULL )
            {
                QMessageBox::critical(this, tr("Error"), tr("Could not parse file: <derived> not found for component:")+item->name);
                scene->clearAllItems();
                projectName.clear();
                commands.clear();
                return;
            }
            item->derivedType = QString(pDerivedElement->Attribute("type"));
            TiXmlElement * pElement;
            for( pElement = pDerivedElement->FirstChildElement(); pElement != NULL; pElement = pElement->NextSiblingElement() )
            {
                item->myAttributes()->insertBack( pElement->Value(), pElement->GetText() );
            }
            TiXmlElement * pElementA = pNodeElement->FirstChildElement("port_a");
            TiXmlElement * pElementB = pNodeElement->FirstChildElement("port_b");
            std::map<QString, HEESGraphicsItem*>::iterator it;
            if( pElementA != NULL && (it = nameIndex.find(QString(pElementA->GetText()))) != nameIndex.end() )
            {
                if( it->second->myType() != CONVERTER && it->second->myType() != MANAGER )
                    item->setLeftItem(it->second);
            }
            if( pElementB != NULL && (it = nameIndex.find(QString(pElementB->GetText()))) != nameIndex.end() )
            {
                if( it->second->myType() != CONVERTER && it->second->myType() != MANAGER )
                    item->setRightItem(it->second);
            }
            nameIndex[item->name] = item;
            scene->addItem(item);
        }
        // Thrid pass: add sensors
        for( pNodeElement = pProjectElement->FirstChildElement("sensor"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("sensor") )
        {
            QString target(pNodeElement->Attribute("target"));
            QString prop(pNodeElement->Attribute("property"));
            if( nameIndex.find(target) != nameIndex.end() )
            {
                nameIndex[target]->mySensors()->insertBack(prop);
            }
        }
        // Fourth pass: add commands
        for( pNodeElement = pProjectElement->FirstChildElement("cmd"); pNodeElement != NULL; pNodeElement = pNodeElement->NextSiblingElement("cmd") )
        {
            QString type(pNodeElement->Attribute("type"));
            double time;
            pNodeElement->Attribute("time", &time);
            if( type == QString("set") || type == QString("get") )
            {
                QString targetName(pNodeElement->Attribute("target"));
                for(TiXmlElement * pElement = pNodeElement->FirstChildElement(); pElement != NULL; pElement = pElement->NextSiblingElement() )
                {
                    Command command;
                    command.type = type;
                    command.time = time;
                    command.targetName = targetName;
                    command.propertyName = QString(pElement->Value());
                    command.propertyValue = QString(pElement->GetText());
                    commands.insertBack(command);
                }
            }
            else if( type == QString("finish") || type == QString("sim") )
            {
                Command command;
                command.type = type;
                command.time = time;
                commands.insertBack(command);
            }
        }
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("XML Files (*.xml)"));

    if(fileName != "")
    {
        TiXmlDocument doc;
        // Declaration
        TiXmlDeclaration declaration(std::string("1.0"), std::string(), std::string() );
        // InsertEndChild method uses deep copy
        doc.InsertEndChild( declaration );
        TiXmlElement project("project");
        TiXmlElement node("name");
        // Use TiXmlText to insert text body to an element
        // Covert QString back to std::string
        TiXmlText text(projectName.toStdString());
        node.InsertEndChild(text);
        project.InsertEndChild(node);

        // And all the HEES components
        QList<QGraphicsItem*> itemList = scene->items();
        for(int i=0;i<itemList.size();i++)
        {
            HEESGraphicsItem * item = dynamic_cast<HEESGraphicsItem *>(itemList[i]);
            if( item )
            {
                TiXmlElement comp("comp");
                switch(item->myType())
                {
                case SOURCE:
                    comp.SetAttribute("type","source");
                    break;
                case LOAD:
                    comp.SetAttribute("type","load");
                    break;
                case BANK:
                    comp.SetAttribute("type","bank");
                    break;
                case CTI:
                    comp.SetAttribute("type","cti");
                    break;
                case CONVERTER:
                    comp.SetAttribute("type","converter");
                    break;
                case MANAGER:
                    comp.SetAttribute("type","manager");
                    break;
                }

                comp.SetAttribute("x_pos", item->x());
                comp.SetAttribute("y_pos", item->y());

                TiXmlElement nameNode("name");
                text.SetValue(item->name.toStdString());
                nameNode.InsertEndChild(text);
                comp.InsertEndChild(nameNode);

                if(item->myType() == CONVERTER)
                {
                    TiXmlElement portANode("port_a");
                    text.SetValue(item->getPortAName().toStdString());
                    portANode.InsertEndChild(text);
                    comp.InsertEndChild(portANode);

                    TiXmlElement portBNode("port_b");
                    text.SetValue(item->getPortBName().toStdString());
                    portBNode.InsertEndChild(text);
                    comp.InsertEndChild(portBNode);
                }

                TiXmlElement derivedNode("derived");
                if(item->myType() != CTI)
                {
                    derivedNode.SetAttribute("type",item->derivedType.toStdString());
                }

                DerivedAttributes * attributes = item->myAttributes();
                for(int j=0; j<attributes->rowCount()-1; j++)
                {
                    TiXmlElement attributeNode(attributes->nameAt(j).toStdString());
                    text.SetValue(attributes->valueAt(j).toStdString());
                    attributeNode.InsertEndChild(text);
                    derivedNode.InsertEndChild(attributeNode);
                }

                comp.InsertEndChild(derivedNode);
                project.InsertEndChild(comp);
            }
        }

        // Add all the sensors
        for(int i=0;i<itemList.size();i++)
        {
            HEESGraphicsItem * item = dynamic_cast<HEESGraphicsItem *>(itemList[i]);
            if( item )
            {
                SensorList * sensorList = item->mySensors();
                for(int j=0; j<sensorList->size(); j++)
                {
                    TiXmlElement sensorNode("sensor");
                    sensorNode.SetAttribute("target", item->name.toStdString());
                    sensorNode.SetAttribute("property", sensorList->sensorAt(j).toStdString());
                    project.InsertEndChild(sensorNode);
                }
            }
        }

        // Add all the commands
        for(int i=0;i<commands.size();i++)
        {
            TiXmlElement cmdNode("cmd");
            Command * cmd = commands.commandAt(i);
            cmdNode.SetAttribute("time", cmd->time);
            cmdNode.SetAttribute("type", cmd->type.toStdString());
            if( !cmd->targetName.isEmpty() )
                cmdNode.SetAttribute("target", cmd->targetName.toStdString());
            if( !cmd->propertyName.isEmpty() )
            {
                TiXmlElement propertyNode(cmd->propertyName.toStdString());
                text.SetValue(cmd->propertyValue.toStdString());
                propertyNode.InsertEndChild(text);
                cmdNode.InsertEndChild(propertyNode);
            }
            project.InsertEndChild(cmdNode);
        }

        doc.InsertEndChild(project);

        if(!doc.SaveFile(fileName.toStdString()))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        }
    }
}

void MainWindow::newFile()
{
    // Has nothing to do with file, just to follow the same convention as save and open
    if( !scene->items().isEmpty() )
    {
        if( QMessageBox::question(this, tr("Warning"), tr("All unsaved data will be lost! Continue?"), QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok )
        {
            scene->clearAllItems();
            projectName.clear();
            commands.clear();
        }
    }
}
