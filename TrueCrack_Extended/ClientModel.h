

#ifndef CLIENTMODEL_H_
#define CLIENTMODEL_H_

// *******************************************************************************************
// QT Includes
// *******************************************************************************************

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QProcess>
#include <QHostAddress>


// *******************************************************************************************
// Struct Definitions
// *******************************************************************************************

// Struct for decryption result
struct DC_Result
{
    bool pwFound;           // Password found?
    QString password;       // If yes, what is the password?
};


// *******************************************************************************************
// Class:			ClientModel
//
// Description:		The ClientModel-Class represents the model of the client. It provides
//                  a bunch of methods for the server-client communication and the
//                  processing of received data (password lists and header files)
// *******************************************************************************************
class ClientModel : public QObject
{

	Q_OBJECT

public:

	ClientModel();
	virtual ~ClientModel();

    bool initialize();
    void startHeaderDecryption();

    void setControlPort(int controlPort);
    void setDataPort(int dataTransferPort);

public slots:

	void receiveDecryptInfo();
    void receiveControls();
    void slotDataConnected();
    void slotControlConnected();    
    void getDecryptionResult();
    void setServerAddress(QHostAddress ipAddress);

private:        

    DC_Result *DC_result;                   // Struct holding the result of decryption

    QHostAddress serverAddress;             // QHostAddress of the server

    int i_dataPort;                         // Server Ports
    int i_controlPort;

    QTcpSocket *dataSocket;                 // Sockets
    QTcpSocket *controlSocket;

    QProcess *tcProcess;                    // Process for calling TrueCrack

    QByteArray BA_volumeHeader;             // ByteArray to store received data
    QByteArray BA_passwords;                // ByteArray to store passwords

    QFile *F_volumeHeader;                  // File to store Header
    QFile *F_passwords;                     // File to store password list


    int i_packageCounter;                   // Package counter

    bool b_infoDone;                        // Variables to control behavior when client gets data
    bool b_headerDone;
    bool b_passwordsDone;

    QString S_truecrackProgram;             // Variables containing path to the TrueCrack binary, the header and passwordlist filename
    QString S_headerPath;
    QString S_passwordListPath;

    QString S_blockSize;                    // Blocksize for decryption, number of parallel blocks calculated by GPU

    qint64 sizePWList;                      // Stores size of password list



    void sendDecryptionResult();            // Method

};

#endif /* CLIENTMODEL_H_ */
