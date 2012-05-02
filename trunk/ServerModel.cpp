

// *******************************************************************************************
// Project Includes
// *******************************************************************************************

#include "ServerModel.h"


// *******************************************************************************************
// QT Includes
// *******************************************************************************************

#include <QByteArray>
#include <QString>
#include <QTcpSocket>
#include <QNetworkAddressEntry>
#include <QHostAddress>


// *******************************************************************************************
// C++ Includes
// *******************************************************************************************

#include <stdlib.h>


// *******************************************************************************************
// Constructor
//
// Description: Constructor initializes some variables and returns a ServerModel object
// *******************************************************************************************
ServerModel::ServerModel()
{
    // Initialize
    b_donePasswordLoading = false;
    b_doneHeaderLoading = false;

    S_headerFolderPath = "";
    S_passwordFolderPath = "";
    S_outputFile = "result.txt";

    // Initialize servers with 0
    dataServer = 0;
    controlServer = 0;
}


// *******************************************************************************************
// Destructor
//
// Description: ###
// *******************************************************************************************
ServerModel::~ServerModel()
{
}


///////////////////////////////////// SETTER METHODS /////////////////////////////////////////

// *******************************************************************************************
// setControlPort
//
// Description: Sets the control port
// *******************************************************************************************
void ServerModel::setControlPort(int controlPort)
{
    this->controlPort = controlPort;
}


// *******************************************************************************************
// setDataTransferPort
//
// Description: Sets the data transfer port
// *******************************************************************************************
void ServerModel::setDataPort(int dataPort)
{
    this->dataPort = dataPort;
}


// *******************************************************************************************
// setPasswordBlockSize
//
// Description: Sets the password block size for every client
// *******************************************************************************************
void ServerModel::setPasswordBlockSize(int passwordBlockSize)
{
    this->pwBlockSize = (float)passwordBlockSize;
}



///////////////////////////////////// GETTER METHODS /////////////////////////////////////////

// *******************************************************************************************
// getPasswordCount
//
// Description: Returns the number of Passwords of the passwordList
// *******************************************************************************************
float ServerModel::getPasswordCount()
{
    return this->SL_currentFilePasswords.size();
}


// *******************************************************************************************
// getFinishedPasswordCount
//
// Description: sends terminate-signal to all clients
// *******************************************************************************************
float ServerModel::getFinishedPasswordCount()
{
    return this->finishedPasswordsCount;
}


// *******************************************************************************************
// getClientList()
//
// Description: Returns clientList
// *******************************************************************************************
QMap<QString, ClientInfo*> ServerModel::getClientList()
{
    return this->clientList;
}



///////////////////////////////////// SERVER METHODS /////////////////////////////////////////

// *******************************************************************************************
// setPasswordFolder
//
// Description: Sets the given passwordList-Folder
// *******************************************************************************************
void ServerModel::setPasswordFolder(QString S_passwordFolderPath)
{
    // Set bool
    b_donePasswordLoading = false;
    // Reset finishedPasswordFiles
    finishedPasswordFiles.clear();

    // Save given folder path
    this->S_passwordFolderPath = S_passwordFolderPath;
}


// *******************************************************************************************
// readNextPasswordFile
//
// Description: Reads the next passwordList-File
// *******************************************************************************************
bool ServerModel::readNextPasswordFile()
{
    // Set bool
    b_donePasswordLoading = false;
    // Set finishedPasswordsCount
    finishedPasswordsCount = 0;

    // Refresh list of passwordFiles
    QDir passwordDir(S_passwordFolderPath);
    passwordFiles = passwordDir.entryList(QStringList("*.txt"), QDir::Files, QDir::Name);
    log("Password folder contains " + QString::number(passwordFiles.size()) + " password files.");

    // Check if not empty
    if(passwordFiles.empty())
    {
        log("No password files found!");
        return false; // Abort
    }

    // Clear Blocklist
    pwBlockList.clear();

    // Get next unused passwordFile
    QString S_nextPasswordFile;
    bool foundNext = false;

    for(int i = 0; i < passwordFiles.size(); i++)
    {
        S_nextPasswordFile = passwordFiles.at(i);

        // Take passwordFile if it is not yet in list of finished files
        if(!finishedPasswordFiles.contains(S_nextPasswordFile))
        {
            foundNext = true;
            finishedPasswordFiles.append(S_nextPasswordFile);
            break;
        }
    }

    // Read File if unused file found
    if(foundNext)
    {
        QFile F_passwordList(S_passwordFolderPath + "/" + S_nextPasswordFile);
        F_passwordList.open(QIODevice::ReadOnly);
        BA_passwordList = F_passwordList.readAll();

        // Split into parts of size pwBlockSize
        QString S_pwL(BA_passwordList);
        QStringList pwSL = S_pwL.split('\n',QString::SkipEmptyParts);

        // Copy Password StringList to class-member password string list for further use
        SL_currentFilePasswords = pwSL;

        // Divide into blocks
        i_numberOfBlocks = ceil(pwSL.size() / pwBlockSize);        
        int i_counter = 0;

        // Split the StringList and put into byteArrays
        QByteArray BA_helpArray;

        while(!pwSL.isEmpty())
        {
            // Check for pwBlockSize
            if(i_counter == pwBlockSize)
            {
                // Append BA_helpArray to pwBlockList
                pwBlockList.append(BA_helpArray);
                BA_helpArray.clear(); // New BA_helpArray
                i_counter = 0; // Reset counter
            }

            // Append next string + line break to BA_helpArray
            BA_helpArray.append(pwSL.takeFirst()+ '\n');

            // Increase counter
            i_counter++;
        }

        // Append last block
        pwBlockList.append(BA_helpArray);

        // Set bool
        b_donePasswordLoading = true;

        log("Password list '" + S_nextPasswordFile + "' loaded");
        log("Passwords read: " + QString::number(SL_currentFilePasswords.size()));
        log("--------------------------------------------------------");

        return true;
    }
    else
    {
        log("All password files have been processed for current header.");
        log("Switching to next header.");

        return readNextHeader();
    }
}


// *******************************************************************************************
// setHeaderFolder
//
// Description: Sets the given Header-Folder
// *******************************************************************************************
void ServerModel::setHeaderFolder(QString S_headerFolderPath)
{
    // Set bool
    b_doneHeaderLoading = false;
    // Reset finishedHeaderFiles
    finishedHeaderFiles.clear();

    // Save given folder path
    this->S_headerFolderPath = S_headerFolderPath;
}


// *******************************************************************************************
// readNextHeader
//
// Description: Reads next header file, if successfull triggers readNextPasswordFile()
// *******************************************************************************************
bool ServerModel::readNextHeader(bool loadPassword)
{
    // Set bool
    b_doneHeaderLoading = false;

    // Reset finishedPasswordFiles for new Header
    finishedPasswordFiles.clear();

    // Refresh list of headerFiles
    QDir headerDir(S_headerFolderPath);
    headerFiles = headerDir.entryList(QStringList("*"), QDir::Files, QDir::Name);
    log("Header folder contains " + QString::number(headerFiles.size()) + " header files.");

    // Check if headerFiles list empty
    if(headerFiles.empty())
    {
        log("No header files found!");
        return false; // Abort
    }

    // Get next unused headerFile
    QString S_nextHeaderFile;
    bool foundNext = false;

    for(int i = 0; i < headerFiles.size(); i++)
    {
        S_nextHeaderFile = headerFiles.at(i);

        // Take passwordFile if it is not yet in list of finished files
        if(!finishedHeaderFiles.contains(S_nextHeaderFile))
        {
            foundNext = true;
            finishedHeaderFiles.append(S_nextHeaderFile);
            break;
        }
    }

    ///// Read header /////
    if(foundNext)
    {        
        QFile F_volumeHeader(S_headerFolderPath + "/" + S_nextHeaderFile);
        // Reading TrueCrypt volume header
        F_volumeHeader.open(QIODevice::ReadOnly);
        BA_volumeHeader = F_volumeHeader.read(1024);

        // Set bool
        b_doneHeaderLoading = true;

        log("Header File " + S_nextHeaderFile + " loaded");
        log("Header information extracted");
        log("--------------------------------------------------------");

        if(loadPassword)
        {            
            return readNextPasswordFile();
        }
        else
            return true;
    }
    else
    {
        log("All header files have been processed.");
        log("You can now chose a new header + password folder and start again.\n");
        keepAliveTimer->stop();
        emit(signal_resetButtons());
        pwBlockList.clear();

        return false;
    }
}


// *******************************************************************************************
// setOutputFile
//
// Description: Sets the given Output-File
// *******************************************************************************************
void ServerModel::setOutputFile(QString S_outputFile)
{
    this->S_outputFile = S_outputFile;
}


// *******************************************************************************************
// initialize()
//
// Description: ###
// *******************************************************************************************
void ServerModel::initialize(bool b_firstStart)
{
    dataServer = new QTcpServer(this);
    controlServer = new QTcpServer(this);

    mutex = new QMutex();
    controlMutex = new QMutex();

    // Setup dataServer
    connect(dataServer, SIGNAL(newConnection()), this, SLOT(newDataConnection()));
    // Setup controlServer
    connect(controlServer, SIGNAL(newConnection()), this, SLOT(newControlConnection()));

    if(b_firstStart)
    {
        if (!dataServer->listen(QHostAddress::Any, dataPort))
            log("Data-Server could not be started: \n" + dataServer->errorString());
        if (!controlServer->listen(QHostAddress::Any, controlPort))
            log("Control-Server could not be started: \n" + controlServer->errorString());
    }

    log("-------- Server successfully started! --------");

    // Distribute decryption data to already connected clients
    distributeDecryptionData();

    // Start keep-alive timer
    keepAliveTimer = new QTimer(this);
    connect(keepAliveTimer, SIGNAL(timeout()), this, SLOT(keepAliveCheck()));
    keepAliveTimer->start(10000);
}


// *******************************************************************************************
// distributeDecryptionData()
//
// Description: Starts distributing the data to the clients
// *******************************************************************************************
void ServerModel::distributeDecryptionData()
{
    // Check if there are clients connected to the server
    if(!clientList.isEmpty())
    {
        // Start sending packages to connected clients
        QMapIterator<QString, ClientInfo*> i(clientList);

        while(i.hasNext())
        {
            // If pwBlockList is empty -> break (just for performance reasons)
            if(pwBlockList.isEmpty())
                break;

            i.next();
            if(i.value()->status == Idle) // Only if client is Idle
                sendDecryptInfo(i.value()->dataSocket);
        }
    }
}


// *******************************************************************************************
// log()
//
// Description: Emits the logInfo-Signal with which the ViewClass-Log-Method is connected
// *******************************************************************************************
void ServerModel::log(QString logInformation)
{
    emit logInfo(logInformation);
}


// *******************************************************************************************
// newDataConnection()
//
// Description: Is called when a client wants to connect to the dataServer. A new socket
//				object is created which contains the connection to the client.
// *******************************************************************************************
void ServerModel::newDataConnection()
{
    // Get next connection
    QTcpSocket *dataSocket = dataServer->nextPendingConnection();

    QString ipAdress = dataSocket->peerAddress().toString();

    // Connect controlSocket to readyRead()-Signal to receive results from client
    connect(dataSocket, SIGNAL(readyRead()), this, SLOT(receiveClientResult()));

    // lock Mutex to synchronise access of the clientList
    mutex->lock();

    // Check if client is already in list
    if(clientList.contains(ipAdress))
    {
        ClientInfo *client = clientList.value(ipAdress);
        client->dataSocket = dataSocket;
        client->i_finishedPasswordsCount = 0;
        client->runsSinceDate = QDate::currentDate();
        client->runsSinceTime = QTime::currentTime();
    }
    else
    {
        ClientInfo *client = new ClientInfo;
        client->dataSocket = dataSocket;
        client->status = Idle;
        client->b_waitingForResponse = false;
        client->i_finishedPasswordsCount = 0;
        client->lastBlock.clear();
        client->runsSinceDate = QDate::currentDate();
        client->runsSinceTime = QTime::currentTime();
        clientList.insert(ipAdress, client);
    }

    // unlock Mutex
    mutex->unlock();

    // Send decrypt information
    sendDecryptInfo(dataSocket);

    emit signal_newDataConnection();
}


// *******************************************************************************************
// newControlConnection()
//
// Description: Is called when a client wants to connect to the controlServer. A new socket
//				object is created which contains the connection to the client.
// *******************************************************************************************
void ServerModel::newControlConnection()
{
    // Get next connection
    QTcpSocket *controlSocket = controlServer->nextPendingConnection();

    QString ipAdress = controlSocket->peerAddress().toString();

    // Connect controlSocket to readyRead()-Signal to receive results from client
    connect(controlSocket, SIGNAL(readyRead()), this, SLOT(receiveKeepAliveResponse()));

    // lock Mutex to synchronise access of the clientList
    mutex->lock();

    // Check if client is already in list
    if(clientList.contains(ipAdress))
    {
        ClientInfo *client = clientList.value(ipAdress);
        client->controlSocket = controlSocket;
        client->i_finishedPasswordsCount = 0;
        client->runsSinceDate = QDate::currentDate();
        client->runsSinceTime = QTime::currentTime();
    }
    else
    {
        ClientInfo *client = new ClientInfo;
        client->controlSocket = controlSocket;
        client->status = Idle;
        client->b_waitingForResponse = false;
        client->i_finishedPasswordsCount = 0;
        client->lastBlock.clear();
        client->runsSinceDate = QDate::currentDate();
        client->runsSinceTime = QTime::currentTime();
        clientList.insert(ipAdress, client);
    }
    // unlock Mutex
    mutex->unlock();

    //log("ControlServer: Client connected from adress " + controlSocket->peerAddress().toString() + ".");
    //log("Now there are " + QString::number(clientList.size()) + " connected clients.");

    // unlock Mutex
    mutex->unlock();
}


// *******************************************************************************************
// receiveClientResult()
//
// Description: Receive result from a client via the connected dataSocket
// *******************************************************************************************
void ServerModel::receiveClientResult()
{
    // Get Socket where the data is beeing received
    QObject* sender = const_cast<QObject*>(QObject::sender());
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender);

    // Identify client by peerAddress
    ClientInfo *client = clientList.value(socket->peerAddress().toString());

    client->runEndDate = QDate::currentDate();
    client->runEndTime = QTime::currentTime();

    // Increase number of finishedPasswort per client and in total
    // Calculate size of last calculated block for progress obervation        
    QString S_pwBlock(client->lastBlock);
    QStringList pwBL = S_pwBlock.split('\n',QString::SkipEmptyParts);
    finishedPasswordsCount += pwBL.size();
    client->i_finishedPasswordsCount += pwBL.size();

    // Calculate passwords per Second
    QDateTime *startTime = new QDateTime(client->runsSinceDate, client->runsSinceTime);
    QDateTime *endTime = new QDateTime(client->runEndDate, client->runEndTime);

    int seconds = startTime->secsTo(*endTime);

    client->i_passwordsPerSecond = client->i_finishedPasswordsCount / seconds;

    // Receive pwFound
    if (client->pwFound == 0)
    {   
        client->dataSocket->read((char*)&client->pwFound, sizeof(quint64));

        if(client->pwFound)
        {
            log("\n\n###################################");
            log("Password found !!!");
        }
    }

    // Receive password if pwFound = true
    if(client->pwFound)
    {
        // Receive password size after pwFound has been received
        if (client->pwSize == 0)
        {
            client->dataSocket->read((char*)&client->pwSize, sizeof(quint64));
        }

        if(client->pwSize != 0 && client->password == "")
        {         
            // Check for bytesAvailable            
            if(client->dataSocket->bytesAvailable() < client->pwSize)
                return; // Wait
            else
            {                
                // Receive password
                client->password = QString(client->dataSocket->read(client->pwSize));
                log("--------------------------------------------------------");
                log("Password : " + client->password);
            }
        }
    }

    // Set client stats
    client->status = Idle;

    if(client->pwFound && client->password != "")
    {
        log("--------------------------------------------------------");
        log("Found by : " + socket->peerAddress().toString());
        log("--------------------------------------------------------");
        log("Writing result to outputFile.");
        log("Interrupting all clients and switching to next header file.");
        log("###################################");

        // Write result to file
        QFile outputFile(S_outputFile);
        outputFile.open(QIODevice::Append);
        QString S_result("The password for volume file '" + finishedHeaderFiles.last() + "' is: ");
        S_result.append(client->password + "\n");
        QByteArray BA_result;
        BA_result.append(S_result);
        outputFile.write(BA_result);
        outputFile.close();

        // Interrupt clients and start over with next header
        interruptClients();
        if(readNextHeader())
        {
            distributeDecryptionData();
        }
    }
    else
    {
        sendDecryptInfo(client->dataSocket);
    }
}


// *******************************************************************************************
// receiveKeepAliveResponse()
//
// Description: This function receives the keepAliveResponse from client
// *******************************************************************************************
void ServerModel::receiveKeepAliveResponse()
{
    // Get Socket where the data is beeing received
    QObject* sender = const_cast<QObject*>(QObject::sender());
    QTcpSocket* controlSocket = static_cast<QTcpSocket*>(sender);

    // Identify client by peerAddress
    ClientInfo *client = clientList.value(controlSocket->peerAddress().toString());

    // Receive control signal
    qint64 keepAliveSignal;
    controlSocket->read((char*)&keepAliveSignal, sizeof(qint64));

    client->b_waitingForResponse = false;
}


// *******************************************************************************************
// sendDecryptInfo()
//
// Description: Sends the decryption information to the client
// *******************************************************************************************
void ServerModel::sendDecryptInfo(QTcpSocket *socket)
{
        // Reset vatriables to enable receiving result from client
        ClientInfo *client = clientList.value(socket->peerAddress().toString());
        client->pwFound = 0;
        client->pwSize = 0;
        client->password = "";

        QByteArray pwBlock;
        // Get next passwordblock
        // Check if still passwordblock available
        if(!pwBlockList.isEmpty())
        {
            // Get next block
            pwBlock = pwBlockList.takeFirst();

            // ############## Transfer size of passwordlist ###############

            qint64 i = pwBlock.size();
            socket->write((const char*)&i, sizeof(qint64));

            // ############## Transfer header ###############
            socket->write(BA_volumeHeader);

            // ############## Transfer passwordlist ###############
            socket->write(pwBlock);

            // Set clients lastBlock
            client->lastBlock = pwBlock;

            // Set client to running
            client->status = Running;
        }
        else
        {
            log("Can't send new passwordblock to client " + client->dataSocket->peerAddress().toString() + ", \nall blocks have already been send.");
            log("Waiting for all clients to finish calculations.\n");
        }
}


// *******************************************************************************************
// interruptClients
//
// Description: sends interrupt-signal to all clients
// *******************************************************************************************
void ServerModel::interruptClients()
{
    // Close all sockets
    QMapIterator<QString, ClientInfo*> i(clientList);

    while(i.hasNext())
    {
        i.next();

        // Interrupt signal (8)
        qint64 interrupt = 8;
        // Send signal
        i.value()->controlSocket->write((const char*)&interrupt, sizeof(qint64));
    }
}


// *******************************************************************************************
// terminateClients
//
// Description: sends terminate-signal to all clients
// *******************************************************************************************
void ServerModel::terminateClients()
{
    // Close all sockets
    QMapIterator<QString, ClientInfo*> i(clientList);

    while(i.hasNext())
    {
        i.next();

        // Terminate signal (9)
        qint64 terminate = 9;
        // Send signal
        i.value()->controlSocket->write((const char*)&terminate, sizeof(qint64));
    }

    // Close Servers so new Clients get rejected
    if(dataServer != 0)
        controlServer->close();
    if(controlServer != 0)
        controlServer->close();
}


// *******************************************************************************************
// keepAliveCheck
//
// Description: Sends a keep alive check to the clients and checks if all clients are done
// *******************************************************************************************
void ServerModel::keepAliveCheck()
{
    // Check if at least 1 client available
    if(!clientList.isEmpty())
    {
        // Close all sockets
        QMapIterator<QString, ClientInfo*> i(clientList);

        // Variable to check if all clients are finished
        bool b_allClientsFinished = true;

        while(i.hasNext())
        {
            i.next();

            ClientInfo *client = i.value();

            // Check if server is still waiting for a keep-alive response
            if(client->b_waitingForResponse)
            {
                // Check if client was registered as "Running"
                if(client->status == Running)
                {
                    // Put current client block back on pwBlickList
                    pwBlockList.append(client->lastBlock);
                }

                if(client->status != Not_Responding)
                {
                    // Set client as Not_Responding
                    client->status = Not_Responding;
                    // Set bool
                    b_allClientsFinished = false;
                }
            }

            // Keep alive request signal (5)
            qint64 stillAlive = 5;
            // Send signal
            client->controlSocket->write((const char*)&stillAlive, sizeof(qint64));

            // Set waitingForResponse
            client->b_waitingForResponse = true;

            // Check if client is still running
            if(client->status == Running)
                b_allClientsFinished = false;
        }

        // Check if still
        if(!b_allClientsFinished)
        {
            distributeDecryptionData();
        }
        else if(pwBlockList.empty()) // Make sure all PW Blocks have been processed
        {
            log("All clients are finished and no password has been found \nfor header file so far, switching to next password file.\n");
            readNextPasswordFile();
            distributeDecryptionData();
        }
    }
}


// *******************************************************************************************
// isDoneLoading()
//
// Description: Returns b_doneLoading
// *******************************************************************************************
bool ServerModel::isDoneLoading()
{
    return (this->b_doneHeaderLoading && this->b_donePasswordLoading);
}

// *******************************************************************************************
// resetClientStats()
//
// Description: Reset client stats
// *******************************************************************************************
void ServerModel::resetClientStats()
{
    // Close all sockets
    QMapIterator<QString, ClientInfo*> i(clientList);

    while(i.hasNext())
    {
        i.next();
        ClientInfo *client = i.value();

        client->i_finishedPasswordsCount = 0;
        client->runsSinceDate = QDate::currentDate();
        client->runsSinceTime = QTime::currentTime();
    }
}

