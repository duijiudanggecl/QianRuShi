/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *page1;
    QPushButton *BY001_button;
    QTextEdit *main_input;
    QLabel *label_2;
    QLabel *label_3;
    QTextEdit *main_output;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(947, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        page1 = new QPushButton(centralwidget);
        page1->setObjectName("page1");
        page1->setGeometry(QRect(0, 0, 161, 71));
        BY001_button = new QPushButton(centralwidget);
        BY001_button->setObjectName("BY001_button");
        BY001_button->setGeometry(QRect(100, 160, 171, 51));
        main_input = new QTextEdit(centralwidget);
        main_input->setObjectName("main_input");
        main_input->setGeometry(QRect(100, 90, 331, 51));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 100, 101, 31));
        QFont font;
        font.setPointSize(14);
        label_2->setFont(font);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 250, 53, 31));
        label_3->setFont(font);
        main_output = new QTextEdit(centralwidget);
        main_output->setObjectName("main_output");
        main_output->setGeometry(QRect(100, 250, 181, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 947, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\240\241\351\252\214\345\267\245\345\205\267", nullptr));
        page1->setText(QCoreApplication::translate("MainWindow", "\350\267\263\350\275\254\345\210\260\347\254\254\344\272\214\351\241\265", nullptr));
        BY001_button->setText(QCoreApplication::translate("MainWindow", "BY001_\345\212\240\346\263\225\346\240\241\351\252\214\347\256\227\346\263\225", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\346\225\260\346\215\256\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\347\273\223\346\236\234\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
