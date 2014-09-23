#ifndef stpreferencesdlg_H
#define stpreferencesdlg_H

#include "ui_stpreferencesdlg.h"

class STPreferencesDlg : public QDialog {
private:
	Ui::stpreferencesdlg ui;
public:
	STPreferencesDlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		update_controls();
	}
	void update_controls();
	void accept();
};

#endif
