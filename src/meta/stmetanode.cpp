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


#include "stmetanode.h"
#include "stmetasequence.h"

QString qstr(const SString &X) {
	#ifdef USING_SSTRING
	return QString(X.data());
	#else
	return X;
	#endif
}

SString sstr(const QString &X) {
	return SString(X.toAscii().data());
}

STMetaNode::STMetaNode() {
	m_parent=0;
	m_node_index=-1;
	m_max_node_index=-1;
	m_tree_index=-3;
	m_link_group=-1;
}
STMetaNode::~STMetaNode() {
	clearChildren();
	clearParameters();
}


void STMetaNode::loadFromText(const QList<QString> &txtlines,long &index) {
	bool done=false;
	while ((!done)&&(index<txtlines.count())) {
		QString line=txtlines[index];
		index++;
		line=line.trimmed();
		QList<QString> vals=line.split("\t");
		if (vals.value(0)=="END")
			done=true;
		else if (vals.value(0)=="PARAMETER") {
			STMetaNodeParameter param;
			param.type=vals.value(1);
			param.name=vals.value(2);
			param.value=vals.value(3);
			param.units=vals.value(4);
			param.is_passive=(vals.value(5)=="passive");
			param.global_parameter=vals.value(6);
			setParameter(param.name,param);
		}
		else if (vals.value(0)=="CHILD") {
			STMetaNode *N=addChild(vals.value(2));
			N->setClassName(vals.value(1));
			N->loadFromText(txtlines,index);
		}
	}
}
void STMetaNode::saveToText(QList<QString> &txtlines,QString spaces) {
	for (int j=0; j<parameterCount(); j++) {
		STMetaNodeParameter param=parameter(j);
		QString passive_str="active";
		if (param.is_passive) passive_str="passive";
		txtlines << spaces+QString("PARAMETER\t%1\t%2\t%3\t%4\t%5\t%6")
							.arg(param.type)
							.arg(param.name)
							.arg(param.value)
							.arg(param.units)
							.arg(passive_str)
							.arg(param.global_parameter);
	}
	for (int j=0; j<childCount(); j++) {
		STMetaNode *C=child(j);
		txtlines << spaces+QString("CHILD\t%1\t%2").arg(C->className()).arg(C->name());
		C->saveToText(txtlines,spaces+"    ");
	}
	txtlines << spaces+"END";
}

/*void STMetaNode::createFromNode(STNode *node) {
	clearChildren();
	clearParameters();
	setName(qstr(node->name()));
	setClassName(qstr(node->objectType()));
	for (int j=0; j<node->parameterCount(); j++) {
		STMetaNodeParameter param;
		STParameter *P=node->parameter(j);
		param.type=qstr(P->objectType());
		param.name=qstr(P->name());
		param.value=qstr(P->toString());
		param.units="";
		param.is_passive=(P->hasBeenSet());
		setParameter(qstr(P->name()),param);
	}
	for (int j=0; j<node->childCount(); j++) {
		STNode *N=node->child(j);
		STMetaNode *C=addChild(qstr(N->name()));
		C->createFromNode(N);
	}
}*/

QString STMetaNode::className() {return m_class_name;}
void STMetaNode::setClassName(QString name) {m_class_name=name;}
QString STMetaNode::name() {return m_name;}
void STMetaNode::setName(QString name) {m_name=name;}

int STMetaNode::parameterCount() {return m_parameters.count();}
STMetaNodeParameter STMetaNode::parameter(int index) {
	return m_parameters[index];
}
STMetaNodeParameter STMetaNode::parameter(QString name) {
	int index=find_parameter_index(name);
	if (index<0) {
		STMetaNodeParameter dummy;
		return dummy;
	}
	return m_parameters[index];
}
void STMetaNode::setParameter(QString name,STMetaNodeParameter param) {
	int index=find_parameter_index(name);
	if (index<0) {
		m_parameters << param;
	}
	else {
		m_parameters[index]=param;
	}
}
int STMetaNode::childCount() {return m_children.count();}
STMetaNode *STMetaNode::child(int index) {
	return m_children[index];
}
STMetaNode *STMetaNode::child(QString name) {
	int index=find_child_index(name);
	if (index<0) {
		return 0;
	}
	return m_children[index];
}
STMetaNode *STMetaNode::addChild(QString name) {
	if (child(name)) return child(name);
	STMetaNode *C=new STMetaNode;
	C->setName(name);
	addChild(C);
	return C;
}

void STMetaNode::setParent(STMetaNode *par) {m_parent=par;}
STMetaNode *STMetaNode::parent() {return m_parent;}

void STMetaNode::clearChildren() {
	qDeleteAll(m_children);
	m_children.clear();
}
void STMetaNode::clearParameters() {
	m_parameters.clear();
}

int STMetaNode::find_child_index(QString name) {
	for (int j=0; j<m_children.count(); j++)
		if (m_children[j]->name()==name)
			return j;
	return -1;
}
int STMetaNode::find_parameter_index(QString name) {
	for (int j=0; j<m_parameters.count(); j++)
		if (m_parameters[j].name==name)
			return j;
	return -1;
}
void STMetaNode::setNodeIndices(int *index) {
	if (!index) {
		int index2=0;
		setNodeIndices(&index2);
	}
	else {
		m_node_index=(*index);
		(*index)++;
		for (int j=0; j<childCount(); j++)
			child(j)->setNodeIndices(index);
		m_max_node_index=(*index)-1;
	}
}


void STMetaNode::update(STMetaSequence *Seq) {
	bool had_children=(childCount()>0); //stores whether this node had children before the update
	STMetaClass *C=Seq->metaClass(className());
	if (C) {
		//hold_parameters contains the parameters of this node
		QHash<QString,STMetaNodeParameter> hold_parameters;
		//hold_children contains the children of this node
		QHash<QString,STMetaNode *> hold_children;
		
		for (int j=0; j<m_parameters.count(); j++) 
			hold_parameters[m_parameters[j].name]=m_parameters[j];
		m_parameters.clear();
		
		for (int j=0; j<m_children.count(); j++) 
			hold_children[m_children[j]->name()]=m_children[j];
		m_children.clear();
		
		QList<STMetaClass *> inherited_classes;
		inherited_classes << C;
		bool done=false;
		while (!done) {
			if (inherited_classes[0]->baseClassName().isEmpty())
				done=true;
			else {
				STMetaClass *base_class=Seq->metaClass(inherited_classes[0]->baseClassName());
				if (!base_class) done=true;
				else {
					inherited_classes.insert(0,base_class);
				}
			}
		}
		for (int cnum=inherited_classes.count()-1; cnum>=0; cnum--) {
			STMetaClass *Class=inherited_classes[cnum];
			for (int j=0; j<Class->parameterCount(); j++) {
				STMetaClassParameter P=Class->parameter(j);
				STMetaNodeParameter param;
				param.name=P.name;
				param.type=P.type;
				param.value=P.default_value;
				param.units=P.units;
				param.is_passive=false;
				if (hold_parameters.contains(param.name)) {
					param.is_passive=hold_parameters[param.name].is_passive;
					param.value=hold_parameters[param.name].value;
					param.global_parameter=hold_parameters[param.name].global_parameter;
				}
				addParameter(param);
				hold_parameters.remove(param.name);
			}
			
			for (int j=0; j<Class->childCount(); j++) {
				STMetaClassChild CH=Class->child(j);
				STMetaNode *ch=0;
				if (hold_children.contains(CH.name)) {
					ch=hold_children[CH.name];
					hold_children.remove(CH.name);
				}
				if (!ch) {
					ch=new STMetaNode;
					ch->setName(CH.name);
				}
				
				bool type_is_ok=false;
				if (ch->className()==CH.type)
					type_is_ok=true;
				else {
					STMetaClass *holdclass=Seq->metaClass(ch->className());
					//if ((holdclass)&&(holdclass->baseClassName()==CH.type))
					if ((holdclass)&&(Seq->inherits(holdclass,CH.type)))				
						type_is_ok=true;
				}
				if (!type_is_ok)
					ch->setClassName(CH.type);
				addChild(ch);
			}
		}
		qDeleteAll(hold_children);
	}
	for (int j=0; j<childCount(); j++) {
		child(j)->update(Seq);
	}
	if ((C)&&(!had_children)&&(childCount()>0)) { //children have been added... must need initialization
		for (int j=0; j<C->defaultValueCount(); j++) {
			QString dname=C->defaultName(j);
			QString dvalue=C->defaultValue(j);
			QString nodestring=Seq->nodeString(this)+"->"+dname;
			STMetaNode *N=Seq->findNodeFromString(nodestring);
			if (!N) { //must be setting parameter
				int index_of_last_arrow=dname.lastIndexOf("->");
				if (index_of_last_arrow>=0) {
					nodestring=Seq->nodeString(this)+"->"+dname.mid(0,index_of_last_arrow);
					QString param_name=dname.mid(index_of_last_arrow+2);
					N=Seq->findNodeFromString(nodestring);
					if (N) {
						STMetaNodeParameter P=N->parameter(param_name);
						if (!P.name.isEmpty()) {
							P.value=dvalue;
							N->setParameter(param_name,P);
						}
					}
				}
			}
			else { //must be promoting node
				N->setClassName(dvalue);
				N->update(Seq);
			}
		}
	}
}

void STMetaNode::addChild(STMetaNode *C) {
	m_children << C;
	C->setParent(this);
}
void STMetaNode::addParameter(STMetaNodeParameter P) {
	m_parameters << P;
}

void STMetaNode::setTreeIndices(int &ind) {
	m_tree_index=ind;
	ind++;
	for (int j=0; j<childCount(); j++)
		child(j)->setTreeIndices(ind);
}

void STMetaNode::copyFrom(STMetaNode &N) {
	clearChildren();
	clearParameters();
	setName(N.name());
	setClassName(N.className());
	for (int j=0; j<N.parameterCount(); j++) {
		addParameter(N.parameter(j));
	}
	for (int j=0; j<N.childCount(); j++) {
		STMetaNode *C=addChild(N.child(j)->name());
		C->copyFrom(*N.child(j));
	}
}

void STMetaNode::pasteValuesFrom(STMetaNode &N) {
	for (int j=0; j<N.parameterCount(); j++) {
		STMetaNodeParameter P=N.parameter(j);
		for (int k=0; k<parameterCount(); k++) {
			if (parameter(k).name==P.name) {
				if (parameter(k).type==P.type) {
					m_parameters[k].value=P.value;
				}
			}
		}
	}
	for (int j=0; j<N.childCount(); j++) {
		STMetaNode *C=N.child(j);
		for (int k=0; k<childCount(); k++) {
			if (child(k)->name()==C->name()) {
				child(k)->pasteValuesFrom(*C);
			}
		}
	}
}
void STMetaNode::pasteStructureFrom(STMetaNode &N) {
	clearChildren();
	clearParameters();
	setClassName(N.className());
	for (int j=0; j<N.parameterCount(); j++) {
		STMetaNodeParameter P=N.parameter(j);
		addParameter(P);
	}
	for (int j=0; j<N.childCount(); j++) {
		STMetaNode *C=N.child(j);
		STMetaNode *C2=addChild(C->name());
		C2->pasteStructureFrom(*C);
	}
}

void STMetaNode::resetLinkGroups() {
	m_link_group=-1;
	for (int j=0; j<childCount(); j++)
		child(j)->resetLinkGroups();
}



