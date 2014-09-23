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


#ifndef STPasteDialog_H
#define STPasteDialog_H

#include "ui_stpastedialog.h"

#define ST_PASTE_OPTION_VALUES 1
#define ST_PASTE_OPTION_STRUCTURE 2

class STPasteDialog : public QDialog {
private:
	Ui::STPasteDialog ui;
public:
	STPasteDialog(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
		m_paste_option=0;
	}
	void accept() {
		if (ui.values_button->isChecked())
			m_paste_option=ST_PASTE_OPTION_VALUES;
		else if (ui.structure_button->isChecked())
			m_paste_option=ST_PASTE_OPTION_STRUCTURE;
		QDialog::accept();
	}
	int pasteOption() {return m_paste_option;}
private:
	int m_paste_option;
};

#endif
