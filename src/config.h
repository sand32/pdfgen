/*
-----------------------------------------------------------------------------
Copyright (c) 2017 Seth Anderson

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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <QtGui/QtGui>

class Config
{
	QPageLayout _pageLayout;
	int _port;
	int _connectionTimeoutMS;

public:
	Config();

	bool load(const QString& filename);

	int port() const;
	int connectionTimeoutMS() const;
	const QPageLayout& pageLayout() const;

private:
	void readPageLayout(const QJsonObject& object);

	static QHash<QString, QPageSize::PageSizeId> PageSizes;
	static QHash<QString, QPageLayout::Orientation> PageOrientations;
	static QHash<QString, QPageLayout::Unit> PageUnits;
};

#endif // _CONFIG_H_
