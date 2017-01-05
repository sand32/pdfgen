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

#include "config.h"

#include "log.h"

//---------------------------------------------------------------------------

Config::Config()
	: _port(0),
	  _connectionTimeoutMS(0)
{
}

//---------------------------------------------------------------------------

bool Config::load(const QString& filename)
{
	QFile file(filename);
	char* buffer = new char[file.size()];
	file.open(QIODevice::ReadOnly);
	qint64 result = file.read(buffer, file.size());
	if(result < 0){
		Log::log(file.errorString(), LL_ERROR);
		file.close();
		return false;
	}
	file.close();

	QJsonParseError error;
	QJsonDocument document = QJsonDocument::fromJson(QByteArray(buffer, result), &error);
	if(document.isNull()){
		Log::log(error.errorString(), LL_ERROR);
		return false;
	}
	if(!document.isObject()){
		Log::log("Invalid JSON config file, top level node must be an object.", LL_ERROR);
		return false;
	}

	QJsonObject topLevelObject = document.object();

	for(QJsonObject::const_iterator it = topLevelObject.constBegin(); it != topLevelObject.constEnd(); ++it){
		QJsonValue value = it.value();
		if(it.key().toLower() == "port"){
			_port = value.toInt();
		}
		if(it.key().toLower() == "connectiontimeoutms"){
			_connectionTimeoutMS = value.toInt();
		}
		if(it.key().toLower() == "renderoptions"){
			readPageLayout(value.toObject());
		}
	}

	return true;
}

int Config::port() const
{
	return _port;
}

int Config::connectionTimeoutMS() const
{
	return _connectionTimeoutMS;
}

const QPageLayout& Config::pageLayout() const
{
	return _pageLayout;
}

//---------------------------------------------------------------------------

void Config::readPageLayout(const QJsonObject& object)
{
	for(QJsonObject::const_iterator it = object.constBegin(); it != object.constEnd(); ++it){
		QJsonValue value = it.value();
		if(it.key().toLower() == "margintop"){
			_pageLayout.setTopMargin(value.toDouble());
		}
		if(it.key().toLower() == "marginbottom"){
			_pageLayout.setBottomMargin(value.toDouble());
		}
		if(it.key().toLower() == "marginleft"){
			_pageLayout.setLeftMargin(value.toDouble());
		}
		if(it.key().toLower() == "marginright"){
			_pageLayout.setRightMargin(value.toDouble());
		}
		if(it.key().toLower() == "pagesize"){
			_pageLayout.setPageSize(QPageSize(PageSizes[value.toString().toLower()]));
		}
		if(it.key().toLower() == "orientation"){
			_pageLayout.setOrientation(PageOrientations[value.toString().toLower()]);
		}
		if(it.key().toLower() == "units"){
			_pageLayout.setUnits(PageUnits[value.toString().toLower()]);
		}
	}
}

//---------------------------------------------------------------------------

QHash<QString, QPageSize::PageSizeId> Config::PageSizes = QHash<QString, QPageSize::PageSizeId>{
	{"a0", QPageSize::A0},
	{"a1", QPageSize::A1},
	{"a2", QPageSize::A2},
	{"a3", QPageSize::A3},
	{"a4", QPageSize::A4},
	{"a5", QPageSize::A5},
	{"a6", QPageSize::A6},
	{"a7", QPageSize::A7},
	{"a8", QPageSize::A8},
	{"a9", QPageSize::A9},
	{"b0", QPageSize::B0},
	{"b1", QPageSize::B1},
	{"b2", QPageSize::B2},
	{"b3", QPageSize::B3},
	{"b4", QPageSize::B4},
	{"b5", QPageSize::B5},
	{"b6", QPageSize::B6},
	{"b7", QPageSize::B7},
	{"b8", QPageSize::B8},
	{"b9", QPageSize::B9},
	{"b10", QPageSize::B10},
	{"c5e", QPageSize::C5E},
	{"comm10e", QPageSize::Comm10E},
	{"dle", QPageSize::DLE},
	{"executive",QPageSize::Executive},
	{"folio", QPageSize::Folio},
	{"ledger", QPageSize::Ledger},
	{"legal",QPageSize::Legal},
	{"letter",QPageSize::Letter},
	{"tabloid", QPageSize::Tabloid},
	{"custom", QPageSize::Custom},
	{"a10", QPageSize::A10},
	{"a3extra", QPageSize::A3Extra},
	{"a4extra", QPageSize::A4Extra},
	{"a4plus", QPageSize::A4Plus},
	{"a4small", QPageSize::A4Small},
	{"a5extra", QPageSize::A5Extra},
	{"b5extra", QPageSize::B5Extra},
	{"jisb0", QPageSize::JisB0},
	{"jisb1", QPageSize::JisB1},
	{"jisb2", QPageSize::JisB2},
	{"jisb3", QPageSize::JisB3},
	{"jisb4", QPageSize::JisB4},
	{"jisb5", QPageSize::JisB5},
	{"jisb6", QPageSize::JisB6},
	{"jisb7", QPageSize::JisB7},
	{"jisb8", QPageSize::JisB8},
	{"jisb9", QPageSize::JisB9},
	{"jisb10", QPageSize::JisB10},
	{"ansia", QPageSize::AnsiA},
	{"ansib", QPageSize::AnsiB},
	{"ansic", QPageSize::AnsiC},
	{"ansid", QPageSize::AnsiD},
	{"ansie", QPageSize::AnsiE},
	{"legalextra", QPageSize::LegalExtra},
	{"letterextra", QPageSize::LetterExtra},
	{"letterplus", QPageSize::LetterPlus},
	{"lettersmall", QPageSize::LetterSmall},
	{"tabloidextra", QPageSize::TabloidExtra},
	{"archa", QPageSize::ArchA},
	{"archb", QPageSize::ArchB},
	{"archc", QPageSize::ArchC},
	{"archd", QPageSize::ArchD},
	{"arche", QPageSize::ArchE},
	{"imperial7x9", QPageSize::Imperial7x9},
	{"imperial8x10", QPageSize::Imperial8x10},
	{"imperial9x11", QPageSize::Imperial9x11},
	{"imperial9x12", QPageSize::Imperial9x12},
	{"imperial10x11", QPageSize::Imperial10x11},
	{"imperial10x13", QPageSize::Imperial10x13},
	{"imperial10x14", QPageSize::Imperial10x14},
	{"imperial12x11", QPageSize::Imperial12x11},
	{"imperial15x11", QPageSize::Imperial15x11},
	{"executivestandard", QPageSize::ExecutiveStandard},
	{"note", QPageSize::Note},
	{"quarto", QPageSize::Quarto},
	{"statement", QPageSize::Statement},
	{"supera", QPageSize::SuperA},
	{"superb", QPageSize::SuperB},
	{"postcard", QPageSize::Postcard},
	{"doublepostcard", QPageSize::DoublePostcard},
	{"prc16k", QPageSize::Prc16K},
	{"prc32k", QPageSize::Prc32K},
	{"prc32kbig", QPageSize::Prc32KBig},
	{"fanfoldus", QPageSize::FanFoldUS},
	{"fanfoldgerman", QPageSize::FanFoldGerman},
	{"fanfoldgermanlegal", QPageSize::FanFoldGermanLegal},
	{"envelopeb4", QPageSize::EnvelopeB4},
	{"envelopeb5", QPageSize::EnvelopeB5},
	{"envelopeb6", QPageSize::EnvelopeB6},
	{"envelopec0", QPageSize::EnvelopeC0},
	{"envelopec1", QPageSize::EnvelopeC1},
	{"envelopec2", QPageSize::EnvelopeC2},
	{"envelopec3", QPageSize::EnvelopeC3},
	{"envelopec4", QPageSize::EnvelopeC4},
	{"envelopec5", QPageSize::EnvelopeC5},
	{"envelopec6", QPageSize::EnvelopeC6},
	{"envelopec65", QPageSize::EnvelopeC65},
	{"envelopec7", QPageSize::EnvelopeC7},
	{"envelopedl", QPageSize::EnvelopeDL},
	{"envelope9", QPageSize::Envelope9},
	{"envelope10", QPageSize::Envelope10},
	{"envelope11", QPageSize::Envelope11},
	{"envelope12", QPageSize::Envelope12},
	{"envelope14", QPageSize::Envelope14},
	{"envelopemonarch", QPageSize::EnvelopeMonarch},
	{"envelopepersonal", QPageSize::EnvelopePersonal},
	{"envelopechou3", QPageSize::EnvelopeChou3},
	{"envelopechou4", QPageSize::EnvelopeChou4},
	{"envelopeinvite", QPageSize::EnvelopeInvite},
	{"envelopeitalian", QPageSize::EnvelopeItalian},
	{"envelopekaku2", QPageSize::EnvelopeKaku2},
	{"envelopekaku3", QPageSize::EnvelopeKaku3},
	{"envelopeprc1", QPageSize::EnvelopePrc1},
	{"envelopeprc2", QPageSize::EnvelopePrc2},
	{"envelopeprc3", QPageSize::EnvelopePrc3},
	{"envelopeprc4", QPageSize::EnvelopePrc4},
	{"envelopeprc5", QPageSize::EnvelopePrc5},
	{"envelopeprc6", QPageSize::EnvelopePrc6},
	{"envelopeprc7", QPageSize::EnvelopePrc7},
	{"envelopeprc8", QPageSize::EnvelopePrc8},
	{"envelopeprc9", QPageSize::EnvelopePrc9},
	{"envelopeprc10", QPageSize::EnvelopePrc10},
	{"envelopeyou4", QPageSize::EnvelopeYou4},
	{"lastpagesize", QPageSize::LastPageSize}
};

QHash<QString, QPageLayout::Orientation> Config::PageOrientations = QHash<QString, QPageLayout::Orientation>{
	{"portrait", QPageLayout::Portrait},
	{"landscape", QPageLayout::Landscape}
};

QHash<QString, QPageLayout::Unit> Config::PageUnits = QHash<QString, QPageLayout::Unit>{
	{"millimeter", QPageLayout::Millimeter},
	{"mm", QPageLayout::Millimeter},
	{"point", QPageLayout::Point},
	{"pt", QPageLayout::Point},
	{"inch", QPageLayout::Inch},
	{"in", QPageLayout::Inch},
	{"pica", QPageLayout::Pica},
	{"didot", QPageLayout::Didot},
	{"cicero", QPageLayout::Cicero}
};
