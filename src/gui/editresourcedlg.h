#ifndef EditResourceDlg_H
#define EditResourceDlg_H

#include "ui_editresourcedlg.h"
#include "stresources.h"

class EditResourceDlg : public QDialog {
private:
	Ui::EditResourceDlg ui;
public:
	EditResourceDlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		m_resource=0;
	}
	void setResource(STResource *R);
	void accept();
private:
	STResource *m_resource;
};

#endif
