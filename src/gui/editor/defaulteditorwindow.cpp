#include "defaulteditorwindow.h"
#include <QDebug>
#include <QTextDocumentFragment>

DefaultEditorWindow::DefaultEditorWindow(QWidget *parent): QWidget(parent){
	
	setupUi(this);
	
	m_editor = new HaiQTextEdit;
	QPalette p;
	p.setColor(QPalette::Inactive,QPalette::Highlight,QColor(255,100,28));
	p.setColor(QPalette::Inactive,QPalette::HighlightedText,QColor(255,255,255));
	m_editor->setPalette(p);
	setFocusProxy(m_editor);
	
	m_editorGroup = new HaiQTextEditGroup(m_editor);
	
	//a small workaround to include HaiQTextEditGroup in the widget and make it expandable 
	vboxLayout = new QVBoxLayout(editor_frame);
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	m_editorGroup->setGeometry(QRect(3, 3, 531, 421));
	QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
    	sizePolicy1.setHorizontalStretch(1);
    	sizePolicy1.setVerticalStretch(1);
    	sizePolicy1.setHeightForWidth(m_editorGroup->sizePolicy().hasHeightForWidth());
    	m_editorGroup->setSizePolicy(sizePolicy1);
    	vboxLayout->addWidget(m_editorGroup);
    	
    	//using application icons for the widget
    	toolNext->setIcon(qApp->property("icon.forward").value<QIcon>());
	toolClose->setIcon(qApp->property("icon.close").value<QIcon>());
	toolPrevious->setIcon(qApp->property("icon.backward").value<QIcon>());
	toolFindInProject->setIcon(qApp->property("icon.search").value<QIcon>());
	wrapText->setPixmap(qApp->property("icon.loop").value<QIcon>().pixmap(32,32));
	
	wrapText->setVisible(false);
	frameFind->setVisible(false);
	frameReplace->setVisible(false);
		   	
    	connect(toolClose,SIGNAL(clicked()),this,SLOT(slot_hidePanel()));
    	connect(editFind,SIGNAL(textEdited(const QString&)),this,SLOT(slot_textEdited(const QString&)));
    	connect(checkCase, SIGNAL(stateChanged(int)),this, SLOT(slot_findOptionsChanged()));
    	connect(checkWholeWords, SIGNAL(stateChanged(int)),this, SLOT(slot_findOptionsChanged()));
    	
    	connect(toolPrevious, SIGNAL(clicked()), this, SLOT(slot_FindPrevious()));
	connect(toolNext, SIGNAL(clicked()), this, SLOT(slot_FindNext()));
	connect(editFind, SIGNAL(returnPressed()), this, SLOT(slot_FindNext()));
	connect(toolFindInProject,SIGNAL(clicked()),this,SLOT(slot_findInActiveProject()));
	connect(toolReplace, SIGNAL(clicked()),this,SLOT(slot_replaceText()));
	connect(checkSelectedText, SIGNAL(stateChanged(int)),this,SLOT(slot_findOptionsChanged()));
	
	connect(m_editor,SIGNAL(key_pressed(QKeyEvent)),this,SLOT(slot_editor_key_pressed(QKeyEvent)));
    
}	

HaiQTextEdit *DefaultEditorWindow::editor(){
	return m_editor;
}

HaiQTextEditGroup *DefaultEditorWindow::editorGroup(){
	return m_editorGroup;
}

void DefaultEditorWindow::showFindPanel(){
	slot_showFindPanel();
}

void DefaultEditorWindow::slot_showReplacePanel(){
	slot_showFindPanel();
	frameReplace->setVisible(true);
} 

void DefaultEditorWindow::slot_hidePanel(){
	frameFind->setVisible(false);
	frameReplace->setVisible(false);
}

void DefaultEditorWindow::slot_showFindPanel(){
	all_text_for_find=m_editor->toPlainText();
	QString selected_text=m_editor->textCursor().selectedText();
	if (!selected_text.isEmpty()) {
		editFind->setText(selected_text.mid(0,50));
	}
	slot_update_find_panel();
	if (!frameFind->isVisible()){
		frameFind->setVisible(true);
	}
	editFind->setFocus(Qt::ShortcutFocusReason);
}

void DefaultEditorWindow::slot_update_find_panel(){
	if(checkSelectedText->isChecked()){
		all_text_for_find=m_editor->textCursor().selectedText();
	}
	QString txt= editFind->text();
	QPalette p = editFind->palette();
	p.setColor(QPalette::Active, QPalette::Base, Qt::white);
	num_hits->setText("");
	if (!txt.isEmpty()) {
		Qt::CaseSensitivity cs;
		if (checkCase->isChecked())
			cs=Qt::CaseSensitive;
		else
			cs=Qt::CaseInsensitive;
		if (checkWholeWords->isChecked())
			m_hits=all_text_for_find.count(QRegExp("\\b"+txt+"\\b",cs));
		else
			m_hits=all_text_for_find.count(txt,cs);
		if (m_hits==0) {
			p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
		}
		if (m_hits>99)
			num_hits->setText(">99");
		else
			num_hits->setText(QString::number(m_hits));
	}
	editFind->setPalette(p);
}

void DefaultEditorWindow::update_allTextForFind(){
	all_text_for_find = m_editor->toPlainText();
	slot_update_find_panel(); //we need to update the text and the panel info.
}

void DefaultEditorWindow::slot_textEdited(const QString &text){
	slot_update_find_panel();
	if(!checkSelectedText->isChecked()){
		find(text,false,false);
	}
}

void DefaultEditorWindow::slot_FindNext(){
	find(editFind->text(),true,false);
}

void DefaultEditorWindow::slot_FindPrevious(){
	find(editFind->text(),false,true);
}

void DefaultEditorWindow::find(QString ttf,bool forward, bool backward)
{
	QTextDocument *doc = m_editor->document();
	QString oldText = editFind->text();
	QTextCursor c = m_editor->textCursor();
	QTextDocument::FindFlags options;
	QPalette p = editFind->palette();
	p.setColor(QPalette::Active, QPalette::Base, Qt::white);

	if (c.hasSelection())
		c.setPosition(forward ? c.position() : c.anchor(), QTextCursor::MoveAnchor);

	QTextCursor newCursor = c;

	if (!ttf.isEmpty()) {
		if (backward)
			options |= QTextDocument::FindBackward;

		if (checkCase->isChecked())
			options |= QTextDocument::FindCaseSensitively;

		if (checkWholeWords->isChecked())
			options |= QTextDocument::FindWholeWords;

		newCursor = doc->find(ttf, c, options);
		wrapText->hide();

		if (newCursor.isNull()) {
			QTextCursor ac(doc);
			ac.movePosition(options & QTextDocument::FindBackward
							? QTextCursor::End : QTextCursor::Start);
			newCursor = doc->find(ttf, ac, options);
			if (newCursor.isNull()) {
				p.setColor(QPalette::Active, QPalette::Base, QColor(255, 102, 102));
				newCursor = c;
			} else
				wrapText->show();
		}
	}
	m_editor->setTextCursor(newCursor);
	editFind->setPalette(p);
}

void DefaultEditorWindow::keyPressEvent(QKeyEvent *e)
{
	int key = e->key();

	if (editFind->hasFocus()) {
		switch (key) {
			case Qt::Key_Escape:
				slot_hidePanel();
				return;
		}
	} 
}


void DefaultEditorWindow::slot_editor_key_pressed(QKeyEvent event) {
	if (event.key()==Qt::Key_Escape) {
		slot_hidePanel();
	}
}

void DefaultEditorWindow::slot_findInActiveProject(){
	emit findInActiveProject(editFind->text());
}

void DefaultEditorWindow::slot_replaceText(){
	Qt::CaseSensitivity cs;
	QRegExp tfr;
	if (checkCase->isChecked())
		cs=Qt::CaseSensitive;
	else
		cs=Qt::CaseInsensitive;
	if (checkWholeWords->isChecked())
		tfr = QRegExp("\\b"+editFind->text()+"\\b",cs);
	else
		tfr = QRegExp(editFind->text(),cs);
		
	if(!checkReplaceAll->isChecked()) {
		if(!checkSelectedText->isChecked()){
			if (m_editor->textCursor().selectedText()!=editFind->text()){
				slot_FindNext(); //before replacing, we have to select the text that has to be replaced.
				return;
			}
			m_editor->textCursor().removeSelectedText();
			m_editor->textCursor().insertText(editReplace->text());
		}
		else{
			QString selection=m_editor->textCursor().selectedText();
			int pos=selection.indexOf(tfr,0);
			selection.replace(pos,editFind->text().length(),editReplace->text());
			m_editor->textCursor().removeSelectedText();
			m_editor->textCursor().insertText(selection);
			//oldcursor.select(QTextCursor::BlockUnderCursor);
			//m_editor->setTextCursor(oldcursor);
		}
	} else {
		if(!checkSelectedText->isChecked()){
			//although it has not been reported, not including this check could lead to a bug.
			if (m_editor->textCursor().selectedText()!=editFind->text()){
				slot_FindNext(); //before replacing, we have to select the text that has to be replaced.
				return;
			}
			QTextCursor oldcursor = m_editor->textCursor();
			int numChanges=0;
			while(tfr.exactMatch(m_editor->textCursor().selectedText())&&(numChanges<m_hits)){
				numChanges+=1;
				m_editor->textCursor().removeSelectedText();
				m_editor->textCursor().insertText(editReplace->text());
				slot_FindNext();
			}
			m_editor->setTextCursor(oldcursor);
			m_editor->ensureCursorVisible();
		}
		else{
			//QTextCursor oldcursor = m_editor->textCursor();
			QString selection = m_editor->textCursor().selectedText();
			if (checkWholeWords->isChecked())
				selection.replace(tfr,editReplace->text());
			else{
				selection.replace(tfr,editReplace->text());
			}
			m_editor->textCursor().removeSelectedText();
			m_editor->textCursor().insertText(selection);
			//oldcursor.select(QTextCursor::BlockUnderCursor);
			//m_editor->setTextCursor(oldcursor);
		}
	}
	update_allTextForFind();
	if(!checkSelectedText->isChecked()){
		slot_FindNext();
	}
}

void DefaultEditorWindow::slot_findOptionsChanged(){
	/*Qt::CaseSensitivity cs;
	QRegExp tfr;
	if (checkCase->isChecked())
		cs=Qt::CaseSensitive;
	else
		cs=Qt::CaseInsensitive;
	if (checkWholeWords->isChecked())
		tfr = QRegExp("\\b"+editFind->text()+"\\b",cs);
	else
		tfr = QRegExp(editFind->text(),cs);
	if(!tfr.exactMatch(m_editor->textCursor().selectedText()))
		slot_FindNext();
	slot_update_find_panel();*/
	
	//easy trick to find the matching word
	if (checkSelectedText->isChecked()){
		//the text where we want to search is not the whole text, so we update this
		slot_update_find_panel();
	}
	else{
		//just in case that we have just unchecked replace in the actual selection, we have to update all_text_for_find
		update_allTextForFind();
		//we select the next match only if we don't have a selection where we want to replace strings.
		slot_FindPrevious();
		slot_FindNext();
		slot_update_find_panel();
	}
}
