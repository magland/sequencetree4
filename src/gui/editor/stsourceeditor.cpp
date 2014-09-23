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


#include "stsourceeditor.h"
#include <QMessageBox>

STSourceEditor::STSourceEditor(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);
	CppHighlighter *m_highlighter_header=new CppHighlighter;
	CppHighlighter *m_highlighter_source=new CppHighlighter;
	m_highlighter_header->setDocument(ui.header->editor()->document());
	m_highlighter_source->setDocument(ui.source->editor()->document());
	
	QMap<QString,QTextCharFormat> formats;
	QTextCharFormat format;
	
	format.setForeground(QBrush(Qt::darkGreen));
	formats["comment"]=format;
	
	format.setForeground(QBrush(Qt::darkBlue));
	formats["cpp_keyword"]=format;
	
	format.setForeground(QBrush(Qt::darkMagenta));
	formats["qt_keyword"]=format;
	
	format.setForeground(QBrush(Qt::darkGray));
	formats["operator"]=format;
	
	format.setForeground(QBrush(Qt::darkGray));
	formats["number"]=format;
	
	format.setForeground(QBrush(Qt::red));
	formats["escape_char"]=format;
	
	format.setForeground(QBrush(Qt::darkBlue));
	formats["macro"]=format;
	
	format.setForeground(QBrush(Qt::darkRed));
	formats["string"]=format;
	
	format.setForeground(QBrush(Qt::black));
	formats["function_name"]=format;

	m_highlighter_header->set_text_formats(formats);
	m_highlighter_source->set_text_formats(formats);
	
	QStringList source_keywords; 
	source_keywords << "ST_CLASS" << "ST_PARAMETER" << "ST_CHILD" << "ST_ALIGN" << "ST_DEFAULT";
	foreach (QString source_keyword,source_keywords)
		m_highlighter_source->addUserKeyword(source_keyword);
	
	m_C=0;
	
	QAction *action_save_changes=new QAction("Save changes to code (Ctrl+U)",this);
	action_save_changes->setShortcut(tr("Ctrl+U"));
	ui.save_changes_button->setDefaultAction(action_save_changes);
	addAction(action_save_changes);
	
	QAction *action_find=new QAction(this);
	action_find->setShortcut(tr("Ctrl+F"));
	addAction(action_find);
	connect(action_find,SIGNAL(triggered()),this,SLOT(slot_find()));
	
	connect(ui.header->editor()->document(),SIGNAL(modificationChanged(bool)),this,SLOT(slot_modification_changed()));
	connect(ui.source->editor()->document(),SIGNAL(modificationChanged(bool)),this,SLOT(slot_modification_changed()));		
	connect(ui.save_changes_button,SIGNAL(clicked()),this,SLOT(slotSaveChanges()));
	connect(ui.reload_button,SIGNAL(clicked()),this,SLOT(slot_reload()));
	connect(action_save_changes,SIGNAL(triggered()),this,SLOT(slotSaveChanges()));
	//connect(ui.actionCompile,SIGNAL(triggered()),this,SIGNAL(signalCompile()));
	
	connect(ui.class_box,SIGNAL(currentIndexChanged(QString)),this,SLOT(slot_class_box_changed(QString)));
	
	
	//ui.actionSaveChanges->setShortcut(tr("Ctrl+S"));
	//ui.actionCompile->setShortcut(tr("F9"));
	
}

void STSourceEditor::setClass(STMetaClass *C) {
	if (m_C==C) return;
	check_to_update_current_class();
	m_C=C;
	update();
}
void STSourceEditor::setSequence(STMetaSequence *Seq) {
	m_sequence=Seq;
	update();
}
void STSourceEditor::update() {		
	if (!m_C) setReadOnly(false);
	else setReadOnly(m_C->isFoundationClass());	
	
	ui.class_box->clear();
	ui.class_box->addItem("");
	
	QStringList cnames;
	if (m_sequence) {
		cnames=m_sequence->metaClassNames();
		qSort(cnames);
		foreach (QString cname,cnames) {
			ui.class_box->addItem(cname);
			int indx=ui.class_box->count()-1;
			if (!m_sequence->metaClass(cname)->isFoundationClass()) {				
				QFont fnt=ui.class_box->itemData(indx,Qt::FontRole).toString();
				fnt.setBold(true);
				ui.class_box->setItemData(indx,fnt,Qt::FontRole);
				ui.class_box->setItemData(indx,Qt::darkMagenta,Qt::ForegroundRole);
			}
			else if (m_sequence->metaClass(cname)->isUserDefined()) {
				QFont fnt=ui.class_box->itemData(indx,Qt::FontRole).toString();
				fnt.setBold(true);
				ui.class_box->setItemData(indx,fnt,Qt::FontRole);
			}			
		}
	}
	if (!m_C) {
		setWindowTitle("");
		ui.header->editor()->setPlainText("");
		ui.source->editor()->setPlainText("");
		ui.class_box->setCurrentIndex(0);
		return;
	}
	else {
		ui.reload_button->setEnabled(!m_C->headerFileName().isEmpty());
		ui.header->editor()->setPlainText(m_C->headerCode());
		ui.source->editor()->setPlainText(m_C->sourceCode());	
		int index=cnames.indexOf(m_C->className());
		if (index>=0)
			ui.class_box->setCurrentIndex(index+1);
		else
			ui.class_box->setCurrentIndex(0);
	}
	setModified(false);
	
}
void STSourceEditor::setReadOnly(bool val) {
	ui.header->editor()->setReadOnly(val);
	ui.source->editor()->setReadOnly(val);
}
bool STSourceEditor::isModified() {
	return ((ui.header->editor()->document()->isModified())||(ui.source->editor()->document()->isModified()));
}
void STSourceEditor::setModified(bool val) {
	ui.header->editor()->document()->setModified(val);
	ui.source->editor()->document()->setModified(val);
	slot_modification_changed();
}


void goto_line(QTextEdit *the_editor,int line_number) {
	line_number--;
	QTextCursor cursor=the_editor->textCursor();
	int num=cursor.blockNumber();
	if (num<line_number) {
		cursor.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,line_number-num);
	}
	else if (num>line_number) {
		cursor.movePosition(QTextCursor::PreviousBlock,QTextCursor::MoveAnchor,num-line_number);
	}
	cursor.movePosition(QTextCursor::StartOfBlock);
	cursor.select(QTextCursor::BlockUnderCursor);
	the_editor->setTextCursor(cursor);
}

void STSourceEditor::gotoHeaderLine(int line) {
	ui.tabWidget->setCurrentWidget(ui.header_tab);
	goto_line(ui.header->editor(),line);
}
void STSourceEditor::gotoSourceLine(int line) {
	ui.tabWidget->setCurrentWidget(ui.source_tab);
	goto_line(ui.source->editor(),line);
}


void STSourceEditor::slot_modification_changed() {
	if ((m_C)&&(isModified())&&(!m_C->isFoundationClass())) ui.save_changes_button->setEnabled(true);
	else ui.save_changes_button->setEnabled(false);
}
void STSourceEditor::slotSaveChanges() {
	if (!m_C) return;
	m_C->setCode(ui.header->editor()->toPlainText(),ui.source->editor()->toPlainText());
	setModified(false);
	update();
	emit classChanged();
}

void STSourceEditor::slot_class_box_changed(QString cname) {
	if (cname.isEmpty()) return;
	if ((m_C)&&(m_C->className()==cname)) return;
	emit changeClass(cname);
}

void STSourceEditor::check_to_update_current_class() {
	if (!m_C) return;
	if (isModified()) {
		if (QMessageBox::question(
				this,
				"Keep changes?",
				QString("Would you like to keep changes to %1?").arg(m_C->className()),
				QMessageBox::Yes|QMessageBox::No,
				QMessageBox::Yes)==QMessageBox::Yes) {
			slotSaveChanges();
		}
	}
}

void STSourceEditor::slot_reload() {
	if (!m_C) return;
	if (m_C->headerFileName().isEmpty()) return;
	QString header_code=read_text_file(m_C->headerFileName());
	QString source_code=read_text_file(m_C->sourceFileName());
	m_C->setCode(header_code,source_code);
	update();
	emit classChanged();
}

QString STSourceEditor::currentClassName() {
	if (!m_C) return "";
	return m_C->className();
}

void STSourceEditor::slot_find() {
	if (ui.tabWidget->currentWidget()==ui.header_tab) {
		ui.header->slot_showFindPanel();
	}
	else if (ui.tabWidget->currentWidget()==ui.source_tab) {
		ui.source->slot_showFindPanel();
	}
}

