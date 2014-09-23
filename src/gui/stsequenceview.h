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


#ifndef STSequenceView_H
#define STSequenceView_H

#include "ui_stsequenceview.h"
#include "stmetasequence.h"
#include "sttreeview.h"
#include "stnodeview.h"
//#include "stsourceeditor.h"

class QProcess;
#include "streconstructionhighlighter.h"

class STSequenceView : public QWidget {
	Q_OBJECT
private:
	Ui::STSequenceView ui;
public:
	STSequenceView(QWidget *parent=0);
	virtual ~STSequenceView();
	void setSequence(STMetaSequence *Seq);
	STMetaSequence *sequence() {return m_sequence;}
	QString fileName() {return m_file_name;}
	void setFileName(QString name) {m_file_name=name;}
	
	bool isCompiled() {
		return m_simulator.isCompiled();
	}
	bool isModified() {return m_is_modified;}
	void setModified(bool val) {
		m_is_modified=val;
		emit modificationChanged();
	}
	void setPreferredSizes();
	STMetaNode *currentNode();
	void updateAllViews(bool reset_compiled);
	QString executableFileName() {
		return m_simulator.executableFileName();
	}
	
	void updateBeforeSave();
	
	void writeRawTemplateFile(QString fname);
	void clearCompileLog();
	
	void updateResources() {ui.treeview->updateResources();}
	void updatePreferences();
signals:
	void compiledChanged();
	void modificationChanged();
	void statusMessage(QString msg);
	void runMRPulseTool(QString fname);
private:
public slots:
	void slot_compile();
private slots:
	void slot_parameter_changed(QString name);
	void slot_edit_code();
	void slot_class_changed();
	void slot_minimize_parameter(QString pname);
	void slot_maximize_parameter(QString pname);
	void slot_link_to_global_parameter(QString pname);
	void slot_unlink_global_parameter(QString pname);
	
	void slot_global_parameter_value_changed(QString pname,QString pvalue);
	void slot_global_parameter_units_changed(QString pname,QString punits);
	void slot_delete_global_parameter(QString pname);
	void slot_move_global_parameter_up(QString pname);
	void slot_move_global_parameter_down(QString pname);
	
	void slot_current_node_changed(STMetaNode *node);
	void slot_add_child_node(STMetaNode *node);
	void slot_customize_node(STMetaNode *node);
	void slot_promote_node(STMetaNode *node);
	void slot_delete_node(STMetaNode *node);
	void slot_rename_node(STMetaNode *node);
	void slot_change_node_type(STMetaNode *node);
	void slot_insert_node(STMetaNode *node);
	void slot_node_activated(STMetaNode *node);
	void slot_link_nodes(STMetaNode *,STMetaNode *);
	void slot_unlink_node(STMetaNode *);
	void slot_resource_activated(QString resource_name);
	
	void slot_class_activated(QString class_name);
	void slot_rename_class(QString class_name);
	void slot_delete_class(QString class_name);
	void slot_export_class(QString class_name);
	
	void slot_change_class(QString cname);
	
	void slot_simulation_started();
	void slot_simulation_message(QString);
	void slot_stat_finished(bool success);
	void slot_run_finished(bool success);
	
	void slot_simview_selected_events_changed(QSet<int> selection_indices);
	void slot_eventlist_selected_events_changed(QSet<int> selection_indices);
	void slot_current_event_changed(STSimScannerEvent event);
	
	void slot_compilation_started();
	void slot_compilation_message(QString);
	void slot_compilation_finished();
	
	void slot_block_changed(STSimScannerBlock *B);
	
	void slot_goto_source_line(QString fname,int line);
	
private:
	STMetaSequence *m_sequence;
	STMetaSequenceSimulator m_simulator;
	//STSourceEditor *m_source_editor;
	bool m_is_modified;
	QString m_file_name;
	QProcess *m_compile_process;
	STReconstructionHighlighter *m_reconstruction_highlighter;
	
	void get_info_for_new_child(STMetaNode *node,QString &child_class,QString &child_name);
	void add_compile_message(QString msg);
	void add_simulate_message(QString msg,bool switch_tab);
	void update_status_info();
	void show_source_for_class(QString cname);
};

#endif
