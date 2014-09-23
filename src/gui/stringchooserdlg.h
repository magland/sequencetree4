/****************************************************************************
 This file is part of HaiQ
 Copyright (C) 2007-2008 Jeremy Magland (Jeremy.Magland@gmail.com)

 HaiQ is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 HaiQ is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with HaiQ; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/


#ifndef stringchooserdlg_H
#define stringchooserdlg_H

#include "ui_stringchooserdlg.h"
#include <QDialog>

class stringchooserdlg : public QDialog {
	Q_OBJECT
private:
	Ui::stringchooserdlg ui;
public:
	QString accepted_string;
	stringchooserdlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		connect(ui.string_list,SIGNAL(itemActivated(QListWidgetItem *)),this,SLOT(slot_list_activated()));
	}
	void set_strings(const QStringList &strings) {
		ui.string_list->clear();

		for (int j=0; j<strings.count(); j++)
			ui.string_list->addItem(strings[j]);
	}
	void set_caption(const QString &str) {
		ui.caption_label->setText(str);
	}
	void accept() {
		if (!ui.string_list->currentItem()) 
			accepted_string="";
		else
			accepted_string=ui.string_list->currentItem()->text();
		QDialog::accept();
	}
	
	static QString choose_string(QWidget *parent,const QStringList &strings,const QString &window_title,const QString &caption) {
		stringchooserdlg dlg(parent);
		dlg.set_strings(strings);
		dlg.setWindowTitle(window_title);
		dlg.set_caption(caption);
		if (dlg.exec()==QDialog::Accepted) {
			return dlg.accepted_string;
		}
		else return QString();
	}
public slots:
	void slot_list_activated() {
		accept();
	}
};

#endif
