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


#include "stsimblockview.h"
#include <math.h>
#include <QPalette>
#include <QDebug>

void STSimBlockPlotTimeChunks::setBlock(STSimScannerBlock *B) {
	m_chunk_start_times.clear();
	m_chunk_end_times.clear();
	m_effective_duration=0;
	if (!B) return;
	QList<double> occupied_chunks;
	long N=(long)(B->duration()/20000);
	for (long j=0; j<N; j++) 
		occupied_chunks << 0;
	for (int j=0; j<B->eventCount(); j++) {
		STSimScannerEvent evt=B->event(j);
		long ind1=(long)(evt.start_time/20000);
		long ind2=(long)((evt.start_time+evt.duration)/20000);
		if (ind1<0) ind1=0;
		for (long ind=ind1; ind<=ind2; ind++)
			if (ind<occupied_chunks.count())
				occupied_chunks[ind]=1;
	} 
	long j=0; 
	while (j+1<N) {
		if ((occupied_chunks[j]==0)&&(occupied_chunks[j+1]==0)) {
			long k=j+1;
			while ((k+1<N)&&(occupied_chunks[k+1]==0))
				k++;
			m_chunk_start_times << j*20000;
			m_chunk_end_times << (k+1)*20000;
			j=k+1;
		}
		else j++;
	}
	m_effective_duration=0;
	double hold_t=0;
	for (int j=0; j<m_chunk_start_times.count(); j++) {
		m_effective_duration+=m_chunk_start_times[j]-hold_t+20000;
		hold_t=m_chunk_end_times[j];
	}
	m_effective_duration+=B->duration()-hold_t;
}
double STSimBlockPlotTimeChunks::effectiveTime(double t) {
	double ret=0;
	double hold_t=0;
	for (int j=0; j<m_chunk_start_times.count(); j++) {
		if (m_chunk_start_times[j]>=t) {
			ret+=(t-hold_t);
			return ret;
		}
		else {
			ret+=m_chunk_start_times[j]-hold_t+20000;
			hold_t=m_chunk_end_times[j];
		}
	}
	ret+=(t-hold_t);
	return ret;
}

double STSimBlockPlotTimeChunks::actualTime(double eff_t) {
	double ret=0; //actual time
	double hold_t=0; //effective time
	for (int j=0; j<chunkCount(); j++) {
		if (m_chunk_start_times[j]>=eff_t) {
			ret+=eff_t-hold_t;
			return ret;
		}
		else {
			hold_t+=m_chunk_start_times[j]-ret+20000;
			ret=m_chunk_end_times[j];
		}
	}
	ret+=eff_t-hold_t;
	return ret;
}

STSimBlockPlot::STSimBlockPlot() {
	m_xmin=m_ymin=-1;
	m_xmax=m_ymax=1;
	m_time_chunks=0;
}

void STSimBlockPlot::setTimeChunks(STSimBlockPlotTimeChunks *time_chunks) {
	m_time_chunks=time_chunks;
}

QPointF STSimBlockPlot::windowPointF(double x,double y) {
	double xdur=m_xmax-m_xmin;
	double xval=x-m_xmin;
	if ((m_time_chunks)&&(m_time_chunks->effectiveDuration()>0)) {
		xdur=m_time_chunks->effectiveDuration();
		xval=m_time_chunks->effectiveTime(x);
	}
	if (xdur<=0) return QPointF(0,0);
	float px=xval/xdur;
	float py=(y-m_ymin)/(m_ymax-m_ymin);
	float retx=(m_window_rect.left()+(m_window_rect.right()-m_window_rect.left())*px);
	float rety=(m_window_rect.bottom()+(m_window_rect.top()-m_window_rect.bottom())*py);
	return QPoint(retx,rety);
}

QPoint STSimBlockPlot::windowPoint(double x,double y) {
	QPointF pt=windowPointF(x,y);
	return QPoint((int)pt.x(),(int)pt.y());
}
double STSimBlockPlot::toX(QPoint pt) {
	double xdur=m_xmax-m_xmin;
	if ((m_time_chunks)&&(m_time_chunks->effectiveDuration()>0)) {
		xdur=m_time_chunks->effectiveDuration();
	}
	double px=((double)pt.x()-m_window_rect.left())/(m_window_rect.right()-m_window_rect.left());
	double ret=m_xmin+px*xdur;
	if ((m_time_chunks)&&(m_time_chunks->effectiveDuration()>0)) {
		ret=m_time_chunks->actualTime(ret);
	}
	return ret;
}
double STSimBlockPlot::toY(QPoint pt) {
	double py=((double)pt.y()-m_window_rect.bottom())/(m_window_rect.top()-m_window_rect.bottom());
	return m_ymin+py*(m_ymax-m_ymin);
}


void STSimBlockPlot::drawEvent(QPainter &painter,STSimScannerEvent &E,bool is_selected,int line_thickness) {
	if (is_selected) line_thickness*=2;
	double start_time=E.start_time;
	if (E.event_type==ST_EVENT_RF_PULSE) {
		QList<double> data_real;
		QList<double> data_imag;
		QList<double> data_times;
		data_real << 0;
		data_imag << 0;
		data_times << start_time;
		for (long j=0; j<E.data_mag.count(); j++) {
			double mag=E.data_mag[j];
			double ph=E.data_phase[j];
			double re=mag*cos(ph*3.141592/180);
			double im=mag*sin(ph*3.141592/180);
			data_real << re;
			data_imag << im;
			data_times << start_time+j*E.timestep;	
		}
		data_real << 0;
		data_imag << 0;
		data_times << start_time+E.timestep*(E.data_mag.count()-1);
		draw_curve(painter,data_times,data_real,Qt::darkBlue,line_thickness);
		draw_curve(painter,data_times,data_imag,Qt::darkRed,line_thickness);
	}
	else if (E.event_type==ST_EVENT_READOUT) {
		QList<double> data_times;
		QList<double> data_vals;
		data_times << start_time << start_time << start_time+E.duration << start_time+E.duration;
		double val=m_ymax*0.7;
		data_vals << 0 << val << val << 0;
		draw_curve(painter,data_times,data_vals,Qt::black,line_thickness);
	}
	else if ((E.event_type==ST_EVENT_GRADIENT)||(E.event_type==ST_EVENT_ARB_GRADIENT)) {
		QList<double> data_times;
		QList<double> data_amps;
		for (long j=0; j<E.amplitudes.count(); j++) {
			data_times << start_time+E.times[j];
			data_amps << E.amplitudes[j];
		}
		QColor col=Qt::black;
		if (E.direction==1) col=Qt::darkBlue;
		else if (E.direction==2) col=Qt::darkRed;
		else if (E.direction==3) col=Qt::darkGreen;
		draw_curve(painter,data_times,data_amps,col,line_thickness);
	}
}

void STSimBlockPlot::draw_curve(QPainter &painter,QList<double> xdata,QList<double> ydata,QColor col,int line_thickness) {
	QPainterPath path;
	for (int j=0; j<xdata.count(); j++) {
		QPointF pt=windowPointF(xdata[j],ydata[j]);
		if (j==0) path.moveTo(pt);
		else path.lineTo(pt);
	}
	painter.setPen(QPen(QBrush(col),line_thickness));
	painter.drawPath(path);
}

void STSimBlockPlot::drawFrame(QPainter &painter) {
	painter.drawRect(m_window_rect);
	if ((m_ymin<0)&&(m_ymax>=0)) {
		QPoint pt1=windowPoint(m_xmin,0);
		QPoint pt2=windowPoint(m_xmax,0);
		painter.drawLine(pt1,pt2);
	}
}

STSimBlockView::STSimBlockView(QWidget *par) : QWidget(par) {
	m_block=0;
	m_line_thickness=3;
	m_rf_plot.setTimeChunks(&m_time_chunks);
	m_gradient_plot.setTimeChunks(&m_time_chunks);
}
STSimBlockView::~STSimBlockView() {
}
void STSimBlockView::setMaxGradientAmp(double maxamp) {
	if (maxamp<1) maxamp=1;
	m_gradient_plot.setYLimits(-maxamp*1.2,maxamp*1.2);
}
void STSimBlockView::setMaxRFAmp(double maxRFamp) {
	if (maxRFamp<1) maxRFamp=1;
	m_rf_plot.setYLimits(-maxRFamp*1.2,maxRFamp*1.2);
}
void STSimBlockView::setBlock(STSimScannerBlock *B,long alternate_block_number) {
	m_alternate_block_number=alternate_block_number;
	clearSelectedEvents();
	m_block=B;
	if (B) {
		m_rf_plot.setXLimits(0,B->duration());
		m_gradient_plot.setXLimits(0,B->duration());
	}
	update();
	emit blockChanged(B);
}
void STSimBlockView::paintEvent(QPaintEvent *event) {
	m_time_chunks.setBlock(0);
	
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	
	
	QColor background_color=palette().color(QPalette::Midlight);
	painter.fillRect(rect(),QBrush(background_color));
	painter.drawText(rect(),Qt::AlignHCenter|Qt::AlignVCenter,m_message);
	
	int left_margin=50;
	int right_margin=40;
	int top_margin=30;
	int bottom_margin=30;
	int spacing=20;
	
	int val_left=left_margin;
	int val_right=width()-right_margin;
	int val_top=top_margin;
	int val_bottom=height()-bottom_margin;
	int val_mid1=height()/2-spacing/2;
	int val_mid2=height()/2+spacing/2;
	
	
	m_rf_plot.setWindowRect(QRect(val_left,val_top,val_right-val_left,val_mid1-val_top));
	m_gradient_plot.setWindowRect(QRect(val_left,val_mid2,val_right-val_left,val_bottom-val_mid2));
	
	
	//draw block number
	QRect R2;
	R2=QRect(val_left,0,val_right-val_left,val_top);
	if (m_block) {
		painter.drawText(R2,Qt::AlignLeft|Qt::AlignVCenter,QString("Block %1       %2 seconds").arg(m_block->blockNumber()+1).arg(m_block->elapsedTime()/1.0E6,0,'f',3));
	}
	else if (m_alternate_block_number>=0) {
		painter.drawText(R2,Qt::AlignLeft|Qt::AlignVCenter,QString("Block %1").arg(m_alternate_block_number+1));
	}
		
			
	m_rf_plot.drawFrame(painter);
	m_gradient_plot.drawFrame(painter);
	
	
	///////////////// Legends ///////////////////////
	int x1,y1,x2,y2;
	
	x1=val_right; y1=val_mid2;
	x2=val_right; y2=val_bottom;
	R2=QRect(x1+5,y1,width(),(y2-y1)/6);
	painter.setPen(QPen(Qt::darkBlue));
	painter.drawText(R2,Qt::AlignLeft|Qt::AlignVCenter,"GX");
	R2=QRect(x1+5,y1+(y2-y1)/6,width(),(y2-y1)/6);
	painter.setPen(QPen(Qt::darkRed));
	painter.drawText(R2,Qt::AlignLeft|Qt::AlignVCenter,"GY");
	R2=QRect(x1+5,y1+2*(y2-y1)/6,width(),(y2-y1)/6);
	painter.setPen(QPen(Qt::darkGreen));
	painter.drawText(R2,Qt::AlignLeft|Qt::AlignVCenter,"GZ");

	x1=val_right; y1=val_top;
	x2=val_right; y2=val_mid1;
	R2=QRect(x1+5,y1,width(),(y2-y1)/6);
	painter.setPen(QPen(Qt::darkBlue));
	painter.drawText(R2,Qt::AlignLeft|Qt::AlignVCenter,"real");
	R2=QRect(x1+5,y1+(y2-y1)/6,width(),(y2-y1)/6);
	painter.setPen(QPen(Qt::darkRed));
	painter.drawText(R2,Qt::AlignLeft|Qt::AlignVCenter,"imag");
	R2=QRect(x1+5,y1+2*(y2-y1)/6,width(),(y2-y1)/6);
	painter.setPen(QPen(Qt::black));
	painter.drawText(R2,Qt::AlignLeft|Qt::AlignVCenter,"ADC");


	///////////// Label Axis
	x1=val_left; y1=val_mid2;
	x2=val_left; y2=val_bottom;
	R2=QRect(0,y1,x2-15,y2-y1);
	QPixmap map(R2.height(),R2.width());
	QPainter painter2(&map);
	QFont font;
	int sz=(int)(R2.width()/3);
	if (sz<6) sz=6;
	if (sz>15) sz=15;
	font.setPixelSize(sz);
	painter2.setFont(font);
	painter2.fillRect(QRect(0,0,map.width(),map.height()),QBrush(background_color));
	painter2.drawText(QRect(0,0,map.width(),map.height()),Qt::AlignCenter,"Gradient\n(mT/m)");
	QMatrix mat; mat.rotate(-90);
	painter.drawPixmap(0,y1,map.transformed(mat));
	painter2.end();
	
	x1=val_left; y1=val_top;
	x2=val_left; y2=val_mid1;
	R2=QRect(0,y1,x2-15,y2-y1);
	map=QPixmap(R2.height(),R2.width());
	QPainter painter3(&map);
	sz=(int)(R2.width()/3);
	if (sz<6) sz=6;
	if (sz>15) sz=15;
	font.setPixelSize(sz);
	painter3.setFont(font);
	painter3.fillRect(QRect(0,0,map.width(),map.height()),QBrush(background_color));
	painter3.drawText(QRect(0,0,map.width(),map.height()),Qt::AlignCenter,"RF (uT)");
	painter.drawPixmap(0,y1,map.transformed(mat));
	painter3.end();
	
	
	if (!m_block) return;
	
	m_time_chunks.setBlock(m_block);
	
	//////////////// draw time chunks
	for (int j=0; j<m_time_chunks.chunkCount(); j++) {
		QString txt=QString("%1 ms").arg((m_time_chunks.endTime(j)-m_time_chunks.startTime(j))/1000);
		QPoint pt1,pt2; QRect rct;
		double x1=m_time_chunks.startTime(j);
		double x2=m_time_chunks.endTime(j);
		pt1=m_rf_plot.windowPoint(x1,m_rf_plot.ymax());
		pt2=m_rf_plot.windowPoint(x2,m_rf_plot.ymin());
		rct=QRect(pt1.x(),pt1.y(),pt2.x()-pt1.x(),pt2.y()-pt1.y());
		painter.fillRect(rct,QBrush(Qt::lightGray));
		painter.drawText(rct,Qt::AlignHCenter|Qt::AlignVCenter,txt);
		pt1=m_gradient_plot.windowPoint(x1,m_gradient_plot.ymax());
		pt2=m_gradient_plot.windowPoint(x2,m_gradient_plot.ymin());
		rct=QRect(pt1.x(),pt1.y(),pt2.x()-pt1.x(),pt2.y()-pt1.y());
		painter.fillRect(rct,QBrush(Qt::lightGray));
		painter.drawText(rct,Qt::AlignHCenter|Qt::AlignVCenter,txt);
	}

	/////////////// draw axis limits
	x1=val_left; y1=val_mid2;
	x2=val_right; y2=val_bottom;
	R2=QRect(x1,y2,1000,1000);
	painter.drawText(R2,Qt::AlignLeft|Qt::AlignTop,"0 ms");
	R2=QRect(x2-1000,y2,1000,1000);
	char holdstr[500]; sprintf(holdstr,"%d ms",(int)(m_gradient_plot.xmax()/1000));
	painter.drawText(R2,Qt::AlignRight|Qt::AlignTop,holdstr);
	R2=QRect(x1-1000,y1,1000-5,1000);
	sprintf(holdstr,"%.3g",m_gradient_plot.ymax());
	painter.drawText(R2,Qt::AlignRight|Qt::AlignTop,holdstr);
	R2=QRect(x1-1000,y2-1000,1000-5,1000);
	sprintf(holdstr,"%.3g",m_gradient_plot.ymin());
	painter.drawText(R2,Qt::AlignRight|Qt::AlignBottom,holdstr);
	x1=val_left; y1=val_top;
	x2=val_right; y2=val_mid1;
	R2=QRect(x1-1000,y1,1000-5,1000);
	sprintf(holdstr,"%.3g",m_rf_plot.ymax());
	painter.drawText(R2,Qt::AlignRight|Qt::AlignTop,holdstr);
	R2=QRect(x1-1000,y2-1000,1000-5,1000);
	sprintf(holdstr,"%.3g",m_rf_plot.ymin());
	painter.drawText(R2,Qt::AlignRight|Qt::AlignBottom,holdstr);
	
	//Trigger
	if (m_block->waitForTrigger()) {
		QPoint pt1=m_rf_plot.windowPoint(0,m_rf_plot.ymax());
		QPoint pt2=m_gradient_plot.windowPoint(0,m_gradient_plot.ymin());
		QRect rct=QRect(pt1.x()-1,pt1.y(),10,pt2.y()-pt1.y());
		painter.fillRect(rct,QBrush(Qt::darkMagenta));
		
	}
	
	for (int j=0; j<m_block->eventCount(); j++) {
		STSimScannerEvent E=m_block->event(j);
		if ((E.event_type==ST_EVENT_RF_PULSE)||(E.event_type==ST_EVENT_READOUT)) {
			m_rf_plot.drawEvent(painter,E,m_selected_event_indices.contains(j),m_line_thickness);
		}
		else if ((E.event_type==ST_EVENT_GRADIENT)||(E.event_type==ST_EVENT_ARB_GRADIENT)) {
			m_gradient_plot.drawEvent(painter,E,m_selected_event_indices.contains(j),m_line_thickness);
		}
	}
	
	
}
void STSimBlockView::setMessage(QString msg) {
	m_message=msg;
	update();
}

void STSimBlockView::mousePressEvent(QMouseEvent *event) {
	if (!m_block) return;
	QPoint pt=event->pos();
	
	QSet<int> selectionlist;
	if (m_rf_plot.contains(pt)) {
		double t=m_rf_plot.toX(pt);
		for (int j=0; j<m_block->eventCount(); j++) {
			STSimScannerEvent evt=m_block->event(j);
			if ((evt.event_type!=ST_EVENT_GRADIENT)&&(evt.event_type!=ST_EVENT_ARB_GRADIENT)) {
				if ((evt.start_time<=t)&&(t<=evt.start_time+evt.duration)) {
					selectionlist << evt.block_index;
				}
			}
		}
	}
	else if (m_gradient_plot.contains(pt)) {
		double t=m_gradient_plot.toX(pt);
		for (int j=0; j<m_block->eventCount(); j++) {
			STSimScannerEvent evt=m_block->event(j);
			if ((evt.event_type==ST_EVENT_GRADIENT)||(evt.event_type==ST_EVENT_ARB_GRADIENT)) {
				if ((evt.start_time<=t)&&(t<=evt.start_time+evt.duration)) {
					selectionlist << evt.block_index;
				}
			}
		}
	}
	setSelectedEvents(selectionlist);
}

void STSimBlockView::setSelectedEvents(QSet<int> event_indices) {
	QSet<int> new_selected_event_indices;
	bool different=false;
	
	foreach (int index,event_indices) {
		if (!m_selected_event_indices.contains(index)) different=true;
		new_selected_event_indices << index;
	}
	if (m_selected_event_indices.count()!=new_selected_event_indices.count()) different=true;
	m_selected_event_indices=new_selected_event_indices;
	if (different) {
		emit selectedEventsChanged(m_selected_event_indices);
	}
	
	update();
}

void STSimBlockView::clearSelectedEvents() {
	m_selected_event_indices.clear();
}
