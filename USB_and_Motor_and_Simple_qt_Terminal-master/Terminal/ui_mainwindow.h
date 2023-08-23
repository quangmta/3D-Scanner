/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPushButton *But_Send2;
    QPushButton *But_Connect;
    QPushButton *But_Settings;
    QComboBox *comboBox;
    QPushButton *But_Send1;
    QLabel *label;
    QCheckBox *checkBox_N_Enable;
    QCheckBox *checkBox_MS1;
    QCheckBox *checkBox_MS2;
    QCheckBox *checkBox_MS3;
    QCheckBox *checkBox_STEP;
    QCheckBox *checkBox_DIR;
    QTextEdit *textEdit;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(556, 343);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(556, 343));
        MainWindow->setMaximumSize(QSize(556, 343));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        But_Send2 = new QPushButton(centralWidget);
        But_Send2->setObjectName(QStringLiteral("But_Send2"));
        But_Send2->setMinimumSize(QSize(150, 50));
        But_Send2->setMaximumSize(QSize(150, 50));
        But_Send2->setText(QStringLiteral("'2' - ecnoder postion"));

        gridLayout->addWidget(But_Send2, 2, 1, 1, 1);

        But_Connect = new QPushButton(centralWidget);
        But_Connect->setObjectName(QStringLiteral("But_Connect"));
        But_Connect->setMinimumSize(QSize(100, 23));
        But_Connect->setMaximumSize(QSize(100, 23));

        gridLayout->addWidget(But_Connect, 0, 2, 1, 1);

        But_Settings = new QPushButton(centralWidget);
        But_Settings->setObjectName(QStringLiteral("But_Settings"));
        But_Settings->setMinimumSize(QSize(100, 23));
        But_Settings->setMaximumSize(QSize(100, 23));

        gridLayout->addWidget(But_Settings, 0, 3, 1, 1);

        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setMinimumSize(QSize(160, 23));
        comboBox->setMaximumSize(QSize(100, 23));

        gridLayout->addWidget(comboBox, 0, 0, 1, 1);

        But_Send1 = new QPushButton(centralWidget);
        But_Send1->setObjectName(QStringLiteral("But_Send1"));
        But_Send1->setMinimumSize(QSize(150, 50));
        But_Send1->setMaximumSize(QSize(150, 50));

        gridLayout->addWidget(But_Send1, 2, 0, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/redStone.png")));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        checkBox_N_Enable = new QCheckBox(centralWidget);
        checkBox_N_Enable->setObjectName(QStringLiteral("checkBox_N_Enable"));

        gridLayout->addWidget(checkBox_N_Enable, 3, 0, 1, 1);

        checkBox_MS1 = new QCheckBox(centralWidget);
        checkBox_MS1->setObjectName(QStringLiteral("checkBox_MS1"));

        gridLayout->addWidget(checkBox_MS1, 4, 0, 1, 1);

        checkBox_MS2 = new QCheckBox(centralWidget);
        checkBox_MS2->setObjectName(QStringLiteral("checkBox_MS2"));

        gridLayout->addWidget(checkBox_MS2, 5, 0, 1, 1);

        checkBox_MS3 = new QCheckBox(centralWidget);
        checkBox_MS3->setObjectName(QStringLiteral("checkBox_MS3"));

        gridLayout->addWidget(checkBox_MS3, 6, 0, 1, 1);

        checkBox_STEP = new QCheckBox(centralWidget);
        checkBox_STEP->setObjectName(QStringLiteral("checkBox_STEP"));

        gridLayout->addWidget(checkBox_STEP, 7, 0, 1, 1);

        checkBox_DIR = new QCheckBox(centralWidget);
        checkBox_DIR->setObjectName(QStringLiteral("checkBox_DIR"));

        gridLayout->addWidget(checkBox_DIR, 8, 0, 1, 1);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 3, 1, 6, 3);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        But_Connect->setText(QApplication::translate("MainWindow", "Connect", 0));
        But_Settings->setText(QApplication::translate("MainWindow", "Settings", 0));
        But_Send1->setText(QApplication::translate("MainWindow", "'1' - drive", 0));
        label->setText(QString());
        checkBox_N_Enable->setText(QApplication::translate("MainWindow", "N_Enable", 0));
        checkBox_MS1->setText(QApplication::translate("MainWindow", "MS1", 0));
        checkBox_MS2->setText(QApplication::translate("MainWindow", "MS2", 0));
        checkBox_MS3->setText(QApplication::translate("MainWindow", "MS3", 0));
        checkBox_STEP->setText(QApplication::translate("MainWindow", "STEP", 0));
        checkBox_DIR->setText(QApplication::translate("MainWindow", "DIR", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
