#ifndef viewmdamodel_H
#define viewmdamodel_H

#include "mda.h"
#include <QObject>
#include <QImage>

class ViewmdaModel : public QObject {
	Q_OBJECT
public:
	ViewmdaModel();
	virtual ~ViewmdaModel();
	
	void setArray(const Mda &X);
	void setHue(const Mda &H);
	void setSaturation(const Mda &S);
	Mda &array();
	
	Complex get(qint32 x=-1,qint32 y=-1,qint32 z=-1);
	void set(const Complex &val, qint32 x=-1,qint32 y=-1,qint32 z=-1);
	unsigned char getMask(qint32 x=-1,qint32 y=-1,qint32 z=-1);
	void setMask(unsigned char val, qint32 x=-1,qint32 y=-1,qint32 z=-1);
	
	int hue(qint32 x=-1,qint32 y=-1,qint32 z=-1);
	real saturation(qint32 x=-1,qint32 y=-1,qint32 z=-1);
	
	//Dimension indices
	int D1(); int D2(); int D3();
	//Set dimension indices
	void setD1(int d); void setD2(int d); void setD3(int d);
	//Array sizes
	qint32 N1(); qint32 N2(); qint32 N3();
	//Current index
	qint32 C1(); qint32 C2(); qint32 C3();
	//Set current index
	void setC1(int i); void setC2(int i); void setC3(int i);
	
	const QImage &selectedPoints();
	void setSelectedPoints(const QImage &X);
	
	const QRect &selectedRect();
	void setSelectedRect(const QRect &R,bool elliptical);
	bool ellipticalSelection();
	
	const QRect &zoomRect();
	void setZoomRect(const QRect &R);
	
	void setWindows(float minval,float maxval);
	float windowMin();
	float windowMax();
	
	float saturationWindowMin();
	void setSaturationWindowMin(float val);
	float saturationWindowMax();
	void setSaturationWindowMax(float val);
	
	void autoSetWindows();
	
	int currentIndex(int dim);
	
	
signals:
	void arrayChanged();
	void dimensionsChanged();
	void currentIndexChanged();
	void currentSliceChanged();
	void selectedPointsChanged();
	void selectedRectChanged();
	void zoomRectChanged();
	void windowsChanged();
	
private:
	Mda m_array;
	Mda m_hue;
	Mda m_saturation;
	int m_d1; int m_d2; int m_d3;
	qint32 m_current_index[MAX_MDA_DIMS];
	QImage m_selected_points;
	QRect m_selected_rect;
	QRect m_zoom_rect;
	float m_window_min;
	float m_window_max;
	float m_saturation_window_min;
	float m_saturation_window_max;
	bool m_elliptical_selection;
	
	void reset_selections();
	void auto_set_saturation_windows();
	bool dimensions_dont_work();
};

#endif
