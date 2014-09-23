#include "haiqtextedit.h"
#include <QTextBlock>
#include <QKeyEvent>
#include <QApplication>
#include <QMenu>
#include <QTextCursor>
#include <QDebug>

void defaultedit_blockdata::add_marker(const haiq_marker &marker) {
	for (int j=0; j<markers.count(); j++) {
		if (markers[j].marker_type==marker.marker_type) {
			markers[j]=marker;
			return;
		}
	}
	markers << marker;
}
void defaultedit_blockdata::remove_marker(const QString &marker_id) {
	bool found=false;
	
	for (int j=0; j<markers.count(); j++) {
		if (markers[j].id==marker_id) {
			found=true;
			markers.removeAt(j);
			j--;
		}
	}
	if (!found) {
		qWarning() << "Unable to remove marker" << marker_id << "from block data";
	}
}
void defaultedit_blockdata::remove_markers_of_type(int marker_type) {
	QList<int> marker_types;
	
	for (int j=0; j<markers.count(); j++) {
		marker_types << markers[j].marker_type;
	}
	for (int j=0; j<marker_types.count(); j++) {
		if (marker_types[j]==marker_type) {
			remove_marker(markers[j].id);
		}
	}
}


HaiQTextEdit::HaiQTextEdit(QWidget *parent) :
        QTextEdit(parent),
	num_tab_spaces(0),
	emit_key_only_mode(false)
{
	setTabStopWidth(20); //pixels
	setFrameStyle(QFrame::NoFrame);

	//here's a temporary workaround to the setLineWrapMode(QTextEdit:NoWrap) bug...
	setLineWrapMode(QTextEdit::NoWrap); //CHECK IF THIS WORKS!! 
	//setLineWrapMode(QTextEdit::FixedPixelWidth);
	//setLineWrapColumnOrWidth(2500);
	////////////////s//////////////////////////////////////////////////////////////////
	
	braces_highlighting_timer.setSingleShot(true);
	braces_highlighting_timer.setInterval(600);
	connect(&braces_highlighting_timer,SIGNAL(timeout()),this,SLOT(slot_clear_braces_highlighting()));
	
	setAcceptRichText(true);

        grabShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Tab), Qt::ApplicationShortcut);
        
        has_temporary_marker=false;
        do_emit_modification_changed=true;
}

HaiQTextEdit::~HaiQTextEdit() {
}

void HaiQTextEdit::set_font(QFont font) {
	setFont(font);
	current_font=font;
}

void HaiQTextEdit::do_marker_highlighting(QTextCursor &cursor) {
	emit highlightMarker(cursor);
}
void HaiQTextEdit::select_current_word() {
	QTextCursor cursor=textCursor();
	QString line=cursor.block().text();
	int index=cursor.columnNumber();
	while ((index-1>=0)&&(is_keyword_character(line[index-1].toAscii()))) {
		index--;
		cursor.movePosition(QTextCursor::Left);
	}
	while ((index<line.count())&&(is_keyword_character(line[index].toAscii()))) {
		index++;
		cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor);
	}
	setTextCursor(cursor);
}
void HaiQTextEdit::mouseDoubleClickEvent ( QMouseEvent * event ) {
	select_current_word();
	emit double_clicked();
}
QString HaiQTextEdit::tab_text(QTextCursor cursor) {
	if (num_tab_spaces==0)
		return "\t";
	else {
		QString ret="";
		
                // ensure that we only add the missing chars up to the next tab stop
                int num = num_tab_spaces - (cursor.columnNumber() % num_tab_spaces);
                for (int j=0; j < num; j++)
			ret+=" ";
		return ret;
	}
}
void HaiQTextEdit::scrollContentsBy ( int dx, int dy ) {
	QTextEdit::scrollContentsBy(dx,dy);
	emit scrolled();
}


void HaiQTextEdit::keyReleaseEvent ( QKeyEvent * event ) {
    if (event->key() == Qt::Key_Backtab) {
        process_tab(*event);
    }
}

void HaiQTextEdit::keyPressEvent ( QKeyEvent * event ) {
	if (isReadOnly()) return;
	last_keypress.start();
	if (emit_key_only_mode) {
		emit key_pressed(*event);
		event->accept();
		return;
	}
	else {
		process_key(*event);
		emit key_pressed(*event);
		event->accept();
		return;
	}
}

void HaiQTextEdit::process_tab(QKeyEvent event) {
        QTextCursor cursor=textCursor();

        // workaround for linux - here Shift+Tab is Qt::Key_Backtab
        if (event.key() == Qt::Key_Backtab && cursor.selectionStart()==cursor.selectionEnd())
            cursor.select(QTextCursor::LineUnderCursor);

	if (cursor.selectionStart()!=cursor.selectionEnd()) {
		QTextCursor cursor1=cursor;
		cursor1.setPosition(cursor.selectionStart());
		QTextCursor cursor2=cursor;
		cursor2.setPosition(cursor.selectionEnd());
		bool done=false;
		while ((cursor1.blockNumber()<=cursor2.blockNumber())&&(!done)) {
			cursor1.movePosition(QTextCursor::StartOfBlock);
                        if (event.key() == Qt::Key_Backtab || (event.key() == Qt::Key_Tab && event.modifiers()==Qt::ShiftModifier)) {
				QString blocktxt=cursor1.block().text();
				if (blocktxt.count()>0) {
					int number=num_tab_spaces;
					if (number==0) 
						number=4;
					if (blocktxt[0]=='\t')
						cursor1.deleteChar();
					else if (blocktxt.mid(0,number)=="    ") {
						int count;
						for (count=1; count<=number; count++)
							cursor1.deleteChar();
					}
				}
			}
			else
				cursor1.insertText(tab_text(cursor1));
			if (!cursor1.movePosition(QTextCursor::NextBlock))
				done=true;
		}
	}
	else 
		cursor.insertText(tab_text(cursor));
}


void HaiQTextEdit::highlight_braces(const QString & keytxt,bool backward) {
	int endchar=0;
	int startchar=0;
	if ((keytxt==")")||(keytxt=="(")) {
		startchar='(';
		endchar=')';
	}
	else if ((keytxt=="}")||(keytxt=="{")) {
		startchar='{';
		endchar='}';
	}
	/*else if ((keytxt==">")||(keytxt=="<")) {
		startchar='<';
		endchar='>';
	}*/	
	
	if (!backward) { //everything is modeled on backward=true... forward case is hacked.
		int holdchar=startchar;
		startchar=endchar;
		endchar=holdchar;
	}
	
	QTextCursor cursor=textCursor();
	QTextBlock current_block=cursor.block();
	int current_col=cursor.columnNumber();
	
	int close_position=-1;
	int open_position=-1;
	QTextBlock block=current_block;
	int ct=0;
	int braces_level=0;
	while (((close_position<0)||(open_position<0))&&(ct<100)) {
		if (!block.isValid()) return;
		QString txt=block.text();
		int col_start=txt.count()-1;
		int col_end=0;
		int increment=-1;
		if (!backward) {
			col_start=0;
			col_end=txt.count()-1;
			increment=+1;
		}
		if (ct==0) {
			col_start=current_col-1;
			if (col_start<0) col_start=0; //to be safe
		}
		
		for (int j=col_start; j!=col_end+increment; j+=increment) {
			if (txt[j]==startchar) {
				if (close_position<0)
					return;
				if (braces_level==0) {
					if (open_position<0)
						open_position=block.position()+j;
				}
				else {
					braces_level--;
				}
			}
			else if (txt[j]==endchar) {
				if (close_position<0) {
					close_position=block.position()+j;
				}
				else {
					braces_level++;
				}
			}
		}
		if (backward)
			block=block.previous();
		else
			block=block.next();
		ct++;
	}
	if ((open_position>=0)&&(close_position>=0)) {
		
		QColor colr;
		if ((startchar=='(')||(startchar==')'))
			colr=qRgb(200,200,255);
		else if ((startchar=='{')||(startchar=='}'))
			colr=qRgb(255,200,200);
		else if ((startchar=='<')||(startchar=='>'))
			colr=qRgb(200,255,200);
		
		QTextCursor c_open=textCursor(); 
		c_open.joinPreviousEditBlock();
			c_open.setPosition(open_position);
			c_open.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
			QTextCharFormat format;
			format=c_open.charFormat();
			format.setBackground(QBrush(colr));
			c_open.setCharFormat(format);
			if (!blocks_highlighted_for_braces.contains(c_open.block()))
				blocks_highlighted_for_braces << c_open.block();
		c_open.endEditBlock();
		
		QTextCursor c_close=textCursor(); 
		c_close.joinPreviousEditBlock();
			c_close.setPosition(close_position);
			c_close.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
			format=c_close.charFormat();
			format.setBackground(QBrush(colr));
			c_close.setCharFormat(format);
			if (!blocks_highlighted_for_braces.contains(c_close.block()))
				blocks_highlighted_for_braces << c_close.block();
		c_close.endEditBlock();

		braces_highlighting_timer.start();
	}
}

void HaiQTextEdit::slot_clear_braces_highlighting() {
	if (last_keypress.elapsed()<60)  { //user is moving so don't clear yet
		braces_highlighting_timer.start();
		return;
	}
		
	
	for (int j=0; j<blocks_highlighted_for_braces.count(); j++) {
		QTextBlock block=blocks_highlighted_for_braces[j];
		QTextCursor cursor(block);
		cursor.joinPreviousEditBlock();
			
			int N=block.text().count();
			for (int j=0; j<N; j++) {
				cursor.joinPreviousEditBlock();
					cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
					QTextCharFormat format=cursor.charFormat();
					format.clearBackground();
					cursor.setCharFormat(format);
					cursor.movePosition(QTextCursor::NoMove,QTextCursor::MoveAnchor);
				cursor.endEditBlock();
			}
		cursor.endEditBlock();
	}
	blocks_highlighted_for_braces.clear();
}

void HaiQTextEdit::process_key(QKeyEvent event) {
	if ((event.text()!="")&&(textCursor().selectedText().count()<=1)) { 
		///this is to avoid spreading of marker highlighting
		bool holdmodified=document()->isModified();
		QTextCharFormat format=currentCharFormat();
		format.clearBackground();
		setCurrentCharFormat(format);
		document()->setModified(holdmodified);
	}
	if ((event.key()==Qt::Key_Tab)||(event.key()==Qt::Key_Backtab)||(event.text()=="\t")) {
		process_tab(event);
	}
        else if (event.key()==Qt::Key_Insert && QApplication::keyboardModifiers() == Qt::NoModifier) {
		setOverwriteMode(!overwriteMode());
	}
	else if ((event.key()==Qt::Key_Home)&&((event.modifiers()==Qt::NoModifier)||(event.modifiers()==Qt::ShiftModifier))) {
		bool shift_key=(event.modifiers()==Qt::ShiftModifier);
		QTextCursor cursor=textCursor();
		int col=cursor.columnNumber();
		QString txt=cursor.block().text();
		if (shift_key)
			cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
		else
			cursor.movePosition(QTextCursor::StartOfLine);
		int ct=0;
		while ((ct<txt.count())&&((txt[ct]==' ')||(txt[ct]=='\t'))) {
			if (shift_key)
				cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor);
			else
				cursor.movePosition(QTextCursor::Right);
			ct++;
		}
		int col2=cursor.columnNumber();
		if (col2==col) { //moved nowhere
			if (shift_key)
				cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
			else
				cursor.movePosition(QTextCursor::StartOfLine);
		}
		setTextCursor(cursor);
	}
	else if (event.key()==Qt::Key_Backspace) {
		process_backspace();
		return;
	}
	else if ((event.key()==Qt::Key_Space)&&(event.modifiers()==Qt::ControlModifier)) {
		return; // don't process a space, because Ctrl+Space is for completion
	}
	else {
		QTextEdit::keyPressEvent(&event);
	}

	if ((event.key()==Qt::Key_Enter)||(event.key()==Qt::Key_Return)) {
		indent_to_previous_line();
		QTextCursor cursor=textCursor();
		do_marker_highlighting(cursor);
		QString line=cursor.block().previous().text();
		QStringList symbols=split_into_symbols(line);
		if ((symbols.count()>0)&&
		    ( (symbols[symbols.count()-1]=="{") || (((symbols[symbols.count()-1]!=";"))&&((symbols[0]=="if") || (symbols[0]=="else") || (symbols[0]=="while") || (symbols[0]=="do") || (symbols[0]=="for"))) ) 
		    ) {
			QString line2=cursor.block().text();
			QStringList symbols2=split_into_symbols(line2);
			if ((symbols2.count()==0)||(symbols2[0]!="}")) //only indent if there is not a "{" starting the line
				cursor.insertText(tab_text(cursor));
		}
		//kind of a hack
		if (!blocks_highlighted_for_braces.isEmpty()) {
			blocks_highlighted_for_braces << textCursor().block();
		}
	}
	if (event.text()=="}") {
		QTextCursor cursor=textCursor();
		QString line=cursor.block().text();
		QStringList symbols=split_into_symbols(line);
		if (symbols.count()==1) {
			//unindent to last brace
			QTextBlock block=cursor.block();
			bool found=false;
			int braces_level=1;
			while ((!found)&&(block.isValid())) {
				block=block.previous();
				QString line2=block.text();
				QStringList symbols=split_into_symbols(line2);
				
				for (int j=symbols.count()-1; j>=0; j--) {
					if (symbols[j]=="{")
						braces_level--;
					if (symbols[j]=="}")
						braces_level++;
					if (braces_level==0)
						found=true;
				}
			}
			if (found) {
				QString line2=block.text();
				
				for (int j=0; j<line.count(); j++)
					cursor.deletePreviousChar();
				int j=0;
				while ((j<line2.count())&&((line2[j]==' ')||(line2[j]=='\t'))) {
					cursor.insertText(QString(line2[j]));
					j++;
				}
				cursor.insertText("}");
			}
		}
	}
	if ((event.text()==")")||(event.text()=="}")/*||(event.text()==">")*/) {
		highlight_braces(event.text(),true);
	}
	if ((event.text()=="(")||(event.text()=="{")/*||(event.text()=="<")*/) {
		highlight_braces(event.text(),false);
	}
}

//return the number of extra initial whitespace characters that str1 has over str2
int whitespace_difference(const QString & str1,const QString &str2) {

	int j;	
	for (j=0; 
			(j<str1.count())&&(j<str2.count())&&
			((str1[j]==' ')||(str1[j]=='\t'))&&
			(str1[j]==str2[j]); 
			j++);
	int common_whitespace_count=j;
	for (int j=0; 
			(j<str1.count())&&
			((str1[j]==' ')||(str1[j]=='\t')); 
			j++);
	int str1_whitespace_count=j;
	for (int j=0; 
			(j<str2.count())&&
			((str2[j]==' ')||(str2[j]=='\t')); 
			j++);
	int str2_whitespace_count=j;
	if (str2_whitespace_count>common_whitespace_count)
		return 0;
	if (str1_whitespace_count>common_whitespace_count)
		return str1_whitespace_count-common_whitespace_count;
	return 0;
}

void HaiQTextEdit::process_backspace() {
	QTextCursor cursor=textCursor();
	QString txt=cursor.block().text();
	int col=cursor.columnNumber();
	
	//first of all, if it's a tab, just delete it
	if ((col>0)&&(txt[col-1]=='\t')) {
		cursor.deletePreviousChar();
		return;
	}
	//if there is selected text, just delete it
	if (cursor.selectedText()!="") {
	        cursor.deleteChar();
	        return;
	}
	
	bool nothing_but_whitespace=true;
	
	for (int j=0; (j<col)&&(nothing_but_whitespace); j++) {
		if ((txt[j]!=' ')&&(txt[j]!='\t')) {
			nothing_but_whitespace=false;
		}
	}
	if (nothing_but_whitespace) {
		QTextCursor cursor2=cursor;
		QString txt1=txt.mid(0,col);
		while (cursor2.movePosition(QTextCursor::PreviousBlock)) {
			QString txt2=cursor2.block().text();
			int diff=whitespace_difference(txt1,txt2);
			if (diff<=0) diff=col;
			if (diff>0) {
				int ct;
				for (ct=0; ct<diff; ct++)
					cursor.deletePreviousChar();
				return;
			}
		}
	}
	if ((col>0)&&(txt[col-1]==' ')) {
	   	cursor.deletePreviousChar();
	   	col--;
	   	if (num_tab_spaces>1) {
	   		int ct;
	   		for (ct=0; ct<num_tab_spaces-1; ct++) {
	   			if ((col>0)&&(txt[col-1]==' ')) {
	   				cursor.deletePreviousChar();
	   				col--;
	   			}
	   		}
	   	}
	}
	else
	   cursor.deletePreviousChar();
}

void HaiQTextEdit::indent_to_previous_line() {
	QTextCursor cursor=textCursor();
	cursor.joinPreviousEditBlock();
	QTextBlock block=cursor.block();
	QTextBlock blockprev=block.previous();
	if (blockprev.isValid()) {
		QString txt=blockprev.text();
		int j=0;
		while ( (j<txt.count()) && ((txt[j]==' ')||(txt[j]==9)) ) {
			cursor.insertText(QString(txt[j]));
			j++;
		}
	}
	cursor.endEditBlock();
}

void HaiQTextEdit::insertFromMimeData ( const QMimeData * source ) {
	QTextCursor cursor=textCursor();
	QString txt=source->text();
	setFont(current_font);
	cursor.insertText(txt);
}

void HaiQTextEdit::contextMenuEvent(QContextMenuEvent *event)
 {
 	//QTextEdit::contextMenuEvent(event);
 	
 	if (textCursor().selectedText().isEmpty())
 		setTextCursor(cursorForPosition(event->pos()));
 	qApp->processEvents();
 	emit context_menu(event->globalPos());
 } 

bool is_keyword_character(char c) {
	if (('a'<=c)&&(c<='z')) return true;
	if (('A'<=c)&&(c<='Z')) return true;
	if (('0'<=c)&&(c<='9')) return true;
	if (c=='_') return true;
	if (c=='~') return true;
	return false;
}

bool is_initial_identifier_character(QChar c) {
	return ((c.isLetter())||(c=='_')||(c=='~'));
}
bool is_identifier_character(QChar c) {
	return ((c.isLetter())||(c=='_')||(c=='~')||(c.isDigit()));
}
bool is_initial_number_character(QChar c) {
	return (c.isDigit());
}
bool is_number_character(QChar c) {
	return ((c.isDigit())||(c=='.'));
}

QStringList split_into_symbols(const QString &str) {
	QStringList ret;
	QStringList list1=str.split(QRegExp("\\s+"),QString::SkipEmptyParts); //split by white space
	
	
	for (int j=0; j<list1.count(); j++) {
		QString hold=list1[j];
		while (hold.count()>0) {
			if (hold.count()>0) {
				if (is_initial_identifier_character(hold[0])) { 
					int k=1;
					while ((k<hold.count())&&(is_identifier_character(hold[k]))) 
						k++;
					ret << hold.mid(0,k);
					hold=hold.mid(k);
				}
				else if (is_initial_number_character(hold[0])) {
					int k=1;
					while ((k<hold.count())&&(is_number_character(hold[k]))) 
						k++;
					ret << hold.mid(0,k);
					hold=hold.mid(k);
				}
				/*else if (hold[0]=='\"') {
					int k=1;
					bool last_character_was_escape=false;
					while ((k<hold.count())&&( (hold[k]!='\"') || (last_character_was_escape) )) {
						if (hold[k]=='\\')
							last_character_was_escape=!last_character_was_escape;
						else
							last_character_was_escape=false;
						k++;
					}
					if ((hold[k]=='\"')&&(!last_character_was_escape)) {
						ret << hold.mid(0,k+1);
						hold=hold.mid(k+1);
					}
					else {
						ret << hold.mid(0,k);
						hold=hold.mid(k);
					}
				}*/
				else {
					ret << QString(hold[0]);
					hold=hold.mid(1);
				}
			}
		}
	}
	return ret;
}

bool remove_single_line_comment(QString &str) {
	bool in_a_string=false;
	bool previous_character_was_escape=false; //relevant for within a string
	int j=0;
	while (j<str.count()-1) {
		if (!in_a_string) {
			if (str[j]=='\"') in_a_string=true;
			else if (str.mid(j,2)=="//") { //single line comment 
				str=str.mid(0,j);
				return true;
			}
		}
		else {
			if (str[j]=='\"') {
				if (!previous_character_was_escape)
					in_a_string=false;
			}
			if (str[j]=='\\')
				previous_character_was_escape=!previous_character_was_escape;
			else 
				previous_character_was_escape=false;
		}
		j++;
	}
	return false;
}
bool remove_multi_line_comments(QString &str,bool in_a_multi_line_comment) { //returns true if multi-line comment extends past this line
	//assumes there are no single line comments
	bool in_a_string=false;
	bool in_a_comment=in_a_multi_line_comment;
	int beginning_of_comment=0;
	bool previous_character_was_escape=false; //relevant for within a string
	int j=0;
	while (j<str.count()-1) {
		if ((!in_a_string)&&(!in_a_comment)) {
			if (str[j]=='\"') 
				in_a_string=true;
			else if ((str.mid(j,2)=="/*")) { //single line comment begin
				beginning_of_comment=j;
				in_a_comment=true;
			}
		}
		else if ((!in_a_string)&&(in_a_comment)) {
			if (str[j]=='\"') 
				in_a_string=true;
			else if ((str.mid(j,2)=="*/")) { //single line comment end
				str.remove(beginning_of_comment,j-beginning_of_comment+2);
				j=beginning_of_comment-1;
				in_a_comment=false;
			}
		}
		else { //in a string
			if (str[j]=='\"') {
				if (!previous_character_was_escape)
					in_a_string=false;
			}
			if (str[j]=='\\')
				previous_character_was_escape=!previous_character_was_escape;
			else 
				previous_character_was_escape=false;
		}
		j++;
	}
	if (in_a_comment) {
		str=str.mid(0,beginning_of_comment);
		return true;
	}
	return false;
}


QStringList split_into_symbols_after_removing_comments(const QString &str) {
	QStringList ret; 

	QStringList lines=str.split("\n");

	//first remove comments
	bool multiline_comment_mode=false;
	
	for (int j=0; j<lines.count(); j++) {
		if (!multiline_comment_mode) {
			remove_single_line_comment(lines[j]);
			multiline_comment_mode=remove_multi_line_comments(lines[j],false);
		}
		else { //in a multiline comment
			multiline_comment_mode=remove_multi_line_comments(lines[j],true);
		}
	}

	for (int j=0; j<lines.count(); j++) {
		ret << split_into_symbols(lines[j]);	
	}

	return ret;
}


bool defaultedit_blockdata::has_marker(int marker_type) {
	
	for (int j=0; j<markers.count(); j++) {
		if (markers[j].marker_type==marker_type)
			return true;
	}
	return false;
}

haiq_marker defaultedit_blockdata::get_marker(int marker_type) {
	
	for (int j=0; j<markers.count(); j++) {
		if (markers[j].marker_type==marker_type)
			return markers[j];
	}
	haiq_marker empty;
	return empty;
}

QList<haiq_marker> HaiQTextEdit::get_markers() {
	QList<haiq_marker> ret;
	QTextCursor cursor=textCursor();
	cursor.movePosition(QTextCursor::Start);
	QTextBlock block=cursor.block();
	int line_number=1;
	while (block.isValid()) {
		defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
		if (block_data) {			
			for (int j=0; j<block_data->markers.count(); j++) {
				haiq_marker marker=block_data->markers[j];
				marker.line_number=line_number;
				ret << marker;
			}
		}
		block=block.next();
		line_number++;
	}
	return ret;
}
void HaiQTextEdit::add_marker(haiq_marker marker) {
	bool hold_emit_modification_changed=do_emit_modification_changed;
	do_emit_modification_changed=false;
	if ((has_temporary_marker)&&(marker.marker_type==MARKER_TYPE_TEMPORARY)) {
		remove_markers_of_type(MARKER_TYPE_TEMPORARY);
	}
	QTextCursor cursor=textCursor();
	int num=cursor.blockNumber();
	if (num<marker.line_number-1) {
		cursor.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,(marker.line_number-1)-num);
	}
	else if (num>marker.line_number-1) {
		cursor.movePosition(QTextCursor::PreviousBlock,QTextCursor::MoveAnchor,num-(marker.line_number-1));
	}
	cursor.movePosition(QTextCursor::StartOfBlock);
	QTextBlock block=cursor.block();
	defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
	if (!block_data) {
		block_data=new defaultedit_blockdata;
		block.setUserData(block_data);
	}
	if (block_data->has_marker(marker.marker_type))
		return; //already has such a marker
	block_data->add_marker(marker);
	if (marker.marker_type==MARKER_TYPE_TEMPORARY) {
		has_temporary_marker=true;
	}
	do_marker_highlighting(cursor);
	markers[marker.id]=marker;
	marker_blocks[marker.id]=cursor.block();
	do_emit_modification_changed=hold_emit_modification_changed;
	emit markersChanged();
};
void HaiQTextEdit::remove_marker(const QString &marker_id) {
	if (!markers.contains(marker_id)) {
		qWarning() << "HaiQTextEdit does not contain marker" << marker_id;
		return;
	}
	bool hold_emit_modification_changed=do_emit_modification_changed;
	haiq_marker marker=markers[marker_id];
	do_emit_modification_changed=false;
	if (marker.marker_type==MARKER_TYPE_TEMPORARY)
		has_temporary_marker=false;
	QTextBlock block=marker_blocks[marker_id];
	markers.remove(marker_id);
	marker_blocks.remove(marker_id);
	bool markers_changed=false;
	if (block.isValid()) {
		QTextCursor cursor(block);
		defaultedit_blockdata *data=(defaultedit_blockdata *)block.userData();
		if (data) {
			data->remove_marker(marker_id);
			marker_blocks.remove(marker_id);
			markers_changed=true;
			do_marker_highlighting(cursor);
		}		
		else {
			qWarning() << "data is NULL in HaiQDefaultEdit::remove_marker";
		}
	}
	else {
		qWarning() << "block is not valid in HaiQDefaultEdit::remove_marker";
	}
	do_emit_modification_changed=hold_emit_modification_changed;
	if (markers_changed) emit markersChanged();
}
void HaiQTextEdit::remove_markers_of_type(int marker_type) {
	QList<haiq_marker> list=markers.values();
	//bool something_removed=false;
	for (int j=0; j<list.count(); j++) {
		haiq_marker marker=list[j];
		if (marker.marker_type==marker_type) {
			remove_marker(marker.id);
		}
	}
};
void HaiQTextEdit::goto_marker(QString id) {
	if (!marker_blocks.contains(id)) return;
	QTextBlock block=marker_blocks[id];
	defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
	if (block_data) {
		setTextCursor(QTextCursor(block));
		ensureCursorVisible();
	}
}
void HaiQTextEdit::toggle_marker(int marker_type,QString path) {
	bool markers_changed=false;
	QTextCursor cursor=textCursor();
	QTextBlock block=cursor.block();
	defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
	if (!block_data) {
		block_data=new defaultedit_blockdata;
		block.setUserData(block_data);
	}	
	if (block_data->has_marker(marker_type))  {
		block_data->remove_markers_of_type(marker_type);
		markers_changed=true;
	}
	else {
		haiq_marker marker;
		marker.path=path;
		marker.marker_type=marker_type;
		marker.line_number=textCursor().blockNumber()+1;
		set_random_id(marker);
		add_marker(marker);
	}
	do_marker_highlighting(cursor);
	if (markers_changed) emit markersChanged();
}

void HaiQTextEdit::goto_previous_bookmark() {
	QTextBlock block=textCursor().block();
	while (block.isValid()) {
		block=block.previous();
		defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
		if (block_data) {
			if (block_data->has_marker(MARKER_TYPE_BOOKMARK)) {
				setTextCursor(QTextCursor(block));
				ensureCursorVisible();
				return;
			}
		}		
	}
}
void HaiQTextEdit::goto_next_bookmark() {
	QTextBlock block=textCursor().block();
	while (block.isValid()) {
		block=block.next();
		defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
		if (block_data) {
			if (block_data->has_marker(MARKER_TYPE_BOOKMARK)) {
				setTextCursor(QTextCursor(block));
				ensureCursorVisible();
				return;
			}
		}		
	}
}

/**
 * Delete all chars from current cursor position to end of line
 */
void HaiQTextEdit::delete_end_of_line() {
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.endEditBlock();
}

/**
 * Delete all chars from current cursor position to end of word
 */
void HaiQTextEdit::delete_end_of_word() {
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.endEditBlock();
}

/**
 * Duplicates the current line and inserts the copy below the line
 */
void HaiQTextEdit::duplicate_line() {
    QTextCursor cursor = textCursor();
    int pos(cursor.position());
    cursor.beginEditBlock();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::EndOfLine);
    cursor.select(QTextCursor::LineUnderCursor);
    QTextDocumentFragment text( cursor.selection() );
    cursor.clearSelection();
    cursor.insertText( QString(QChar::LineSeparator) );
    cursor.insertFragment( text );
    cursor.setPosition(pos);
    cursor.endEditBlock();
}

/**
 * Deletes the current line
 */
void HaiQTextEdit::delete_current_line() {
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::StartOfLine);
    int pos(cursor.position());
    cursor.select(QTextCursor::LineUnderCursor);
    // remove line (and line feed char)
    cursor.removeSelectedText();
    cursor.deleteChar();
    // goto start of next line
    cursor.setPosition(pos);
    cursor.endEditBlock();
}



