


// *******************************************************************************************
// Project Includes
// *******************************************************************************************

#include "ClientView.h"


// *******************************************************************************************
// Using Directives
// *******************************************************************************************

using namespace std;


// *******************************************************************************************
// Constructor
//
// Description: ###
// *******************************************************************************************
ClientView::ClientView()
{
	// TODO Auto-generated constructor stub
}


// *******************************************************************************************
// Destructor
//
// Description: ###
// *******************************************************************************************
ClientView::~ClientView()
{
	// TODO Auto-generated destructor stub
}


// *******************************************************************************************
// start()
//
// Description: This is the main-Method of the Server-Application. All initialization stuff
//				is placed here
// *******************************************************************************************
void ClientView::start(QString ipAdress, int controlPort, int dataPort)
{
	// Show the welcome message
	showWelcomeMessage();

    // QTextStream and QString to read from console
    QTextStream stream(stdin);
    QString line;

	// Create the ClientModel object to get access to the model methods
	clientModel = new ClientModel();

    if(!(ipAdress != "" && controlPort != 0 && dataPort != 0))
    {
        qDebug() << "Please enter the server IP-address:";

        // Read IP-adress
        do {
            line = stream.readLine();
        }while (line.isEmpty());

        // Convert to QHostAddress
        QHostAddress serverAddress(line);
        clientModel->setServerAddress(serverAddress);

        qDebug() << "Please enter the server control port:";

        // Read control-port
        do {
            line = stream.readLine();
        }while (line.isEmpty());

        clientModel->setControlPort(line.toInt());

        qDebug() << "Please enter the server data transfer port:";

        // Read data transfer-port
        do {
            line = stream.readLine();
        }while (line.isEmpty());

        clientModel->setDataPort(line.toInt());
    }
    else
    {
        QHostAddress serverAddress(ipAdress);
        clientModel->setServerAddress(serverAddress);
        clientModel->setControlPort(controlPort);
        clientModel->setDataPort(dataPort);
    }


    if(clientModel->initialize())
    {
        qDebug() << "Client initialized and successfully connected to server!";
    }
    else
    {
        qDebug() << "Client couldn't connect to server, please check your input and try again.";
        // Exit program
        exit(0);
    }
}


// *******************************************************************************************
// ShowWelcomeMessage()
//
// Description: Shows a simple welcome message to show the user which app-mode is activ
// *******************************************************************************************
void ClientView::showWelcomeMessage()
{
    qDebug() << "You have started the TrueCrack Extended client.\n";
}
