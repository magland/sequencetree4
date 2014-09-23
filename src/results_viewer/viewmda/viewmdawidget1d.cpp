#include "viewmdawidget1d.h"
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

ViewmdaWidget1D::ViewmdaWidget1D(QWidget *parent) : QWidget(parent)
{
	m_model=0;
	setFocusPolicy(Qt::StrongFocus);
}

ViewmdaWidget1D::~ViewmdaWidget1D()
{
}

void ViewmdaWidget1D::setModel(ViewmdaModel *M) {
	m_model=M;
	
	connect(m_model,SIGNAL(arrayChanged()),this,SLOT(slot_update_plot()));
	connect(m_model,SIGNAL(dimensionsChanged()),this,SLOT(slot_update_plot()));
	connect(m_model,SIGNAL(currentIndexChanged()),this,SLOT(slot_update_plot()));
	connect(m_model,SIGNAL(currentSliceChanged()),this,SLOT(slot_update_plot()));
	connect(m_model,SIGNAL(selectedPointsChanged()),this,SLOT(slot_update_plot()));
	connect(m_model,SIGNAL(selectedRectChanged()),this,SLOT(slot_update_plot()));
	connect(m_model,SIGNAL(zoomRectChanged()),this,SLOT(slot_update_plot()));
	connect(m_model,SIGNAL(windowsChanged()),this,SLOT(slot_update_plot()));
}

void ViewmdaWidget1D::paintEvent ( QPaintEvent * event ) {
	m_plot_data_real.clear();
	m_plot_data_imag.clear();	
	QPainter painter(this);
	if (!m_model) return;
	
	
	
	for (long j=0; j<m_model->N1(); j++) {
		m_plot_data_real << m_model->get(j).re();
		m_plot_data_imag << m_model->get(j).im();
	}
	m_minval=-m_model->windowMax();
	m_maxval=m_model->windowMax();

	QPainterPath	re_path;
	QPainterPath im_path;
	double dwidth=(double)width();
	double dheight=(double)height();
	for (long j=0; j<m_plot_data_real.count(); j++) {
		
		double re=m_plot_data_real[j];
		double im=m_plot_data_imag[j];
		if (j==0) {
			re_path.moveTo(indexToPoint(j,re));
			im_path.moveTo(indexToPoint(j,im));
		}
		else {
			re_path.lineTo(indexToPoint(j,re));
			im_path.lineTo(indexToPoint(j,im));
		}
	}
	painter.setPen(QPen(Qt::black,1));
	painter.drawLine(indexToPoint(0,0),indexToPoint(m_plot_data_real.count()-1,0));
	painter.setPen(QPen(Qt::darkBlue,3));
	painter.drawPath(re_path);
	painter.setPen(QPen(Qt::darkRed,3));
	painter.drawPath(im_path);
	
	draw_current_position(painter);
}


void ViewmdaWidget1D::draw_current_position(QPainter &painter) {
	if (!m_model) return;
	if (m_model->C1()<0) return;
	
	QPoint pt1=indexToPoint(m_model->C1(),m_minval);
	QPoint pt2=indexToPoint(m_model->C1(),m_maxval);
	
	painter.setPen(QPen(QColor(200,0,200),3));
	painter.drawLine(pt1,pt2);
}

QPoint ViewmdaWidget1D::indexToPoint(long index,double val) {
	double xpos=index*width()/m_plot_data_real.count();
	double ypos=height()-height()*(val-m_minval)/(m_maxval-m_minval);
	return QPoint(xpos,ypos);
}

long ViewmdaWidget1D::pointToIndex(QPoint pt) {
	long N=m_plot_data_real.count(); 
	if (N<=0) return -1;
	return (((double)pt.x())/width())*N;
}

void ViewmdaWidget1D::mousePressEvent ( QMouseEvent * event ) {
	if (!m_model) return;
	if ((event->button()&Qt::LeftButton)||(event->button()&Qt::RightButton)) {
		m_model->setSelectedRect(QRect(-1,-1,1,1),false);
		long index=pointToIndex(event->pos());
		m_model->setC1(index);
	}
}

void ViewmdaWidget1D::mouseMoveEvent ( QMouseEvent * event ) {
	/*if (!m_model) return;
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
	}*/
}

void ViewmdaWidget1D::keyPressEvent( QKeyEvent * event ) {
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


