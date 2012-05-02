/********************************************************************************
** Form generated from reading UI file 'GuiServerMainWindow.ui'
**
** Created: Tue Mar 20 21:59:18 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERVIEW_H
#define UI_SERVERVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerView
{
public:
    QAction *actionAbout;
    QAction *actionContact;
    QAction *actionQuit;
    QWidget *centralwidget;
    QListWidget *LogListWidget;
    QLabel *label;
    QPushButton *QuitPushButton;
    QPushButton *StartServerPushButton;
    QProgressBar *PasswordsProcessedProgressBar;
    QPushButton *SelectVolumeFilePushButton;
    QPushButton *SelectPasswordListFilePushButton;
    QLabel *PasswordsAllLabel;
    QLabel *PasswordsFinishedLabel;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QTableWidget *ClientsTableWidget;
    QSpinBox *PasswordBlockSizeSpinBox;
    QSpinBox *DataPortSpinBox;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *ControlPortSpinBox;
    QLabel *label_4;
    QLabel *label_16;
    QLabel *label_17;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *ServerView)
    {
        if (ServerView->objectName().isEmpty())
            ServerView->setObjectName(QString::fromUtf8("GuiServerMainWindow"));
        ServerView->resize(992, 666);
        ServerView->setStyleSheet(QString::fromUtf8(""));
        actionAbout = new QAction(ServerView);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionContact = new QAction(ServerView);
        actionContact->setObjectName(QString::fromUtf8("actionContact"));
        actionQuit = new QAction(ServerView);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralwidget = new QWidget(ServerView);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        LogListWidget = new QListWidget(centralwidget);
        LogListWidget->setObjectName(QString::fromUtf8("LogListWidget"));
        LogListWidget->setGeometry(QRect(553, 3, 431, 361));
        LogListWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(50, 255, 50);"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 350, 121, 19));
        QuitPushButton = new QPushButton(centralwidget);
        QuitPushButton->setObjectName(QString::fromUtf8("QuitPushButton"));
        QuitPushButton->setGeometry(QRect(840, 588, 141, 41));
        StartServerPushButton = new QPushButton(centralwidget);
        StartServerPushButton->setObjectName(QString::fromUtf8("StartServerPushButton"));
        StartServerPushButton->setGeometry(QRect(60, 280, 461, 31));
        StartServerPushButton->setStyleSheet(QString::fromUtf8(""));
        StartServerPushButton->setDefault(false);
        PasswordsProcessedProgressBar = new QProgressBar(centralwidget);
        PasswordsProcessedProgressBar->setObjectName(QString::fromUtf8("PasswordsProcessedProgressBar"));
        PasswordsProcessedProgressBar->setGeometry(QRect(3, 588, 821, 41));
        PasswordsProcessedProgressBar->setStyleSheet(QString::fromUtf8(""));
        PasswordsProcessedProgressBar->setValue(80);
        SelectVolumeFilePushButton = new QPushButton(centralwidget);
        SelectVolumeFilePushButton->setObjectName(QString::fromUtf8("SelectVolumeFilePushButton"));
        SelectVolumeFilePushButton->setGeometry(QRect(60, 100, 211, 31));
        SelectVolumeFilePushButton->setStyleSheet(QString::fromUtf8(""));
        SelectVolumeFilePushButton->setDefault(false);
        SelectPasswordListFilePushButton = new QPushButton(centralwidget);
        SelectPasswordListFilePushButton->setObjectName(QString::fromUtf8("SelectPasswordListFilePushButton"));
        SelectPasswordListFilePushButton->setGeometry(QRect(310, 100, 211, 31));
        SelectPasswordListFilePushButton->setStyleSheet(QString::fromUtf8(""));
        SelectPasswordListFilePushButton->setDefault(false);
        PasswordsAllLabel = new QLabel(centralwidget);
        PasswordsAllLabel->setObjectName(QString::fromUtf8("PasswordsAllLabel"));
        PasswordsAllLabel->setGeometry(QRect(20, 599, 171, 19));
        PasswordsFinishedLabel = new QLabel(centralwidget);
        PasswordsFinishedLabel->setObjectName(QString::fromUtf8("PasswordsFinishedLabel"));
        PasswordsFinishedLabel->setGeometry(QRect(640, 600, 161, 19));
        PasswordsFinishedLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_13 = new QLabel(centralwidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 20, 31, 19));
        label_13->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Segoe\";"));
        label_13->setAlignment(Qt::AlignCenter);
        label_14 = new QLabel(centralwidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 80, 31, 19));
        label_14->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Segoe\";"));
        label_14->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(centralwidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 170, 31, 19));
        label_15->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Segoe\";"));
        label_15->setAlignment(Qt::AlignCenter);
        ClientsTableWidget = new QTableWidget(centralwidget);
        if (ClientsTableWidget->columnCount() < 5)
            ClientsTableWidget->setColumnCount(5);
        ClientsTableWidget->setObjectName(QString::fromUtf8("ClientsTableWidget"));
        ClientsTableWidget->setGeometry(QRect(10, 380, 970, 192));
        ClientsTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ClientsTableWidget->setColumnCount(5);
        ClientsTableWidget->setProperty("horizontalHeaderMaximumSectionSize", QVariant(194));
        ClientsTableWidget->horizontalHeader()->setDefaultSectionSize(194);
        PasswordBlockSizeSpinBox = new QSpinBox(centralwidget);
        PasswordBlockSizeSpinBox->setObjectName(QString::fromUtf8("PasswordBlockSizeSpinBox"));
        PasswordBlockSizeSpinBox->setGeometry(QRect(333, 35, 151, 29));
        PasswordBlockSizeSpinBox->setMinimum(10);
        PasswordBlockSizeSpinBox->setMaximum(999999999);
        PasswordBlockSizeSpinBox->setSingleStep(50);
        PasswordBlockSizeSpinBox->setValue(1000);
        DataPortSpinBox = new QSpinBox(centralwidget);
        DataPortSpinBox->setObjectName(QString::fromUtf8("DataPortSpinBox"));
        DataPortSpinBox->setGeometry(QRect(390, 190, 91, 29));
        DataPortSpinBox->setMinimum(1024);
        DataPortSpinBox->setMaximum(65535);
        DataPortSpinBox->setValue(2001);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(317, 194, 81, 19));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(90, 194, 81, 19));
        ControlPortSpinBox = new QSpinBox(centralwidget);
        ControlPortSpinBox->setObjectName(QString::fromUtf8("ControlPortSpinBox"));
        ControlPortSpinBox->setGeometry(QRect(200, 190, 91, 29));
        ControlPortSpinBox->setMinimum(1024);
        ControlPortSpinBox->setMaximum(65535);
        ControlPortSpinBox->setValue(2000);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(130, 40, 141, 19));
        label_16 = new QLabel(centralwidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 260, 31, 19));
        label_16->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Segoe\";"));
        label_16->setAlignment(Qt::AlignCenter);
        label_17 = new QLabel(centralwidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(280, 80, 31, 19));
        label_17->setStyleSheet(QString::fromUtf8("font: 75 14pt \"Segoe\";"));
        label_17->setAlignment(Qt::AlignCenter);
        ServerView->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(ServerView);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 992, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        ServerView->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionAbout);

        retranslateUi(ServerView);

        QMetaObject::connectSlotsByName(ServerView);
    } // setupUi

    void retranslateUi(QMainWindow *ServerView)
    {
        ServerView->setWindowTitle(QApplication::translate("GuiServerMainWindow", "TrueCrack Enhanced", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("GuiServerMainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionContact->setText(QApplication::translate("GuiServerMainWindow", "Contact", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("GuiServerMainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GuiServerMainWindow", "Connected clients:", 0, QApplication::UnicodeUTF8));
        QuitPushButton->setText(QApplication::translate("GuiServerMainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        StartServerPushButton->setText(QApplication::translate("GuiServerMainWindow", "Start Server", 0, QApplication::UnicodeUTF8));
        PasswordsProcessedProgressBar->setFormat(QApplication::translate("GuiServerMainWindow", "%p %", 0, QApplication::UnicodeUTF8));
        SelectVolumeFilePushButton->setText(QApplication::translate("GuiServerMainWindow", "Select Header Folder ...", 0, QApplication::UnicodeUTF8));
        SelectPasswordListFilePushButton->setText(QApplication::translate("GuiServerMainWindow", "Select Password Folder ...", 0, QApplication::UnicodeUTF8));
        PasswordsAllLabel->setText(QApplication::translate("GuiServerMainWindow", "Passwords: 0", 0, QApplication::UnicodeUTF8));
        PasswordsFinishedLabel->setText(QApplication::translate("GuiServerMainWindow", "Finished: 0", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("GuiServerMainWindow", "1.", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("GuiServerMainWindow", "2.", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("GuiServerMainWindow", "4.", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GuiServerMainWindow", "Data Port", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("GuiServerMainWindow", "Control Port", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("GuiServerMainWindow", "Password Block Size", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("GuiServerMainWindow", "5.", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("GuiServerMainWindow", "3.", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("GuiServerMainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("GuiServerMainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ServerView: public Ui_ServerView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUISERVERMAINWINDOW_H
