/********************************************************************************
** Form generated from reading UI file 'class2.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLASS2_H
#define UI_CLASS2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_class2
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *class2)
    {
        if (class2->objectName().isEmpty())
            class2->setObjectName("class2");
        class2->resize(874, 525);
        pushButton = new QPushButton(class2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(0, 0, 131, 61));

        retranslateUi(class2);

        QMetaObject::connectSlotsByName(class2);
    } // setupUi

    void retranslateUi(QWidget *class2)
    {
        class2->setWindowTitle(QCoreApplication::translate("class2", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("class2", "\350\267\263\345\210\260\347\254\254\344\270\200\351\241\265", nullptr));
    } // retranslateUi

};

namespace Ui {
    class class2: public Ui_class2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLASS2_H
