#ifndef VSMainWindow_H
#define VSMainWindow_H

#include "ui_vsmainwindow.h"
#include "stmetasequence.h"
#include "stmetasequencesimulator.h"
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include "stcommon.h"
#include "vsabstractscanner.h"
#include <QProgressDialog>
#include <QPushButton>
#include "vsktermscanner.h"

#define MSG_DELAY 3000

class VSMainWindow : public QMainWindow {
	Q_OBJECT
private:
	Ui::VSMainWindow ui;
public:
	VSMainWindow(QWidget *parent=0);
	virtual ~VSMainWindow();
	void openSequence(QString fname);
private slots:
	void slot_open_sequence();
	void slot_select_phantom();
	void slot_compilation_message(QString msg);
	void slot_simulation_message(QString msg);
	void slot_scan();
	void slot_clear_simulate_log();
	void slot_scan_progress(long prog,long total,QString msg);
	void slot_cancel_scan();
	void slot_configuration();
private:
	STMetaSequence m_sequence;
	STMetaSequenceSimulator m_simulator;
	VSPhantom *m_phantom;
	QList<VSPhantom *> m_phantom_choices;
	QProgressDialog *m_progress_dialog;
	QPushButton *m_cancel_button;
	VSKtermScanner *m_scanner;	
	
	void restore_phantom_settings();
	void save_phantom_settings();
	void load_phantom_plugins();
};

#endif
