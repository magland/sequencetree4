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


#include "stnodeview.h"
#include <QDebug>

STNodeViewParameterItem::STNodeViewParameterItem(STMetaNodeParameter param,bool is_compiled) {
	m_param=param;
	setText(0,param.type);
	setText(1,param.name);
	setText(2,param.value);
	setText(3,param.units);
	setText(4,param.global_parameter);
	if (is_compiled) {
		if (!param.is_passive)
			setIcon(0,QIcon(":/images/greenball.bmp"));
		else
			setIcon(0,QIcon(":/images/redball.bmp"));
	}
}
STMetaNodeParameter STNodeViewParameterItem::parameter() {
	m_param.type=text(0);
	m_param.name=text(1);
	m_param.value=text(2);
	m_param.units=text(3);
	m_param.global_parameter=text(4);
	return m_param;
}

STNodeView::STNodeView(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);
	m_node=0;
	m_is_compiled=false;
	ui.parameters->setColumnCount(5);
	QStringList labels; labels << "Type" << "Name" << "Value" << "Units" << "Global Parameter";
	ui.parameters->setHeaderLabels(labels);
	connect(ui.parameters,SIGNAL(itemActivated(QTreeWidgetItem *,int)),this,SLOT(slot_parameter_activated(QTreeWidgetItem *,int)));
	connect(ui.parameters,SIGNAL(itemChanged(QTreeWidgetItem *,int)),this,SLOT(slot_parameter_changed(QTreeWidgetItem *,int)));
	connect(ui.parameters,SIGNAL(itemRightClicked(QTreeWidgetItem *,QPoint)),this,SLOT(slot_item_right_clicked(QTreeWidgetItem *,QPoint)));
	//connect(ui.code_button,SIGNAL(clicked()),this,SIGNAL(editCode()));
	
	m_action_minimize_parameter=popup_menu.addAction("Minimize parameter",this,SLOT(slot_minimize_parameter()));
	m_action_maximize_parameter=popup_menu.addAction("Maximize parameter",this,SLOT(slot_maximize_parameter()));
	m_action_link_to_global_parameter=popup_menu.addAction("Link to global parameter",this,SLOT(slot_link_to_global_parameter()));
	m_action_unlink_global_parameter=popup_menu.addAction("Unlink global parameter",this,SLOT(slot_unlink_global_parameter()));
}
void STNodeView::setNode(STMetaNode *N) {
	m_node=N;
	updateControls();
}

STMetaNode *STNodeView::node() {
	return m_node;
}

void STNodeView::updateControls() {
	ui.parameters->clear();
	//ui.node_name_and_type->setText("");
	//ui.code_button->setEnabled(false);
	if (!m_node) return;
	//ui.code_button->setEnabled(true);
	QString str=QString("%1 (%2)").arg(m_node->name()).arg(m_node->className());
	//ui.node_name_and_type->setText(str);
	for (int j=0; j<m_node->parameterCount(); j++) {
		STNodeViewParameterItem *item=new STNodeViewParameterItem(m_node->parameter(j),m_is_compiled);
		ui.parameters->addTopLevelItem(item);
	}
}

void STNodeView::slot_parameter_activated(QTreeWidgetItem *item,int column) {
	if (column==4) {
		slot_link_to_global_parameter();
		return;
	}
	Qt::ItemFlags holdflags=item->flags();
	item->setFlags(holdflags|Qt::ItemIsEditable);
	ui.parameters->editItem(item,2);
	item->setFlags(holdflags);
	
}
void STNodeView::slot_parameter_changed(QTreeWidgetItem *item,int column) {
	Q_UNUSED(column);
	STNodeViewParameterItem *item2=(STNodeViewParameterItem *)item;
	if (!item2) return;
	if (!m_node) return;
	STMetaNodeParameter newparam=item2->parameter();
	STMetaNodeParameter param=m_node->parameter(newparam.name);
	QString compare1=param.value; compare1.remove(' ');
	QString compare2=newparam.value; compare2.remove(' ');
	if (compare1==compare2) return; //nothing really changed
	param.value=newparam.value;
	m_node->setParameter(newparam.name,param);
	emit parameterChanged(newparam.name);
	
}

void STNodeView::slot_minimize_parameter() {
	STNodeViewParameterItem *item=(STNodeViewParameterItem *)ui.parameters->currentItem();
	if (!item) return;
	emit minimizeParameter(item->parameter().name);
}
void STNodeView::slot_maximize_parameter() {
	STNodeViewParameterItem *item=(STNodeViewParameterItem *)ui.parameters->currentItem();
	if (!item) return;
	emit maximizeParameter(item->parameter().name);
}

void STNodeView::slot_link_to_global_parameter() {
	STNodeViewParameterItem *item=(STNodeViewParameterItem *)ui.parameters->currentItem();
	if (!item) return;
	emit linkToGlobalParameter(item->parameter().name);
}

void STNodeView::slot_unlink_global_parameter() {
	STNodeViewParameterItem *item=(STNodeViewParameterItem *)ui.parameters->currentItem();
	if (!item) return;
	emit unlinkGlobalParameter(item->parameter().name);
}

void STNodeView::slot_item_right_clicked(QTreeWidgetItem *item,QPoint pos) {
	STNodeViewParameterItem *item2=(STNodeViewParameterItem *)item;
	if (!item2) return;
	if (item2->parameter().type!="STReal") {
		m_action_minimize_parameter->setEnabled(false);
		m_action_maximize_parameter->setEnabled(false);
	}
	else {
		m_action_minimize_parameter->setEnabled(true);
		m_action_maximize_parameter->setEnabled(true);
	}
	if (item2->parameter().global_parameter.isEmpty()) {
		m_action_unlink_global_parameter->setEnabled(false);
	}
	else {
		m_action_unlink_global_parameter->setEnabled(true);
	}
	popup_menu.popup(mapToGlobal(pos));	
}




