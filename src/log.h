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

#ifndef _LOG_H_
#define _LOG_H_

#include <QString>

enum LogLevel
{
	LL_INFO,
	LL_WARNING,
	LL_ERROR
};

class Log
{
public:
	static void log(QString message, LogLevel lvl = LL_INFO);
};

#endif // _LOG_H_
