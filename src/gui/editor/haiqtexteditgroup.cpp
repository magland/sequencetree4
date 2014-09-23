#include "haiqtexteditgroup.h"
#include <QPalette>
#include <QFont>
#include <QPainter>
#include <QTextEdit>
#include <QScrollBar>
#include <QTextLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QToolTip>
#include <QDebug>


HaiQEditPanel::HaiQEditPanel(HaiQTextEdit *editor) {
	setMouseTracking(true);
	m_num_marker_columns=0;
	m_currentLine=0;
	textedit=editor;
	connect(textedit,SIGNAL(scrolled()),this,SLOT(update()));
	connect(textedit,SIGNAL(markersChanged()),this,SLOT(slot_markers_changed()));
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
	set_font(QFont("Arial",10));
	setAutoFillBackground( true );
	QPalette p( palette() );
	p.setColor( backgroundRole(), qRgb(230,230,255) );
	setPalette( p );
	//m_hasBlockIcons =true;
}

HaiQEditPanel::~HaiQEditPanel() {
}

void HaiQEditPanel::setBlockIcons(bool enable){
	m_hasBlockIcons = enable;
}

bool HaiQEditPanel::hasBlockIcons(){
	return m_hasBlockIcons;
}

void HaiQEditPanel::set_font(const QFont &font_in) {
	setFont(font_in);
	int window_width=QFontMetrics(font()).width("99999");
	/*if (m_hasBlockIcons){ // we need to make sure that we can display a maximum of 4 icons in a row
				window_width += 4*QFontMetrics(font()).height()+2;
	}*/
	if (window_width!=minimumSize().width()) {
		setMinimumSize(window_width,0);
		setMaximumSize(window_width,10000);
	}
	update();
}

void HaiQEditPanel::draw_icon(QPainter &painter, const haiq_marker &marker, int line_number, int xpos, int ypos, const QPixmap &pm) {
	HaiQEditPanelVisibleIcon new_icon;
	new_icon.marker=marker;
	new_icon.line_number=line_number;
	new_icon.rect=QRect(xpos,ypos,pm.width(),pm.height());
	m_visible_icons << new_icon;
	painter.drawPixmap(xpos,ypos,pm);
}

void HaiQEditPanel::paintEvent ( QPaintEvent * event ) {
	if (!textedit) return;
	QTextCursor cursor=textedit->cursorForPosition(QPoint(0,0));
	QTextBlock block=cursor.block();
	//if (!cursor.block().layout()) return;
	
	m_visible_icons.clear();
	
	int yoffset=-textedit->verticalScrollBar()->value();
	
	int line_number=cursor.blockNumber();
	
	int num_lines=textedit->document()->blockCount();
	QString holdnum=QString::number(num_lines);
	for (int k=0; k<holdnum.count(); k++)
		holdnum[k]='0'; //turn it to all 0s so we don't get shifting in the middle of editing
	int window_width=QFontMetrics(font()).width(holdnum)+5;
	bool done=false;
	if (m_hasBlockIcons){
		/*int max_icons=0;
		while(!done){
			defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
			if (block_data){
				if(block_data->markers.count()>max_icons)
					max_icons=block_data->markers.count();
			}
			block=block.next();
			if (!block.isValid()) 
				done=true;
		}		
		window_width += max_icons*(QFontMetrics(font()).height()+1);*/
		window_width += m_num_marker_columns*(QFontMetrics(font()).height()-4);
	}
		
	if (window_width!=minimumSize().width()) {
		setMinimumSize(window_width,0);
		setMaximumSize(window_width,10000);
	}
	block=cursor.block(); //we revert the changes in block.
	QPainter painter(this);
	painter.setPen(Qt::black);
	//painter.setFont(font());
	QFont f = font();
	QFont fBold = f;
	fBold.setBold(true);
	
	//int yadjust=textedit->viewport()->y()-y();
	int editor_height=textedit->viewport()->height();
	done=false;
	while (!done) {
		int ypos=(int)block.layout()->position().y()+yoffset;
		int yheight=(int)block.layout()->boundingRect().height();
		if (line_number == m_currentLine)
			painter.setFont(fBold);
		else
			painter.setFont(f);
		painter.drawText(0,ypos,width(),yheight,Qt::AlignVCenter|Qt::AlignRight,QString::number(line_number+1));
		
		//marker icons
		if (m_hasBlockIcons){
			defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
			if (block_data) {
				int yicon = ypos+2;// m_iconSize>QFontMetrics(font()).height()?ypos+QFontMetrics(font()).height()/2:ypos+m_iconSize/2;
				int hicon = QFontMetrics(font()).height()-4;
				int xicon =0;
				xicon = width()-2-QFontMetrics(font()).width(QString::number(line_number+1))-hicon;
				//order these conditionals to display icons from left to right. 
				if (block_data->has_marker(MARKER_TYPE_BREAKPOINT)){
					haiq_marker m=block_data->get_marker(MARKER_TYPE_BREAKPOINT);
					draw_icon(painter,m,line_number+1,xicon,yicon,qApp->property("icon.debug->breakpoint").value<QIcon>().pixmap(hicon,hicon));
                        	xicon -= (hicon+1);
				}	
			    	if (block_data->has_marker(MARKER_TYPE_TEMPORARY)){
			    		haiq_marker m=block_data->get_marker(MARKER_TYPE_TEMPORARY);
					draw_icon(painter,m,line_number+1,xicon,yicon,qApp->property("icon.debug->tempbreakpoint").value<QIcon>().pixmap(hicon,hicon));
                        	xicon -= (hicon+1);
				}
				if (block_data->has_marker(MARKER_TYPE_BUILD_ERROR)){
					haiq_marker m=block_data->get_marker(MARKER_TYPE_BUILD_ERROR);
					draw_icon(painter,m,line_number+1,xicon,yicon,qApp->property("icon.error").value<QIcon>().pixmap(hicon,hicon));
                        	xicon -= (hicon+1);				
				}	
				if (block_data->has_marker(MARKER_TYPE_BUILD_WARNING)){
					haiq_marker m=block_data->get_marker(MARKER_TYPE_BUILD_WARNING);
					draw_icon(painter,m,line_number+1,xicon,yicon,qApp->property("icon.warning").value<QIcon>().pixmap(hicon,hicon));
                        	xicon -= (hicon+1);				
			     }
				if (block_data->has_marker(MARKER_TYPE_FIND_STRING)){
					haiq_marker m=block_data->get_marker(MARKER_TYPE_FIND_STRING);
					draw_icon(painter,m,line_number+1,xicon,yicon,qApp->property("icon.search").value<QIcon>().pixmap(hicon,hicon));
                        	xicon -= (hicon+1);				
				}	
				if (block_data->has_marker(MARKER_TYPE_DEBUG_POSITION)){
			     	haiq_marker m=block_data->get_marker(MARKER_TYPE_DEBUG_POSITION);
					draw_icon(painter,m,line_number+1,xicon,yicon,qApp->property("icon.forward").value<QIcon>().pixmap(hicon,hicon));
                        	xicon -= (hicon+1);				
				}				
				if (block_data->has_marker(MARKER_TYPE_BOOKMARK)){
			     	haiq_marker m=block_data->get_marker(MARKER_TYPE_BOOKMARK);
					draw_icon(painter,m,line_number+1,xicon,yicon,qApp->property("icon.bookmark").value<QIcon>().pixmap(hicon,hicon));
                        	xicon -= (hicon+1);				
				}
			}
		}
		block=block.next();
		line_number++;
		if (!block.isValid()) 
			done=true;
		if (ypos>editor_height)
			done=true;
	}
}

// The event doesn't work properly, as i can't detect the line number where the event was emited.
/*void HaiQEditPanel::mouseDoubleClickEvent ( QMouseEvent * event ) {
	if (event->button()==Qt::LeftButton){
		if (m_hasBlockIcons){
			QPoint linePos = event->globalPos();
			linePos.rx()+=width()+10;
			int line = textedit->cursorForPosition(linePos).blockNumber();
			emit addBookmark(line);
		}
	}
}*/

int HaiQEditPanel::visible_icon_index_at(QPoint pos) {
	for (int j=0; j<m_visible_icons.count(); j++) {
		if (m_visible_icons[j].rect.contains(pos)) {
			return j;
		}
	}
	return -1;
}

void HaiQEditPanel::mousePressEvent(QMouseEvent *event) {
	if ((event->button()==Qt::LeftButton)||(event->button()==Qt::RightButton)) {
		int index=visible_icon_index_at(event->pos());
		if (index>=0) {
			emit markerClicked(event->pos(),m_visible_icons[index].marker,m_visible_icons[index].line_number);
		}
	}
	else QWidget::mousePressEvent(event);
}


void HaiQEditPanel::mouseMoveEvent(QMouseEvent *event) {
	int index=visible_icon_index_at(event->pos());
	if (index>=0) {
		if (!m_visible_icons[index].marker.description.isEmpty()) {
			QToolTip::showText(mapToGlobal(event->pos()),m_visible_icons[index].marker.description,this,m_visible_icons[index].rect);
			return;
		}
	}
	QToolTip::showText(mapToGlobal(event->pos()),"",this);
}

int HaiQEditPanel::currentLine(){
	return m_currentLine;
}
void HaiQEditPanel::setCurrentLine(int line){
	m_currentLine = line;
	update();
}

HaiQTextEditGroup::HaiQTextEditGroup(HaiQTextEdit *editor_in,QWidget *parent) : QWidget(parent) {
	editor=editor_in;
	panel=new HaiQEditPanel(editor);
	
	QHBoxLayout *layout=new QHBoxLayout;
	layout->setMargin(0); 
	layout->setSpacing(3); 
	setLayout(layout);
	layout->addWidget(panel);
	layout->addWidget(editor);
	setFocusProxy(editor);
	
	connect(editor,SIGNAL(cursorPositionChanged()),this,SLOT(slot_cursorPositionChanged()));
	connect(panel,SIGNAL(addBreakPoint(int)),this,SIGNAL(addBreakPoint(int)));
	connect(editor,SIGNAL(highlightMarker(QTextCursor)),this,SIGNAL(highlightMarker(QTextCursor)));
	connect(panel,SIGNAL(markerClicked(QPoint, haiq_marker, int)),this,SIGNAL(markerClicked(QPoint, haiq_marker, int)));
}
HaiQTextEditGroup::~HaiQTextEditGroup() {}

void HaiQTextEditGroup::set_font(QFont font) {
	editor->set_font(font);
	panel->set_font(font);
}

void HaiQTextEditGroup::slot_cursorPositionChanged(){
	QTextCursor c = editor->textCursor();
	if (panel->currentLine() != c.blockNumber())
		panel->setCurrentLine(c.blockNumber());
}

void HaiQTextEditGroup::showPanel(bool show){
	//if(panel->isVisible()!=show)
		panel->setVisible(show);
}

void HaiQTextEditGroup::updatePanel(){
	panel->update();
}

bool HaiQTextEditGroup::isPanelVisible(){
	return panel->isVisible();
}

void HaiQTextEditGroup::displayMarkerIcons(bool enable){
	panel->setBlockIcons(enable);
}

void HaiQEditPanel::slot_markers_changed() {
	m_num_marker_columns=0;
	QHash<QString,QTextBlock> marker_blocks=textedit->marker_blocks;
	foreach (QTextBlock block,marker_blocks) {
		if (block.isValid()) {
			defaultedit_blockdata *block_data=(defaultedit_blockdata *)block.userData();
			if (block_data) {
				if (block_data->markers.count()>m_num_marker_columns)
					m_num_marker_columns=block_data->markers.count();
			}
		}
	}
	update();
}
