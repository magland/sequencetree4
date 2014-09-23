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


#include "stmetaclass.h"
#include <QDebug>

#include <QFile>
#include <QStringList>

QString read_text_file(QString fname) {
	/*FILE *inf=fopen(fname.toAscii().data(),"r");
	if (!inf) return "";
	QString ret;
	while (!feof(inf)) {
		ret.append(fgetc(inf));
	}
	fclose(inf);
	return ret;*/
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
		return "";
	QString ret=file.readAll();
	file.close();
	return ret;
}
bool write_text_file(const QString & fname,const QString &txt) {
	/*FILE *outf=fopen(fname.toAscii().data(),"w");
	if (!outf) return false;
	for (long j=0; j<txt.count(); j++) {
		char c=txt[j].toAscii();
		fputc(c,outf);
	}
	fclose(outf);
	return true;*/
	QFile file(fname);
	if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
		return false;
	file.write(txt.toAscii());
	file.close();
	return true;
}

bool write_text_file_if_needed(const QString & fname,const QString &txt) {
	QString txt2=read_text_file(fname);
	if (txt==txt2) return true;
	return write_text_file(fname,txt);
}



STMetaClass::STMetaClass() {
	m_is_foundation_class=false;
	m_is_user_defined=true;
}

QString STMetaClass::className() {return m_class_name;}
void STMetaClass::setClassName(QString class_name) {m_class_name=class_name;}
QString STMetaClass::baseClassName() {return m_base_class_name;}
QString STMetaClass::headerFileName() {return m_header_file_name;}
QString STMetaClass::sourceFileName() {
	if (m_header_file_name.isEmpty()) return "";
	QString ret=m_header_file_name;
	if ((ret.count()>2)&&(ret.mid(ret.count()-2)==".h"))
		ret=ret.mid(0,ret.count()-2);
	ret=ret+".cpp";
	return ret;
}

void STMetaClass::setHeaderFileName(QString fname) {
	m_header_file_name=fname;
	setCode(read_text_file(headerFileName()),read_text_file(sourceFileName()));
}

QString STMetaClass::headerCode() {
	return m_header_code;
}
QString STMetaClass::sourceCode() {
	return m_source_code;
}
void STMetaClass::setCode(QString header,QString source) {

	header=header.trimmed()+"\n";
	source=source.trimmed()+"\n";	
	
	m_parameters.clear();
	m_children.clear();
	m_default_names.clear();
	m_default_values.clear();
	
	m_header_code=header;
	m_source_code=source;
	QList<QString> header_lines=m_header_code.split("\n");
	QList<QString> source_lines = m_source_code.split("\n");
	
	
	foreach (QString line,source_lines) {
		line=line.trimmed();
		if (line.mid(0,8)=="ST_CLASS") {
			process_st_class_line(line);
		}
		else if (line.mid(0,12)=="ST_PARAMETER") {
			process_st_parameter_line(line);
		}
		else if (line.mid(0,8)=="ST_CHILD") {
			process_st_child_line(line);
		}
		else if (line.mid(0,10)=="ST_DEFAULT") {
			process_st_default_line(line);
		}
	}
	
	bool sequencetree_section=false;
	for (int j=0; j<header_lines.count(); j++) {
		QString line=header_lines[j];
		if (line.indexOf("BEGIN SEQUENCETREE")>=0) {
			sequencetree_section=true;
		}
		else if (line.indexOf("END SEQUENCETREE")>=0) {
			if (sequencetree_section) {
				for (int k=0; k<parameterCount(); k++) {
					STMetaClassParameter param=parameter(k);
					QString str=QString("\t%1 %2;").arg(param.type).arg(param.name);
					header_lines.insert(j,str);
					j++;
				}
				for (int k=0; k<childCount(); k++) {
					STMetaClassChild ch=child(k);
					QString str=QString("\t%1 *%2;").arg(ch.type).arg(ch.name);
					header_lines.insert(j,str);
					j++;
				}
			}
			sequencetree_section=false;
		}
		else {
			if (sequencetree_section) {
				header_lines.removeAt(j);
				j--;
			}
		}
	}
	m_header_code="";
	for (int j=0; j<header_lines.count(); j++) {
		if (j>0) m_header_code+="\n";
		m_header_code+=header_lines[j];
	}
}

void STMetaClass::loadFromText(const QList<QString> &txtlines,long &index) {
	bool done=false;
	bool header_mode=false;
	bool source_mode=false;
	QString header_code;
	QString source_code;
	while ((!done)&&(index<txtlines.count())) {
		QString line=txtlines[index];
		index++;
		QString trimmedline=line.trimmed();
		if (trimmedline.mid(0,10)=="++++++ END")
			done=true;
		else if (trimmedline.mid(0,13)=="++++++ HEADER") {
			header_mode=true;
			source_mode=false;
		}
		else if (trimmedline.mid(0,13)=="++++++ SOURCE") {
			header_mode=false;
			source_mode=true;
		}
		else {
			if (header_mode) {
				if (!header_code.isEmpty())
					header_code+="\n";
				header_code+=line;
			}
			else if (source_mode) {
				if (!source_code.isEmpty()) 
					source_code+="\n";
				source_code+=line;
			}
		}
	}
	setCode(header_code,source_code);
}
void STMetaClass::saveToText(QList<QString> &txtlines,QString spaces) {
	txtlines << spaces+="++++++ HEADER";
	QStringList header_lines=m_header_code.split("\n");
	foreach (QString line,header_lines) 
		txtlines << line;
	txtlines << spaces+="++++++ SOURCE";
	QStringList source_lines=m_source_code.split("\n");
	foreach (QString line,source_lines) 
		txtlines << line;
	txtlines << spaces+"++++++ END";
}

bool STMetaClass::isFoundationClass() {
	return m_is_foundation_class;
}
void STMetaClass::setFoundationClass(bool val) {
	m_is_foundation_class=val;
}
bool STMetaClass::isUserDefined() {
	return m_is_user_defined;
}
void STMetaClass::setUserDefined(bool val) {
	m_is_user_defined=val;
}

QString extract_stuff_in_paren(QString line) {
	int begin_paren_ind=line.indexOf('(');
	if (begin_paren_ind<0) return "";
	int paren_level=1;
	int end_paren_ind=-1;
	for (int ind=begin_paren_ind+1; (ind<line.count())&&(end_paren_ind<0); ind++) {
		if (line[ind]=='(') paren_level++;
		else if (line[ind]==')') {
			paren_level--;
			if (paren_level==0) {
				end_paren_ind=ind;
			}
		}
	}
	if (end_paren_ind<0) return "";
	if (end_paren_ind<=begin_paren_ind+1) return "";
	return line.mid(begin_paren_ind+1,end_paren_ind-begin_paren_ind-1);
	
}

QStringList split_arguments_by_commas(QString str) {
	str.replace("%",""); //remove % characters
	int paren_level=0;
	for (int ind=0; ind<str.count(); ind++) {
		if (str[ind]==',') {
			if (paren_level==0) str[ind]='%';
		}
		else if ((str[ind]=='(')||(str[ind]=='['))
			paren_level++;
		else if ((str[ind]==')')||(str[ind]==']'))
			paren_level--;
	}
	QStringList ret=str.split('%');
	for (int j=0; j<ret.count(); j++) {
		ret[j]=ret[j].trimmed();
	}
	return ret;
}

void STMetaClass::process_st_class_line(QString line) {
	QString str=extract_stuff_in_paren(line);
	QStringList args=split_arguments_by_commas(str);
	if (!args.value(0).isEmpty()) setClassName(args.value(0));
	m_base_class_name=args.value(1);
	
}
void STMetaClass::process_st_parameter_line(QString line) {
	QString str=extract_stuff_in_paren(line);
	QStringList args=split_arguments_by_commas(str);
	STMetaClassParameter param;
	param.type=args.value(0);
	param.name=args.value(1);
	param.default_value=args.value(2);
	param.units=args.value(3);
	m_parameters << param;
}
void STMetaClass::process_st_child_line(QString line) {
	QString str=extract_stuff_in_paren(line);
	QStringList args=split_arguments_by_commas(str);
	STMetaClassChild child;
	child.type=args.value(0);
	child.name=args.value(1);
	m_children << child;
}
void STMetaClass::process_st_default_line(QString line) {
	QString str=extract_stuff_in_paren(line);
	QStringList args=split_arguments_by_commas(str);
	m_default_names << args.value(0);
	m_default_values << args.value(1);
}


void STMetaClass::addChild(QString child_class,QString child_name) {
	QString hold_header=m_header_code;
	QString hold_source=m_source_code;
	QStringList hold_header_lines=hold_header.split("\n");
	QStringList hold_source_lines=hold_source.split("\n");
	
	bool child_mode=false;
	bool done=false;
	for (int j=0; (j<hold_source_lines.count())&&(!done); j++) {
		QString line=hold_source_lines[j];
		line=line.trimmed();
		if (line.contains("ST_CHILDREN")) {
			child_mode=true;
		}
		else if (child_mode) {
			if (!line.contains("ST_CHILD")) {
				QString newline=QString("\tST_CHILD(%1,%2);").arg(child_class).arg(child_name);
				hold_source_lines.insert(j,newline);
				j++;
				child_mode=false;
				done=true;
			}
		}
		else if (line.contains("ST_CHILD")) {
			child_mode=true;
		}
	}
	if (!done) {
		qWarning() << QString("Unable to add child to class %1, unable to find insertion point in source code.").arg(className());
	}
	
	hold_header="";
	for (int j=0; j<hold_header_lines.count(); j++) {
		if (j>0) hold_header+="\n";
		hold_header+=hold_header_lines[j];
	}
	hold_source="";
	for (int j=0; j<hold_source_lines.count(); j++) {
		if (j>0) hold_source+="\n";
		hold_source+=hold_source_lines[j];
	}
	
	setCode(hold_header,hold_source);
}

void STMetaClass::insertChild(QString before_child,QString child_class,QString child_name) {
	QString line=get_child_code_line(before_child);
	line=QString("\tST_CHILD(%1,%2);").arg(child_class,child_name)+"\n"+line;
	set_child_code_line(before_child,line);
}

void STMetaClass::changeChildType(QString child_name,QString new_class_name) {
	int chind=get_child_index(child_name);
	if (chind<0) return;
	QString old_class_name=child(chind).type;
	QString line=get_child_code_line(child_name);
	line.replace(QRegExp("\\b"+old_class_name+"\\b"),new_class_name);
	set_child_code_line(child_name,line);
}
void STMetaClass::changeClassName(QString class_name) {
	QString old_class_name=className();
	m_header_code.replace(QRegExp("\\b"+old_class_name+"\\b"),class_name);
	m_source_code.replace(QRegExp("\\b"+old_class_name+"\\b"),class_name);
	setCode(m_header_code,m_source_code);
	setClassName(class_name);
}

void STMetaClass::removeChild(QString child_name) {
	set_child_code_line(child_name,"");
}

bool STMetaClass::hasChild(QString childname) {
	return (get_child_index(childname)>=0);
}

int STMetaClass::get_child_index(QString child_name) {
	for (int j=0; j<m_children.count(); j++)
		if (m_children[j].name==child_name) return j;
	return -1;
		
}

QString STMetaClass::get_child_code_line(QString child_name) {
	QStringList source_code_lines=m_source_code.split("\n");
	foreach (QString line,source_code_lines) {
		QString line2=line.trimmed();
		if (line2.indexOf("ST_CHILD")==0) {
			QString str=extract_stuff_in_paren(line);
			QStringList args=split_arguments_by_commas(str);
			if (args.value(1)==child_name)
				return line;
		}
	}
	return "";
}
void STMetaClass::set_child_code_line(QString child_name,QString new_line) {
	QStringList source_code_lines=m_source_code.split("\n");
	QStringList new_source_code_lines;
	foreach (QString line,source_code_lines) {
		bool skip_this_line=false;
		QString line2=line.trimmed();
		if (line2.indexOf("ST_CHILD")==0) {
			QString str=extract_stuff_in_paren(line);
			QStringList args=split_arguments_by_commas(str);
			if (args.value(1)==child_name) {
				line=new_line;
				if (line.isEmpty()) skip_this_line=true;
			}
		}
		if (!skip_this_line)
			new_source_code_lines << line;
	}
	QString new_source_code;
	for (int j=0; j<new_source_code_lines.count(); j++) {
		if (j>0) new_source_code+="\n";
		new_source_code += new_source_code_lines[j];
		
	}
	setCode(m_header_code,new_source_code);
}

void STMetaClass::renameChild(QString old_name,QString new_name) {
	QString line=get_child_code_line(old_name);
	line.replace(QRegExp("\\b"+old_name+"\\b"),new_name);
	set_child_code_line(old_name,line);
}



