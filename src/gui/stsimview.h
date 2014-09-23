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


#ifndef STSimView_H
#define STSimView_H

#include "ui_stsimview.h"
#include "stmetasequencesimulator.h"

class STSimView : public QWidget {
	Q_OBJECT
private:
	Ui::STSimView ui;
public:
	STSimView(QWidget *parent=0) : QWidget(parent) {
		ui.setupUi(this);
		m_simulator=0;
		m_current_block_number=0;
		
		connect(ui.scrollbar,SIGNAL(valueChanged(int)),this,SLOT(slot_scrollbar_value_changed()));
		connect(ui.blockview,SIGNAL(blockChanged(STSimScannerBlock *)),this,SIGNAL(blockChanged(STSimScannerBlock *)));
		connect(ui.blockview,SIGNAL(selectedEventsChanged(QSet<int>)),this,SIGNAL(selectedEventsChanged(QSet<int>)));
	}
	virtual ~STSimView() {}
	void setSimulator(STMetaSequenceSimulator *S);
	long currentBlockNumber();
	void updatePlots();
	void setCurrentBlockNumber(long val);
	void setSelectedEvents(const QSet<int> &event_indices) {ui.blockview->setSelectedEvents(event_indices);}
	STSimScannerBlock *currentBlock() {return ui.blockview->block();}
	void updatePreferences();
signals:
	void simulationStarted();
	void simulationMessage(QString msg);
	void simulationFinished(bool val);
	void blockChanged(STSimScannerBlock *);
	void selectedEventsChanged(QSet<int>);
	
private slots:
	void slot_scrollbar_value_changed();
	void slot_run_finished(bool result);
private:
	STMetaSequenceSimulator *m_simulator;
	long m_current_block_number;
};

#endif
