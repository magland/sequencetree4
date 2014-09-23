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


#include "stclassview.h"
#include <QMouseEvent>

STClassViewItem::STClassViewItem(STMetaClass *C,QSet<QString> &used_class_names) {
	if (!C) return;
	m_class_name=C->className();
	m_is_foundation_class=C->isFoundationClass();
	m_is_used=used_class_names.contains(C->className());
	m_can_export=false;
	if (!C->isFoundationClass()) m_can_export=true;
	else if ((C->isUserDefined())&&(C->headerFileName().isEmpty())) m_can_export=true;
	QColor col=Qt::black;
	QFont fnt=font(0);
	if (m_is_used) fnt.setBold(true);
	if (C->isFoundationClass()) {
		col=Qt::black;
		if (C->isUserDefined()) {
			col=Qt::darkBlue;
		}
	}
	else {
		col=Qt::darkMagenta;
	}
	QString labeltxt=C->className();
	if (!m_is_used) {
		labeltxt+=" (not used)";
	}
	setText(0,labeltxt);
	setText(1,C->baseClassName());
	setFont(0,fnt); setFont(1,fnt);
	setForeground(0,QBrush(col)); setForeground(1,QBrush(col));
}
STClassViewItem::~STClassViewItem() {
}

STClassView::STClassView(QWidget *parent) : QTreeWidget(parent) {
	QStringList labels; labels << "Class" << "Base";
	setHeaderLabels(labels);
	connect(this,SIGNAL(itemActivated(QTreeWidgetItem *,int)),this,SLOT(slot_item_activated(QTreeWidgetItem *)));
	
	m_rename_class_act=
		m_popup_menu.addAction("Rename Class...",this,SLOT(slot_rename_class()));
	m_delete_class_act=
		m_popup_menu.addAction("Delete Class...",this,SLOT(slot_delete_class()));
	m_export_class_act=
		m_popup_menu.addAction("Export Class...",this,SLOT(slot_export_class()));
}
STClassView::~STClassView() {
}

void STClassView::setSequence(STMetaSequence *Seq) {
	m_sequence=Seq;
	updateClasses();
}

void STClassView::updateClasses() {
	clear();
	if (!m_sequence) return;
	QSet<QString> used_class_names=m_sequence->usedClassNames();
	QStringList cnames=m_sequence->metaClassNames();
	qSort(cnames);
	foreach (QString cname,cnames) {
		addTopLevelItem(new STClassViewItem(m_sequence->metaClass(cname),used_class_names));
	}
	resizeColumnToContents(0);
}

void STClassView::slot_item_activated(QTreeWidgetItem *item) {
	STClassViewItem *item2=(STClassViewItem *)item;
	if (!item2) return;
	emit classActivated(item2->className());
}

void STClassView::mousePressEvent ( QMouseEvent * event ) {
	QTreeWidget::mousePressEvent(event);
	if (event->button()==Qt::RightButton) {
		STClassViewItem *item=(STClassViewItem *)currentItem();
		if (item) {
			show_popup_menu(event->pos(),item->className(),item->isFoundationClass(),item->isUsed(),item->canExport());
		}
	}
	
}

void STClassView::show_popup_menu(QPoint pos, QString class_name, bool is_foundation_class, bool is_used, bool can_export) {
	m_export_class_act->setEnabled(can_export);
	if ((is_foundation_class)||(class_name=="STRoot")) {
		m_rename_class_act->setEnabled(false);
		m_delete_class_act->setEnabled(false);
	}
	else {
		m_delete_class_act->setEnabled(!is_used);
		m_rename_class_act->setEnabled(true);
	}
	m_popup_menu.popup(mapToGlobal(pos));
}

void STClassView::slot_rename_class() {
	STClassViewItem *item=(STClassViewItem *)currentItem();
	if (!item) return;
	emit renameClass(item->className());
}

void STClassView::slot_delete_class() {
	STClassViewItem *item=(STClassViewItem *)currentItem();
	if (!item) return;
	emit deleteClass(item->className());
}

void STClassView::slot_export_class() {
	STClassViewItem *item=(STClassViewItem *)currentItem();
	if (!item) return;
	emit exportClass(item->className());
}

