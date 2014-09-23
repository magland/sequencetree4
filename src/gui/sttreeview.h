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


#ifndef sttreeview_H
#define sttreeview_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "stmetasequence.h"
#include <QMenu>
#include <QMouseEvent>


class STTreeViewItem : public QTreeWidgetItem {
public:
	STTreeViewItem(int nodelist_index);
	virtual ~STTreeViewItem();
public:
	int m_nodelist_index;
	QString m_resource_name;
	QString m_resource_path_name;
	
};

class STTreeView : public QTreeWidget {
	Q_OBJECT
private:
	QAction *unlinkNodeAct,*addChildAct,
		    *insertNodeAct,*renameNodeAct,
			*deleteNodeAct,*customizeNodeAct,*promoteNodeAct,
			*changeNodeTypeAct;
public:
	STMetaSequence *Seq;
	QMenu popup_menu;
	QMenu resource_popup_menu;
	QList<STMetaNode *> nodelist;

	STTreeView(QWidget *parent=0);
	virtual ~STTreeView();

	void setSequence(STMetaSequence *Seq_in) {
		setColumnCount(2);
		QStringList labels; labels << "Node" << "Type";
		setHeaderLabels(labels);
		Seq=Seq_in;
		setup_nodelist();
		update();
		update_resources();
	}
	void setup_nodelist();
	void add_to_nodelist(STMetaNode *node);
	STTreeViewItem *findItem(int tree_index);

	void update_node(STTreeViewItem *root_node);
	void show_popup_menu(QPoint pos);
	STMetaNode *current_node();
	void set_current_node(STMetaNode *N);
	void set_current_node(int node_index);
	void set_icon(STTreeViewItem *N);

	STTreeViewItem *get_tree_item(STMetaNode *N,STTreeViewItem *startfrom=0);
	
	void expandNode(STMetaNode *N);
	
	void setCurrentNode(int tree_index);
	
	void updateResources() {update_resources();}

protected:
	void resizeEvent(QResizeEvent*event);
	void mousePressEvent ( QMouseEvent * event );
	void keyPressEvent ( QKeyEvent * event );
	void mouseDoubleClickEvent (QMouseEvent *event);
private:
	void update_resources();
public slots:

	void update();
	
	void on_add_child_node();
	void on_insert_node();
	void on_rename_node();
	void on_delete_node();
	void on_customize_node();
	void on_promote_node();
	void on_change_node_type();
	void on_unlink_node();
	
	void slot_remove_resource();
	
	void slot_item_activated(QTreeWidgetItem *item);
signals:
	void currentNodeChanged(STMetaNode *node);
	void addChildNode(STMetaNode *node);
	void insertNode(STMetaNode *node);
	void renameNode(STMetaNode *node);
	void deleteNode(STMetaNode *node);
	void customizeNode(STMetaNode *node);
	void promoteNode(STMetaNode *node);
	void changeNodeType(STMetaNode *node);
	void nodeActivated(STMetaNode *node);
	void unlinkNode(STMetaNode *node);
	void linkNodes(STMetaNode *node1,STMetaNode *node2);
	
	void resourceActivated(QString resource_name);
};



#endif
