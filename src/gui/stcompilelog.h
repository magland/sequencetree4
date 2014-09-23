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


#ifndef stcompilelog_H
#define stcompilelog_H

#include <QListWidget>
#include <QListWidgetItem>

class STCompileLogItem : public QListWidgetItem {
public:
	STCompileLogItem(QString msg);
	
	QString fileName();
	int lineNumber();
private:
	QString m_message;
};

class STCompileLog : public QListWidget {
	Q_OBJECT
public:
	STCompileLog(QWidget *parent=0);
	virtual ~STCompileLog();
	
	void addMessage(QString msg);
signals:
	void gotoLine(QString filename,int line);
private slots:
	void slot_item_activated(QListWidgetItem *item);
};

#endif
