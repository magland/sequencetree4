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

#include "stsimview.h"
#include <QFile>
#include <QProcess>
#include <QSettings>

void STSimView::setSimulator(STMetaSequenceSimulator *S) {
	m_simulator=S;
	setCurrentBlockNumber(0);
	connect(S,SIGNAL(runFinished(bool)),this,SLOT(slot_run_finished(bool)));
}

long STSimView::currentBlockNumber() {
	return m_current_block_number;
}
void STSimView::setCurrentBlockNumber(long val) {
	m_current_block_number=val;
	ui.scrollbar->setValue(m_current_block_number);
	updatePlots();
}

void STSimView::updatePlots() {
	if (!m_simulator) return;
	if (!m_simulator->metaSequence()) return;
	if (!m_simulator->isCompiled()) ui.blockview->setMessage("Press F9 to compile...");
	else ui.blockview->setMessage("");
	if ((m_simulator->numBlocks()>0)&&(m_current_block_number>=m_simulator->numBlocks())) {
		m_current_block_number=m_simulator->numBlocks()-1;
		ui.scrollbar->setValue(m_current_block_number);
	}
	STSimScannerBlock *B=m_simulator->retrieveBlock(m_current_block_number);
	if (!B) {
		if (m_simulator->isCompiled())
			ui.blockview->setBlock(0,m_current_block_number);
		else
			ui.blockview->setBlock(0);
		return;
	}
	ui.blockview->setMaxGradientAmp(m_simulator->metaSequence()->parameter("maxamp").value.toDouble());
	ui.blockview->setMaxRFAmp(m_simulator->maxRFAmplitude());
	ui.blockview->setBlock(B);
	if (m_simulator->numBlocks()>0) ui.scrollbar->setRange(0,m_simulator->numBlocks()-1);
	else ui.scrollbar->setRange(0,m_simulator->maxLoadedBlockIndex()+100);
}

void STSimView::slot_scrollbar_value_changed() {
	m_current_block_number=ui.scrollbar->value();
	updatePlots();
}

void STSimView::slot_run_finished(bool result) {
	Q_UNUSED(result)
	updatePlots();
	if (!ui.blockview->block()) {
		if (currentBlockNumber()>0) {
			//setCurrentBlockNumber(0);
		}
	}
}

void STSimView::updatePreferences() {
	QSettings settings("Magland","SequenceTree4");
	ui.blockview->setLineThickness(settings.value("line_thickness",2).toInt());
}



