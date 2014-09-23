#ifndef ViewmdaWidget_H
#define ViewmdaWidget_H

#include "ui_viewmdawidget.h"
#include "viewmdamodel.h"
#include "viewmdawidget2d.h"
#include "brightnesstool.h"
class QComboBox;
class QToolButton;
class QLabel;

class ViewmdaWidget : public QMainWindow {
	Q_OBJECT
private:
	Ui::ViewmdaWidget ui;
	ViewmdaModel m_model;
	QComboBox *m_dim1_box;
	QComboBox *m_dim2_box;
	QComboBox *m_dim3_box;
	QToolButton *m_dimensions_button;
	QAction *m_dim1_box_action;
	QAction *m_dim2_box_action;
	QAction *m_dim3_box_action;
	QAction *m_dimensions_action;
	QLabel *m_value_label;
	BrightnessTool *m_brightness_tool;
public:
	ViewmdaWidget(QWidget *parent=0);
	void setArray(const Mda &X);
	void setHue(const Mda &H);
	void setSaturation(const Mda &S);
private slots:
	void slot_array_changed();
	void slot_dimensions_changed();
	void slot_current_index_changed();
	void slot_current_slice_changed();
	void slot_selected_points_changed();
	void slot_selected_rect_changed();
	void slot_zoom_rect_changed();
	void slot_scrollbar_value_changed(int vale);
	void slot_dim_box_changed();
	void slot_dimension_button_clicked();
	void slot_brightness_button_clicked();
	void slot_lighter();
	void slot_darker();
	void slot_windows_changed();
	void slot_brightness_levels_changed(float minlevel,float maxlevel);
	void slot_save_as();
	void slot_open();
	void slot_export_imageJ();
	void slot_1d_fft();
	void slot_2d_fft();
	void slot_3d_fft();
	void slot_1d_ifft();
	void slot_2d_ifft();
	void slot_3d_ifft();
private:
	void update_scrollbar();
	void update_status_bar();
	void update_dim_boxes();
	void show_dimension_boxes(bool visible);
	
	bool m_do_change_dimensions;
};

#endif
