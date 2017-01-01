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

#include "printer.h"
#include "log.h"

#include <iostream>

//---------------------------------------------------------------------------

Server::Server(Printer* printer)
	: _printer(printer)
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
		connect(socket, &QIODevice::readyRead, this, &Server::onSocketRecv);
		connect(socket, &QAbstractSocket::disconnected, this, &Server::onSocketDisconnect);
	}
}

void Server::onSocketRecv()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	Request& request = _requests[socket->socketDescriptor()];
	if(request.BytesRemaining == 0
	&& request.Content.length() == 0
	&& !readHeader(socket, socket->bytesAvailable())){
		Log::log("Invalid header received.", LL_ERROR);
		_requests.remove(socket->socketDescriptor());
		socket->abort();
		socket->deleteLater();
		return;
	}

	qint64 size;
	while(size = socket->bytesAvailable()){
		// Read available bytes up to 1KB
		char* buffer = new char[qMin(size, (qint64)1024)];
		qint64 result = socket->read(buffer, size);
		if(result < 0){
			Log::log(socket->errorString(), LL_ERROR);
			_requests.remove(socket->socketDescriptor());
			socket->abort();
			socket->deleteLater();
			return;
		}

		// If we've received more content in this request than promised: esplode
		if((qint64)request.BytesRemaining - result < 0){
			Log::log("Invalid content size detected, dropping connection.", LL_ERROR);
			_requests.remove(socket->socketDescriptor());
			socket->abort();
			socket->deleteLater();
			return;
		}

		// All clear, let's update the request
		request.BytesRemaining -= result;
		request.Content.append(buffer, result);

		// If we've received all the request's content, send it for printing
		if(request.BytesRemaining == 0){
			switch(request.Type){
				case RT_HTML:
					_printer->renderHtml(request.Content);
					break;
				case RT_JSON:
					_printer->renderFromJson(request.Content);
					break;
			}
			_requests.remove(socket->socketDescriptor());
		}
	}
}

void Server::onSocketDisconnect()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	_requests.remove(socket->socketDescriptor());
	socket->deleteLater();
}

//---------------------------------------------------------------------------

bool Server::readHeader(QTcpSocket* socket, qint64 bytesAvailable)
{
	if(bytesAvailable >= 12){
		char* buffer = new char[8];
		qint64 result = socket->read(buffer, 8);
		if(result < 0){
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

			request.Content.reserve(size);
			_requests[socket->socketDescriptor()] = request;
			return true;
		}
	}
	return false;
}
