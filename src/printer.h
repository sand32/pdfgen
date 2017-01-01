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

#ifndef _PRINTER_H_
#define _PRINTER_H_

#include <QtWebEngineWidgets>

typedef void (*LoadFinishedCallback)(const char* pdf, int size);

class Printer : public QWebEnginePage
{
	Q_OBJECT

	LoadFinishedCallback _loadFinishedCallback;
	QPageLayout _layout;

public:
	Printer();
	~Printer();

	void renderHtml(const QString& html);
	void renderFromJson(const QByteArray& json);

protected:
	void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& sourceID);

private slots:
	void onLoadFinished(bool ok);
};

#endif // _PRINTER_H_
