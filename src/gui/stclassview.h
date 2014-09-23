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


#ifndef stclassview_H
#define stclassview_H

#include "stmetasequence.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>

class STClassViewItem : public QTreeWidgetItem {
public:
	STClassViewItem(STMetaClass *C,QSet<QString> &used_class_names);
	virtual ~STClassViewItem();
	QString className() {return m_class_name;}
	bool isFoundationClass() {return m_is_foundation_class;}
	bool isUsed() {return m_is_used;}
	bool canExport() {return m_can_export;}
private:
	QString m_class_name;
	bool m_is_foundation_class;
	bool m_is_used;
	bool m_can_export;
};

class STClassView : public QTreeWidget {
	Q_OBJECT
public:
	STClassView(QWidget *parent=0);
	virtual ~STClassView();
	STMetaSequence *sequence() {return m_sequence;}
	void setSequence(STMetaSequence *Seq);
	void updateClasses();
signals:
	void classActivated(QString class_name);
	void renameClass(QString class_name);
	void deleteClass(QString class_name);
	void exportClass(QString class_name);
protected:
	void mousePressEvent ( QMouseEvent * event );
private slots:
	void slot_item_activated(QTreeWidgetItem *item);
	void slot_rename_class();
	void slot_delete_class();
	void slot_export_class();
private:
	STMetaSequence *m_sequence;
	
	QMenu m_popup_menu;
	QAction *m_rename_class_act;
	QAction *m_delete_class_act;
	QAction *m_export_class_act;
	
	void show_popup_menu(QPoint pos, QString class_name, bool is_foundation_class, bool is_used, bool can_export);
	
};

#endif
