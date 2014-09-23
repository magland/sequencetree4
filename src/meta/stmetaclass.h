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


#ifndef stmetaclass_H
#define stmetaclass_H

#include <QString>
#include <QList>
#include <QStringList>

QString read_text_file(QString fname);
bool write_text_file(const QString & fname,const QString &txt);
bool write_text_file_if_needed(const QString & fname,const QString &txt);

struct STMetaClassParameter {
	QString type;
	QString name;
	QString default_value;
	QString units;
};

struct STMetaClassChild {
	QString type;
	QString name;
};

class STMetaClass {
public:
	STMetaClass();
	virtual ~STMetaClass() {}
	QString className();
	void setClassName(QString class_name);
	QString baseClassName();
	QString headerFileName(); //empty for custom classes
	QString sourceFileName(); //empty for custom classes
	bool isFoundationClass();
	void setFoundationClass(bool val);
	bool isUserDefined();
	void setUserDefined(bool val);
	void setHeaderFileName(QString fname);
	void setCode(QString header,QString source);
	
	virtual QString headerCode(); //virtual because there may be some preprocessing of code
	virtual QString sourceCode(); //virtual because there may be some preprocessing of code
	
	int parameterCount() {return m_parameters.count();}
	STMetaClassParameter parameter(int index) {return m_parameters[index];}
	int childCount() {return m_children.count();}
	STMetaClassChild child(int index) {return m_children[index];}
	STMetaClassChild child(QString child_name) {
		int index=get_child_index(child_name);
		if (index<0) {
			STMetaClassChild dummy;
			return dummy;
		}
		else {
			return child(index);
		}
	}
	
	void loadFromText(const QList<QString> &txtlines,long &index);
	void saveToText(QList<QString> &txtlines,QString spaces="");
	
	void addChild(QString child_class,QString child_name);
	void insertChild(QString before_child,QString child_class,QString child_name);
	void changeChildType(QString child_name,QString new_class_name);
	void removeChild(QString child_name);
	void renameChild(QString old_name,QString new_name);
	void changeClassName(QString class_name);
	
	bool hasChild(QString childname);
	
	int defaultValueCount() {return m_default_names.count();}
	QString defaultName(int j) {return m_default_names[j];}
	QString defaultValue(int j) {return m_default_values[j];}
	
private:
	QString m_class_name;
	QString m_base_class_name;
	QString m_header_code;
	QString m_source_code;
	QString m_header_file_name;
	bool m_is_foundation_class;
	bool m_is_user_defined;
	QList<STMetaClassParameter> m_parameters;
	QList<STMetaClassChild> m_children;
	QStringList m_default_names;
	QStringList m_default_values;
	
	void process_st_class_line(QString line);
	void process_st_parameter_line(QString line);
	void process_st_child_line(QString line);
	void process_st_default_line(QString line);
	
	int get_child_index(QString child_name);
	QString get_child_code_line(QString child_name);
	void set_child_code_line(QString child_name,QString line);
};

#endif
