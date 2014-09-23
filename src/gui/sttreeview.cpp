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


#include "sttreeview.h"
#include <QApplication>
#include <QAction>
#include <QFont>

STTreeViewItem::STTreeViewItem(int nodelist_index) {
	m_nodelist_index=nodelist_index;
}
STTreeViewItem::~STTreeViewItem() {
}

STTreeView::STTreeView(QWidget *parent) : QTreeWidget(parent) {
	Seq=0;

	headerItem()->setTextAlignment(0,Qt::AlignLeft);
	
	setSelectionMode(QAbstractItemView::SingleSelection);

	//Add child node
	addChildAct = new QAction(tr("Add child node..."), this);
	connect(addChildAct, SIGNAL(triggered()), this, SLOT(on_add_child_node()));
	popup_menu.addAction(addChildAct);

	//Insert node
	insertNodeAct = new QAction(tr("Insert child node..."), this);
	connect(insertNodeAct, SIGNAL(triggered()), this, SLOT(on_insert_node()));
	popup_menu.addAction(insertNodeAct);

	///////////////////////////////////////////////////////////////////
	popup_menu.addSeparator();
	
	//Customize node
	customizeNodeAct = new QAction(tr("Customize node..."), this);
	connect(customizeNodeAct, SIGNAL(triggered()), this, SLOT(on_customize_node()));
	popup_menu.addAction(customizeNodeAct);
	
	//Change node type
	changeNodeTypeAct = new QAction(tr("Change node type..."), this);
	connect(changeNodeTypeAct, SIGNAL(triggered()), this, SLOT(on_change_node_type()));
	popup_menu.addAction(changeNodeTypeAct);
	
	//Promote node
	promoteNodeAct = new QAction(tr("Promote node..."), this);
	connect(promoteNodeAct, SIGNAL(triggered()), this, SLOT(on_promote_node()));
	popup_menu.addAction(promoteNodeAct);

	//Rename node
	renameNodeAct = new QAction(tr("Rename node..."), this);
	connect(renameNodeAct, SIGNAL(triggered()), this, SLOT(on_rename_node()));
	popup_menu.addAction(renameNodeAct);

	//Delete node
	deleteNodeAct = new QAction(tr("Delete node"), this);
	connect(deleteNodeAct, SIGNAL(triggered()), this, SLOT(on_delete_node()));
	popup_menu.addAction(deleteNodeAct);
	
	popup_menu.addSeparator();
	
	//Unlink node
	unlinkNodeAct = new QAction(tr("Unlink node"), this);
	connect(unlinkNodeAct, SIGNAL(triggered()), this, SLOT(on_unlink_node()));
	popup_menu.addAction(unlinkNodeAct);
	
	//Remove resource
	resource_popup_menu.addAction("Remove resource",this,SLOT(slot_remove_resource()));
	
	connect(this,SIGNAL(itemActivated(QTreeWidgetItem *,int)),this,SLOT(slot_item_activated(QTreeWidgetItem *)));
}
STTreeView::~STTreeView() {
	if (Seq) delete Seq;
}

void STTreeView::set_icon(STTreeViewItem *N2) {
	if (!Seq) return;
	STTreeViewItem *N=(STTreeViewItem *)N2;
	if (!N) return;

	bool selected=N->isSelected();

	int ind=N->m_nodelist_index;
	STMetaNode *node=0;
	if ((ind>=0)&&(ind<nodelist.count()))
		node=nodelist[ind];
	if (!node) return;
	STMetaClass *C=Seq->metaClass(node->className());
	if (!C) return;
	
	QColor textcol;
	if (C->isFoundationClass()) {
		if (C->isUserDefined())
			textcol=Qt::darkBlue;
		else
			textcol=Qt::black;
	}
	else {
		textcol=Qt::darkMagenta;
	}
	N->setForeground(1,textcol);
	
	QString icon_fname="";
	if (Seq->inherits(node->className(),"STLoop")) {
		if (C->isFoundationClass()) {
			if (selected)
				icon_fname=":/images/selected_loop.bmp";
			else
				icon_fname=":/images/loop.bmp";
		}
		else {
			if (selected)
				icon_fname=":/images/selected_custom_loop.bmp";
			else
				icon_fname=":/images/custom_loop.bmp";
		}
	}
	else if ((node->parent())&&(Seq->inherits(node->parent()->className(),"STLoop"))) {
		if (C->isFoundationClass()) {
			if (selected)
				icon_fname=":/images/selected_block.bmp";
			else
				icon_fname=":/images/block.bmp";
		}
		else {
			if (selected)
				icon_fname=":/images/selected_custom_block.bmp";
			else
				icon_fname=":/images/custom_block.bmp";
		}
	}
	else {
		if (C->isFoundationClass()) {
			if (selected)
				icon_fname=":/images/selected_segment.bmp";
			else
				icon_fname=":/images/segment.bmp";
		}
		else {
			if (selected)
				icon_fname=":/images/selected_custom_segment.bmp";
			else
				icon_fname=":/images/custom_segment.bmp";
		}
	}
	if (!icon_fname.isEmpty())
		N->setIcon(0,QIcon(icon_fname));
}


void STTreeView::update_node(STTreeViewItem *N) {
	if (!N) return;
	if (!Seq) return;
	
	int ind=N->m_nodelist_index;
	STMetaNode *node=0;
	if ((ind>=0)&&(ind<nodelist.count()))
		node=nodelist[ind];
	if (!node) return;
	
	N->setForeground(0,QBrush(Qt::black));
	QString labeltxt=node->name();
	if (node->linkGroup()>=0) labeltxt+=QString(" (Link %1)").arg(node->linkGroup());
	N->setText(0,labeltxt);
	N->setText(1,node->className());

	set_icon(N);

	/*char nodestr[1000];
	node->get_string(nodestr);*/

	int j;
	for (j=0; j<node->childCount(); j++) {
		STTreeViewItem *child=0;
		if (j<N->childCount()) {
			child=(STTreeViewItem *)N->child(j);
		}
		else {
			child=new STTreeViewItem(-1);
			N->addChild(child);
		}
		/*if (node->child(j)->link_group_number>=0)
			child->setText(0,QString(node->child(j)->name)+" (Link "+QString::number(node->child(j)->link_group_number)+")");
		else*/
		child->m_nodelist_index=node->child(j)->m_node_index;
		update_node(child);
	}

	//delete extra children
	while (N->childCount()>node->childCount()) {
		delete N->takeChild(N->childCount()-1);
	}
}

void add_resource(STTreeViewItem *resource_node,STResource *R,QStringList name_path,QString base_name) {
	if (!R) return;
	if (!resource_node) return;
	if (name_path.count()>=1) {		
		STTreeViewItem *newitem=0;
		for (int j=0; j<resource_node->childCount(); j++) {
			STTreeViewItem *childitem=(STTreeViewItem *)resource_node->child(j);
			if (childitem->m_resource_path_name==name_path[0])
				newitem=childitem;
		}
		if (!newitem) {
			newitem=new STTreeViewItem(-1);
			if (name_path[0].mid(name_path[0].count()-4)==".mrp") {
				newitem->setIcon(0,QIcon(":/images/sinc.png"));
			}
			newitem->setText(0,name_path[0]);
			resource_node->addChild(newitem);
		}
		newitem->m_resource_path_name=name_path[0];
		newitem->m_resource_name=base_name+name_path[0];
		if (name_path.count()==1) {			
			QString txt=newitem->m_resource_path_name;
			if (R->doubleListCount()>0) {
				txt+=" =";
				if (R->doubleListCount()>1) txt+=QString(" (%1)").arg(R->doubleListCount());
				for (long ct=0; (ct<R->doubleListCount())&&(ct<5); ct++) {
					txt+=QString(" %1").arg(R->getDoubleList(ct));
				}
				if (R->doubleListCount()>5) {
					txt+="...";
				}
			}
			newitem->setText(0,txt);
		}
		else {
			add_resource(newitem,R,name_path.mid(1),base_name+name_path[0]+"->");
		}
	}
}

void STTreeView::update_resources() {
	STTreeViewItem *resource_node=0;
	if (topLevelItemCount()>=2) resource_node=(STTreeViewItem *)topLevelItem(1);
	if (!resource_node) {
		if (!Seq) return;
		if (Seq->resourceCount()==0) return;
		resource_node=new STTreeViewItem(-1);
		resource_node->setIcon(0,QIcon(":/images/redsquare.bmp"));
		resource_node->setText(0,"Resources");
		addTopLevelItem(resource_node);
	}
	while (resource_node->childCount()>0) {
		delete resource_node->takeChild(resource_node->childCount()-1);
	}
	if (!Seq) return;
	for (int j=0; j<Seq->resourceCount(); j++) {
		STResource *R=Seq->resource(j);
		add_resource(resource_node,R,qstr(R->name()).split("->"),"");
	}
}

void STTreeView::update() {
	if (!Seq) {
		clear();
		return;
	}
	Seq->update();

	setup_nodelist();

	/////////////////////////////////////////
	/*QTreeWidgetItem *settings_node=topLevelItem(0);
	if (!settings_node) {
		settings_node=new QTreeWidgetItem(QStringList("Settings"));
		addTopLevelItem(settings_node);
	}
	settings_node->setData(2,0,(uint)(Seq->Settings.node_index));*/
	
	STTreeViewItem *root_node=0;
	if (topLevelItemCount()>=1) root_node=(STTreeViewItem *)topLevelItem(0);
	if (!root_node) {
		root_node=new STTreeViewItem(-1);
		addTopLevelItem(root_node);
	}
	root_node->m_nodelist_index=Seq->m_node_index;

	update_node(root_node);
	
	expandNode(Seq);

	qApp->processEvents();
	repaint();
}

void STTreeView::show_popup_menu(QPoint pos) {
	if (!Seq) return;
	STMetaNode *node=current_node();
	if (node) {
		STMetaNode *par=node->parent();
		STMetaClass *parclass=0;
		if (par) parclass=Seq->metaClass(par->className());	
		STMetaClass *C=Seq->metaClass(node->className());
		
		if (C->isFoundationClass()) {
			addChildAct->setText("Add child node");
			addChildAct->setEnabled(false);
		}
		else {
			if (Seq->inherits(node->className(),"STSequence")) { 
				addChildAct->setText("Add Loop");
				addChildAct->setEnabled(true);
			}
			else if (Seq->inherits(node->className(),"STLoop")) { 
				addChildAct->setText("Add Block");
				addChildAct->setEnabled(true);
			}
			else if (Seq->inherits(node->className(),"STChain")) { 
				addChildAct->setText("Add Segment");
				addChildAct->setEnabled(true);
			}
			else  { 
				addChildAct->setText("Add child node");
				addChildAct->setEnabled(false);
			}
		}
		
		if ((!parclass)||(parclass->isFoundationClass())) {
			insertNodeAct->setText("Insert node");
			insertNodeAct->setEnabled(false);
		}
		else {
			if (Seq->inherits(parclass->className(),"STSequence")) { 
				insertNodeAct->setText("Insert Loop");
				insertNodeAct->setEnabled(true);
			}
			else if (Seq->inherits(parclass->className(),"STLoop")) { 
				insertNodeAct->setText("Insert Block");
				insertNodeAct->setEnabled(true);
			}
			else if (Seq->inherits(parclass->className(),"STChain")) { 
				insertNodeAct->setText("Insert Segment");
				insertNodeAct->setEnabled(true);
			}
			else  { 
				insertNodeAct->setText("Insert node");
				insertNodeAct->setEnabled(false);
			}
		}
		
		if ((parclass)&&(!parclass->isFoundationClass())) {
			changeNodeTypeAct->setEnabled(true);
			renameNodeAct->setEnabled(true);
			deleteNodeAct->setEnabled(true);
		}
		else {
			changeNodeTypeAct->setEnabled(false);
			renameNodeAct->setEnabled(false);
			deleteNodeAct->setEnabled(false);
		}
		
		if (parclass)
			promoteNodeAct->setEnabled(true);
		else
			promoteNodeAct->setEnabled(false);
		
		if ((parclass)&&(!parclass->isFoundationClass())) {
			customizeNodeAct->setEnabled(true);
		}
		else {
			if (Seq->isPromoted(node))
				customizeNodeAct->setEnabled(true);
			else
				customizeNodeAct->setEnabled(true);
		}
		
		if (node->linkGroup()>=0) {
			unlinkNodeAct->setEnabled(true);
		}
		else {
			unlinkNodeAct->setEnabled(false);
		}
	
	
		popup_menu.popup(mapToGlobal(pos));
	}
	else {
		resource_popup_menu.popup(mapToGlobal(pos));
	}
}

void STTreeView::mousePressEvent ( QMouseEvent * event ) {
	STMetaNode *node1=current_node();
	STTreeViewItem *N1=(STTreeViewItem *)currentItem();
	QTreeWidget::mousePressEvent(event);
	STMetaNode *node2=current_node();
	STTreeViewItem *N2=(STTreeViewItem *)currentItem();
	set_icon(N1);
	set_icon(N2);
	if (event->modifiers()&Qt::ControlModifier) {
		emit linkNodes(node1,node2);
	}
	if (event->button()==Qt::RightButton) {
		STTreeViewItem *item=(STTreeViewItem *)currentItem();
		if (item) {
			show_popup_menu(event->pos());
		}
	}
	emit currentNodeChanged(node2);
}

void STTreeView::mouseDoubleClickEvent (QMouseEvent *event) {
	QTreeWidget::mousePressEvent(event);
	STTreeViewItem *item=(STTreeViewItem *)currentItem();
	if (item) emit itemActivated(item,0);
}

void STTreeView::keyPressEvent ( QKeyEvent * event ) {
	STTreeViewItem *N1=(STTreeViewItem *)currentItem();
	QTreeWidget::keyPressEvent(event);
	STTreeViewItem *N2=(STTreeViewItem *)currentItem();
	set_icon(N1);
	set_icon(N2);
	STMetaNode *node=current_node();
	if (node) {
		emit currentNodeChanged(node);
	}
}

STMetaNode *STTreeView::current_node() {
	STTreeViewItem *item=(STTreeViewItem *)currentItem();
	if (!item) return 0;
	int ind=item->m_nodelist_index;
	STMetaNode *node=0;
	if ((ind>=0)&&(ind<nodelist.count())) {
		node=nodelist[ind];
	}
	return node;
}

STTreeViewItem *STTreeView::get_tree_item(STMetaNode *N,STTreeViewItem *startfrom) {
	if (!Seq) return 0;
	if (!startfrom) {
		//if (N==&Seq->Settings)
		//	return topLevelItem(0);
		startfrom=(STTreeViewItem *)topLevelItem(0);
	}
	if (!startfrom) return 0;
	int ind=startfrom->m_nodelist_index;
	if (ind==N->m_node_index)
		return startfrom;
	int j;
	for (j=0; j<startfrom->childCount(); j++) {
		STTreeViewItem *ret=get_tree_item(N,(STTreeViewItem *)(startfrom->child(j)));
		if (ret) return ret;
	}
	return 0;
}

void STTreeView::set_current_node(int node_index) {
	if (!Seq) return;
	
	STMetaNode *node=0;
	if ((node_index>=0)&&(node_index<nodelist.count()))
		node=nodelist[node_index];
	if (node)
		set_current_node(node);
}

void STTreeView::set_current_node(STMetaNode *N) {
	if (!N) return;
	if (!Seq) return;
	STTreeViewItem *item=get_tree_item(N);
	if (!item) return;
	setCurrentItem(item);
}


void STTreeView::on_add_child_node() {
	STMetaNode *node=current_node();
	emit addChildNode(node);
}

void STTreeView::on_insert_node() {
	STMetaNode *node=current_node();
	emit insertNode(node);
}

void STTreeView::on_rename_node() {
	STMetaNode *node=current_node();
	emit renameNode(node);
}

void STTreeView::on_change_node_type() {
	STMetaNode *node=current_node();
	emit changeNodeType(node);
}

void STTreeView::on_delete_node() {
	STMetaNode *node=current_node();
	emit deleteNode(node);
}

void STTreeView::on_unlink_node() {
	STMetaNode *node=current_node();
	emit unlinkNode(node);
}

void STTreeView::on_customize_node() {
	STMetaNode *node=current_node();
	emit customizeNode(node);
}

void STTreeView::on_promote_node() {
	STMetaNode *node=current_node();
	emit promoteNode(node);
}

void STTreeView::slot_remove_resource() {
	if (!Seq) return;
	STTreeViewItem *item=(STTreeViewItem *)currentItem();
	if (!item->m_resource_name.isEmpty()) {
		Seq->removeResource(item->m_resource_name,true);
		updateResources();
	}
}

void STTreeView::setup_nodelist() {
	nodelist.clear();
	if (!Seq) return;
	Seq->setNodeIndices();
	int j;
	for (j=0; j<=Seq->m_max_node_index; j++)
		nodelist.append((STMetaNode *)0);
	//add_to_nodelist(&Seq->Settings);
	add_to_nodelist(Seq);
}
void STTreeView::add_to_nodelist(STMetaNode *node) {
	if (!node) return;
	int ind=node->m_node_index;
	if ((0<=ind)&&(ind<nodelist.count()))
		nodelist[ind]=node;
	int j;
	for (j=0; j<node->childCount(); j++)
		add_to_nodelist(node->child(j));
}

void STTreeView::expandNode(STMetaNode *N) {
	STTreeViewItem *item=get_tree_item(N);
	if (!item) return;
	item->setExpanded(true);
}

void STTreeView::resizeEvent(QResizeEvent*event) {
	QTreeWidget::resizeEvent(event);
	int total_column_width=columnWidth(0)+columnWidth(1);
	setColumnWidth(0,2*total_column_width/3);
	setColumnWidth(1,1*total_column_width/3);
}

void STTreeView::setCurrentNode(int tree_index) {
	if (!Seq) return;
	STMetaNode *N=Seq->findNode(tree_index);
	if (!N) return;
	STTreeViewItem *item=get_tree_item(N);
	if (!item) return;
	setCurrentItem(item);
	scrollToItem(item);
	emit currentNodeChanged(N);
}

STTreeViewItem *STTreeView::findItem(int tree_index) {
	if (!Seq) return 0;
	STMetaNode *N=Seq->findNode(tree_index);
	if (!N) return 0;
	return get_tree_item(N);
}

void STTreeView::slot_item_activated(QTreeWidgetItem *item2) {
	STTreeViewItem *item=(STTreeViewItem *)item2;
	int ind=item->m_nodelist_index;
	STMetaNode *node=0;
	if ((ind>=0)&&(ind<nodelist.count())) {
		node=nodelist[ind];
	}
	if (node) emit nodeActivated(node);
	else {
		QString resource_name=item->m_resource_name;
		if (!resource_name.isEmpty())
			emit resourceActivated(resource_name);
	}
}

