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

#include "printer.h"

#include "log.h"
#include "config.h"

//---------------------------------------------------------------------------

Printer::Printer(const Config& config)
{
	connect(this, &QWebEnginePage::loadFinished, this, &Printer::onLoadFinished);
}

Printer::~Printer()
{
}

//---------------------------------------------------------------------------

void Printer::renderHtml(const QString& html)
{
	QFile file("./temp.txt");
	file.open(QIODevice::WriteOnly);
	file.write(html.toUtf8());
	file.close();
	//setHtml(html);
}

void Printer::renderFromJson(const QByteArray& json)
{
	QJsonDocument document = QJsonDocument::fromBinaryData(json);
	if(document.isNull() || !document.isObject()){
		Log::log("Invalid json received, aborting render.", LL_ERROR);
		return;
	}

	QJsonObject object = document.object();
}

//---------------------------------------------------------------------------

void Printer::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& sourceID)
{
	LogLevel lvl;
	switch(level){
		case QWebEnginePage::WarningMessageLevel:
			lvl = LL_WARNING;
			break;
		case QWebEnginePage::ErrorMessageLevel:
			lvl = LL_ERROR;
			break;
	}
	Log::log(message, lvl);
}

//---------------------------------------------------------------------------

void Printer::onLoadFinished(bool ok)
{
	if(ok){
		printToPdf([this](const QByteArray& result){
			_loadFinishedCallback(result.constData(), result.size());
		}, _layout);
	}
}
