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


#ifndef STNodeView_H
#define STNodeView_H

#include "ui_stnodeview.h"
#include "stmetanode.h"
#include <QMenu>

class STNodeViewParameterItem : public QTreeWidgetItem {
public:
	STNodeViewParameterItem(STMetaNodeParameter param,bool is_compiled);
	STMetaNodeParameter parameter();
private:
	STMetaNodeParameter m_param;
};


class STNodeView : public QWidget {
	Q_OBJECT
private:
	Ui::STNodeView ui;
public:
	STNodeView(QWidget *parent=0);
	void setNode(STMetaNode *N);
	STMetaNode *node();
	void updateControls();
	void setCompiled(bool val) {
		m_is_compiled=val;
		updateControls();
	}
signals:
	void parameterChanged(QString name);
	void editCode();
	void minimizeParameter(QString);
	void maximizeParameter(QString);
	void linkToGlobalParameter(QString);
	void unlinkGlobalParameter(QString);
private slots:
	void slot_parameter_activated(QTreeWidgetItem *item,int column);
	void slot_parameter_changed(QTreeWidgetItem *item,int column);
	void slot_item_right_clicked(QTreeWidgetItem *item,QPoint pos);
	void slot_minimize_parameter();
	void slot_maximize_parameter();
	void slot_link_to_global_parameter();
	void slot_unlink_global_parameter();
private:
	STMetaNode *m_node;
	bool m_is_compiled;
	QMenu popup_menu;
	
	QAction *m_action_minimize_parameter;
	QAction *m_action_maximize_parameter;
	QAction *m_action_link_to_global_parameter;
	QAction *m_action_unlink_global_parameter;
};

#endif
