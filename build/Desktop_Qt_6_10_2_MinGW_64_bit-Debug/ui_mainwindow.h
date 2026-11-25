/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QLabel *label_2;
    QLabel *label;
    QPushButton *connect;
    QLineEdit *s_IP;
    QLineEdit *S_TCP;
    QGroupBox *groupBox_2;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_3;
    QLineEdit *slave_address;
    QLineEdit *start_data;
    QLineEdit *register_2;
    QGroupBox *groupBox_3;
    QLabel *label_6;
    QLineEdit *plot_p;
    QPushButton *update;
    QPushButton *clear;
    QGroupBox *groupBox_4;
    QComboBox *comboBox;
    QPushButton *request;
    QCustomPlot *plot;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1155, 726);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 10, 301, 131));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 50, 63, 20));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 80, 101, 20));
        connect = new QPushButton(groupBox);
        connect->setObjectName("connect");
        connect->setGeometry(QRect(210, 36, 81, 81));
        s_IP = new QLineEdit(groupBox);
        s_IP->setObjectName("s_IP");
        s_IP->setGeometry(QRect(90, 50, 101, 20));
        S_TCP = new QLineEdit(groupBox);
        S_TCP->setObjectName("S_TCP");
        S_TCP->setGeometry(QRect(120, 80, 71, 20));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(340, 10, 281, 131));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(10, 32, 101, 20));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(10, 66, 131, 20));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 100, 101, 20));
        slave_address = new QLineEdit(groupBox_2);
        slave_address->setObjectName("slave_address");
        slave_address->setGeometry(QRect(120, 33, 151, 20));
        start_data = new QLineEdit(groupBox_2);
        start_data->setObjectName("start_data");
        start_data->setGeometry(QRect(140, 67, 131, 20));
        register_2 = new QLineEdit(groupBox_2);
        register_2->setObjectName("register_2");
        register_2->setGeometry(QRect(120, 100, 151, 20));
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(810, 10, 321, 131));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(10, 50, 81, 20));
        plot_p = new QLineEdit(groupBox_3);
        plot_p->setObjectName("plot_p");
        plot_p->setGeometry(QRect(10, 80, 101, 20));
        update = new QPushButton(groupBox_3);
        update->setObjectName("update");
        update->setGeometry(QRect(120, 40, 91, 71));
        clear = new QPushButton(groupBox_3);
        clear->setObjectName("clear");
        clear->setGeometry(QRect(220, 40, 91, 71));
        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(640, 10, 151, 131));
        comboBox = new QComboBox(groupBox_4);
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(10, 40, 131, 21));
        request = new QPushButton(groupBox_4);
        request->setObjectName("request");
        request->setGeometry(QRect(30, 80, 90, 29));
        plot = new QCustomPlot(centralwidget);
        plot->setObjectName("plot");
        plot->setGeometry(QRect(10, 160, 1131, 511));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1155, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Server Conection:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Server IP", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Server TCP Port", nullptr));
        connect->setText(QCoreApplication::translate("MainWindow", "CONNECT", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Request Config: ", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Slave Address", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Start data address", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Register count", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Plot config:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Plot points", nullptr));
        update->setText(QCoreApplication::translate("MainWindow", "UPDATE", nullptr));
        clear->setText(QCoreApplication::translate("MainWindow", "CLEAR PLOT", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Resquest Interval:", nullptr));
        request->setText(QCoreApplication::translate("MainWindow", "Request", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
