

// *******************************************************************************************
// Project Includes
// *******************************************************************************************

#include "ClientModel.h"


// *******************************************************************************************
// QT Includes
// *******************************************************************************************

#include <QByteArray>
#include <QString>
#include <QHostAddress>


// *******************************************************************************************
// C++ Includes
// *******************************************************************************************

#include <stdlib.h>


// *******************************************************************************************
// Constructor
//
// Description: Constructor creates the TcpSocket-Object and does the required QT
//				connections (### evtl. auslagern in eigene Methode ???)
// *******************************************************************************************
ClientModel::ClientModel()
{
}


// *******************************************************************************************
// Destructor
//
// Description: ###
// *******************************************************************************************
ClientModel::~ClientModel()
{
}


///////////////////////////////////// SETTER METHODS /////////////////////////////////////////

// *******************************************************************************************
// setServerAddress
//
// Description: Sets the IP-address of the server
// *******************************************************************************************
void ClientModel::setServerAddress(QHostAddress ipAddress)
{
    this->serverAddress = ipAddress;
}


// *******************************************************************************************
// setControlPort
//
// Description: Sets control port
// *******************************************************************************************
void ClientModel::setControlPort(int controlPort)
{
    this->i_controlPort = controlPort;
}


// *******************************************************************************************
// setDataTransferPort
//
// Description: Sets data transfer port
// *******************************************************************************************
void ClientModel::setDataPort(int dataTransferPort)
{
    this->i_dataPort = dataTransferPort;
}


// *******************************************************************************************
// initialize()
//
// Description: initialize connections
//              returns true if successfull, else false
// *******************************************************************************************
bool ClientModel::initialize()
{
    // Initialize result struct
    DC_result = new DC_Result;

    // Initialize package counter
    i_packageCounter = 0;

    // Set path variables execution of TrueCrack
    S_truecrackProgram = "TrueCrack/./truecrack";
    S_headerPath = "header";
    S_passwordListPath = "passwordList";
    QObject *parent = new QObject();
    tcProcess = new QProcess(parent);

    connect(tcProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(getDecryptionResult()));

    // Set Blocksize
    S_blockSize = "1024";

    // Initialize sockets
    dataSocket = new QTcpSocket(this);
    controlSocket = new QTcpSocket(this);

    // Setup dataSocket
    connect(dataSocket, SIGNAL(connected()), this, SLOT(slotDataConnected()));
    connect(dataSocket, SIGNAL(readyRead()), this, SLOT(receiveDecryptInfo()));

    // Connect the dataSocket to receive TrueCrack data
    dataSocket->connectToHost(serverAddress, i_dataPort);
    // Check if connection successfull
    if(!dataSocket->waitForConnected(1000))
        return false; // Timeout

    // Setup controlSocket
    connect(controlSocket, SIGNAL(connected()), this, SLOT(slotControlConnected()));
    connect(controlSocket, SIGNAL(readyRead()), this, SLOT(receiveControls()));

    // Connect the controlSocket to receive commands or send result to server
    controlSocket->connectToHost(serverAddress, i_controlPort);
    // Check if connection successfull
    if(!controlSocket->waitForConnected(1000))
        return false; // Timeout

    // Initialize variables needed to receive decrypt info
    b_infoDone = false;
    b_headerDone = false;
    b_passwordsDone = false;

    // All connections established
    return true;
}


// *******************************************************************************************
// slotDataConnected()
//
// Description: This function is called when the client is successfully connected
//				to a server
// *******************************************************************************************
void ClientModel::slotDataConnected()
{
    qDebug() << "Connected to Data-Server.";
}


// *******************************************************************************************
// slotControlConnected()
//
// Description: This function is called when the client is successfully connected
//				to a server
// *******************************************************************************************
void ClientModel::slotControlConnected()
{
    qDebug() << "Connected to Control-Server.";
}


// *******************************************************************************************
// receiveDecryptInfo()
//
// Description: This function is called when new data is available for reading
//				from the dataServer
// *******************************************************************************************
void ClientModel::receiveDecryptInfo()
{
    // Receive size of password list
    if (!b_infoDone)
    {
        dataSocket->read((char*)&sizePWList, sizeof(qint64));        
        b_infoDone = true;
    }

    // If header not yet received and password size got received
    if(b_infoDone && !b_headerDone)
    {
        // Check if the header is complete
        if(dataSocket->bytesAvailable() < 1024)
            return; // Wait
        else
        {
            // Read header
            QByteArray header;
            header = dataSocket->read(1024);

            // Write header to file
            QFile headerFile(S_headerPath);
            headerFile.open(QIODevice::WriteOnly);
            headerFile.write(header);
            headerFile.close();

            // Header received
            b_headerDone = true;
        }
    }

    // Only after sizePWList got received
    if(b_infoDone && !b_passwordsDone)
    {
        QByteArray passwords;

        // Check if passwords are complete
        if(dataSocket->bytesAvailable() < sizePWList)
            return; // Wait
        else
        {
            // Increase packageCounter
            i_packageCounter++;

            // Read passwords
            passwords = dataSocket->read(sizePWList);

            // Write passwords to file
            QFile passwordFile(S_passwordListPath);
            passwordFile.open(QIODevice::WriteOnly);
            passwordFile.write(passwords);
            passwordFile.close();

            // Get number of passwords in this block
            QString S_pwBlock(passwords);
            QStringList pwBL = S_pwBlock.split('\n',QString::SkipEmptyParts);
            S_blockSize = QString::number(pwBL.size());

            // Passwords received
            b_passwordsDone = true;
        }
    }

    // If all data got received, start header decryption
    if(b_infoDone && b_headerDone && b_passwordsDone)
    {
        // Make sure this is only called once and client can receive new data
        b_infoDone = false;
        b_headerDone = false;
        b_passwordsDone = false;

        qDebug() << "Received decryption data package no. " << i_packageCounter << ", starting decryption...";

        startHeaderDecryption();
    }
}


// *******************************************************************************************
// sendDecryptionResult()
//
// Description: This function sends the result back to the server
// *******************************************************************************************
void ClientModel::sendDecryptionResult()
{    
    // Send bool DC_result->pwFound as qint64
    qint64 i_found = DC_result->pwFound;
    dataSocket->write((const char*)&i_found, sizeof(qint64));

    // If pwFound, send password too
    if(i_found)
    {
        // Get password size and send over to server
        QByteArray BA_password;
        BA_password.append(DC_result->password);
        qint64 pwSize = BA_password.size();
        dataSocket->write((const char*)&pwSize, sizeof(qint64));

        // Send password itself
        dataSocket->write(BA_password);
    }
}


// *******************************************************************************************
// startHeaderDecryption()
//
// Description: This function calls truecrack with the header and passwordlist
// *******************************************************************************************
void ClientModel::startHeaderDecryption()
{
    // If process still running, kill it
    if(tcProcess->state() == tcProcess->Running)
    {
        tcProcess->terminate();
        tcProcess->waitForFinished();
    }

    // Build arguments
    QStringList args;

    args << "-t" << S_headerPath;
    args << "-w" << S_passwordListPath;
    args << "-b" << S_blockSize;

    tcProcess->start(S_truecrackProgram, args);
}


// *******************************************************************************************
// getDecryptionResult()
//
// Description: This function extracts the result of TrueCrack
// *******************************************************************************************
void ClientModel::getDecryptionResult()
{    
    // Set readchannel to StandardOutput
    tcProcess->setReadChannel(QProcess::StandardOutput);

    QByteArray BA_output = tcProcess->readAll();

    QString S_output(BA_output.data());
    QRegExp regex("Password\\[\\d+\\]: (\\S+)");    

    if(regex.indexIn(S_output) != -1)
    {   // Password found
        DC_result->pwFound = true;
        DC_result->password = regex.cap(1);
    }
    else
    {   // Password not found
        DC_result->pwFound = false;
        DC_result->password = "";
    }

    if(DC_result->pwFound)
        qDebug() << "Password for TrueCrypt Volume found: " << DC_result->password;
    else
        qDebug() << "Password for TrueCrypt Volume not found.";

    qDebug() << "Sending result to server...";

    // Terminate Process and go idle
    tcProcess->terminate();

    sendDecryptionResult();
}


// *******************************************************************************************
// receiveControls()
//
// Description: This function is called when new data is available for reading
//				from the controlServer
// *******************************************************************************************
void ClientModel::receiveControls()
{
    // Receive control signal
    qint64 controlCommand;

    controlSocket->read((char*)&controlCommand, sizeof(qint64));

    // Initialize response
    qint64 aliveSignal = 1;

    switch(controlCommand)
    {
    case 5:
        // Received Keep-Alive Request
        // Send response (1)
        controlSocket->write((const char*)&aliveSignal, sizeof(qint64));
        break;

    case 8:
        qDebug() << "Received interrupt signal from server.";
        qDebug() << "Killing decryption process and resetting stats.";

        // If process is running, kill
        if(tcProcess->state() == QProcess::Running)
        {
            tcProcess->kill();
        }

        // Reset stats
        i_packageCounter = 0;
        b_infoDone = false;
        b_headerDone = false;
        b_passwordsDone = false;
        break;

    case 9:
        qDebug() << "Received terminate signal, server shutting down.";
        qDebug() << "Killing decryption process and exiting client.";

        // If process is running, kill
        if(tcProcess->state() == QProcess::Running)
        {
            tcProcess->kill();
        }

        exit(1);
        break;

    default:
        break;
    }
}








