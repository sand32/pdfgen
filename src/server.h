/*
-----------------------------------------------------------------------------
Copyright (c) 2016 Seth Anderson

This software is provided 'as-is', without any express or implied warranty. 
In no event will the authors be held liable for any damages arising from the 
use of this software.

Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it 
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not 
claim that you wrote the original software. If you use this software in a 
product, an acknowledgment in the product documentation would be appreciated 
but is not required.

2. Altered source versions must be plainly marked as such, and must not be 
misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
-----------------------------------------------------------------------------
*/

#ifndef _SERVER_H_
#define _SERVER_H_

#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include "client.h"

class Config;
class Printer;

class Server : public QTcpServer
{
	Q_OBJECT

	const Config& _config;
	Printer* _printer;
	QHash<QString, Client> _clients;
public:
	Server(Printer* printer, const Config& config);
	~Server();

private slots:
	void onNewConnection();
	void onSocketRecv();
	void onSocketDisconnect();

private:
	bool readHeader(QTcpSocket* socket, qint64 bytesAvailable);
	QString socketID(QTcpSocket* socket) const;
};

#endif // _SERVER_H_
