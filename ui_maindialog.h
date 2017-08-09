/********************************************************************************
** Form generated from reading UI file 'maindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINDIALOG_H
#define UI_MAINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MainDialog
{
public:
    QPushButton *moduleButton;
    QPushButton *panelButton;

    void setupUi(QDialog *MainDialog)
    {
        if (MainDialog->objectName().isEmpty())
            MainDialog->setObjectName(QStringLiteral("MainDialog"));
        MainDialog->resize(223, 359);
        MainDialog->setStyleSheet(QStringLiteral("background-image: url(:/new/prefix1/14.jpeg);"));
        moduleButton = new QPushButton(MainDialog);
        moduleButton->setObjectName(QStringLiteral("moduleButton"));
        moduleButton->setGeometry(QRect(30, 30, 161, 131));
        moduleButton->setStyleSheet(QLatin1String("background-image: url(:/new/prefix1/12.jpeg);\n"
"font: 11pt \"Ubuntu\";"));
        panelButton = new QPushButton(MainDialog);
        panelButton->setObjectName(QStringLiteral("panelButton"));
        panelButton->setGeometry(QRect(30, 200, 161, 131));
        panelButton->setStyleSheet(QStringLiteral("background-image: url(:/new/prefix1/12.jpeg);"));

        retranslateUi(MainDialog);

        QMetaObject::connectSlotsByName(MainDialog);
    } // setupUi

    void retranslateUi(QDialog *MainDialog)
    {
        MainDialog->setWindowTitle(QApplication::translate("MainDialog", "Simulator", Q_NULLPTR));
        moduleButton->setText(QApplication::translate("MainDialog", "Sub Module Software", Q_NULLPTR));
        panelButton->setText(QApplication::translate("MainDialog", "Main CPU", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainDialog: public Ui_MainDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINDIALOG_H
