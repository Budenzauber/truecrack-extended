


// *******************************************************************************************
// QT Includes
// *******************************************************************************************

#include <QApplication>

#include <QProcess>
#include <QDebug>

#include <QStringList>

#include <QByteArray>
#include <QString>
#include <QTextStream>


// *******************************************************************************************
// C++ Includes
// *******************************************************************************************

#include <c++/4.4/iostream>
#include <stdio.h>


// *******************************************************************************************
// Using Directives
// *******************************************************************************************

using namespace std;


// *******************************************************************************************
// Project Includes
// *******************************************************************************************
#include "ClientView.h"
#include "ServerView.h"


// *******************************************************************************************
// printHelp
//
// Description: Print general Help
// *******************************************************************************************
void printHelp()
{
    qDebug() << "---------- TrueCrack Extended General Help ----------";
    qDebug() << "You can use the following parameters: ";
    qDebug() << "-s \t\t Start TrueCrack Extended in Server-Mode";
    qDebug() << "-c \t\t Start TrueCrack Extended in Client-Mode";
    qDebug() << "\nFor further information on Client or Server try starting with '-c -h' or '-s -h'";
}

// *******************************************************************************************
// printHelpClient
//
// Description: Print Client Help
// *******************************************************************************************
void printHelpClient()
{
    qDebug() << "---------- TrueCrack Extended Client Help ----------";
    qDebug() << "You must use the following mandatory parameters in addition to -c: ";
    qDebug() << "-ip \t\t Parameter for the server ip-adress";
    qDebug() << "-cp \t\t Parameter for the server control-port";
    qDebug() << "-dp \t\t Parameter for the server data-port";
}

// *******************************************************************************************
// printHelpServer
//
// Description: Print Server Help
// *******************************************************************************************
void printHelpServer()
{
    qDebug() << "---------- TrueCrack Extended Server Help ----------";
    qDebug() << "You can use the following optional parameters in addition to -s: ";
    qDebug() << "-hf \t\t Parameter for the absolute Header-Folder-Path \n \t\t Standard: 'currentDir/HeaderFiles/'";
    qDebug() << "-pf \t\t Parameter for the absolute Password-Folder-Path \n \t\t Standard: 'currentDir/PasswordFiles/'";
    qDebug() << "-cp \t\t Parameter for the server control-port \n \t\t Standard: 2000";
    qDebug() << "-dp \t\t Parameter for the server data-port \n \t\t Standard: 2001";
    qDebug() << "-b \t\t Parameter for the server password blocksize \n \t\t Standard: 100000";
    qDebug() << "-o \t\t Parameter for the server output-File \n \t\t Standard: 'currentDir/result.txt'";
}


// *******************************************************************************************
// Main Method
//
// Description: Main Method
// *******************************************************************************************
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Fetch arguments passed to the application
    QStringList args = app.arguments();

    // Create a serverView and a clientView object. All further operations are
    // by these objects
    ClientView *clientView = new ClientView();

    ServerView *serverMainWindow;

    if (args.count() < 2) // No Arguments have been passed
    {
        //*** Dieser Fall wird obsolet, ist nur zum Testen noch da
        //*** Es sollte hinterher immer MINDESTENS -c oder -s als erster parameter übergeben werden


        // Input if user wants to start a server or a client
        cout << "\n\nWould you like to start a server or a client? [s|c]:" << endl;

        // Get the entered letter
        char i = getchar();

        if (i == 's')
        {
            // Call the entry method for server
            serverMainWindow = new ServerView();
            serverMainWindow->initialize();
            serverMainWindow->show();
        }
        else if(i == 'c')
        {
            // Call the entry method for client
            clientView->start();
        }
    }
    else // Console arguments have been passed
    {
        // Check if client- or server-mode
        if(args.at(1) == "-c")
        {
            QString ipAdress = 0;
            int controlPort = 0;
            int dataPort = 0;

            // Client
            // Parse arguments
            for(int i = 2; i < args.count(); i++)
            {
                if(args.at(i) == "-h")
                {
                    printHelpClient();
                    exit(1);
                }
                else if(args.at(i) == "-cp")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).at(0).isDigit())
                    {
                        controlPort = args.at(i).toInt();
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -cp! (Must be integer)";
                        exit(0);
                    }
                }
                else if(args.at(i) == "-dp")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).at(0).isDigit())
                    {
                        dataPort = args.at(i).toInt();
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -dp! (Must be integer)";
                        exit(0);
                    }
                }
                else if(args.at(i) == "-ip")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).at(0).isDigit())
                    {
                        ipAdress = args.at(i);
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -ip!";
                        exit(0);
                    }
                }
                else // Default
                {
                    qDebug() << "Unknown argument: " << args.at(i);
                    exit(0);
                }
            }

            qDebug() << "IP: " << ipAdress;
            qDebug() << "controlPort: " << controlPort;
            qDebug() << "dataPort: " << dataPort;

            if(ipAdress != 0 && controlPort != 0 && dataPort != 0)
            {
                // Call the entry method for client
                clientView->start(ipAdress, controlPort, dataPort);
            }
            else
            {
                qDebug() << "For the client all 3 arguments -ip, -cd and -dp are mandatory!";
                exit(0);
            }
        }
        else if(args.at(1) == "-s")
        {
            // Server
            // Arguments with standard values
            int controlPort = 2000;
            int dataPort = 2001;
            QString headerFolder = QDir::currentPath() + "/HeaderFiles/";
            QString passwordFolder = QDir::currentPath() + "/PasswordFiles/";
            QString outputFile = QDir::currentPath() + "/result.txt";
            int blockSize = 100000;

            // Parse arguments
            for(int i = 2; i < args.count(); i++)
            {
                if(args.at(i) == "-h")
                {
                    printHelpServer();
                    exit(1);
                }
                else if(args.at(i) == "-cp")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).at(0).isDigit())
                    {
                        controlPort = args.at(i).toInt();
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -cp! (Must be integer)";
                        exit(0);
                    }
                }
                else if(args.at(i) == "-dp")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).at(0).isDigit())
                    {
                        dataPort = args.at(i).toInt();
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -dp! (Must be integer)";
                        exit(0);
                    }
                }
                else if(args.at(i) == "-pf")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).startsWith('/'))
                    {
                        passwordFolder = args.at(i);
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -pf! (Must start with '/')";
                        exit(0);
                    }
                }
                else if(args.at(i) == "-hf")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).startsWith('/'))
                    {
                        headerFolder = args.at(i);
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -hf! (Must start with '/')";
                        exit(0);
                    }
                }
                else if(args.at(i) == "-o")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).startsWith('/'))
                    {
                        outputFile = args.at(i);
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -o! (Must start with '/')";
                        exit(0);
                    }
                }
                else if(args.at(i) == "-b")
                {
                    // Increase Counter
                    i++;
                    if(i < args.count() && args.at(i).at(0).isDigit())
                    {
                        blockSize = args.at(i).toInt();
                    }
                    else
                    {
                        qDebug() << "Missing value for argument -b! (Must be integer)";
                        exit(0);
                    }
                }
                else
                {
                    qDebug() << "Unknown argument: " << args.at(i);
                    exit(0);
                }
            }

            // Call the entry method for server
            serverMainWindow = new ServerView();
            serverMainWindow->initialize(controlPort, dataPort, headerFolder, passwordFolder, outputFile, blockSize);
            serverMainWindow->show();
        }
        else if(args.at(1) == "-h")
        {
            printHelp();
            exit(0);
        }
        else
        {
            qDebug() << "First argument MUST be the mode! (Client -c or Server -s)";
            exit(0);
        }
    }

    return app.exec();
}

