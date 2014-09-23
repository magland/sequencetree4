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


#ifndef stsimblockview_H
#define stsimblockview_H

#include <QWidget>
#include "stsimscannerblocklist.h"
#include <QPainter>
#include <QPaintEvent>
#include <QSet>

class STSimBlockPlotTimeChunks {
public:
	void setBlock(STSimScannerBlock *B);
	double effectiveDuration() {return m_effective_duration;}
	double effectiveTime(double t);
	double actualTime(double eff_t);
	long chunkCount() {return m_chunk_start_times.count();}
	double startTime(long index) {return m_chunk_start_times[index];}
	double endTime(long index) {return m_chunk_end_times[index];}
	
private:
	double m_effective_duration;
	QList<double> m_chunk_start_times;
	QList<double> m_chunk_end_times;
	
};

class STSimBlockPlot {
public:
	STSimBlockPlot();
	void setWindowRect(QRect rect) {m_window_rect=rect;}
	QRect windowRect() {return m_window_rect;}
	void setXLimits(double xmin,double xmax) {m_xmin=xmin; m_xmax=xmax;}
	void setYLimits(double ymin,double ymax) {m_ymin=ymin; m_ymax=ymax;}
	QPoint windowPoint(double x,double y);
	QPointF windowPointF(double x,double y);
	void setTimeChunks(STSimBlockPlotTimeChunks *time_chunks);
	double toX(QPoint pt);
	double toY(QPoint pt);
	
	double xmin() {return m_xmin;}
	double xmax() {return m_xmax;}
	double ymin() {return m_ymin;}
	double ymax() {return m_ymax;}
	
	void drawFrame(QPainter &painter);
	void drawEvent(QPainter &painter,STSimScannerEvent &E,bool is_selected,int line_thickness);
	
	bool contains(QPoint pt) {return ((m_xmin<=toX(pt))&&(toX(pt)<=m_xmax)&&(m_ymin<=toY(pt))&&(toY(pt)<=m_ymax));}
	
private:
	QRect m_window_rect;
	double m_xmin,m_xmax;
	double m_ymin,m_ymax;
	STSimBlockPlotTimeChunks *m_time_chunks;
	
	void draw_curve(QPainter &painter,QList<double> xdata,QList<double> ydata,QColor col,int line_thickness);
};

class STSimBlockView : public QWidget {
	Q_OBJECT
	Q_PROPERTY(int lineThickness READ lineThickness WRITE setLineThickness)
public:
	STSimBlockView(QWidget *par=0);
	virtual ~STSimBlockView();
	void setMaxGradientAmp(double maxamp);
	void setMaxRFAmp(double maxRFamp);
	void setBlock(STSimScannerBlock *B,long alternate_block_number=-1);
	STSimScannerBlock *block() {return m_block;}
	void paintEvent(QPaintEvent *event);
	void setMessage(QString msg);
	void setSelectedEvents(QSet<int> event_indices);
	void clearSelectedEvents();
	int lineThickness() {return m_line_thickness;}
	void setLineThickness(int val) {m_line_thickness=val;}
protected:
	void mousePressEvent(QMouseEvent *event);
signals:
	void blockChanged(STSimScannerBlock *);
	void selectedEventsChanged(QSet<int> event_indices);
private:
	STSimScannerBlock *m_block;
	STSimBlockPlot m_rf_plot;
	STSimBlockPlot m_gradient_plot;
	QSet<int> m_selected_event_indices;
	QString m_message;
	long m_alternate_block_number;
	STSimBlockPlotTimeChunks m_time_chunks;
	int m_line_thickness;
};

#endif
