/****************************************************************************
 This file is part of SequenceTree
 Copyright (C) 2005-2008 Jeremy Magland (Jeremy.Magland@gmail.com)

 SequenceTree is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 SequenceTree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SequenceTree; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/


#include "stcompilelog.h"

bool has_error(QString msg) {
	static QStringList errorPatterns;
	if (errorPatterns.isEmpty())
		errorPatterns << "^.*: fatal error .*$" << "^.*: error .*$" << "^.*error:.*$" << "^Unable to load.*$" << "^.*error in script:.*$" << "No such file" << "invalid preprocessing directive" << "^make:.*(error|Fehler).*$" << ": In function" << "undefined reference";
	for (int j=0; j<errorPatterns.count(); j++)
		if (msg.indexOf(QRegExp(errorPatterns[j]))>=0)
			return true;
	return false;
}

bool has_warning(QString msg) {
	static QStringList warningPatterns;
	if (warningPatterns.isEmpty())
		warningPatterns << "^.*: warning .*$" << "^.*warning:.*$" << "^FAIL!.*$" << "^Loc:\\s+\\[(.*)\\((\\d+)\\)\\]\\s*$";
	for (int j=0; j<warningPatterns.count(); j++)
		if (msg.indexOf(QRegExp(warningPatterns[j]))>=0)
			return true;
	return false;
}

STCompileLogItem::STCompileLogItem(QString msg) {
	m_message=msg;
	setText(msg);
	QColor col=Qt::black;
	if (has_error(msg))
		col=Qt::red;
	else if (has_warning(msg))
		col=Qt::blue;
	
	setForeground(QBrush(col));
}

QString STCompileLogItem::fileName() {
	int index1=m_message.indexOf("(");
	int index2=m_message.indexOf(")");
	int index1b=m_message.indexOf(":");
	if ((index1b>=0)&&((index1b<index1)||(index1<0))) { //mingw
		return m_message.mid(0,index1b);
	}
	else {
		if ((index1>0)&&(index2>index1+1)) {
			return m_message.mid(0,index1);
		}
	}
	return "";
}
int STCompileLogItem::lineNumber() {
	int index1=m_message.indexOf("(");
	int index2=m_message.indexOf(")");
	int index1b=m_message.indexOf(":");
	int index2b=index1b+1+m_message.mid(index1b+1).indexOf(":");
	if ((index1b>=0)&&((index1b<index1)||(index1<0))) { //mingw
		if (index2b>index1b+1)
			return m_message.mid(index1b+1,index2b-index1b-1).toInt();
	}
	else {
		if ((index1>0)&&(index2>index1+1)) {
			return m_message.mid(index1+1,index2-index1-1).toInt();
		}
	}
	return 0;
}

STCompileLog::STCompileLog(QWidget *parent) : QListWidget(parent) 
{
	connect(this,SIGNAL(itemActivated(QListWidgetItem *)),this,SLOT(slot_item_activated(QListWidgetItem *)));
}

STCompileLog::~STCompileLog()
{
}

void STCompileLog::addMessage(QString msg) {
	addItem(new STCompileLogItem(msg));
	scrollToBottom();
}


void STCompileLog::slot_item_activated(QListWidgetItem *item) {
	STCompileLogItem *item2=(STCompileLogItem *)item;
	if (!item2) return;
	QString fname=item2->fileName();
	int line=item2->lineNumber();
	if (!fname.isEmpty())
		emit gotoLine(fname,line);
}



