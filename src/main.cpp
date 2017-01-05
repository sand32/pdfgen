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

#include <QtGui/QtGui>
#include <QtWebEngine/QtWebEngine>
#include "log.h"
#include "config.h"
#include "printer.h"
#include "server.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc = __argc;
	char** argv = __argv;
#else
int main(int argc, char* argv[])
{
#endif
	Config config;
	bool success = config.load("./config.json");
	if(!success){
		return 1;
	}

	QApplication app(argc, argv);
	QtWebEngine::initialize();

	Printer printer(config);
	Server server(&printer, config);
	server.listen(QHostAddress::Any, config.port());

	Log::log("Listening on port " + QString::number(config.port()));
	Log::log("-------------------------------------------------");
	Log::log("             Initialization Complete");
	Log::log("-------------------------------------------------");

	app.exec();

	return 0;
}
