#ifndef STControllerConfigDlg_H
#define STControllerConfigDlg_H

#include "ui_stcontrollerconfigdlg.h"
#include <QSettings>

class STControllerConfigDlg : public QDialog {
private:
	Ui::STControllerConfigDlg ui;
public:
	STControllerConfigDlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		
		QSettings settings("Magland","ST4Controller");
		ui.raw_data_dir->setText(settings.value("raw_data_dir").toString());
		ui.seq_bin_dir->setText(settings.value("seq_bin_dir").toString());
		ui.timer_interval->setText(QString::number(settings.value("timer_interval",2).toDouble()));
	}
	void accept() {
		QSettings settings("Magland","ST4Controller");
		settings.setValue("raw_data_dir",ui.raw_data_dir->text());
		settings.setValue("seq_bin_dir",ui.seq_bin_dir->text());
		settings.setValue("timer_interval",ui.timer_interval->text().toDouble());
		QDialog::accept();
	}
};

#endif
