

// *******************************************************************************************
// Project Includes
// *******************************************************************************************

#include "ServerView.h"
#include "ui_ServerView.h"
#include "aboutwindow.h"


// *******************************************************************************************
// Constructor
//
// Description: Creates ServerView and initializes some variables/signals
// *******************************************************************************************
ServerView::ServerView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerView)
{
    // Start UI
    ui->setupUi(this);
}


// *******************************************************************************************
// Destructor
//
// Description: Destroys ServerView instance
// *******************************************************************************************
ServerView::~ServerView()
{
    delete ui;
}


// *******************************************************************************************
// initialize
//
// Description: Initializes the server
// *******************************************************************************************
void ServerView::initialize(int i_controlPort, int i_dataPort, QString S_headerFolder,
                                     QString S_passwordFolder, QString S_outputFile, int i_blockSize)
{
    // Initialisation of server
    serverModel = new ServerModel();
    // Set first start bool
    b_firstStart = true;

    // Set Connection for log information
    connect(serverModel, SIGNAL(logInfo(QString)), this, SLOT(addLogInformation(QString)));
    // Set Connection for resetButtons
    connect(serverModel, SIGNAL(signal_resetButtons()), this, SLOT(slot_resetButtons()));

    // Set connection for select-Buttons
    connect(ui->SelectVolumeFilePushButton, SIGNAL(clicked()), this, SLOT(selectVolumeFilePushButton_clicked()));
    connect(ui->SelectPasswordListFilePushButton, SIGNAL(clicked()), this, SLOT(selectPasswordListFilePushButton_clicked()));


    // Set some Gui Variables
    QStringList headerLabels;
    headerLabels << "IP Address"<< "Finished" << "Runs since" << "Passwords per sec" << "Status";
    ui->ClientsTableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->ControlPortSpinBox->setValue(i_controlPort);
    ui->DataPortSpinBox->setValue(i_dataPort);
    ui->PasswordBlockSizeSpinBox->setValue(i_blockSize);

    serverModel->setPasswordBlockSize(i_blockSize);
    serverModel->setHeaderFolder(S_headerFolder);
    serverModel->setPasswordFolder(S_passwordFolder);
    serverModel->setOutputFile(S_outputFile);

    finishedPasswordsCount = 0;

    ui->PasswordsProcessedProgressBar->setValue(0);
    ui->LogListWidget->clear();

    // Initialize GUI Timer
    timerGUI = new QTimer(this);
    connect(timerGUI, SIGNAL(timeout()), this, SLOT(updateGUIData()));

    // Read header
    serverModel->readNextHeader(true); // True -> call readNextPasswordfile

    // Set password progress bar
    allPasswordsCount = serverModel->getPasswordCount();
    ui->PasswordsAllLabel->setText("Passwords: " + QString::number(allPasswordsCount, 'f', 0));
}


// *******************************************************************************************
// updateGUIData()
//
// Description: Updates the GUI data by getting the actual information from the ServerModel
//              and refreshes the GUI
// *******************************************************************************************
void ServerView::updateGUIData()
{    
    // Get finished passworts
    finishedPasswordsCount = serverModel->getFinishedPasswordCount();
    // Get password count
    allPasswordsCount = serverModel->getPasswordCount();


    // Refresh labels
    ui->PasswordsAllLabel->setText("Passwords: " + QString::number(allPasswordsCount, 'f', 0));
    ui->PasswordsFinishedLabel->setText("Finished: " + QString::number(finishedPasswordsCount, 'f', 0));

    // Set progressbar
    if (allPasswordsCount != 0)
    {
        ui->PasswordsProcessedProgressBar->setValue((finishedPasswordsCount / allPasswordsCount) * 100);
    }

    // Update client list (especially the passwordsFinished Label)
    QMap<QString, ClientInfo*> clientList;

    clientList = serverModel->getClientList();

    QMapIterator<QString, ClientInfo*> i(clientList);

    ui->ClientsTableWidget->setRowCount(clientList.size());

    int counter = 0;

    //ui->ClientsTableWidget->clear();

    //QStringList headerLabels;
    //headerLabels << "IP Address"<< "Finished" << "Runs since" << "Passwords per sec" << "Status";
    //ui->ClientsTableWidget->setHorizontalHeaderLabels(headerLabels);

    while(i.hasNext())
    {
        i.next();

        //ui->ClientsTableWidget->insertRow(counter);
        QLabel *ip = new QLabel(i.key());
        QLabel *passwordsFinished = new QLabel(QString::number(i.value()->i_finishedPasswordsCount));
        QLabel *runsSince = new QLabel(i.value()->runsSinceDate.toString() + " - " + i.value()->runsSinceTime.toString());


        QDateTime *start;
        QDateTime *end;

        if (i.value()->status == Running)
        {
            start = new QDateTime(i.value()->runsSinceDate, i.value()->runsSinceTime);
            end = new QDateTime(QDate::currentDate(), QTime::currentTime());
        }
        else
        {
            start = new QDateTime(i.value()->runsSinceDate, i.value()->runsSinceTime);
            end = new QDateTime(i.value()->runEndDate, i.value()->runEndTime);
        }


        //QLabel *runsFor = new QLabel(QString::number(seconds) + " seconds");
        QLabel *pwdPerSec = new QLabel(QString::number(i.value()->i_passwordsPerSecond) + " Pwd/sec");

        QLabel *status = new QLabel(QString((i.value()->status == Running) ? "Running" : (i.value()->status == Idle) ? "Idle" : "Not_Responding"));        

        ui->ClientsTableWidget->setCellWidget(counter, 0, ip);
        ui->ClientsTableWidget->setCellWidget(counter, 1, passwordsFinished);
        ui->ClientsTableWidget->setCellWidget(counter, 2, runsSince);
        ui->ClientsTableWidget->setCellWidget(counter, 3, pwdPerSec);
        ui->ClientsTableWidget->setCellWidget(counter, 4, status);

        // Increase row counter
        counter++;
    }
}


// *******************************************************************************************
// selectPasswordListFilePushButton_clicked
//
// Description: Triggers open file dialog and reads passwordList from file
// *******************************************************************************************
void ServerView::selectPasswordListFilePushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,
         "Select Password List Folder", QDir::currentPath(), QFileDialog::ShowDirsOnly);

    if(path != "")
    {
        // Set Blocksize
        serverModel->setPasswordBlockSize(ui->PasswordBlockSizeSpinBox->value());
        // Set and read passwordfolder
        serverModel->setPasswordFolder(path);
        serverModel->readNextPasswordFile();
        allPasswordsCount = serverModel->getPasswordCount();
        ui->PasswordsAllLabel->setText("Passwords: " + QString::number(allPasswordsCount, 'f', 0));
    }
}


// *******************************************************************************************
// selectVolumeFilePushButton_clicked
//
// Description: Triggers open file dialog and reads volume header from file
// *******************************************************************************************
void ServerView::selectVolumeFilePushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,
         "Select Header Folder", QDir::currentPath() , QFileDialog::ShowDirsOnly);

    if(path != "")
    {
        serverModel->setHeaderFolder(path);
        serverModel->readNextHeader(false); // false = don't call readNextPasswordFile
    }
}


// *******************************************************************************************
// addLogInformation
//
// Description: Adds string to LogListWidget (Console-like)
// *******************************************************************************************
void ServerView::addLogInformation(QString logInformation)
{
    ui->LogListWidget->addItem(logInformation);
    ui->LogListWidget->scrollToBottom();
}


// *******************************************************************************************
// newClientConnected
//
// Description: Gets triggered when a new Client connected to the Server
//              Updates the GUI with client information
// *******************************************************************************************
void ServerView::newClientConnected()
{
    QMap<QString, ClientInfo*> clientList;

    clientList = serverModel->getClientList();

    QMapIterator<QString, ClientInfo*> i(clientList);

    int counter = 0;

    while(i.hasNext())
    {
        counter++;
        i.next();        

        ui->ClientsTableWidget->insertRow(counter-1);
        QLabel *ip = new QLabel(i.value()->dataSocket->peerAddress().toString());
        QLabel *passwordsFinished = new QLabel("0");
        QLabel *gpus = new QLabel("???");

        ui->ClientsTableWidget->setCellWidget(counter-1, 0, ip);
        ui->ClientsTableWidget->setCellWidget(counter-1, 1, passwordsFinished);
        ui->ClientsTableWidget->setCellWidget(counter-1, 2, gpus);
    }
}


// *******************************************************************************************
// on_StartServerPushButton_clicked
//
// Description: Initializes the server, setting up the QTcpServers
// *******************************************************************************************
void ServerView::on_StartServerPushButton_clicked()
{    
    if(serverModel->isDoneLoading())
    {
        // Reset client stats
        serverModel->resetClientStats();

        // Disable select- and start-Buttons
        ui->StartServerPushButton->setDisabled(true);
        ui->SelectPasswordListFilePushButton->setDisabled(true);
        ui->SelectVolumeFilePushButton->setDisabled(true);
        ui->ControlPortSpinBox->setDisabled(true);
        ui->DataPortSpinBox->setDisabled(true);
        ui->PasswordBlockSizeSpinBox->setDisabled(true);

        // Set Blocksize
        serverModel->setPasswordBlockSize(ui->PasswordBlockSizeSpinBox->value());
        // Set ports
        serverModel->setControlPort(ui->ControlPortSpinBox->value());
        serverModel->setDataPort(ui->DataPortSpinBox->value());

        // Initialize and start server
        if(b_firstStart)
        {
            serverModel->initialize(b_firstStart);
            b_firstStart = false;
        }
        else
            serverModel->initialize(b_firstStart);


        // Start GUI-Timer
        timerGUI->start(1000);
    }
    else
    {
        addLogInformation("Server isn't done loading header and passwordlist.");
    }
}


// *******************************************************************************************
// on_QuitPushButton_clicked
//
// Description: Terminates all clients and closes the program
// *******************************************************************************************
void ServerView::on_QuitPushButton_clicked()
{
    serverModel->terminateClients();
    ServerView::close();
}


// *******************************************************************************************
// on_actionQuit_triggered
//
// Description: (Menu Button Quit) Terminates all clients and closes the program
// *******************************************************************************************
void ServerView::on_actionQuit_triggered()
{
    serverModel->terminateClients();
    ServerView::close();
}


// *******************************************************************************************
// on_actionAbout_triggered
//
// Description: (Menu Button About) Shows the About-Window
// *******************************************************************************************
void ServerView::on_actionAbout_triggered()
{
    AboutWindow *aboutWindow = new AboutWindow();

    aboutWindow->show();
}


// *******************************************************************************************
// slot_resetButtons
//
// Description: Terminates all clients and closes the program
// *******************************************************************************************
void ServerView::slot_resetButtons()
{
    // Enable GUI elements
    ui->StartServerPushButton->setDisabled(false);
    ui->SelectPasswordListFilePushButton->setDisabled(false);
    ui->SelectVolumeFilePushButton->setDisabled(false);
    ui->ControlPortSpinBox->setDisabled(false);
    ui->DataPortSpinBox->setDisabled(false);
    ui->PasswordBlockSizeSpinBox->setDisabled(false);
}
