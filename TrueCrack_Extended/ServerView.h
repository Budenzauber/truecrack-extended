

#ifndef SERVERVIEW_H
#define SERVERVIEW_H

// *******************************************************************************************
// Project Includes
// *******************************************************************************************

#include "ServerModel.h"


// *******************************************************************************************
// QT Includes
// *******************************************************************************************

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTcpSocket>
#include <QFileDialog>
#include <QTimer>
#include <QDateTime>
#include <QDate>
#include <QTime>


namespace Ui
{
    class ServerView;
}


// *******************************************************************************************
// Class:			GuiServerMainWindow
//
// Description:		The GuiServerMainWindow is derived from QMainWindow and represents
//                  the server gui. It provides serveral controls like the log, information
//                  table about connected clients and some other controls to adjust the
//                  requires options and to start the server
// *******************************************************************************************

class ServerView : public QMainWindow
{

    Q_OBJECT
    
private:

    Ui::ServerView *ui;

    ServerModel *serverModel;

    float allPasswordsCount;
    float finishedPasswordsCount;

    bool b_firstStart;

    // Timer for GUI update
    QTimer *timerGUI;

public:

    explicit ServerView(QWidget *parent = 0);    
    ~ServerView();
    void initialize(int controlPort = 2000, int dataPort = 2001, QString S_headerFolder = QDir::currentPath() + "/HeaderFiles/",
                    QString S_passwordFolder = QDir::currentPath() + "/PasswordFiles/", QString outputFile = QDir::currentPath() + "/result.txt",
                    int i_blockSize = 100000);

public slots:

    void addLogInformation(QString logInformation);
    void newClientConnected();

    void selectVolumeFilePushButton_clicked();
    void selectPasswordListFilePushButton_clicked();

    void updateGUIData();
    void slot_resetButtons();

private slots:

    void on_StartServerPushButton_clicked();
    void on_QuitPushButton_clicked();
    void on_actionQuit_triggered();

    void on_actionAbout_triggered();

};

#endif // SERVERVIEW_H
