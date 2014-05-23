// **********************************************
// Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPlainTextEdit>
#include "heessystemscene.h"
#include "commandsmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void selectionChangedInScene();
    void selectPortInScene();
    void removeItemInScene(HEESGraphicsItem* item);
    void openFile();
    void saveFile();
    void newFile();
    
private slots:
    void on_addButton_clicked();

    void on_cmdEditButton_clicked();

    void on_projectEdit_editingFinished();

private:
    Ui::MainWindow *ui;
    HEESSystemScene *scene;

    QString projectName;
    CommandsModel commands;
};

#endif // MAINWINDOW_H
