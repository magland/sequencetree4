#ifndef STControllerMainWindow_H
#define STControllerMainWindow_H

#include "ui_stcontrollermainwindow.h"
#include "stcontroller.h"

class STControllerMainWindow : public QMainWindow {
	Q_OBJECT
private:
	Ui::STControllerMainWindow ui;
	STController m_controller;
public:
	STControllerMainWindow(QWidget *parent=0) : QMainWindow(parent) {
		ui.setupUi(this);
		ui.sequences->initialize();
		ui.raw_data_files->initialize();		
		
		connect(ui.actionConfiguration,SIGNAL(triggered()),this,SLOT(slot_configuration()));
		connect(&m_controller,SIGNAL(message(QString,bool)),this,SLOT(slot_controller_message(QString,bool)));
		connect(&m_controller,SIGNAL(sequenceFilesChanged()),this,SLOT(slot_sequence_files_changed()));
		connect(&m_controller,SIGNAL(rawDataFilesChanged()),this,SLOT(slot_raw_data_files_changed()));
		connect(ui.refresh_sequences_button,SIGNAL(clicked()),this,SLOT(slot_sequence_files_changed()));
		connect(ui.open_sequence_folder_button,SIGNAL(clicked()),this,SLOT(slot_open_sequence_folder()));
		connect(ui.refresh_raw_data_button,SIGNAL(clicked()),this,SLOT(slot_raw_data_files_changed()));
		connect(ui.view_header_button,SIGNAL(clicked()),this,SLOT(slot_view_header()));
		connect(ui.parse_button,SIGNAL(clicked()),this,SLOT(slot_parse()));
		connect(ui.open_raw_folder_button,SIGNAL(clicked()),this,SLOT(slot_open_raw_folder()));
		update_configuration();
		m_controller.startTimer();
	}
private slots:
	void slot_configuration();
	void slot_controller_message(QString msg,bool bold);
	void slot_sequence_files_changed();
	void slot_open_sequence_folder();
	void slot_raw_data_files_changed();
	void slot_view_header();
	void slot_parse();
	void slot_open_raw_folder();
private:
	void update_configuration();	
	void show_message(QString msg);
};

#endif
