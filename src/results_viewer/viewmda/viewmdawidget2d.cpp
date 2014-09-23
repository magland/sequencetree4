#include "viewmdawidget2d.h"
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

ViewmdaWidget2D::ViewmdaWidget2D(QWidget *parent) : QWidget(parent)
{
	m_need_to_update_the_image=true;
	m_model=0;
	setFocusPolicy(Qt::StrongFocus);
}

ViewmdaWidget2D::~ViewmdaWidget2D()
{
}

void ViewmdaWidget2D::setModel(ViewmdaModel *M) {
	m_model=M;
	
	connect(m_model,SIGNAL(arrayChanged()),this,SLOT(slot_array_changed()));
	connect(m_model,SIGNAL(dimensionsChanged()),this,SLOT(slot_dimensions_changed()));
	connect(m_model,SIGNAL(currentIndexChanged()),this,SLOT(slot_current_index_changed()));
	connect(m_model,SIGNAL(currentSliceChanged()),this,SLOT(slot_current_slice_changed()));
	connect(m_model,SIGNAL(selectedPointsChanged()),this,SLOT(slot_selected_points_changed()));
	connect(m_model,SIGNAL(selectedRectChanged()),this,SLOT(slot_selected_rect_changed()));
	connect(m_model,SIGNAL(zoomRectChanged()),this,SLOT(slot_zoom_rect_changed()));
	connect(m_model,SIGNAL(windowsChanged()),this,SLOT(slot_windows_changed()));
}

void ViewmdaWidget2D::paintEvent ( QPaintEvent * event ) {
	QPainter painter(this);
	if (!m_model) return;
	if (m_need_to_update_the_image) {
		m_need_to_update_the_image=false;
		m_the_image=QImage(1,1,QImage::Format_RGB32);
		if ((m_model->N1()>0)&&(m_model->N2()>0)) {
			m_the_image=QImage(m_model->N1(),m_model->N2(),QImage::Format_RGB32);
			for (qint32 x=0; x<m_model->N1(); x++)
				for (qint32 y=0; y<m_model->N2(); y++) {
					QColor col=get_color(m_model->get(x,y),m_model->getMask(x,y),m_model->hue(x,y),m_model->saturation(x,y));
					m_the_image.setPixel(x,y,qRgb(col.red(),col.green(),col.blue()));
				}
		}
	}	
	int iwidth=m_the_image.width();
	int iheight=m_the_image.height();
	if (width()*iheight<iwidth*height()) { //width is the limiting direction
		int target_height=(int)(width()*iheight/(double)iwidth);
		m_target_rect=QRect(0,(height()-target_height)/2,width(),target_height);
	}
	else { //height is the limiting direction
		int target_width=(int)(height()*iwidth/(double)iheight);
		m_target_rect=QRect((width()-target_width)/2,0,target_width,height());
	}
	painter.drawImage(m_target_rect,m_the_image,QRect(0,0,m_the_image.width(),m_the_image.height()));
	draw_current_position(painter);
}

QColor ViewmdaWidget2D::get_color(Complex X,unsigned char M,int hue,float saturation) {
	if (!m_model) return qRgb(0,0,0);
	float wmin=m_model->windowMin();
	float wmax=m_model->windowMax();
	real val=abs(X);
	int hold=0;
	if (wmin==wmax) {
		if (val<wmin) hold=0;
		else if (val>wmax) hold=255;
		else hold=127;
	}
	else if (wmax>wmin) {
		if (val<wmin) val=wmin;
		if (val>wmax) val=wmax;
		hold=(int)((val-wmin)/(wmax-wmin)*255);
	}
	if (M==0) {
		float swmin=m_model->saturationWindowMin();
		float swmax=m_model->saturationWindowMax();
		int sat=0;
		int hue=0;
		if (saturation<0) {
			saturation=-saturation;
			hue=(hue+180)%360;
		}
		if (swmax>swmin) sat=(int)((saturation-swmin)/(swmax-swmin)*255);
		return QColor::fromHsv(hue,sat,hold);
	}
	else {
		if (M==1) return qRgb(hold,hold,255);
		else if (M==2) return qRgb(hold,255,hold);
		else if (M==3) return qRgb(255,hold,hold);
		else return qRgb(255,255,hold);
	}
	
}


void ViewmdaWidget2D::slot_array_changed() {
	m_need_to_update_the_image=true;
	repaint();
}
void ViewmdaWidget2D::slot_dimensions_changed() {
	m_need_to_update_the_image=true;
	repaint();
}
void ViewmdaWidget2D::slot_current_index_changed() {
	repaint();
}
void ViewmdaWidget2D::slot_current_slice_changed() {
	m_need_to_update_the_image=true;
	repaint();
}
void ViewmdaWidget2D::slot_selected_points_changed() {
	m_need_to_update_the_image=true;
	repaint();
}
void ViewmdaWidget2D::slot_selected_rect_changed() {
	repaint();
}
void ViewmdaWidget2D::slot_zoom_rect_changed() {
	m_need_to_update_the_image=true;
	repaint();
}

void ViewmdaWidget2D::draw_current_position(QPainter &painter) {
	if (!m_model) return;
	painter.setPen(QPen(QColor(200,0,200),3));
	if (m_model->selectedRect().x()<0) {
		QPoint pt=indexToPoint(QPoint(m_model->C1(),m_model->C2()));
		painter.drawLine(pt.x()-5,pt.y(),pt.x()+5,pt.y());
		painter.drawLine(pt.x(),pt.y()-5,pt.x(),pt.y()+5);
	}
	else {
		QRect SR=m_model->selectedRect();
		QPoint pt1=indexToPoint(QPoint(SR.x(),SR.y()));
		QPoint pt2=indexToPoint(QPoint(SR.x()+SR.width(),SR.y()+SR.height()));
		if (m_model->ellipticalSelection())
			painter.drawEllipse(QRect(pt1.x(),pt1.y(),pt2.x()-pt1.x(),pt2.y()-pt1.y()));
		else
			painter.drawRect(QRect(pt1.x(),pt1.y(),pt2.x()-pt1.x(),pt2.y()-pt1.y()));
		
	}
}

QPoint ViewmdaWidget2D::indexToPoint(QPoint index) {
	if (!m_model) return QPoint(0,0);
	if (m_model->N1()==0) return QPoint(0,0);
	if (m_model->N2()==0) return QPoint(0,0);
	//index.setY(m_model->N2()-1-index.y());
	QPoint pt=QPoint(m_target_rect.x()+(index.x()+0.5)/m_model->N1()*m_target_rect.width(),m_target_rect.y()+(index.y()+0.5)/m_model->N2()*m_target_rect.height());
	return pt;
}

QPoint ViewmdaWidget2D::pointToIndex(QPoint pt) {
	if (!m_model) return QPoint(0,0);
	if (m_target_rect.width()==0) return QPoint(0,0);
	if (m_target_rect.height()==0) return QPoint(0,0);
	float pctx=((float)pt.x()-m_target_rect.x())/(m_target_rect.width());
	float pcty=((float)pt.y()-m_target_rect.y())/(m_target_rect.height());
	int retx=(int)(pctx*m_model->N1());
	int rety=(int)(pcty*m_model->N2());
	if (retx<0) retx=0;
	if (retx>=m_model->N1())
		retx=m_model->N1()-1;
	if (rety<0) rety=0;
	if (rety>=m_model->N2())
		rety=m_model->N2()-1;
	return QPoint(retx,/*m_model->N2()-1-*/rety);
}

void ViewmdaWidget2D::mousePressEvent ( QMouseEvent * event ) {
	if (!m_model) return;
	if ((event->button()&Qt::LeftButton)||(event->button()&Qt::RightButton)) {
		m_model->setSelectedRect(QRect(-1,-1,1,1),false);
		QPoint index=pointToIndex(event->pos());
		m_model->setC1(index.x());
		m_model->setC2(index.y());
	}
}

void ViewmdaWidget2D::mouseMoveEvent ( QMouseEvent * event ) {
	if (!m_model) return;
	if ((event->buttons()&Qt::LeftButton)||(event->buttons()&Qt::RightButton)) {
		QPoint index=pointToIndex(event->pos());
		int c1=m_model->C1();
		int c2=m_model->C2();
		int diffx=qAbs(c1-index.x());
		int diffy=qAbs(c2-index.y());
		if ((diffx>0)&&(diffy>0)) {
			bool elliptical=false;
			if (event->buttons()&Qt::RightButton) 
				elliptical=true;
			QRect R(qMin(c1,index.x()),qMin(c2,index.y()),diffx,diffy);
			m_model->setSelectedRect(R,elliptical);
		}
	}
}

void ViewmdaWidget2D::keyPressEvent( QKeyEvent * event ) {
	if (!m_model) return;
	int key=event->key();
	switch (key) {
		case Qt::Key_Up:
			if (event->modifiers()&Qt::ShiftModifier)
				m_model->setC3(m_model->C3()+1);
			else
				m_model->setC2(m_model->C2()-1);
			break;
		case Qt::Key_Down:
			if (event->modifiers()&Qt::ShiftModifier)
				m_model->setC3(m_model->C3()-1);
			else
				m_model->setC2(m_model->C2()+1);
			break;
		case Qt::Key_Left:
			if (event->modifiers()&Qt::ShiftModifier)
				m_model->setC3(m_model->C3()-1);
			else
				m_model->setC1(m_model->C1()-1);
			break;
		case Qt::Key_Right:
			if (event->modifiers()&Qt::ShiftModifier)
				m_model->setC3(m_model->C3()+1);
			else
				m_model->setC1(m_model->C1()+1);
			break;
	}
	event->accept();
}

void ViewmdaWidget2D::slot_windows_changed() {
	m_need_to_update_the_image=true;
	repaint();
}

