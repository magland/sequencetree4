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


#include "stmetasequence.h"
#include "stcommon.h"
#include <QDir>
#include <QApplication>

STMetaSequence::STMetaSequence() {
	setName("Root");
	setClassName("STRoot");
	m_total_duration=0;
	m_num_blocks=0;
	m_resolution=0;
}
STMetaSequence::~STMetaSequence() {
	clearClasses();
}
QStringList STMetaSequence::metaClassNames() {
	return m_meta_classes.keys();
}
STMetaClass *STMetaSequence::metaClass(QString class_name) {
	if (class_name.isEmpty()) return 0;
	if (!m_meta_classes.contains(class_name)) return 0;
	return m_meta_classes[class_name];
}
void STMetaSequence::addMetaClass(STMetaClass *C) {
	m_meta_classes[C->className()]=C;
}

void STMetaSequence::clearClasses() {
	qDeleteAll(m_meta_classes);
	m_meta_classes.clear();
}

void STMetaSequence::loadFromText(const QList<QString> &txtlines,long &index) {
	clearClasses();
	m_link_groups.clear();
	m_global_parameters.clear();
	m_other_sequence_lines.clear();
	setNotes("");
	setReconstructionCode("");
	loadFoundationClasses(ST_FOUNDATION_DIR,false);
	loadFoundationClasses(ST_USER_DIR,true);	
	
	long N=txtlines.count();
	while (index<N) {
		QString line=txtlines[index];
		line=line.trimmed();
		index++;
		if (line.mid(0,12)=="++++++ NOTES") {
			QString hold_notes;
			bool done=false;
			while ((!done)&&(index<txtlines.count())) {
				QString line2=txtlines[index];
				index++;
				if (line2.mid(0,10)=="++++++ END") {
					done=true;
				}
				else {
					if (!hold_notes.isEmpty()) hold_notes+="\n";
					hold_notes+=line2;
				}
			}
			setNotes(hold_notes);
		}
		else if (line.mid(0,13)=="++++++ GLOBAL") {
			QHash<QString,QStringList> hold_connections;
			QHash<QString,QString> hold_values;
			QHash<QString,QString> hold_units;
			QStringList GP_names;
			bool done=false;
			while ((!done)&&(index<txtlines.count())) {
				QString line2=txtlines[index];
				index++;
				if (line2.mid(0,10)=="++++++ END") {
					done=true;
				}
				else {
					QStringList list=line2.split('\t');
					if (list.value(1)=="->") {
						if (!hold_connections.contains(list.value(0))) {
							GP_names << list.value(0);
						}
						hold_connections[list.value(0)].append(list.value(2));
					}
					else {
						if (!list.value(1).isEmpty()) hold_values[list.value(0)]=list.value(1);
						if (!list.value(2).isEmpty()) hold_units[list.value(0)]=list.value(2);
					}
				}
			}
			foreach (QString GP_name,GP_names) {
				STGlobalParameter GP;
				GP.name=GP_name;
				GP.value=hold_values[GP_name];
				GP.units=hold_units[GP_name];
				GP.connections=hold_connections[GP_name];
				m_global_parameters.append(GP);
			}
		}
		else if (line.mid(0,21)=="++++++ RECONSTRUCTION") {
			QString hold_code;
			bool done=false;
			while ((!done)&&(index<txtlines.count())) {
				QString line2=txtlines[index];
				index++;
				if (line2.mid(0,10)=="++++++ END") {
					done=true;
				}
				else {
					if (!hold_code.isEmpty()) hold_code+="\n";
					hold_code+=line2;
				}
			}
			setReconstructionCode(hold_code);
		}
		else if (line.mid(0,12)=="++++++ NODES") {
			STMetaNode::loadFromText(txtlines,index);	
		}
		else if (line.mid(0,12)=="++++++ CLASS") {
			QString class_name=line.mid(12);
			class_name.remove(" ");
			class_name.remove("+");
			STMetaClass *C=new STMetaClass;
			C->setClassName(class_name);
			addMetaClass(C);
			C->loadFromText(txtlines,index);
		}
		else if (line.mid(0,16)=="++++++ USERCLASS") {
			QString class_name=line.mid(16);
			class_name.remove(" ");
			class_name.remove("+");
			if (!metaClass(class_name)) { //does not already exist
				STMetaClass *C=new STMetaClass;
				C->setClassName(class_name);
				addMetaClass(C);
				C->loadFromText(txtlines,index);
				C->setFoundationClass(true);
				C->setUserDefined(true);
			}
		}
		else if (line.mid(0,18)=="++++++ LINK GROUPS") {
			bool done=false;
			while (!done) {
				QString line2=txtlines[index];
				line2=line2.trimmed();
				index++;
				if (line2.mid(0,10)=="++++++ END") {
					done=true;
				}
				else {
					QStringList list=line2.split('\t');
					if ((list.count()>0)&&(list[0]=="GROUP")) {
						STMetaSequenceLinkGroup newG;
						for (int ind=1; ind<list.count(); ind++)
							newG.node_strings << list[ind];
						m_link_groups << newG;
					}
				}
			}
		}
		else if (line.mid(0,16)=="++++++ RESOURCES") {
			bool done=false;
			SString holdtxt;
			while (!done) {
				QString line2=txtlines[index];
				index++;
				if (line2.mid(0,10)=="++++++ END") {
					done=true;
				}
				else {
					holdtxt.append(sstr(line2)+"\n");
				}
			}
			m_resources.readFromText(holdtxt);
		}
		/*else if (line.mid(0,7)=="++++++ ") {
			m_other_sequence_lines << line;
			bool done=false;
			while ((!done)&&(index<txtlines.count())) {
				QString line2=txtlines[index];
				index++;
				if (line2.mid(0,10)=="++++++ END") {
					done=true;
				}
				m_other_sequence_lines << line2;
			}
		}*/
	}
	updateLinkGroups();
	synchronize_global_parameters();
}

QString get_ST_version() {
	QString ret=read_text_file(ST_ROOT_DIR+"/stversion.txt");
	QStringList list=ret.split("\n");
	return list.value(0);
}

void STMetaSequence::saveToText(QList<QString> &txtlines,QString spaces) {
	txtlines << "++++++ HEADER ++++++";
	txtlines << QString("STVERSION=%1").arg(get_ST_version());
	txtlines << "++++++ END HEADER ++++++";
	txtlines << "++++++ NOTES ++++++";
	QStringList splitnotes=(notes().trimmed()+"\n").split("\n");
	foreach (QString notesline,splitnotes) {
		txtlines << notesline;
	}
	txtlines << "++++++ END NOTES ++++++";
	
	txtlines << "++++++ GLOBAL ++++++";
	for (int j=0; j<globalParameterCount(); j++) {
		STGlobalParameter *GP=globalParameter(j);
		txtlines << QString("%1\t%2\t%3").arg(GP->name).arg(GP->value).arg(GP->units);
	}
	for (int j=0; j<globalParameterCount(); j++) {
		STGlobalParameter *GP=globalParameter(j);
		foreach (QString connection,GP->connections) {
			txtlines << QString("%1\t->\t%2").arg(GP->name).arg(connection);
		}
	}
	txtlines << "++++++ END GLOBAL ++++++";
	
	txtlines << "++++++ RECONSTRUCTION ++++++";
	QStringList splitcode=(reconstructionCode().trimmed()+"\n").split("\n");
	foreach (QString codeline,splitcode) {
		txtlines << codeline;
	}
	txtlines << "++++++ END NOTES ++++++";
	
	txtlines << "++++++ LINK GROUPS ++++++";
	txtlines << "";
	for (int j=0; j<m_link_groups.count(); j++) {
		QString line="GROUP";
		for (int k=0; k<m_link_groups[j].node_strings.count(); k++) {
			line+="\t"+m_link_groups[j].node_strings[k];
		}
		txtlines << line;
	}
	txtlines << "";
	txtlines << "++++++ END LINK GROUPS ++++++";
	
	txtlines << "";
	txtlines << "";
	
	txtlines << "++++++ NODES ++++++";
	txtlines << "";
	STMetaNode::saveToText(txtlines,spaces);
	txtlines << "";
	txtlines << "++++++ END NODES ++++++";
	
	QStringList classnames=metaClassNames();
	qSort(classnames);
	foreach (QString cname,classnames) {
		if (!metaClass(cname)->isFoundationClass()) {
			txtlines << "";
			txtlines << QString("++++++ CLASS %1 ++++++").arg(metaClass(cname)->className());
			metaClass(cname)->saveToText(txtlines,spaces);
		}
		else if (metaClass(cname)->isUserDefined()) {
			txtlines << "";
			txtlines << QString("++++++ USERCLASS %1 ++++++").arg(metaClass(cname)->className());
			metaClass(cname)->saveToText(txtlines,spaces);
		}
	}
	
	txtlines << "++++++ RESOURCES ++++++";
	txtlines << "";
	SString holdtxt;
	m_resources.writeToText(holdtxt);
	QList<QString> holdlist=qstr(holdtxt).split("\n");
	foreach (QString holdline, holdlist)
		txtlines << holdline;
	txtlines << "";
	txtlines << "++++++ END RESOURCES ++++++";
	
	txtlines << "";
	foreach (QString other_line, m_other_sequence_lines)
		txtlines << other_line;
	txtlines << "";
}

void STMetaSequence::loadFoundationClasses(QString directory,bool user_defined) {
	QDir dir(directory);
	QStringList list=dir.entryList(QStringList("*.h"));
	foreach (QString str,list) {
		QString header_fname=directory+"/"+str;
		QString source_fname=directory+"/"+QFileInfo(str).baseName()+".cpp";
		QString header=read_text_file(header_fname);
		QString source=read_text_file(source_fname);
		QStringList header_lines=header.split("\n");
		QStringList source_lines=source.split("\n");
		header="";
		source="";
		bool exclude_mode=false;
		foreach (QString line,header_lines) {
			if (line.contains("BEGIN EXCLUDE")) exclude_mode=true;
			else if (line.contains("END EXCLUDE")) exclude_mode=false;
			else if (!exclude_mode) {
				if (!header.isEmpty()) header+="\n";
				header+=line;
			}
		}
		exclude_mode=false;
		foreach (QString line,source_lines) {
			if (line.contains("BEGIN EXCLUDE")) exclude_mode=true;
			else if (line.contains("END EXCLUDE")) exclude_mode=false;
			else if (!exclude_mode) {
				if (!source.isEmpty()) source+="\n";
				source+=line;
			}
		}
		STMetaClass *C=new STMetaClass;
		C->setCode(header,source);
		if (C->className().isEmpty()) {
			delete C;
		}
		else {
			C->setFoundationClass(true);
			C->setUserDefined(user_defined);
			if (metaClass(C->className())) {
				qWarning() << "Class has already been loaded:" << C->className();
				delete C;
			}
			else {
				C->setHeaderFileName(header_fname);
				addMetaClass(C);
			}
		}
	}
	
}

void STMetaSequence::update() {
	STMetaNode::update(this);
	int ind=0;
	STMetaNode::setTreeIndices(ind);
	synchronize_global_parameters();
}

bool STMetaSequence::inherits(QString class_name,QString base_class_name) {
	STMetaClass *C=metaClass(class_name);
	if (!C) return false;
	return inherits(C,base_class_name);
}
bool STMetaSequence::inherits(STMetaClass *C,QString base_class_name) {
	int ct=0;
	STMetaClass *ptr=C;
	if (!ptr) return false;
	if (ptr->className()==base_class_name) return true;
	while ((ptr)&&(ct<500)) {
		if (ptr->baseClassName()==base_class_name) return true;
		ptr=metaClass(ptr->baseClassName());	
		ct++;
	}	
	if (ct>=500) {
		qWarning() << "There appears to be a cycle in the inheritance graph!!!";
	}
	return false;
}

QStringList STMetaSequence::classesThatInherit(QString base_class_name,QString exclude_base_class_name) {
	QStringList ret;
	QStringList cnames=metaClassNames();
	foreach (QString cname,cnames) {
		if (inherits(cname,base_class_name)) {
			if ((exclude_base_class_name.isEmpty())||(!inherits(cname,exclude_base_class_name)))
				ret << cname;
		}
	}
	return ret;
}

STMetaClass *STMetaSequence::duplicateClass(STMetaClass *C) {
	STMetaClass *C2=new STMetaClass;
	QString class_name=C->className();
	QString new_class_name=get_unused_class_name(class_name);
	QString header_code=C->headerCode();
	QString source_code=C->sourceCode();
	replace_word_in_text(header_code,class_name,new_class_name);
	replace_word_in_text(source_code,class_name,new_class_name);
	C2->setCode(header_code,source_code);
	addMetaClass(C2);
	return C2;
}
void STMetaSequence::removeClass(QString cname) {
	STMetaClass *C=metaClass(cname);
	if (!C) return;
	m_meta_classes.remove(cname);
	delete C;
}

QString STMetaSequence::get_unused_class_name(QString class_name) {
	QString new_class_name=class_name;
	bool done=false;
	while (!done) {
		if (!metaClass(new_class_name)) {
			done=true;
		}
		else {
			QStringList list=new_class_name.split("_");
			QString lastword="";
			if (list.count()>0) lastword=list.value(list.count()-1);
			bool only_digits=true;
			for (int j=0; j<lastword.count(); j++)
				if (!lastword[j].isDigit()) only_digits=false;
			if ((lastword.count()>0)&&(only_digits)) {
				int num=lastword.toInt();
				new_class_name=new_class_name.mid(0,new_class_name.count()-lastword.count())+QString::number(num+1);
			}
			else {
				new_class_name=new_class_name+"_1";
			}
		}
	}
	return new_class_name;
}
void STMetaSequence::replace_word_in_text(QString &txt,QString old_word,QString new_word) {
	txt.replace(QRegExp("\\b"+old_word+"\\b"),new_word);
	txt.replace(QRegExp("\\b"+old_word+"_H\\b"),new_word+"_H");
}

void STMetaSequence::loadParametersFromFile(QString file_name) {
	QString txt=read_text_file(file_name);
	QStringList list=txt.split('\n');
	STMetaNode *ptr=this;
	int active_level=0;
	for (long j=0; j<list.count(); j++) {
		QString line=list[j];
		QStringList vals=line.split('\t');
		if (vals.value(0)=="CHILD") {
			if (active_level>0) active_level++;
			else {			
				if (ptr->child(vals.value(1))) {
					ptr=ptr->child(vals.value(1));
					ptr->setClassName(vals.value(2));
				}
				else {
					active_level++;
				}
			}
		}
		else if (vals.value(0)=="ENDCHILD") {
			if (active_level>0) active_level--;
			else ptr=ptr->parent();
		}
		else if (vals.value(0)=="PARAM") {
			if (active_level==0) {
				STMetaNodeParameter param=ptr->parameter(vals.value(1));
				if (param.name==vals.value(1)) {
					param.value=vals.value(2);
					param.is_passive=(vals.value(3)=="passive");
					ptr->setParameter(vals.value(1),param);
				}
			}
		}
	}
}
void STMetaSequence::writeParametersToFile(QString file_name) {
	QString txt;
	write_parameters_to_text(txt,this);
	write_text_file(file_name,txt);
}
void STMetaSequence::write_parameters_to_text(QString &txt,STMetaNode *ptr) {
	if (!ptr) ptr=this;
	for (int j=0; j<ptr->parameterCount(); j++) {
		STMetaNodeParameter param=ptr->parameter(j);
		QString str="PARAM\t"+param.name+"\t"+param.value;
		txt+=str+"\n";
	}
	for (int j=0; j<ptr->childCount(); j++) {
		STMetaNode *ch=ptr->child(j);
		QString str="CHILD\t"+ch->name()+"\t"+ch->className();
		txt+=str+"\n";
		write_parameters_to_text(txt,ch);
		txt+="ENDCHILD\n";
	}
}

void STMetaSequence::writeFoundationClassesHeader(QString fname) {
	QString txt;
	txt += "#ifndef foundationclasses_H\n";
	txt += "#define foundationclasses_H\n";
	txt += "\n";
	txt += "#include \"st4.h\"\n";
	txt += "\n";
	txt += "\n";
	
	QStringList foundation_class_names=metaClassNames();
	for (int j=0; j<foundation_class_names.count(); j++) {
		if ((!metaClass(foundation_class_names[j])->isFoundationClass())||(metaClass(foundation_class_names[j])->isUserDefined())) {
			foundation_class_names.removeAt(j);
			j--;
		}
	}
	foundation_class_names.removeAll("STSequence");
	
	//sort appropriately
	int ct=0;
	bool done=false;
	while (!done) {	
		bool something_changed=false;
		for (int j=0; j<foundation_class_names.count(); j++) {
			QString base_class=metaClass(foundation_class_names[j])->baseClassName();
			int index=foundation_class_names.indexOf(base_class);
			if (index>j) {
				foundation_class_names.removeAt(index);
				foundation_class_names.insert(j,base_class);				
				something_changed=true;
			}
		}
		if (!something_changed) done=true;
		ct++;
		if (ct>100) done=true;
	}	
	
	foreach (QString cname,foundation_class_names) {
		txt+= QString("class %1;\n").arg(cname);
	}
	
	txt += "\n";
	
	foreach (QString cname,foundation_class_names) {
		txt += QString("////// %1 //////").arg(cname);
		txt += "\n";
		txt += metaClass(cname)->headerCode();
		txt += "\n";
		txt += "\n";
	}
	
	txt += "#endif\n";
	
	write_text_file_if_needed(fname,txt);
}
void STMetaSequence::writeFoundationClassesSource(QString fname) {
	QString txt;
	txt += "#include \"foundationclasses.h\"\n";
	txt += "\n";
	
	QStringList foundation_class_names=metaClassNames();
	for (int j=0; j<foundation_class_names.count(); j++) {
		if ((!metaClass(foundation_class_names[j])->isFoundationClass())||(metaClass(foundation_class_names[j])->isUserDefined())) {
			foundation_class_names.removeAt(j);
			j--;
		}
	}
	foundation_class_names.removeAll("STSequence");
	
	foreach (QString cname,foundation_class_names) {
		txt += QString("////// %1 //////").arg(cname);
		txt += "\n";
		txt += metaClass(cname)->sourceCode();
		txt += "\n";
		txt += "\n";
	}
	
	write_text_file_if_needed(fname,txt);
}



void STMetaSequence::writeCustomClassesHeader(QString fname) {
	QString txt;
	txt += "#ifndef customclasses_H\n";
	txt += "#define customclasses_H\n";
	txt += "\n";
	txt += "#include \"foundationclasses.h\"\n";
	txt += "\n";
	
	QSet<QString> used_class_names=usedClassNames();
	
	QStringList custom_class_names=metaClassNames();
	for (int j=0; j<custom_class_names.count(); j++) {
		if ((metaClass(custom_class_names[j])->isFoundationClass())&&(!metaClass(custom_class_names[j])->isUserDefined())) {
			custom_class_names.removeAt(j);
			j--;
		}
		else if (!used_class_names.contains(custom_class_names[j])) {		
			custom_class_names.removeAt(j);
			j--;
		}	
	}
	foreach (QString cname,custom_class_names) {
		txt+= QString("class %1;\n").arg(cname);
	}
	
	txt += "\n";
	
	foreach (QString cname,custom_class_names) {
		txt += QString("////// %1 //////").arg(cname);
		txt += "\n";
		txt += QString("#line %1 \"header-of-%2\"\n").arg(1).arg(cname);
		txt += metaClass(cname)->headerCode();
		txt += "\n";
		txt += "\n";
	}
	
	txt += "#endif\n";
	
	write_text_file_if_needed(fname,txt);
}


void STMetaSequence::writeCustomClassesSource(QString fname) {
	QString txt;
	txt += "#include \"customclasses.h\"\n";
	txt += "\n";
	
	QSet<QString> used_class_names=usedClassNames();
	
	QStringList custom_class_names=metaClassNames();
	for (int j=0; j<custom_class_names.count(); j++) {
		if ((metaClass(custom_class_names[j])->isFoundationClass())&&(!metaClass(custom_class_names[j])->isUserDefined())) {
			custom_class_names.removeAt(j);
			j--;
		}
		else if (!used_class_names.contains(custom_class_names[j])) {		
			custom_class_names.removeAt(j);
			j--;
		}	
	}
	
	foreach (QString cname,custom_class_names) {
		txt += QString("////// %1 //////").arg(cname);
		txt += "\n";
		txt += QString("#line %1 \"source-of-%2\"\n").arg(1).arg(cname);
		txt += metaClass(cname)->sourceCode();
		txt += "\n";
		txt += "\n";
	}
	
	write_text_file_if_needed(fname,txt);
}

struct UIParameter {
	QString name;
	QString label;
	QString type;
	QString units;
	double min,max,step;
	double factor;
	QString defaultval;
};

QList<UIParameter> load_ui_parameters(STMetaSequence *Seq) {
	QList<UIParameter> ret;
	QString notes_txt=Seq->notes();
	QList<QString> notes_lines=notes_txt.split("\n");
	foreach (QString line,notes_lines) {
		QStringList list=line.split("\t",QString::SkipEmptyParts);
		if (list.value(0).mid(0,2)=="UI") {
			QHash<QString,QString> uivalues;
			uivalues["type"]=list.value(0);
			for (int j=1; j<list.count(); j++) {
				QString hold=list[j];
				QStringList holdlist=hold.split("=");
				if (holdlist.count()==2) {
					holdlist[1].remove("\""); //remove quotes
					uivalues[holdlist[0].toLower()]=holdlist[1].trimmed();
				}
			}
			if (uivalues.contains("name")) {
				UIParameter new_record;
				new_record.name=uivalues["name"];
				new_record.label=uivalues["label"];
				new_record.units=uivalues["units"];
				new_record.type=uivalues["type"];
				QString range=uivalues["range"];
				QStringList rangelist=range.split(":");
				if (rangelist.count()==1) {
					rangelist.append("1");
					rangelist.append(rangelist.value(0));
				}
				else if (rangelist.count()==2) {
					rangelist.insert(1,"1");
				}
				new_record.min=rangelist.value(0).toDouble();
				new_record.step=rangelist.value(1).toDouble();
				new_record.max=rangelist.value(2).toDouble();
				new_record.factor=uivalues["factor"].toDouble();
				if (new_record.factor==0) new_record.factor=1;
				if (Seq->globalParameter(new_record.name)) {
					new_record.defaultval=Seq->globalParameter(new_record.name)->value;
					ret << new_record;
				}
			}
		}
		else if (list.value(0)=="RESOLUTION") {
			Seq->setResolution(list.value(1).toDouble());
		}
	}
	//put standard parameters at end
	QStringList standard_parameter_names;
	standard_parameter_names << "TE" << "TR" << "flip_angle";
	QList<UIParameter> ret2;
	for (int passnum=1; passnum<=2; passnum++) {
		for (int j=0; j<ret.count(); j++) {
			bool is_standard=standard_parameter_names.contains(ret[j].name);
			if ((passnum==1)&&(!is_standard)) {
				ret2 << ret[j];			
			}
			if ((passnum==2)&&(is_standard)) {
				ret2 << ret[j];			
			}
		}
	}
	return ret2;
}

void STMetaSequence::writeCreateSequenceSource(QString fname,QString sequence_export_name) {
	QString txt;
	txt += "#include \"createsequence.h\"\n";
	txt += "#include \"foundationclasses.h\"\n";
	txt += "#include \"customclasses.h\"\n";
	txt += "#include \"loadparametersfile.h\"\n";
	txt += "#include <string.h>\n";
	txt += "\n";

	txt += "STSequence *createSequence(char *parameter_fname) {\n";
	txt += "\tSTRoot *Root=new STRoot;\n";
	
	create_code_for_promoting_nodes(this,txt);
	
	txt += "\tif (parameter_fname) {\n";
	txt += "\t\tRoot->loadParametersFromFile(parameter_fname);\n";
	txt += "\t}\n";
	txt += "\telse {\n";
	txt += "\t\tSString parameter_txt;\n";
	txt += "\t\tloadParametersFile(parameter_txt);\n";
	txt += "\t\tRoot->loadParametersFromText(parameter_txt);\n";
	txt += "\t}\n";
	txt += "\tSString resource_text;\n";
	txt += "\tloadResourceText(resource_text);\n";	
	txt += "\tRoot->resources()->readFromText(resource_text);\n";

	for (int j=0; j<globalParameterCount(); j++) {
		STGlobalParameter *GP=globalParameter(j);
		for (int k=0; k<GP->connections.count(); k++) {
			txt+=QString("\tRoot->addGlobalParameterConnection(\"%1\",\"%2\");\n").arg(GP->name).arg(GP->connections[k]);
		}
	}
	
	txt += "\treturn Root;\n";
	txt += "}\n";
	txt += "\n";
	
	txt += "void getSequenceName(char *ret) {\n";
	txt += QString("strcpy(ret,\"%1\");\n").arg(sequence_export_name);
	txt += "}\n";
	txt += "\n";
	
	txt += "bool setGlobalParameter(STSequence *Seq,SString pname,SString pval) {\n";
	txt += "\tSTParameter *P;\n";
	txt += "\tbool ret=true;\n";
	for (int j=0; j<globalParameterCount(); j++) {
		STGlobalParameter *GP=globalParameter(j);
		if (j==0)
			txt += QString("\tif (pname==\"%1\") {\n").arg(GP->name);
		else
			txt += QString("\telse if (pname==\"%1\") {\n").arg(GP->name);
		for (int k=0; k<GP->connections.count(); k++) {
			txt += QString("\t\tP=Seq->findParameterFromString(\"%1\");\n").arg(GP->connections[k]);
			txt += "\t\tif (P) P->setFromString(pval);\n";
			txt += "\t\telse ret=false;\n";
		}
		txt += "\t}\n";
	}
	if (globalParameterCount()>0) {
		txt += "\telse ret=false;\n";
	}
	txt += "\treturn ret;\n";
	txt += "}\n";
	txt += "\n";
	
	QList<UIParameter> ui_parameters=load_ui_parameters(this);
	for (int j=0; j<ui_parameters.count(); j++) {
		if (!globalParameter(ui_parameters[j].name)) {
			ui_parameters.removeAt(j);
		}
	}
	
	txt += "int uiParameterCount() {\n";
	txt += QString("\treturn %1;\n").arg(ui_parameters.count());
	txt += "}\n";
	txt += "const char *uiParameterName(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return \"%2\";\n").arg(j).arg(ui_parameters[j].name);
	}
	txt += "\treturn \"\";\n";
	txt += "}\n";
	txt += "const char *uiParameterLabel(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return \"%2\";\n").arg(j).arg(ui_parameters[j].label);
	}
	txt += "\treturn \"\";\n";
	txt += "}\n";
	txt += "const char *uiParameterType(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return \"%2\";\n").arg(j).arg(ui_parameters[j].type);
	}
	txt += "\treturn \"\";\n";
	txt += "}\n";
	txt += "const char *uiParameterUnits(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return \"%2\";\n").arg(j).arg(ui_parameters[j].units);
	}
	txt += "\treturn \"\";\n";
	txt += "}\n";
	txt += "double uiParameterMin(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return %2;\n").arg(j).arg(ui_parameters[j].min);
	}
	txt += "\treturn 1;\n";
	txt += "}\n";
	txt += "double uiParameterMax(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return %2;\n").arg(j).arg(ui_parameters[j].max);
	}
	txt += "\treturn 10;\n";
	txt += "}\n";
	txt += "double uiParameterStep(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return %2;\n").arg(j).arg(ui_parameters[j].step);
	}
	txt += "\treturn 1;\n";
	txt += "}\n";
	txt += "double uiParameterFactor(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return %2;\n").arg(j).arg(ui_parameters[j].factor);
	}
	txt += "\treturn 1;\n";
	txt += "}\n";
	txt += "const char *uiParameterDefault(int index) {\n";
	for (int j=0; j<ui_parameters.count(); j++) {
		txt += QString("\tif (index==%1) return \"%2\";\n").arg(j).arg(ui_parameters[j].defaultval);
	}
	txt += "\treturn \"1\";\n";
	txt += "}\n";
	txt += "int getResolution() {\n";
		txt += QString("return %1;\n").arg(resolution());
	txt += "}\n";
	write_text_file_if_needed(fname,txt);

}

void STMetaSequence::writeLoadParametersFileSource(QString fname) {
	
	
	QString txt;
	txt += "#include \"loadparametersfile.h\"\n";
	txt += "\n";
	{
		QString parameter_txt;
		write_parameters_to_text(parameter_txt);
		QStringList parameter_lines=parameter_txt.split('\n');
		if (parameter_lines.isEmpty()) parameter_lines << "";
		txt += "\n";
		txt += "static const unsigned char st_parameter_data[] = {\n";
		long ct=0;
		foreach (QString line,parameter_lines) {
			txt += "//"+line+"\n";
			line += "\n";
			for (int j=0; j<line.count(); j++) {
				int charcode=line[j].toAscii();
				txt+= QString("0x%1,").arg(QString::number(charcode,16));
				ct++;
			}
			txt += "\n";
		}
		txt += "};\n";
		txt += "\n";
		txt += "\n";
		txt += "void loadParametersFile(SString &ret) {\n";
		txt += QString("\tret.allocate(%1+10);\n").arg(ct);
		txt += QString("\tret=\"\";\n");
		txt += QString("\tfor (long j=0; j<%1; j++) {\n").arg(ct);
		txt += "\t\tret.append(st_parameter_data[j]);\n";
		txt += "\t}\n";
		txt += "}\n";
	}
	
	
	{
		SString resource_txt;
		m_resources.writeToText(resource_txt);
		QStringList resource_lines=qstr(resource_txt).split('\n');
		if (resource_lines.isEmpty()) resource_lines << "";
		txt += "\n";
		txt += "static const unsigned char st_resource_data[] = {\n";
		long ct=0;
		foreach (QString line,resource_lines) {
			txt += "//"+line+"\n";
			line += "\n";
			for (int j=0; j<line.count(); j++) {
				int charcode=line[j].toAscii();
				txt+= QString("0x%1,").arg(QString::number(charcode,16));
				ct++;
			}
			txt += "\n";
		}
		txt += "};\n";
		txt += "\n";
		txt += "\n";
		txt += "void loadResourceText(SString &ret) {\n";
		txt += QString("\tret.allocate(%1+10);\n").arg(ct);
		txt += QString("\tret=\"\";\n");
		txt += QString("\tfor (long j=0; j<%1; j++) {\n").arg(ct);
		txt += "\t\tret.append(st_resource_data[j]);\n";
		txt += "\t}\n";
		txt += "}\n";
	}
	
	write_text_file_if_needed(fname,txt);	
}

QString access_string(STMetaNode *N) {
	if (!N->parent()) return N->name();
	return access_string(N->parent())+"->"+N->name();
}

void STMetaSequence::create_code_for_promoting_nodes(STMetaNode *N,QString &output_txt) {
	STMetaNode *par=N->parent();
	STMetaClass *parclass=0; if (par) parclass=metaClass(par->className());
	if (parclass) {
		if (isPromoted(N)) {
			output_txt+="\t{STNode *pnode;\n";
			output_txt+=QString("\tpnode=Root->findNodeFromString(\"%1\");\n").arg(access_string(par));
			output_txt+=QString("\t((%1 *)pnode)->%2=(%3 *)pnode->replaceChild(((%1 *)pnode)->%2,new %3);\n")
						.arg(parclass->className())
						.arg(N->name())
						.arg(N->className());
			output_txt+="\t}\n";
			/*QString codeline=QString("%1=(%2 *)%3->replaceChild(%1,new %4);")
				.arg(access_string(N))
				.arg(parclass->child(N->name()).type)
				.arg(access_string(par))
				.arg(N->className());
			output_txt+="\t"+codeline+"\n";*/
		}
	}
	for (int j=0; j<N->childCount(); j++)
		create_code_for_promoting_nodes(N->child(j),output_txt);
}

void STMetaSequence::read(QString file_name) {
	long index=0;
	QString txt=read_text_file(file_name);
	QStringList txtlines=txt.split("\n");
	loadFromText(txtlines,index);
}

void STMetaSequence::write(QString file_name) {
	QList<QString> txtlines;
	saveToText(txtlines);
	QString txt;
	foreach (QString line,txtlines) {
		if (!txt.isEmpty()) txt+="\n";
		txt+=line;
	}
	write_text_file(file_name,txt);
}

STMetaNode *STMetaSequence::findNode(int tree_index,STMetaNode *start_at) {
	if (!start_at) start_at=this;
	if (start_at->treeIndex()==tree_index) return start_at;
	if (start_at->treeIndex()>tree_index) return 0;
	for (int chnum=1; chnum<start_at->childCount(); chnum++) {
		if (start_at->child(chnum)->treeIndex()>tree_index)
			return findNode(tree_index,start_at->child(chnum-1));
	}
	if (start_at->childCount()>0) {
		return findNode(tree_index,start_at->child(start_at->childCount()-1));
	}
	return 0;
}
STMetaNode *STMetaSequence::findNodeFromString(QString str) {
	QStringList list=str.split("->");
	if (list.isEmpty()) return 0;
	if (list[0]!="Root") return 0;
	int index=0;
	STMetaNode *ptr=this;
	while ((ptr)&&(index+1<list.count())) {
		ptr=ptr->child(list[index+1]);
		index++;
	}
	return ptr;
}
QString STMetaSequence::nodeString(STMetaNode *N) {
	if (!N) return "";
	QString ret=N->name();
	STMetaNode *ptr=N->parent();
	while (ptr) {
		ret=ptr->name()+"->"+ret;
		ptr=ptr->parent();
	}
	return ret;
}

void STMetaSequence::renameClass(QString old_name,QString new_name) {
	if (old_name==new_name) return;
	if (old_name=="STRoot") return;
	STMetaClass *C=metaClass(old_name);
	if (!C) return;
	if (C->isFoundationClass()) return;
	QStringList cnames=metaClassNames();
	foreach (QString cname,cnames) {
		STMetaClass *C2=metaClass(cname);
		if (!C2->isFoundationClass()) {
			for (int j=0; j<C2->childCount(); j++) {
				if (C2->child(j).type==old_name) {
					C2->changeChildType(C2->child(j).name,new_name);
				}
			}
		}
	}
	change_node_class_name(this,old_name,new_name);
	C->changeClassName(new_name);
	m_meta_classes[new_name]=C;
	m_meta_classes.remove(old_name);
}

bool STMetaSequence::isPromoted(STMetaNode *node) {
	if (!node->parent()) return false;
	STMetaClass *parC=metaClass(node->parent()->className());
	if (!parC) return false;
	if (parC->child(node->name()).type!=node->className())
		return true;
	return false;
}

void STMetaSequence::updateLinkGroups() {
	resetLinkGroups();
	for (int j=0; j<m_link_groups.count(); j++) {
		STMetaSequenceLinkGroup *G=&m_link_groups[j];
		for (int k=0; k<G->node_strings.count(); k++) {
			STMetaNode *N=findNodeFromString(G->node_strings[k]);
			if (N) {
				N->setLinkGroup(j);
			}
			else {
				G->node_strings.removeAt(k);
				k--;
			}
		}
		if (G->node_strings.count()==1) { //only one left, erase
			STMetaNode *N=findNodeFromString(G->node_strings[0]);
			N->setLinkGroup(-1);
			G->node_strings.clear();
		}
		if (G->node_strings.isEmpty()) {
			m_link_groups.removeAt(j);
			j--;
		}
	}
}
void STMetaSequence::linkNodes(STMetaNode *N1,STMetaNode *N2) {
	if (!N1) return;
	if (!N2) return;
	QString str1=nodeString(N1);
	QString str2=nodeString(N2);
	bool found=false;
	for (int j=0; (j<m_link_groups.count())&&(!found); j++) {
		if (m_link_groups[j].node_strings.contains(str1)) {
			if (m_link_groups[j].node_strings.contains(str2)) return; //already linked
			m_link_groups[j].node_strings.append(str2);
			found=true;
		}
		else if (m_link_groups[j].node_strings.contains(str2)) {
			if (m_link_groups[j].node_strings.contains(str1)) return; //already linked
			m_link_groups[j].node_strings.append(str1);
			found=true;
		}
	}
	if (!found) {
		STMetaSequenceLinkGroup newG;
		newG.node_strings.append(str1);
		newG.node_strings.append(str2);
		m_link_groups << newG;
	}
	updateLinkGroups();
}
void STMetaSequence::unlinkNode(STMetaNode *N) {
	if (!N) return;
	QString str=nodeString(N);
	for (int j=0; j<m_link_groups.count(); j++) {
		m_link_groups[j].node_strings.removeAll(str);
	}
	updateLinkGroups();
}

void STMetaSequence::handleLinksAfterNodeChanged(STMetaNode *N,QString pname) {
	if (!N) return;
	STMetaNode *ptr=N;
	while (ptr) {
		if (ptr->linkGroup()>=0) {
			int index=ptr->linkGroup();
			if (index<m_link_groups.count()) {
				STMetaSequenceLinkGroup G=m_link_groups[index];
				for (int j=0; j<G.node_strings.count(); j++) {
					STMetaNode *ptr2=findNodeFromString(G.node_strings[j]);
					if ((ptr2)&&(ptr2!=ptr)) {
						ptr2->pasteValuesFrom(*ptr);
						copy_global_links(ptr,ptr2);
					}
				}
			}
		}
		ptr=ptr->parent();
	}
	if (!N->parameter(pname).global_parameter.isEmpty()) {
		setGlobalParameterValue(N->parameter(pname).global_parameter,N->parameter(pname).value);
	}
}

QSet<QString> STMetaSequence::usedClassNames(STMetaNode *start_at) {
	if (!start_at) start_at=this;
	QSet<QString> ret;
	ret.insert(start_at->className());
	for (int j=0; j<start_at->childCount(); j++) {
		ret+=usedClassNames(start_at->child(j));
	}
	if (start_at==this) { //add all child types and base classes
		bool done=false;
		while (!done) {
			done=true;
			foreach (QString cname,ret) {
				STMetaClass *C2=metaClass(cname);
				if (C2) {
					if (!ret.contains(C2->baseClassName())) {
						ret.insert(C2->baseClassName());
						done=false;
					}
					for (int j=0; j<C2->childCount(); j++) {
						if (!ret.contains(C2->child(j).type)) {
							ret.insert(C2->child(j).type);
							done=false;
						}
					}
				}
			}
		}
	}
	return ret;
}

void STMetaSequence::deleteClass(QString class_name) {
	if (m_meta_classes.contains(class_name)) {
		delete m_meta_classes[class_name];
		m_meta_classes.remove(class_name);
	}
}
bool STMetaSequence::exportClass(QString class_name,QString fname) {
	STMetaClass *C=metaClass(class_name);
	if (!C) return false;
	if ((C->isFoundationClass())&&(!C->isUserDefined())) {
		qWarning() << __FUNCTION__ << "Cannot export foundation class.";
		return false;
	}
	if (!C->headerFileName().isEmpty()) {
		qWarning() << __FUNCTION__ << "Cannot export foundation class that is already on disk.";
		return false;
	}
	int ct=fname.count();
	if (fname.mid(ct-2)!=".h") {
		qWarning() << __FUNCTION__ << "Cannot export class, filename must end with .h.";
		return false;	
	}
	QString header_fname=ST_USER_DIR+"/"+fname;
	QString source_fname=ST_USER_DIR+"/"+fname.mid(0,ct-2)+".cpp";
	if (QFile(header_fname).exists()) {
		qWarning() << __FUNCTION__ << "Cannot export class, header file already exists.";
		return false;
	}
	if (QFile(source_fname).exists()) {
		qWarning() << __FUNCTION__ << "Cannot export class, source file already exists.";
		return false;
	}
	if (!write_text_file(header_fname,C->headerCode())) {
		qWarning() << __FUNCTION__ << "Cannot export class, cannot write file: "+header_fname;
		return false;
	}
	if (!write_text_file(source_fname,C->sourceCode())) {
		qWarning() << __FUNCTION__ << "Cannot export class, cannot write file: "+source_fname;
		QFile::remove(header_fname);
		return false;
	}
	C->setFoundationClass(true);
	C->setUserDefined(true);
	C->setHeaderFileName(header_fname);
	return true;
}

void STMetaSequence::linkGlobalParameter(STMetaNode *N,QString param_name,QString global_param_name) {
	if (!N) return;
	
	QString paramstr=access_string(N)+"->"+param_name;
	//first remove it
	for (int j=0; j<globalParameterCount(); j++) {	
		STGlobalParameter *GP=globalParameter(j);
		if (GP->connections.contains(paramstr)) {
			GP->connections.removeAll(paramstr);
		}
	}	
	if (!global_param_name.isEmpty()) {
	
		STGlobalParameter *GP=globalParameter(global_param_name);
		if (GP) {
			if (GP->connections.contains(paramstr))
				return; //already there
		}
		else {
			STGlobalParameter newparam;
			newparam.name=global_param_name;
			newparam.value=N->parameter(param_name).value;
			m_global_parameters.append(newparam);
			GP=globalParameter(global_param_name);
		}
		if (!GP) return;
		GP->connections.append(paramstr);
	}
	synchronize_global_parameters();
}

void STMetaSequence::synchronize_global_parameters() {
	reset_global_parameters_in_nodes();
	for (int j=0; j<globalParameterCount(); j++) {
		STGlobalParameter *GP=&(m_global_parameters[j]);
		if (GP->connections.isEmpty()) {
			m_global_parameters.removeAt(j);
			j--;
		}
		else {
			QStringList connections=GP->connections;
			foreach (QString connection,connections) {
				int index_of_last_arrow=connection.lastIndexOf("->");
				STMetaNode *N=0;
				QString pname;
				if (index_of_last_arrow==0) {
					N=this;
					pname=connection;
				}
				else {
					N=findNodeFromString(connection.mid(0,index_of_last_arrow));
					pname=connection.mid(index_of_last_arrow+2);
				}
				if (!N) GP->connections.removeAll(connection);
				else {
					STMetaNodeParameter P=N->parameter(pname);
					if (P.name.isEmpty()) GP->connections.removeAll(connection);
					else {
						P.global_parameter=GP->name;
						P.value=GP->value;
						N->setParameter(pname,P);
					}
				}
			}
		}
	}
}

void STMetaSequence::reset_global_parameters_in_nodes(STMetaNode *start_node) {
	if (!start_node) start_node=this;
	for (int j=0; j<start_node->parameterCount(); j++) {
		STMetaNodeParameter P=start_node->parameter(j);
		if (!P.global_parameter.isEmpty()) {
			P.global_parameter="";
			start_node->setParameter(P.name,P);
		}
	}
	for (int j=0; j<start_node->childCount(); j++)
		reset_global_parameters_in_nodes(start_node->child(j));
}

STGlobalParameter *STMetaSequence::globalParameter(QString name) {
	for (int j=0; j<globalParameterCount(); j++)
		if (globalParameter(j)->name==name)
			return globalParameter(j);
	return 0;
}

void STMetaSequence::copy_global_links(STMetaNode *src,STMetaNode *dst) {
	for (int j=0; j<src->parameterCount(); j++) {
		STMetaNodeParameter P=src->parameter(j);
		STMetaNodeParameter P2=dst->parameter(P.name);
		if (!P2.name.isEmpty()) {
			if (P.global_parameter!=P2.global_parameter) {
				linkGlobalParameter(dst,P.name,P.global_parameter);
			}
		}
	}
	for (int j=0; j<src->childCount(); j++) {
		STMetaNode *C=src->child(j);
		STMetaNode *C2=dst->child(C->name());
		if (C2) {
			copy_global_links(C,C2);
		}
	}
}

void STMetaSequence::setGlobalParameterValue(QString pname,QString pvalue) {
	STGlobalParameter *GP=globalParameter(pname);
	if (!GP) return;
	GP->value=pvalue;
	synchronize_global_parameters();
}

void STMetaSequence::setGlobalParameterUnits(QString pname,QString punits) {
	STGlobalParameter *GP=globalParameter(pname);
	if (!GP) return;
	GP->units=punits;
}


void STMetaSequence::deleteGlobalParameter(QString pname) {
	for (int j=0; j<m_global_parameters.count(); j++) {
		if (m_global_parameters[j].name==pname) {
			m_global_parameters.removeAt(j);
			synchronize_global_parameters();
			return;
		}
	}
}

void STMetaSequence::moveGlobalParameterUp(QString pname) {
	for (int j=1; j<m_global_parameters.count(); j++) {
		if (m_global_parameters[j].name==pname) {
			STGlobalParameter holdGP=m_global_parameters[j];
			m_global_parameters[j]=m_global_parameters[j-1];
			m_global_parameters[j-1]=holdGP;
			return;
		}
	}
}
void STMetaSequence::moveGlobalParameterDown(QString pname) {
	for (int j=0; j<m_global_parameters.count()-1; j++) {
		if (m_global_parameters[j].name==pname) {
			STGlobalParameter holdGP=m_global_parameters[j];
			m_global_parameters[j]=m_global_parameters[j+1];
			m_global_parameters[j+1]=holdGP;
			return;
		}
	}
}

void STMetaSequence::change_node_class_name(STMetaNode *start_at,QString old_name,QString new_name) {
	if (!start_at) return;
	if (start_at->className()==old_name) start_at->setClassName(new_name);
	for (int j=0; j<start_at->childCount(); j++)
		change_node_class_name(start_at->child(j),old_name,new_name);
}

void STMetaSequence::removeResource(QString name,bool remove_children_also) {
	for (int j=0; j<resourceCount(); j++) {
		if (qstr(resource(j)->name())==name) {
			m_resources.removeResource(j);
			j--;
		}
	}
	if (remove_children_also) {
		QString holdstr=name+"->";
		for (long j=0; j<resourceCount(); j++) {
			if (qstr(resource(j)->name()).indexOf(holdstr)==0) {
				m_resources.removeResource(j);
				j--;
			}
		}
	}
}

