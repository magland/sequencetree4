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


#include "steventlist.h"
#include <QDebug>

STEventListItem::STEventListItem(STSimScannerEvent evt) {
	m_event=evt;
	if (evt.event_type==ST_EVENT_RF_PULSE) {
		QFont fnt=font(0); fnt.setBold(true);
		for (int j=0; j<7; j++) setFont(j,fnt);
		setText(0,"RF");
		setText(6,QString::number(evt.phase));
		setText(7,QString::number(evt.frequency));
		setText(8,QString::number(evt.data_mag.count()));
		setText(9,QString::number(evt.timestep));
	}
	else if ((evt.event_type==ST_EVENT_GRADIENT)||(evt.event_type==ST_EVENT_ARB_GRADIENT)) {
		if (evt.direction==1) setText(0,"GX");
		if (evt.direction==2) setText(0,"GY");
		if (evt.direction==3) setText(0,"GZ");
		setText(4,QString::number(evt.amplitudes.value(1)));
		double rt1=evt.times.value(1)-evt.times.value(0);
		double plateau=evt.times.value(2)-evt.times.value(1);
		double rt2=evt.times.value(3)-evt.times.value(2);
		setText(5,QString("[%1,%2,%3]").arg(rt1).arg(plateau).arg(rt2));
	}
	else if (evt.event_type==ST_EVENT_READOUT) {
		QFont fnt=font(0); fnt.setBold(true);
		for (int j=0; j<7; j++) setFont(j,fnt);
		setText(0,"Readout");
		setText(6,QString::number(evt.phase));
		setText(7,QString::number(evt.frequency));
		setText(8,QString::number(evt.num_points));
		setText(9,QString::number(evt.timestep));
		setText(10,QString::number(evt.currentLine));
		setText(11,QString::number(evt.currentPartition));
		setText(12,QString::number(evt.currentAverage));
		setText(13,QString::number(evt.currentSlice));
		setText(14,QString::number(evt.currentEcho));		
		setText(15,QString::number(evt.currentPhase));
		setText(16,QString::number(evt.currentRepetition));
		setText(17,QString::number(evt.currentSet));
		setText(18,QString::number(evt.currentSegment));
		setText(19,QString::number(evt.currentIda));
		setText(20,QString::number(evt.currentIdb));
		setText(21,QString::number(evt.currentIdc));
		setText(22,QString::number(evt.currentIdd));
		setText(23,QString::number(evt.currentIde));
	}
	setText(1,QString::number(evt.start_time));
	setText(2,QString::number(evt.reference_time+evt.start_time));
	setText(3,QString::number(evt.duration));
}

STEventList::STEventList(QWidget *parent) : QTreeWidget(parent) {
	QStringList labels; labels << "Event" << "Start [us]" << "Ref. [us]" << "Dur. [us]" << "Amp. [mT/m]" << "Ramps [us]" << "Phase [deg]" << "Freq. [Hz]" << "# Pts." << "Step [us]" << "Line" << "Part" << "Avg" << "Slice" << "Echo" << "Phase" << "Rep" << "Set" << "Seg" << "Ida" << "Idb" << "Idc" << "Idd" << "Ide";
	setHeaderLabels(labels);
	//setSortingEnabled(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	
	connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(slot_item_selection_changed()));
	connect(this,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(slot_current_item_changed()));
	
	m_do_emit=true;
}

void STEventList::setBlock(STSimScannerBlock *B) {
	clear();
	if (!B) return;
	for (int j=0; j<B->eventCount(); j++) {
		addTopLevelItem(new STEventListItem(B->event(j)));
	}
	for (int j=0; j<columnCount(); j++) {
		resizeColumnToContents(j);
	}
}

void STEventList::setSelectedEvents(QSet<int> event_indices) {
	bool hold_do_emit=m_do_emit;
	m_do_emit=false;
	bool selection_changed=false;
	bool first=true;
	for (int j=0; j<topLevelItemCount(); j++) {
		if (event_indices.contains(j)) {
			if (!topLevelItem(j)->isSelected()) {
				selection_changed=true;
				topLevelItem(j)->setSelected(true);
				if (first) {
					setCurrentItem(topLevelItem(j));
					first=false;
				}
			}
		}
		else {
			if (topLevelItem(j)->isSelected()) {
				topLevelItem(j)->setSelected(false);
				selection_changed=true;
			}
		}
	}
	m_do_emit=hold_do_emit;
	if (selection_changed) slot_item_selection_changed();
}

void STEventList::slot_item_selection_changed() {
	if (!m_do_emit) return;
	QSet<int> selection_indices;
	QList<QTreeWidgetItem *> selected_items=selectedItems();
	for (int j=0; j<selected_items.count(); j++) {
		STEventListItem *item=(STEventListItem *)selected_items[j];
		selection_indices << item->event().block_index;
	}
	emit selectedEventsChanged(selection_indices);
}

void STEventList::slot_current_item_changed() {
	STEventListItem *item=(STEventListItem *)currentItem();
	if (!item) return;
	emit currentEventChanged(item->event());
}

