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


#ifndef STMainWindow_H
#define STMainWindow_H

#include "ui_stmainwindow.h"
#include "stsequenceview.h"
#include "stpreferencesdlg.h"


class STMainWindow : public QMainWindow {
	Q_OBJECT
private:
	Ui::STMainWindow ui;
public:
	STMainWindow(QWidget *parent=0);
	void openSequence(QString sequence_fname);
	void openSequenceFromText(const QString fname, const QString &txt);
	int sequenceViewCount() {return ui.tabWidget->count();}
	STSequenceView *sequenceView(int index) {return (STSequenceView *)ui.tabWidget->widget(index);}
	STSequenceView *sequenceView(QString fname);
	STSequenceView *currentSequenceView() {return (STSequenceView *)ui.tabWidget->currentWidget();}
	STMetaSequence *currentSequence() {if (!currentSequenceView()) return 0; return currentSequenceView()->sequence();}
	void updatePreferences();
protected:
	void closeEvent(QCloseEvent *event);
private:
	void update_tab_labels();
	void connect_sequence_view(STSequenceView *SV);
	void update_window_title();
	void update_recent_file_menu();
	void add_to_recent_files(QString fname);
	void update_actions();
private slots:
	void slot_new_sequence();
	void slot_open_sequence();
	//void slot_open_sequence_from_web();
	bool slot_close_sequence();
	void slot_save_sequence();
	void slot_save_sequence_as();
	void slot_upload_sequence_to_web();
	void slot_open_recent_file();
	
	void slot_copy_node();
	void slot_paste_node();
	
	void slot_compile();
	
	void slot_add_resource();
	void slot_import_rf_pulse_waveform();
	
	void slot_configuration();
	void slot_export_to_scanner();
	void slot_update_export_to_scanner();
	void slot_export_to_virtual_scanner();
	void slot_read_raw_data();
	void slot_create_parameter_file();
	//void slot_update_code_from_web();
	void slot_chainlink();
	void slot_rtst();
	void slot_mr_pulse_tool();
	void slot_run_mr_pulse_tool(QString fname);
	void slot_preferences();
	
	void slot_about_sequencetree();
	void slot_sequencetree_wiki();
	
	void slot_modification_changed();
	void slot_compiled_changed();
	void slot_current_changed();
	void slot_status_message(QString msg);
private:
	STMetaNode m_clipboard_node;
};

bool exports_are_enabled();

#endif
