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


#ifndef stmetasequence_H
#define stmetasequence_H

#include "stmetaclass.h"
#include "stmetanode.h"
#include "stresources.h"

QString get_ST_version();

struct STMetaSequenceLinkGroup {
	QStringList node_strings;
};

struct STGlobalParameter {
	QString name;
	QString value;
	QString units;
	QStringList connections;
};

class STMetaSequence : public STMetaNode {
public:
	STMetaSequence();
	virtual ~STMetaSequence();
	QStringList metaClassNames();
	STMetaClass *metaClass(QString class_name);
	void addMetaClass(STMetaClass *C);
	void clearClasses();
	void loadFoundationClasses(QString directory,bool user_defined);
	
	void loadFromText(const QList<QString> &txtlines,long &index);
	void saveToText(QList<QString> &txtlines,QString spaces="");
	
	void update();
	
	bool inherits(QString class_name,QString base_class_name);
	bool inherits(STMetaClass *C,QString base_class_name);
	
	QStringList classesThatInherit(QString base_class_name,QString exclude_base_class_name="");
	QSet<QString> usedClassNames(STMetaNode *start_at=0);
	
	STMetaClass *duplicateClass(STMetaClass *C);
	void removeClass(QString cname);	
	
	void loadParametersFromFile(QString file_name);	
	void writeParametersToFile(QString file_name);
	
	void writeFoundationClassesHeader(QString fname);
	void writeFoundationClassesSource(QString fname);
	void writeCustomClassesHeader(QString fname);
	void writeCustomClassesSource(QString fname);
	void writeCreateSequenceSource(QString fname,QString sequence_export_name);
	void writeLoadParametersFileSource(QString fname);
	
	void read(QString file_name);
	void write(QString file_name);
	
	double totalDuration() {return m_total_duration;}
	double numBlocks() {return m_num_blocks;}
	
	STMetaNode *findNode(int tree_index,STMetaNode *start_at=0);
	STMetaNode *findNodeFromString(QString str);
	QString nodeString(STMetaNode *N);
	
	void renameClass(QString old_name,QString new_name);
	void deleteClass(QString class_name);
	bool exportClass(QString class_name,QString fname);
	void write_parameters_to_text(QString &txt,STMetaNode *ptr=0);
	
	bool isPromoted(STMetaNode *node);
	void updateLinkGroups();
	void linkNodes(STMetaNode *N1,STMetaNode *N2);
	void unlinkNode(STMetaNode *N);
	void handleLinksAfterNodeChanged(STMetaNode *N,QString pname);
	
	QString fileName() {return m_file_name;}
	void setFileName(QString val) {m_file_name=val;}
	
	int resolution() {return m_resolution;}
	void setResolution(int val) {m_resolution=val;}
	
	QString notes() {return m_notes;}
	void setNotes(QString val) {m_notes=val;}
	void setReconstructionCode(QString val) {m_reconstruction_code=val;}
	QString reconstructionCode() {return m_reconstruction_code;}
	
	int globalParameterCount() {return m_global_parameters.count();}
	STGlobalParameter *globalParameter(int j) {return &m_global_parameters[j];}
	STGlobalParameter *globalParameter(QString name);
	void linkGlobalParameter(STMetaNode *N,QString param_name,QString global_param_name);
	void setGlobalParameterValue(QString pname,QString pvalue);
	void setGlobalParameterUnits(QString pname,QString punits);
	void deleteGlobalParameter(QString pname);
	void moveGlobalParameterUp(QString pname);
	void moveGlobalParameterDown(QString pname);
	
	long resourceCount() {return m_resources.count();}
	STResource *resource(long index) {return m_resources.resource(index);}
	STResource *resource(QString name) {return m_resources.resource(sstr(name));}
	void addResource(STResource *R) {m_resources.addResource(R);}
	void removeResource(QString name,bool remove_children_also);
private:
	QHash<QString,STMetaClass *> m_meta_classes;
	double m_total_duration;
	long m_num_blocks;
	QList<STMetaSequenceLinkGroup> m_link_groups;
	QString m_file_name;
	QString m_notes;
	QString m_reconstruction_code;
	QList<STGlobalParameter> m_global_parameters;
	QStringList m_other_sequence_lines;
	STResources m_resources;
	int m_resolution;
	
	QString get_unused_class_name(QString name);
	void replace_word_in_text(QString &txt,QString old_word,QString new_word);
	
	void create_code_for_promoting_nodes(STMetaNode *N,QString &output_txt);
	
	void synchronize_global_parameters();
	void reset_global_parameters_in_nodes(STMetaNode *start_node=0);
	void copy_global_links(STMetaNode *src,STMetaNode *dst);
	
	void change_node_class_name(STMetaNode *start_at,QString old_name,QString new_name);
	
};

#endif

