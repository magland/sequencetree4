#ifndef viewmdawidget2d_H
#define viewmdawidget2d_H
#include "viewmdamodel.h"
#include <QWidget>
#include <QImage>

class ViewmdaWidget2D : public QWidget {
	Q_OBJECT
	ViewmdaModel *m_model;
public:
	ViewmdaWidget2D(QWidget *parent=0);
	virtual ~ViewmdaWidget2D();
	void setModel(ViewmdaModel *M);
protected:
	void paintEvent ( QPaintEvent * event );
	void mousePressEvent ( QMouseEvent * event );
	void keyPressEvent( QKeyEvent * event );
	void mouseMoveEvent ( QMouseEvent * event );
private slots:
	void slot_array_changed();
	void slot_dimensions_changed();
	void slot_current_index_changed();
	void slot_current_slice_changed();
	void slot_selected_points_changed();
	void slot_selected_rect_changed();
	void slot_zoom_rect_changed();
	void slot_windows_changed();
private:

	QImage m_the_image;
	QRect m_target_rect;
	bool m_need_to_update_the_image;
	
	QPoint indexToPoint(QPoint index);
	QPoint pointToIndex(QPoint pt);
	
	QColor get_color(Complex X,unsigned char M,int hue=0,float saturation=0);
	void draw_current_position(QPainter &painter);
};

#endif
