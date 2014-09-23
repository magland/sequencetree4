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


#ifndef STConfigurationDlg_H
#define STConfigurationDlg_H

#include "ui_stconfigurationdlg.h"
#include <QSettings>

class STConfigurationDlg : public QDialog {
private:
	Ui::STConfigurationDlg ui;
public:
	STConfigurationDlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		
		QSettings settings("Magland","SequenceTree4");
		#ifdef WIN32
		QString defaultval="mingw";
		#else
			#ifdef Q_WS_MAC
			QString defaultval="mac";		
			#else
			QString defaultval="linux";
			#endif
		#endif
		QString compiler_str=settings.value("compiler",defaultval).toString();
		if (compiler_str=="mingw") {
			ui.mingw_button->setChecked(true);
		}
		else if (compiler_str=="msvc") {
			ui.msvc_button->setChecked(true);
		}
		else if (compiler_str=="linux") {
			ui.linux_button->setChecked(true);
		}
		else if (compiler_str=="mac") {
			ui.mac_button->setChecked(true);
		}
	}
	void accept() {
		QSettings settings("Magland","SequenceTree4");
		if (ui.mingw_button->isChecked()) {
			settings.setValue("compiler","mingw");
		}
		else if (ui.msvc_button->isChecked()) {
			settings.setValue("compiler","msvc");
		}
		else if (ui.linux_button->isChecked()) {
			settings.setValue("compiler","linux");
		}
		else if (ui.mac_button->isChecked()) {
			settings.setValue("compiler","mac");
		}
		QDialog::accept();
	}
};

#endif
