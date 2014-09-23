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


#ifndef STSourceEditor_H
#define STSourceEditor_H

#include "ui_stsourceeditor.h"
#include "cpphighlighter.h"
#include "stmetaclass.h"
#include "stmetasequence.h"

class STSourceEditor : public QWidget {
	Q_OBJECT
private:
	Ui::STSourceEditor ui;
public:
	STSourceEditor(QWidget *parent=0);
	void setClass(STMetaClass *C);
	QString currentClassName();
	void setSequence(STMetaSequence *Seq);
	void update();
	void setReadOnly(bool val);
	bool isModified();
	void setModified(bool val);
	void gotoHeaderLine(int line);
	void gotoSourceLine(int line);
signals:
	void classChanged();
	void signalCompile();
	void changeClass(QString cname);
public slots:
	void slotSaveChanges();
private slots:
	void slot_modification_changed();
	void slot_class_box_changed(QString cname);
	void slot_reload();	
	void slot_find();
private:
	CppHighlighter *m_highlighter_header;
	CppHighlighter *m_highlighter_source;
	STMetaClass *m_C;
	STMetaSequence *m_sequence;
	
	void check_to_update_current_class();

};

#endif
