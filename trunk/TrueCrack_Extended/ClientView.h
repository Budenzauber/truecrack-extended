

#ifndef CLIENTVIEW_H_
#define CLIENTVIEW_H_

// *******************************************************************************************
// Project Includes
// *******************************************************************************************

#include "ClientModel.h"


// *******************************************************************************************
// C++ Includes
// *******************************************************************************************

#include <c++/4.4/iostream>


// *******************************************************************************************
// Class:			ClientView
//
// Description:		The ClientView class handles all furher user input and console output.
//					It controls all processes of the client application
// *******************************************************************************************
class ClientView
{

private:

	ClientModel *clientModel;

public:

	ClientView();
	virtual ~ClientView();

    void start(QString ipAdress = "", int controlPort = 0, int dataPort = 0);
	void showWelcomeMessage();

};

#endif /* CLIENTVIEW_H_ */
