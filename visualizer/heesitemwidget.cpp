// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#include "heesitemwidget.h"

HEESItemWidget::HEESItemWidget(QWidget *parent) :
    QWidget(parent)
{
    // Construct layout
    typeLabel = new QLabel(QString("Type:"), parent);
    typeNameLabel = new QLabel(parent);

    nameLabel = new QLabel(QString("Name:"), parent);
    nameEdit = new QLineEdit(parent);

    portALabel = new QLabel(QString("Converter Port A:"), parent);
    portANameLabel = new QLabel(parent);
    selectPortAButton = new QPushButton(QString("Select"),parent);

    portBLabel = new QLabel(QString("Converter Port B:"), parent);
    portBNameLabel = new QLabel(parent);
    selectPortBButton = new QPushButton(QString("Select"),parent);

    derivedLabel = new QLabel("Derived Type:", parent);
    derivedEdit = new QLineEdit(parent);
    attributeTableView = new QTableView(parent);
    removeButton = new QPushButton(QString("Remove Component"),parent);
    sensorButton = new QPushButton(QString("Set Sensors"),parent);

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(typeLabel);
    hLayout1->addWidget(typeNameLabel);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(nameLabel);
    hLayout2->addWidget(nameEdit);

    QHBoxLayout *hLayout3 = new QHBoxLayout;
    hLayout3->addWidget(portALabel);
    hLayout3->addWidget(portANameLabel, 1);
    hLayout3->addWidget(selectPortAButton);

    QHBoxLayout *hLayout4 = new QHBoxLayout;
    hLayout4->addWidget(portBLabel);
    hLayout4->addWidget(portBNameLabel, 1);
    hLayout4->addWidget(selectPortBButton);

    QHBoxLayout *hLayout5 =  new QHBoxLayout;
    hLayout5->addWidget(derivedLabel);
    hLayout5->addWidget(derivedEdit);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    vLayout->addLayout(hLayout4);
    vLayout->addLayout(hLayout5);
    vLayout->addWidget(attributeTableView, 1);
    vLayout->addWidget(sensorButton);
    vLayout->addWidget(removeButton);

    QGridLayout *layout = new QGridLayout;
    layout->addLayout(vLayout, 0, 0);

    setLayout(layout);

    setModel(NULL);

    connect(nameEdit, SIGNAL(editingFinished()), this, SLOT(nameEditFinished()));
    connect(derivedEdit, SIGNAL(editingFinished()), this, SLOT(derivedEditFinished()));
    connect(selectPortAButton, SIGNAL(clicked()), this, SLOT(selectAClicked()));
    connect(selectPortBButton, SIGNAL(clicked()), this, SLOT(selectBClicked()));
    connect(sensorButton, SIGNAL(clicked()), this, SLOT(sensorClicked()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeClicked()));
}

void HEESItemWidget::setModel(HEESGraphicsItem *modelItem)
{
    item = modelItem;
    if( item == NULL)
    {
        // Clear and disable everything
        setEnabled(false);
        typeNameLabel->clear();
        nameEdit->clear();
        portANameLabel->clear();
        portBNameLabel->clear();
        derivedEdit->clear();
        attributeTableView->setModel(NULL);
    }
    else
    {
        setEnabled(true);
        selectPortAButton->setEnabled(false);
        selectPortBButton->setEnabled(false);
        portALabel->setEnabled(false);
        portBLabel->setEnabled(false);
        portANameLabel->clear();
        portBNameLabel->clear();

        switch( item->myType() )
        {
        case SOURCE:
            typeNameLabel->setText(QString("Source"));
            break;
        case BANK:
            typeNameLabel->setText(QString("Bank"));
            break;
        case LOAD:
            typeNameLabel->setText(QString("Load"));
            break;
        case CTI:
            typeNameLabel->setText(QString("CTI"));
            break;
        case CONVERTER:
            typeNameLabel->setText(QString("Converter"));
            selectPortAButton->setEnabled(true);
            selectPortBButton->setEnabled(true);
            portALabel->setEnabled(true);
            portBLabel->setEnabled(true);
            portANameLabel->setText(item->getPortAName());
            portBNameLabel->setText(item->getPortBName());
            break;
        }

        nameEdit->setText(item->name);
        derivedEdit->setText(item->derivedType);
        attributeTableView->setModel(item->myAttributes());
    }
}

void HEESItemWidget::nameEditFinished()
{
    item->name = nameEdit->text();
}

void HEESItemWidget::derivedEditFinished()
{
    item->derivedType = derivedEdit->text();
}

void HEESItemWidget::selectAClicked()
{
    isPortA = true;
    emit selectPort();
}

void HEESItemWidget::selectBClicked()
{
    isPortA = false;
    emit selectPort();
}

void HEESItemWidget::removeClicked()
{
    emit removeItem(item);
}

void HEESItemWidget::sensorClicked()
{
    // Open a dialog to edit the sensor list
    QDialog * dialog = new QDialog(this, Qt::WindowCloseButtonHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    QListView * listview = new QListView(dialog);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(listview);
    listview->setModel(item->mySensors());
    dialog->setWindowTitle(item->name + QString(": Edit Sensors"));
    dialog->setLayout(layout);
    dialog->layout()->setSizeConstraint( QLayout::SetFixedSize );
    // Main window will freeze
    dialog->exec();
}

void HEESItemWidget::portSelectedFromScene(HEESGraphicsItem *port)
{
    // Converters shall not be connected to other converters or a manager
    if( port == NULL || port->myType() == CONVERTER || port->myType() == MANAGER )
        return;
    // Components other than CTI can connect to only one converter
    if( port->myType() != CTI )
    {
        // The special case where the other port of the converter is connected to the component
        // and should be cleared.
        if( item->getLeftItem() == port )
        {
            portANameLabel->setText("");
        }
        if( item->getRightItem() == port )
        {
            portBNameLabel->setText("");
        }
        port->removeAllConverters();
    }

    if( isPortA )
    {
        item->setLeftItem(port);
        portANameLabel->setText(item->getPortAName());

    }
    else
    {
        item->setRightItem(port);
        portBNameLabel->setText(item->getPortBName());
    }
}
