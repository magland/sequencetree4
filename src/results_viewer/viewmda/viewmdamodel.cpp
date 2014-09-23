#include "viewmdamodel.h"
#include <QImage>
#include <QDebug>

ViewmdaModel::ViewmdaModel() 
{
	m_window_min=0;
	m_window_max=1;
	m_saturation_window_min=0;
	m_saturation_window_max=255;
	Mda dummy;
	dummy.allocate(MDA_TYPE_REAL,1,1);
	setArray(dummy);
	m_elliptical_selection=false;
	m_selected_rect=QRect(-1,-1,1,1);
	m_d1=0; m_d2=1; m_d3=2;
}

ViewmdaModel::~ViewmdaModel()
{
}

void ViewmdaModel::setArray(const Mda &X) {
	m_selected_rect=QRect(-1,-1,1,1);
	m_array=X;
	if (dimensions_dont_work()) {
		m_d1=0;
		m_d2=1;
		m_d3=2;
	}
	for (int j=0; j<X.dimCount(); j++)
		m_current_index[j]=(X.size(j)-1)/2;
	reset_selections();
	emit arrayChanged();
}
void ViewmdaModel::setHue(const Mda &H) {
	m_hue=H;
	emit arrayChanged();
}
void ViewmdaModel::setSaturation(const Mda &S) {
	m_saturation=S;
	auto_set_saturation_windows();
	emit arrayChanged();
}
Mda &ViewmdaModel::array() {
	return m_array;
}

Complex ViewmdaModel::get(qint32 x,qint32 y,qint32 z) {
	qint32 hold[MAX_MDA_DIMS];
	for (int j=0; j<m_array.dimCount(); j++) {
		if ((j==m_d1)&&(x>=0)) hold[j]=x;
		else if ((j==m_d2)&&(y>=0)) hold[j]=y;
		else if ((j==m_d3)&&(z>=0)) hold[j]=z;
		else hold[j]=m_current_index[j];
	}
	return m_array.get(hold);
}

int ViewmdaModel::hue(qint32 x,qint32 y,qint32 z) {
	qint32 hold[MAX_MDA_DIMS];
	for (int j=0; j<m_array.dimCount(); j++) {
		if ((j==m_d1)&&(x>=0)) hold[j]=x;
		else if ((j==m_d2)&&(y>=0)) hold[j]=y;
		else if ((j==m_d3)&&(z>=0)) hold[j]=z;
		else hold[j]=m_current_index[j];
	}
	return (int)m_hue.get(hold).re();
}
real ViewmdaModel::saturation(qint32 x,qint32 y,qint32 z) {
	qint32 hold[MAX_MDA_DIMS];
	for (int j=0; j<m_array.dimCount(); j++) {
		if ((j==m_d1)&&(x>=0)) hold[j]=x;
		else if ((j==m_d2)&&(y>=0)) hold[j]=y;
		else if ((j==m_d3)&&(z>=0)) hold[j]=z;
		else hold[j]=m_current_index[j];
	}
	return m_saturation.get(hold).re();
}

void ViewmdaModel::set(const Complex &val, qint32 x,qint32 y,qint32 z) {
	qint32 hold[MAX_MDA_DIMS];
	for (int j=0; j<m_array.dimCount(); j++) {
		if ((j==m_d1)&&(x>=0)) hold[j]=x;
		else if ((j==m_d2)&&(y>=0)) hold[j]=y;
		else if ((j==m_d3)&&(z>=0)) hold[j]=z;
		else hold[j]=m_current_index[j];
	}
	m_array.get(hold)=val;
}
unsigned char ViewmdaModel::getMask(qint32 x,qint32 y,qint32 z) {
	qint32 hold[MAX_MDA_DIMS];
	for (int j=0; j<m_array.dimCount(); j++) {
		if ((j==m_d1)&&(x>=0)) hold[j]=x;
		else if ((j==m_d2)&&(y>=0)) hold[j]=y;
		else if ((j==m_d3)&&(z>=0)) hold[j]=z;
		else hold[j]=m_current_index[j];
	}
	return m_array.getmask(hold);
}
void ViewmdaModel::setMask(unsigned char val, qint32 x,qint32 y,qint32 z) {
	qint32 hold[MAX_MDA_DIMS];
	for (int j=0; j<m_array.dimCount(); j++) {
		if ((j==m_d1)&&(x>=0)) hold[j]=x;
		else if ((j==m_d2)&&(y>=0)) hold[j]=y;
		else if ((j==m_d3)&&(z>=0)) hold[j]=z;
		else hold[j]=m_current_index[j];
	}
	m_array.mask(hold)=val;
}

int ViewmdaModel::D1() {
	return m_d1;
}
int ViewmdaModel::D2() {
	return m_d2;
}
int ViewmdaModel::D3() {
	return m_d3;
}
void ViewmdaModel::setD1(int d) {
	if (m_d1==d) return;
	if (m_d2==d) m_d2=m_d1;
	if (m_d3==d) m_d3=m_d1;
	m_d1=d;
	reset_selections();
	emit dimensionsChanged();
}
void ViewmdaModel::setD2(int d) {
	if (m_d2==d) return;
	if (m_d1==d) m_d1=m_d2;
	if (m_d3==d) m_d3=m_d2;
	m_d2=d;
	reset_selections();
	emit dimensionsChanged();
}
void ViewmdaModel::setD3(int d) {
	if (m_d3==d) return;
	if (m_d1==d) m_d1=m_d3;
	if (m_d2==d) m_d2=m_d3;
	m_d3=d;
	reset_selections();
	emit dimensionsChanged();
}

qint32 ViewmdaModel::N1() {
	if (m_d1<0) return 1;
	return m_array.size(m_d1);
}
qint32 ViewmdaModel::N2() {
	if (m_d2<0) return 1;
	return m_array.size(m_d2);
}
qint32 ViewmdaModel::N3() {
	if (m_d3<0) return 1;
	return m_array.size(m_d3);
}
qint32 ViewmdaModel::C1() {
	if (m_d1<0) return -1;
	return m_current_index[m_d1];
} 
qint32 ViewmdaModel::C2() {
	if (m_d2<0) return -1;
	return m_current_index[m_d2];
} 
qint32 ViewmdaModel::C3() {
	if (m_d3<0) return -1;
	return m_current_index[m_d3];
} 

void ViewmdaModel::setC1(int i) {
	if (m_d1<0) return;
	if (C1()==i) return;
	if ((i<0)||(i>=N1())) return;
	m_current_index[m_d1]=i;
	emit currentIndexChanged();	
} 

void ViewmdaModel::setC2(int i) {
	if (m_d2<0) return;
	if (C2()==i) return;
	if ((i<0)||(i>=N2())) return;
	m_current_index[m_d2]=i;
	emit currentIndexChanged();	
} 

void ViewmdaModel::setC3(int i) {
	if (m_d3<0) return;
	if (C3()==i) return;
	if ((i<0)||(i>=N3())) return;
	m_current_index[m_d3]=i;
	emit currentSliceChanged();	
} 

const QImage &ViewmdaModel::selectedPoints() {
	return m_selected_points;
}
void ViewmdaModel::setSelectedPoints(const QImage &X) {
	m_selected_points=X;
	emit selectedPointsChanged();
}

const QRect &ViewmdaModel::selectedRect() {
	return m_selected_rect;
}
void ViewmdaModel::setSelectedRect(const QRect &R,bool elliptical_selection) {
	if ((m_elliptical_selection==elliptical_selection)&&(m_selected_rect==R)) return;
	m_selected_rect=R;
	m_elliptical_selection=elliptical_selection;
	emit selectedRectChanged();
}

const QRect &ViewmdaModel::zoomRect() {
	return m_zoom_rect;
}
void ViewmdaModel::setZoomRect(const QRect &R) {
	if (m_zoom_rect==R) return;
	m_zoom_rect=R;
	emit zoomRectChanged();
}

void ViewmdaModel::reset_selections() {
	m_zoom_rect=QRect(-1,-1,1,1);
	m_selected_rect=QRect(-1,-1,1,1);
	m_selected_points=QImage(N1(),N2(),QImage::Format_Mono);
}

void ViewmdaModel::autoSetWindows() {
	float minval=0,maxval=0;
	bool first=true;
	for (qint32 x=0; x<N1(); x++)
		for (qint32 y=0; y<N2(); y++) {
			real val=abs(get(x,y));
			if ((first)||(val<minval))
				minval=val;
			if ((first)||(val>maxval))
				maxval=val;
			first=false;
		}
	m_window_min=0;
	m_window_max=maxval*1.2F;
	emit windowsChanged();
}

void ViewmdaModel::auto_set_saturation_windows() {
	float minval=0,maxval=0;
	bool first=true;
	for (qint32 j=0; j<m_saturation.size(); j++) {
		float val=qAbs(m_saturation.get(j).re());
		if ((first)||(val<minval)) 
			minval=val;
		if ((first)||(val>maxval)) 
			maxval=val;
		first=false;
	}
	m_saturation_window_min=0;
	m_saturation_window_max=maxval*1.0F;
	if (m_saturation_window_max==m_saturation_window_min) {
		m_saturation_window_min=0;
		m_saturation_window_max=255;
	}
}



float ViewmdaModel::windowMin() {
	return m_window_min;
}
void ViewmdaModel::setWindows(float minval,float maxval) {
	if ((m_window_min==minval)&&(m_window_max==maxval)) return;
	m_window_min=minval;
	m_window_max=maxval;
	emit windowsChanged();
}
float ViewmdaModel::windowMax() {
	return m_window_max;
}

float ViewmdaModel::saturationWindowMin() {
	return m_saturation_window_min;
}
void ViewmdaModel::setSaturationWindowMin(float val) {
	if (m_saturation_window_min==val) return;
	m_saturation_window_min=val;
	emit windowsChanged();
}
float ViewmdaModel::saturationWindowMax() {
	return m_saturation_window_max;
}
void ViewmdaModel::setSaturationWindowMax(float val) {
	if (m_saturation_window_max==val) return;
	m_saturation_window_max=val;
	emit windowsChanged();
}



int ViewmdaModel::currentIndex(int dim) {
	return m_current_index[dim];
}

bool ViewmdaModel::ellipticalSelection() {
	return m_elliptical_selection;
}

bool ViewmdaModel::dimensions_dont_work() {
	if (m_d1<0) return true;
	if (m_d1>=m_array.dimCount()) return true;
	if (m_d2<0) return true;
	if (m_d2>=m_array.dimCount()) return true;
	if (m_array.dimCount()>2) {
		if (m_d3<0) return true;
		if (m_d3>=m_array.dimCount()) return true;		
	}
	return false;
}



