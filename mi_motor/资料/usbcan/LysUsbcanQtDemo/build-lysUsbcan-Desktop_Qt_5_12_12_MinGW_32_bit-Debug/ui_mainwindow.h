/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_13;
    QFrame *line_4;
    QGridLayout *gridLayout_2;
    QComboBox *comboBox_devType;
    QComboBox *comboBox_devIndex;
    QLabel *label_7;
    QLabel *label_9;
    QComboBox *comboBox_chIndex;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QFrame *line_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_19;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_3;
    QComboBox *comboBox_fliterMode;
    QComboBox *comboBox_dBaud;
    QLabel *label_timing0;
    QLineEdit *lineEdit_btr0;
    QLineEdit *lineEdit_btr1;
    QLabel *label_dBaud;
    QLabel *label_timingTip;
    QComboBox *comboBox_iso;
    QLabel *label_maskCode;
    QLineEdit *lineEdit_acc;
    QLabel *label_timing1;
    QComboBox *comboBox_aBaud;
    QLabel *label_aBaud;
    QLabel *label_accCode;
    QLabel *label_fliterMode;
    QLabel *label_iso;
    QLabel *label_workPro;
    QComboBox *comboBox_workPro;
    QLineEdit *lineEdit_mask;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_21;
    QComboBox *comboBox_workMode;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_devOpen;
    QPushButton *pushButton_devClose;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_20;
    QLabel *label_15;
    QSpacerItem *horizontalSpacer_4;
    QGridLayout *gridLayout;
    QComboBox *comboBox_frameFormat;
    QComboBox *comboBox_sendMode;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *comboBox_frameType;
    QLineEdit *lineEdit_sendData;
    QLineEdit *lineEdit_sendId;
    QLabel *label_2;
    QPushButton *pushButton_send;
    QLabel *label_6;
    QLabel *label_framePro;
    QComboBox *comboBox_framePro;
    QFrame *line;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_18;
    QLabel *label_14;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_clr;
    QPushButton *pushButton_exit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(810, 599);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        label_13 = new QLabel(centralwidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        verticalLayout->addWidget(label_13);

        line_4 = new QFrame(centralwidget);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_4);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        comboBox_devType = new QComboBox(centralwidget);
        comboBox_devType->addItem(QString());
        comboBox_devType->addItem(QString());
        comboBox_devType->addItem(QString());
        comboBox_devType->addItem(QString());
        comboBox_devType->addItem(QString());
        comboBox_devType->setObjectName(QString::fromUtf8("comboBox_devType"));

        gridLayout_2->addWidget(comboBox_devType, 0, 1, 1, 1);

        comboBox_devIndex = new QComboBox(centralwidget);
        comboBox_devIndex->addItem(QString());
        comboBox_devIndex->setObjectName(QString::fromUtf8("comboBox_devIndex"));

        gridLayout_2->addWidget(comboBox_devIndex, 0, 4, 1, 1);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 2, 1, 1);

        label_9 = new QLabel(centralwidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 0, 7, 1, 1);

        comboBox_chIndex = new QComboBox(centralwidget);
        comboBox_chIndex->addItem(QString());
        comboBox_chIndex->setObjectName(QString::fromUtf8("comboBox_chIndex"));

        gridLayout_2->addWidget(comboBox_chIndex, 0, 8, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 9, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        line_2 = new QFrame(centralwidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_19 = new QLabel(centralwidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setPixmap(QPixmap(QString::fromUtf8(":/key.png")));

        horizontalLayout_2->addWidget(label_19);

        label_12 = new QLabel(centralwidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_2->addWidget(label_12);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_2);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        comboBox_fliterMode = new QComboBox(centralwidget);
        comboBox_fliterMode->addItem(QString());
        comboBox_fliterMode->addItem(QString());
        comboBox_fliterMode->setObjectName(QString::fromUtf8("comboBox_fliterMode"));

        gridLayout_3->addWidget(comboBox_fliterMode, 2, 4, 1, 1);

        comboBox_dBaud = new QComboBox(centralwidget);
        comboBox_dBaud->setObjectName(QString::fromUtf8("comboBox_dBaud"));

        gridLayout_3->addWidget(comboBox_dBaud, 1, 6, 1, 1);

        label_timing0 = new QLabel(centralwidget);
        label_timing0->setObjectName(QString::fromUtf8("label_timing0"));

        gridLayout_3->addWidget(label_timing0, 0, 1, 1, 1);

        lineEdit_btr0 = new QLineEdit(centralwidget);
        lineEdit_btr0->setObjectName(QString::fromUtf8("lineEdit_btr0"));

        gridLayout_3->addWidget(lineEdit_btr0, 0, 2, 1, 1);

        lineEdit_btr1 = new QLineEdit(centralwidget);
        lineEdit_btr1->setObjectName(QString::fromUtf8("lineEdit_btr1"));

        gridLayout_3->addWidget(lineEdit_btr1, 1, 2, 1, 1);

        label_dBaud = new QLabel(centralwidget);
        label_dBaud->setObjectName(QString::fromUtf8("label_dBaud"));

        gridLayout_3->addWidget(label_dBaud, 1, 5, 1, 1);

        label_timingTip = new QLabel(centralwidget);
        label_timingTip->setObjectName(QString::fromUtf8("label_timingTip"));

        gridLayout_3->addWidget(label_timingTip, 2, 2, 1, 1);

        comboBox_iso = new QComboBox(centralwidget);
        comboBox_iso->addItem(QString());
        comboBox_iso->addItem(QString());
        comboBox_iso->setObjectName(QString::fromUtf8("comboBox_iso"));

        gridLayout_3->addWidget(comboBox_iso, 2, 6, 1, 1);

        label_maskCode = new QLabel(centralwidget);
        label_maskCode->setObjectName(QString::fromUtf8("label_maskCode"));

        gridLayout_3->addWidget(label_maskCode, 1, 3, 1, 1);

        lineEdit_acc = new QLineEdit(centralwidget);
        lineEdit_acc->setObjectName(QString::fromUtf8("lineEdit_acc"));

        gridLayout_3->addWidget(lineEdit_acc, 0, 4, 1, 1);

        label_timing1 = new QLabel(centralwidget);
        label_timing1->setObjectName(QString::fromUtf8("label_timing1"));

        gridLayout_3->addWidget(label_timing1, 1, 1, 1, 1);

        comboBox_aBaud = new QComboBox(centralwidget);
        comboBox_aBaud->setObjectName(QString::fromUtf8("comboBox_aBaud"));

        gridLayout_3->addWidget(comboBox_aBaud, 0, 6, 1, 1);

        label_aBaud = new QLabel(centralwidget);
        label_aBaud->setObjectName(QString::fromUtf8("label_aBaud"));

        gridLayout_3->addWidget(label_aBaud, 0, 5, 1, 1);

        label_accCode = new QLabel(centralwidget);
        label_accCode->setObjectName(QString::fromUtf8("label_accCode"));

        gridLayout_3->addWidget(label_accCode, 0, 3, 1, 1);

        label_fliterMode = new QLabel(centralwidget);
        label_fliterMode->setObjectName(QString::fromUtf8("label_fliterMode"));

        gridLayout_3->addWidget(label_fliterMode, 2, 3, 1, 1);

        label_iso = new QLabel(centralwidget);
        label_iso->setObjectName(QString::fromUtf8("label_iso"));

        gridLayout_3->addWidget(label_iso, 2, 5, 1, 1);

        label_workPro = new QLabel(centralwidget);
        label_workPro->setObjectName(QString::fromUtf8("label_workPro"));

        gridLayout_3->addWidget(label_workPro, 0, 7, 1, 1);

        comboBox_workPro = new QComboBox(centralwidget);
        comboBox_workPro->addItem(QString());
        comboBox_workPro->addItem(QString());
        comboBox_workPro->setObjectName(QString::fromUtf8("comboBox_workPro"));

        gridLayout_3->addWidget(comboBox_workPro, 0, 8, 1, 1);

        lineEdit_mask = new QLineEdit(centralwidget);
        lineEdit_mask->setObjectName(QString::fromUtf8("lineEdit_mask"));

        gridLayout_3->addWidget(lineEdit_mask, 1, 4, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_7, 0, 9, 1, 1);

        label_21 = new QLabel(centralwidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_3->addWidget(label_21, 2, 7, 1, 1);

        comboBox_workMode = new QComboBox(centralwidget);
        comboBox_workMode->addItem(QString());
        comboBox_workMode->setObjectName(QString::fromUtf8("comboBox_workMode"));

        gridLayout_3->addWidget(comboBox_workMode, 2, 8, 1, 1);


        verticalLayout->addLayout(gridLayout_3);

        line_3 = new QFrame(centralwidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        pushButton_devOpen = new QPushButton(centralwidget);
        pushButton_devOpen->setObjectName(QString::fromUtf8("pushButton_devOpen"));

        horizontalLayout_4->addWidget(pushButton_devOpen);

        pushButton_devClose = new QPushButton(centralwidget);
        pushButton_devClose->setObjectName(QString::fromUtf8("pushButton_devClose"));

        horizontalLayout_4->addWidget(pushButton_devClose);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_20 = new QLabel(centralwidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setPixmap(QPixmap(QString::fromUtf8(":/file.png")));

        horizontalLayout_6->addWidget(label_20);

        label_15 = new QLabel(centralwidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_6->addWidget(label_15);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_6);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBox_frameFormat = new QComboBox(centralwidget);
        comboBox_frameFormat->addItem(QString());
        comboBox_frameFormat->addItem(QString());
        comboBox_frameFormat->setObjectName(QString::fromUtf8("comboBox_frameFormat"));

        gridLayout->addWidget(comboBox_frameFormat, 2, 1, 1, 1);

        comboBox_sendMode = new QComboBox(centralwidget);
        comboBox_sendMode->addItem(QString());
        comboBox_sendMode->setObjectName(QString::fromUtf8("comboBox_sendMode"));

        gridLayout->addWidget(comboBox_sendMode, 0, 1, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        comboBox_frameType = new QComboBox(centralwidget);
        comboBox_frameType->addItem(QString());
        comboBox_frameType->addItem(QString());
        comboBox_frameType->setObjectName(QString::fromUtf8("comboBox_frameType"));

        gridLayout->addWidget(comboBox_frameType, 1, 1, 1, 1);

        lineEdit_sendData = new QLineEdit(centralwidget);
        lineEdit_sendData->setObjectName(QString::fromUtf8("lineEdit_sendData"));

        gridLayout->addWidget(lineEdit_sendData, 1, 3, 1, 1);

        lineEdit_sendId = new QLineEdit(centralwidget);
        lineEdit_sendId->setObjectName(QString::fromUtf8("lineEdit_sendId"));

        gridLayout->addWidget(lineEdit_sendId, 0, 3, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        pushButton_send = new QPushButton(centralwidget);
        pushButton_send->setObjectName(QString::fromUtf8("pushButton_send"));

        gridLayout->addWidget(pushButton_send, 2, 4, 1, 1);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 1, 2, 1, 1);

        label_framePro = new QLabel(centralwidget);
        label_framePro->setObjectName(QString::fromUtf8("label_framePro"));

        gridLayout->addWidget(label_framePro, 2, 2, 1, 1);

        comboBox_framePro = new QComboBox(centralwidget);
        comboBox_framePro->addItem(QString());
        comboBox_framePro->addItem(QString());
        comboBox_framePro->addItem(QString());
        comboBox_framePro->setObjectName(QString::fromUtf8("comboBox_framePro"));

        gridLayout->addWidget(comboBox_framePro, 2, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_18 = new QLabel(centralwidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setPixmap(QPixmap(QString::fromUtf8(":/msg_icon_info.png")));

        horizontalLayout_5->addWidget(label_18, 0, Qt::AlignLeft);

        label_14 = new QLabel(centralwidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_5->addWidget(label_14);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        horizontalLayout->addWidget(tableWidget);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        pushButton_clr = new QPushButton(centralwidget);
        pushButton_clr->setObjectName(QString::fromUtf8("pushButton_clr"));

        horizontalLayout_3->addWidget(pushButton_clr);

        pushButton_exit = new QPushButton(centralwidget);
        pushButton_exit->setObjectName(QString::fromUtf8("pushButton_exit"));

        horizontalLayout_3->addWidget(pushButton_exit);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalLayout->setStretch(13, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 810, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\345\217\202\346\225\260:", nullptr));
        comboBox_devType->setItemText(0, QApplication::translate("MainWindow", "USBCAN1", nullptr));
        comboBox_devType->setItemText(1, QApplication::translate("MainWindow", "USBCAN2", nullptr));
        comboBox_devType->setItemText(2, QApplication::translate("MainWindow", "USBCANFDMini", nullptr));
        comboBox_devType->setItemText(3, QApplication::translate("MainWindow", "USBCANFD1", nullptr));
        comboBox_devType->setItemText(4, QApplication::translate("MainWindow", "USBCANFD2", nullptr));

        comboBox_devIndex->setItemText(0, QApplication::translate("MainWindow", "0", nullptr));

        label_7->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\264\242\345\274\225:", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "\351\200\232\351\201\223\347\264\242\345\274\225:", nullptr));
        comboBox_chIndex->setItemText(0, QApplication::translate("MainWindow", "0", nullptr));

        label->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\261\273\345\236\213:", nullptr));
        label_19->setText(QString());
        label_12->setText(QApplication::translate("MainWindow", "CAN\351\200\232\350\256\257\345\217\202\346\225\260:", nullptr));
        comboBox_fliterMode->setItemText(0, QApplication::translate("MainWindow", "\345\217\214\346\273\244\346\263\242\346\250\241\345\274\217", nullptr));
        comboBox_fliterMode->setItemText(1, QApplication::translate("MainWindow", "\345\215\225\346\273\244\346\263\242\346\250\241\345\274\217", nullptr));

        label_timing0->setText(QApplication::translate("MainWindow", "\345\256\232\346\227\266\345\231\2500(TIME0) 0x:", nullptr));
        lineEdit_btr0->setText(QApplication::translate("MainWindow", "00", nullptr));
        lineEdit_btr1->setText(QApplication::translate("MainWindow", "1C", nullptr));
        label_dBaud->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\237\237\346\263\242\347\211\271\347\216\207:", nullptr));
        label_timingTip->setText(QApplication::translate("MainWindow", "\346\263\250:500K(TIME0:0x00 TIME1:0x1C)", nullptr));
        comboBox_iso->setItemText(0, QApplication::translate("MainWindow", "NON-ISO", nullptr));
        comboBox_iso->setItemText(1, QApplication::translate("MainWindow", "CANFD-ISO", nullptr));

        label_maskCode->setText(QApplication::translate("MainWindow", "\345\261\217\350\224\275\347\240\201:0x", nullptr));
        lineEdit_acc->setText(QApplication::translate("MainWindow", "00000000", nullptr));
        label_timing1->setText(QApplication::translate("MainWindow", "\345\256\232\346\227\266\345\231\2501(TIME1) 0x", nullptr));
        label_aBaud->setText(QApplication::translate("MainWindow", "\344\273\262\350\243\201\345\237\237\346\263\242\347\211\271\347\216\207:", nullptr));
        label_accCode->setText(QApplication::translate("MainWindow", "\351\252\214\346\224\266\347\240\201:0x", nullptr));
        label_fliterMode->setText(QApplication::translate("MainWindow", "\346\273\244\346\263\242\346\250\241\345\274\217:", nullptr));
        label_iso->setText(QApplication::translate("MainWindow", "CANFD\346\240\207\345\207\206:", nullptr));
        label_workPro->setText(QApplication::translate("MainWindow", "\345\267\245\344\275\234\345\215\217\350\256\256:", nullptr));
        comboBox_workPro->setItemText(0, QApplication::translate("MainWindow", "CANFD", nullptr));
        comboBox_workPro->setItemText(1, QApplication::translate("MainWindow", "CAN", nullptr));

        lineEdit_mask->setText(QApplication::translate("MainWindow", "FFFFFFFF", nullptr));
        label_21->setText(QApplication::translate("MainWindow", "\345\267\245\344\275\234\346\250\241\345\274\217:", nullptr));
        comboBox_workMode->setItemText(0, QApplication::translate("MainWindow", "\346\255\243\345\270\270", nullptr));

        pushButton_devOpen->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\350\256\276\345\244\207", nullptr));
        pushButton_devClose->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\350\256\276\345\244\207", nullptr));
        label_20->setText(QString());
        label_15->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\217\221\351\200\201:", nullptr));
        comboBox_frameFormat->setItemText(0, QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\270\247", nullptr));
        comboBox_frameFormat->setItemText(1, QApplication::translate("MainWindow", "\350\277\234\347\250\213\345\270\247", nullptr));

        comboBox_sendMode->setItemText(0, QApplication::translate("MainWindow", "\346\255\243\345\270\270\345\217\221\351\200\201", nullptr));

        label_3->setText(QApplication::translate("MainWindow", "\345\270\247\347\261\273\345\236\213:", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\345\270\247\346\240\274\345\274\217:", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\345\270\247ID:", nullptr));
        comboBox_frameType->setItemText(0, QApplication::translate("MainWindow", "\346\240\207\345\207\206\345\270\247", nullptr));
        comboBox_frameType->setItemText(1, QApplication::translate("MainWindow", "\346\211\251\345\261\225\345\270\247", nullptr));

        lineEdit_sendData->setText(QApplication::translate("MainWindow", "00 01 02 03 04 05 06 07", nullptr));
        lineEdit_sendId->setText(QApplication::translate("MainWindow", "00000000", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\346\250\241\345\274\217:", nullptr));
        pushButton_send->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\346\225\260\346\215\256:", nullptr));
        label_framePro->setText(QApplication::translate("MainWindow", "\345\270\247\345\215\217\350\256\256:", nullptr));
        comboBox_framePro->setItemText(0, QApplication::translate("MainWindow", "CAN", nullptr));
        comboBox_framePro->setItemText(1, QApplication::translate("MainWindow", "CANFD", nullptr));
        comboBox_framePro->setItemText(2, QApplication::translate("MainWindow", "CANFD\345\212\240\351\200\237", nullptr));

        label_18->setText(QString());
        label_14->setText(QApplication::translate("MainWindow", "\344\277\241\346\201\257:", nullptr));
        pushButton_clr->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        pushButton_exit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
