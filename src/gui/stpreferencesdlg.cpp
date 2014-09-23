#include "stpreferencesdlg.h"
#include <QSettings>

void STPreferencesDlg::update_controls() {
	QSettings settings("Magland","SequenceTree4");
	ui.line_thickness->setValue(settings.value("line_thickness",2).toInt());
};

void STPreferencesDlg::accept() {
	QSettings settings("Magland","SequenceTree4");
	settings.setValue("line_thickness",ui.line_thickness->value());
	QDialog::accept();
}