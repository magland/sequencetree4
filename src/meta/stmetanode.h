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


#ifndef stmetanode_H
#define stmetanode_H

#include <QDebug>

#include <QString>
#include <QHash>
#include <QStringList>
#include "sstring.h"

QString qstr(const SString &X);
SString sstr(const QString &X);

class STMetaSequence;

struct STMetaNodeParameter {
	QString type;
	QString name;
	QString value;
	QString units;
	bool is_passive;
	QString global_parameter;
};

class STMetaNode {
public:
	int m_node_index;
	int m_max_node_index;
	STMetaNode();
	virtual ~STMetaNode();
	QString className();
	void setClassName(QString name);
	QString name();
	void setName(QString name);
	
	int parameterCount();
	STMetaNodeParameter parameter(int index);
	STMetaNodeParameter parameter(QString name);
	void setParameter(QString name,STMetaNodeParameter param);
	int childCount();
	STMetaNode *child(int index);
	STMetaNode *child(QString name);
	STMetaNode *addChild(QString name);
	void addChild(STMetaNode *C);
	void addParameter(STMetaNodeParameter P);
	
	
	void setParent(STMetaNode *par);
	STMetaNode *parent();
	
	void clearChildren();
	void clearParameters();
	virtual void loadFromText(const QList<QString> &txtlines,long &index);
	virtual void saveToText(QList<QString> &txtlines,QString spaces="");
	
	//void createFromNode(STNode *node);
	
	int find_child_index(QString name);
	int find_parameter_index(QString name);
	void setNodeIndices(int *index=0);
	
	void update(STMetaSequence *Seq);
	void setTreeIndices(int &ind);
	int treeIndex() {return m_tree_index;}
	void copyFrom(STMetaNode &N);
	void pasteValuesFrom(STMetaNode &N);
	void pasteStructureFrom(STMetaNode &N);
	
	void resetLinkGroups();
	void setLinkGroup(int group_number) {m_link_group=group_number;}
	int linkGroup() {return m_link_group;}
private:
	QString m_name;
	QString m_class_name;
	QList<STMetaNodeParameter> m_parameters;
	QList<STMetaNode *> m_children;
	STMetaNode *m_parent;
	int m_tree_index;
	long m_link_group;
};

#endif
