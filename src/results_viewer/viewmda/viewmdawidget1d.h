#ifndef viewmdawidget1d_H
#define viewmdawidget1d_H
#include "viewmdamodel.h"
#include <QWidget>
#include <QImage>

class ViewmdaWidget1D : public QWidget {
	Q_OBJECT
	ViewmdaModel *m_model;
public:
	ViewmdaWidget1D(QWidget *parent=0);
	virtual ~ViewmdaWidget1D();
	void setModel(ViewmdaModel *M);
protected:
	void paintEvent ( QPaintEvent * event );
	void mousePressEvent ( QMouseEvent * event );
	void keyPressEvent( QKeyEvent * event );
	void mouseMoveEvent ( QMouseEvent * event );
private slots:
	void slot_update_plot() {
		update();
	}
private:
	QList<double> m_plot_data_real;
	QList<double> m_plot_data_imag;
	double m_minval,m_maxval;
	
	QPoint indexToPoint(long index,double val);
	long pointToIndex(QPoint pt);
	
	void draw_current_position(QPainter &painter);
};

#endif
