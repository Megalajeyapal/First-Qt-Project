/********************************************************************************
** Form generated from reading UI file 'popup.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPUP_H
#define UI_POPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PopUp
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QTableWidget *tableWidget;

    void setupUi(QWidget *PopUp)
    {
        if (PopUp->objectName().isEmpty())
            PopUp->setObjectName(QStringLiteral("PopUp"));
        PopUp->resize(1228, 482);
        PopUp->setStyleSheet(QStringLiteral("background-color: rgb(114, 159, 207);"));
        pushButton = new QPushButton(PopUp);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(550, 400, 151, 41));
        pushButton->setStyleSheet(QLatin1String("background-color: rgb(186, 189, 182);\n"
"font: 12pt \"Ubuntu\";"));
        label = new QLabel(PopUp);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 30, 281, 17));
        label->setStyleSheet(QStringLiteral("font: 12pt \"Ubuntu\";"));
        tableWidget = new QTableWidget(PopUp);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(60, 70, 1121, 271));
        tableWidget->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

        retranslateUi(PopUp);

        QMetaObject::connectSlotsByName(PopUp);
    } // setupUi

    void retranslateUi(QWidget *PopUp)
    {
        PopUp->setWindowTitle(QApplication::translate("PopUp", "Form", Q_NULLPTR));
        pushButton->setText(QApplication::translate("PopUp", "Close", Q_NULLPTR));
        label->setText(QApplication::translate("PopUp", "Message Received from the Module :", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PopUp: public Ui_PopUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPUP_H
