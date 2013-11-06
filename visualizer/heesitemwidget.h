// **********************************************
// Copyright (c) 2013 SPORTS Lab(http://atrak.usc.edu/~sport/),
// University of Southern California
// **********************************************
#ifndef HEESITEMWIDGET_H
#define HEESITEMWIDGET_H

#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QGraphicsView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <QTableView>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QDialog>
#include <QListView>

#include "heesgraphicsitem.h"

// View for HEESGraphicsItem
class HEESItemWidget : public QWidget
{
    Q_OBJECT
    
public:
    HEESItemWidget(QWidget *parent = 0);
    void setModel( HEESGraphicsItem *modelItem );

signals:
    void selectPort();
    void removeItem(HEESGraphicsItem *item);

public slots:
    void portSelectedFromScene(HEESGraphicsItem *port);

private slots:
    void nameEditFinished();
    void derivedEditFinished();
    void selectAClicked();
    void selectBClicked();
    void removeClicked();
    void sensorClicked();

private:
    QLabel *typeLabel;
    QLabel *typeNameLabel;

    QLabel *nameLabel;
    QLineEdit *nameEdit;

    QLabel *portALabel;
    QLabel *portANameLabel;
    QPushButton *selectPortAButton;

    QLabel *portBLabel;
    QLabel *portBNameLabel;
    QPushButton *selectPortBButton;

    QLabel *derivedLabel;
    QLineEdit *derivedEdit;
    QTableView *attributeTableView;
    QPushButton *removeButton;
    QPushButton *sensorButton;

    HEESGraphicsItem *item;

    bool isPortA;
};

#endif
