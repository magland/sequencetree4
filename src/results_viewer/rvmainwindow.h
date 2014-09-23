#ifndef RVMainWindow_H
#define RVMainWindow_H

#include "ui_rvmainwindow.h"
#ifdef CHAINLINK_FOUND
	#include "chainlinkcoreplugininterface.h"
#endif

class RVMainWindow : public QMainWindow {
	Q_OBJECT
private:
	Ui::RVMainWindow ui;
public:
	RVMainWindow(QWidget *parent=0) : QMainWindow(parent) {
		ui.setupUi(this);
		ui.splitter->setStretchFactor(0,0);
		ui.splitter->setStretchFactor(1,1);
		ui.tabWidget->clear();
		QToolButton *close_button=new QToolButton(this);
		close_button->setIcon(QIcon(":/images/cross.png"));
		connect(close_button,SIGNAL(clicked()),this,SLOT(slot_close_tab()));
		ui.tabWidget->setCornerWidget(close_button);
		
		connect(ui.actionOpenExperiment,SIGNAL(triggered()),this,SLOT(slot_open_experiment()));
		connect(ui.actionSaveExperiment,SIGNAL(triggered()),this,SLOT(slot_save_experiment()));
		connect(ui.actionConfiguration,SIGNAL(triggered()),this,SLOT(slot_configuration()));
		
		connect(ui.file_view,SIGNAL(fileActivated(QString)),this,SLOT(slot_file_activated(QString)));
		
		#ifdef CHAINLINK_FOUND
			m_chainlink_core=0;
			set_chainlink_core();
			connect(ui.actionReconstruction,SIGNAL(triggered()),this,SLOT(slot_reconstruction()));
		#else
			qDebug()  << "Chainlink not found during compilation.";
			ui.actionReconstruction->setEnabled(false);
		#endif
	}
	void openExperiment(QString foldername) {
		ui.file_view->setDirectory(foldername);
	}
private slots:
	void slot_close_tab();
	
	void slot_open_experiment();
	void slot_save_experiment();
	void slot_configuration();
	
	void slot_file_activated(QString fname);
	
#ifdef CHAINLINK_FOUND
	void slot_reconstruction();
#endif

private:	
#ifdef CHAINLINK_FOUND
	ChainlinkCoreInterface *m_chainlink_core;
	void set_chainlink_core();
#endif
	QWidget *current_widget() {
		return ui.tabWidget->currentWidget();
	}
};

#endif
