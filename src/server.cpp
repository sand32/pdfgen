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

#include "server.h"

#include "config.h"
#include "log.h"
#include "printer.h"

//---------------------------------------------------------------------------

Server::Server(Printer* printer, const Config& config)
	: _printer(printer),
	  _config(config)
{
	connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server()
{
}

//---------------------------------------------------------------------------

void Server::onNewConnection()
{
	QTcpSocket* socket;
	while(socket = nextPendingConnection()){
		_clients[socketID(socket)].Socket = socket;

		connect(socket, &QIODevice::readyRead, this, &Server::onSocketRecv);
		connect(socket, &QAbstractSocket::disconnected, this, &Server::onSocketDisconnect);

		QTimer* timer = new QTimer(socket);
		connect(timer, &QTimer::timeout, [socket](){
			Log::log("Connection timeout");
			socket->abort();
		});
		timer->start(_config.connectionTimeoutMS());
		_clients[socketID(socket)].Timer = timer;
	}
}

void Server::onSocketRecv()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	// Pause the connection timeout for the duration of recv
	_clients[socketID(socket)].Timer->stop();

	Request& request = _clients[socketID(socket)].CurrentRequest;
	if(request.BytesRemaining == 0
	&& request.Content.length() == 0
	&& !readHeader(socket, socket->bytesAvailable())){
		Log::log("Invalid header received.", LL_ERROR);
		socket->abort();
		return;
	}

	qint64 size;
	while(size = socket->bytesAvailable()){
		// Read available bytes up to 1KB
		char* buffer = new char[qMin(size, (qint64)1024)];
		qint64 result = socket->read(buffer, size);
		if(result < 0){
			Log::log("Socket error: " + socket->errorString(), LL_ERROR);
			socket->abort();
			return;
		}

		// If we've received more content in this request than promised: esplode
		if(static_cast<qint64>(request.BytesRemaining) - result < 0){
			Log::log("Invalid content size detected, dropping connection.", LL_ERROR);
			socket->abort();
			return;
		}

		// All clear, let's update the request
		request.BytesRemaining -= result;
		request.Content.append(buffer, result);
	}

	// If we've received all the request's content, send it for printing
	if(request.BytesRemaining == 0){
		switch(request.Type){
			case RT_HTML:
				_printer->renderHtml(request.Content, [this](const QByteArray& result, const QString& socketID){
					// QFile file("./temp.pdf");
					// file.open(QIODevice::WriteOnly);
					// file.write(result);
					// file.close();
					if(_clients.count(socketID)){
						QTcpSocket* socket = _clients[socketID].Socket;
						bool success = socket->write(result);
						Log::log(QString::number(success));
						socket->flush();
					}else{
						Log::log("Socket closed, cannot respond.");
					}
				}, socketID(socket));
				break;
			case RT_JSON:
				_printer->renderFromJson(request.Content);
				break;
		}
		socket->close();
	}else{
		// Restart the connection timeout
		_clients[socketID(socket)].Timer->start(_config.connectionTimeoutMS());
	}
}

void Server::onSocketDisconnect()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	_clients.remove(socketID(socket));
	socket->deleteLater();
}

//---------------------------------------------------------------------------

bool Server::readHeader(QTcpSocket* socket, qint64 bytesAvailable)
{
	const int headerSize = 12;
	if(bytesAvailable >= headerSize){
		char* buffer = new char[headerSize];
		qint64 result = socket->read(buffer, headerSize);
		if(result != 12){
			Log::log("bad header size");
			return false;
		}

		if(buffer[0] == 'P'
		&& buffer[1] == 'D'
		&& buffer[2] == 'F'
		&& buffer[3] == 'G'){
			quint32 type = static_cast<quint32>(buffer[4]);
			quint32 size = static_cast<quint32>(buffer[8]);
			Request request;
			request.BytesRemaining = size;
			request.Type = static_cast<RequestType>(type);
			if(request.Type != RT_HTML
			&& request.Type != RT_JSON){
				return false;
			}
			if(size > 4000000){
				Log::log("Unwilling to accomodate payload size of " + QString::number(size));
				return false;
			}

			request.Content.reserve(size);
			_clients[socketID(socket)].CurrentRequest = request;
			return true;
		}
	}
	return false;
}

QString Server::socketID(QTcpSocket* socket) const
{
	return socket->peerAddress().toString() + QString::number(socket->peerPort());
}
