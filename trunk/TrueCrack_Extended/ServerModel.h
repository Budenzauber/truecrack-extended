

#ifndef SERVERMODEL_H_
#define SERVERMODEL_H_

// *******************************************************************************************
// C++ Includes
// *******************************************************************************************

#include <c++/4.4/iostream>


// *******************************************************************************************
// QT Includes
// *******************************************************************************************

#include <QTcpServer>
#include <QString>
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QList>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <math.h>


// *******************************************************************************************
// Struct Definitions
// *******************************************************************************************

// Enum for Clientstatus
enum ClientStatus
{
    Running,
    Idle,
    Not_Responding
};

// Struct for ClientInfo
struct ClientInfo
{
    QTcpSocket* dataSocket;
    QTcpSocket* controlSocket;

    ClientStatus status;                // Enum for ClientStatus

    bool b_waitingForResponse;
    bool pwFound;                       // Password found?

    qint64 pwSize;                      // Password size (in byte)
    QString password;                   // Password

    QDate runsSinceDate;                // Saves date when client connected to server
    QTime runsSinceTime;                // Saves time when client connected to server

    QDate runEndDate;                   // Saves date when client is ready
    QTime runEndTime;                   // Saves time when client is ready

    QByteArray lastBlock;               // Last Block

    int i_finishedPasswordsCount;       // Finished passwords

    int i_passwordsPerSecond;             // Passwords per second
};


// *******************************************************************************************
// Class:			ServerModel
//
// Description:		???
// *******************************************************************************************
class ServerModel : public QObject
{

	Q_OBJECT

public:

    ServerModel();
	virtual ~ServerModel();

    void initialize(bool b_firstStart);

    bool readNextPasswordFile();
    bool readNextHeader(bool loadPassword = true);
    void terminateClients();
    void resetClientStats();

    // Getter
    QMap<QString, ClientInfo*> getClientList();
    float getPasswordCount();
    float getFinishedPasswordCount();
    bool isDoneLoading();

    // Setter
    void setPasswordFolder(QString S_passwordFolderPath);    
    void setHeaderFolder(QString S_headerFolderPath);
    void setOutputFile(QString S_outputFile);

public slots:

    void newDataConnection();
    void newControlConnection();
    void receiveClientResult();
    void keepAliveCheck();
    void receiveKeepAliveResponse();

    void setControlPort(int controlPort);
    void setDataPort(int dataPort);
    void setPasswordBlockSize(int passwordBlockSize);

signals:

    void logInfo(QString logInformation);
    void signal_newDataConnection();
    void signal_resetButtons();    

private:

    // Methods
    void interruptClients();
    void sendDecryptInfo(QTcpSocket *socket);
    void log(QString logInformation);
    void distributeDecryptionData();


    QTimer *keepAliveTimer;                 // Timer for keep-alive check

    // Header and passwordlist
    QByteArray BA_passwordList;
    QByteArray BA_volumeHeader;

    QMutex *mutex;                          // Mutex to synchronize access to dataSocketList
    QMutex *controlMutex;                   // Mutex to synchronize access to controlSocketList
    QTcpServer *dataServer;                 // Server to distribute header/passwords
    QTcpServer *controlServer;              // Server to remotecntrol client
    QMap<QString, ClientInfo*> clientList;  // Stores all incoming connections

    // Password and Header variables
    QList<QByteArray> pwBlockList;          // List containing the blocks of passwords of current passwordfile
    QStringList SL_currentFilePasswords;    // List containing all passwords of current passwordfile

    // Variables for reading header folder
    QString S_headerFolderPath;             // Path of the header folder
    QStringList headerFiles;                // List containing all the headerfiles in the headerFolder
    QStringList finishedHeaderFiles;        // List containing all the finished headerfiles

    // Variables for reading password folder
    QString S_passwordFolderPath;           // Path to password folder
    QStringList passwordFiles;              // List containing all the passwordfiles in the passwordFolder
    QStringList finishedPasswordFiles;      // List containing all the finished passwordfiles for the actual header

    bool b_donePasswordLoading;             // Is constructor done loading
    bool b_doneHeaderLoading;

    int i_numberOfBlocks;                   // numberOfBlocks for current password file

    int controlPort;                        // Ports
    int dataPort;

    float finishedPasswordsCount;           // Number of finished Passwords

    float pwBlockSize;                      // Passwords block size

    QString S_outputFile;                   // Output file for results

};

#endif /* SERVERMODEL_H_ */
