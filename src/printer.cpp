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
#include "document.h"

//---------------------------------------------------------------------------

Printer::Printer(const Config& config)
{
	_layout = config.pageLayout();
}

Printer::~Printer()
{
}

//---------------------------------------------------------------------------

void Printer::renderHtml(const QString& html, LoadFinishedCallback callback)
{
	Document* page = new Document();
	page->connect(page, &QWebEnginePage::loadFinished, [=](bool ok){
		if(ok){
			page->printToPdf([=](const QByteArray& result){
				callback(result);
				page->deleteLater();
			}, _layout);
		}
	});
	page->setHtml(html);
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
