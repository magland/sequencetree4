#include "viewmdawidget.h"
#include <QComboBox>
#include <QDebug>
#include <QToolButton>
#include <QLabel>
#include <QMenu>
#include <QFileDialog>
#include "apply_fft.h"
#include "export_to_imagej.h"

void ViewmdaWidget::setArray(const Mda &X) {
	m_model.setArray(X);
}

void ViewmdaWidget::setHue(const Mda &H) {
	m_model.setHue(H);
}
void ViewmdaWidget::setSaturation(const Mda &S) {
	m_model.setSaturation(S);
}

ViewmdaWidget::ViewmdaWidget(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	ui.widget2d->setModel(&m_model);
	ui.widget1d->setModel(&m_model);
	
	ui.splitter->setStretchFactor(0,1);
	ui.splitter->setStretchFactor(1,0);
	
	connect(&m_model,SIGNAL(arrayChanged()),this,SLOT(slot_array_changed()));
	connect(&m_model,SIGNAL(dimensionsChanged()),this,SLOT(slot_dimensions_changed()));
	connect(&m_model,SIGNAL(currentIndexChanged()),this,SLOT(slot_current_index_changed()));
	connect(&m_model,SIGNAL(currentSliceChanged()),this,SLOT(slot_current_slice_changed()));
	connect(&m_model,SIGNAL(selectedPointsChanged()),this,SLOT(slot_selected_points_changed()));
	connect(&m_model,SIGNAL(selectedRectChanged()),this,SLOT(slot_selected_rect_changed()));
	connect(&m_model,SIGNAL(zoomRectChanged()),this,SLOT(slot_zoom_rect_changed()));
	connect(&m_model,SIGNAL(windowsChanged()),this,SLOT(slot_windows_changed()));
	
	connect(ui.scrollbar,SIGNAL(valueChanged(int)),this,SLOT(slot_scrollbar_value_changed(int)));
	
	//File
	QToolButton *filebutton=new QToolButton();
	filebutton->setPopupMode(QToolButton::InstantPopup);
	filebutton->setText("File");
	QMenu *filemenu=new QMenu;
	filebutton->setMenu(filemenu);
	ui.toolBar->addWidget(filebutton);
	filemenu->addAction("Save as...",this,SLOT(slot_save_as()),tr("Ctrl+S"));
	filemenu->addAction("Open...",this,SLOT(slot_open()),tr("Ctrl+O"));
	filemenu->addSeparator();
	filemenu->addAction("Export Abs. Val. to ImageJ",this,SLOT(slot_export_imageJ()),tr("Ctrl+I"));
	
	//Dimensions
	m_dim1_box=new QComboBox(this);
	m_dim2_box=new QComboBox(this);
	m_dim3_box=new QComboBox(this);
	m_dimensions_button=new QToolButton(this);
	m_dimensions_button->setText("Dimensions");
	m_dimensions_action=ui.toolBar->addWidget(m_dimensions_button);
	m_dim1_box_action=ui.toolBar->addWidget(m_dim1_box);
	m_dim2_box_action=ui.toolBar->addWidget(m_dim2_box);
	m_dim3_box_action=ui.toolBar->addWidget(m_dim3_box);
	connect(m_dim1_box,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_dim_box_changed()));
	connect(m_dim2_box,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_dim_box_changed()));
	connect(m_dim3_box,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_dim_box_changed()));
	connect(m_dimensions_button,SIGNAL(clicked()),this,SLOT(slot_dimension_button_clicked()));
	m_do_change_dimensions=true;
	show_dimension_boxes(false);
	
	//Brightness
	QToolButton *brightnessbutton=new QToolButton();
	brightnessbutton->setPopupMode(QToolButton::InstantPopup);
	brightnessbutton->setText("Brightness");
	QMenu *brightnessmenu=new QMenu;
	brightnessbutton->setMenu(brightnessmenu);
	ui.toolBar->addWidget(brightnessbutton);	
	QAction *brightness_action=brightnessmenu->addAction("Adjust levels...",this,SLOT(slot_brightness_button_clicked()));
	brightnessmenu->addAction("Lighter",this,SLOT(slot_lighter()),tr("Ctrl+L"));
	brightnessmenu->addAction("Darker",this,SLOT(slot_darker()),tr("Ctrl+D"));
	m_brightness_tool=new BrightnessTool(this);
	m_brightness_tool->hide();
	connect(m_brightness_tool,SIGNAL(levelsChanged(float,float)),this,SLOT(slot_brightness_levels_changed(float,float)));
	
	//Process
	QToolButton *processbutton=new QToolButton();
	processbutton->setPopupMode(QToolButton::InstantPopup);
	processbutton->setText("Process");
	QMenu *processmenu=new QMenu;
	processbutton->setMenu(processmenu);
	ui.toolBar->addWidget(processbutton);
	processmenu->addAction("1D FFT",this,SLOT(slot_1d_fft()),tr("Ctrl+1"));
	processmenu->addAction("2D FFT",this,SLOT(slot_2d_fft()),tr("Ctrl+2"));
	processmenu->addAction("3D FFT",this,SLOT(slot_3d_fft()),tr("Ctrl+3"));
	processmenu->addAction("1D IFFT",this,SLOT(slot_1d_ifft()),tr("Ctrl+Shift+1"));
	processmenu->addAction("2D IFFT",this,SLOT(slot_2d_ifft()),tr("Ctrl+Shift+2"));
	processmenu->addAction("3D IFFT",this,SLOT(slot_3d_ifft()),tr("Ctrl+Shift+3"));
	
	//Status value label	
	m_value_label=new QLabel;
	m_value_label->setFrameStyle(QFrame::StyledPanel);
	statusBar()->addPermanentWidget(m_value_label);
		
	
	
	
}

void ViewmdaWidget::slot_array_changed() {
	update_scrollbar();
	update_status_bar();
	update_dim_boxes();
	double maxval=0;
	double meanval=0;
	qint32 ct=0;
	if (m_model.array().isSparse()) {
		QList<qlonglong> indices=m_model.array().sparseIndices(); 
		foreach (qint64 index,indices) {
			float hold=abs(m_model.array().get(index));
			if (hold>maxval)
				maxval=hold;
			meanval+=hold;
			ct++;
		}
	}
	else {
		for (qint32 j=0; j<m_model.array().size(); j++) {
			float hold=abs(m_model.array().get(j));
			if (hold>maxval)
				maxval=hold;
			meanval+=hold;
			ct++;
		}
	}
	if (ct>0) meanval/=ct;
	m_brightness_tool->setRange(-maxval*0.5,maxval*1.5);
	
	maxval=0;
	for (qint32 x=0; x<m_model.N1(); x++)
	for (qint32 y=0; y<m_model.N2(); y++) {
		float hold=abs(m_model.get(x,y));
		if (hold>maxval) maxval=hold;
	}
	//m_brightness_tool->setLevels(0,maxval*1.2);
	m_model.autoSetWindows();
	
}
void ViewmdaWidget::slot_windows_changed() {
	m_brightness_tool->setLevels(m_model.windowMin(),m_model.windowMax(),false);
}
void ViewmdaWidget::slot_dimensions_changed() {
	update_scrollbar();
	update_status_bar();
	update_dim_boxes();
}
void ViewmdaWidget::slot_current_index_changed() {
	update_status_bar();
}
void ViewmdaWidget::slot_current_slice_changed() {
	update_scrollbar();
	update_status_bar();
}
void ViewmdaWidget::slot_selected_points_changed() {
	update_status_bar();
}
void ViewmdaWidget::slot_selected_rect_changed() {
	update_status_bar();
}
void ViewmdaWidget::slot_zoom_rect_changed() {
	update_status_bar();
}

void ViewmdaWidget::update_scrollbar() {
	if (m_model.N3()<=1) 
		ui.scrollbar->hide();
	else {
		ui.scrollbar->show();
		ui.scrollbar->setRange(0,m_model.N3()-1);
		ui.scrollbar->setValue(m_model.C3());
	}
}

void ViewmdaWidget::slot_scrollbar_value_changed(int value) {
	m_model.setC3(value);
}

QString toString(const Complex &val) {
	if (val.im()==0) return QString::number(val.re());
	else if (val.re()==0) {
		if (val.im()==1) return "i";
		else if (val.im()==-1) return "-i";
		else return QString::number(val.im())+"i";
	}
	else if (val.im()<0) {
		if (val.im()==-1) return QString("%1 - i").arg(val.re());
		else return QString("%1 - %2i").arg(val.re()).arg(-val.im());
	}
	else {
		if (val.im()==1) return QString("%1 + i").arg(val.re());
		else return QString("%1 + %2i").arg(val.re()).arg(val.im());
	}
}


void ViewmdaWidget::update_status_bar() {
	QString txt;
	if (m_model.N3()>1) {
		txt+=tr("Slice %1 of %2;   ").arg(m_model.C3()+1).arg(m_model.N3());
	}
	txt+=tr("Pos (%1, %2)/(%3, %4);   ").arg(m_model.C1()+1).arg(m_model.C2()+1).arg(m_model.N1()).arg(m_model.N2());
	if ((m_model.array().dimCount()>3)||(m_model.D1()!=0)||(m_model.D2()!=1)||(m_model.D3()!=2)) {
		txt+=tr("Index (");
		for (int j=0; j<m_model.array().dimCount(); j++) {
			if (j>0) txt+=", ";
			txt+=QString::number(m_model.currentIndex(j)+1);
		}
		txt+=")/(";
		for (int j=0; j<m_model.array().dimCount(); j++) {
			if (j>0) txt+=", ";
			txt+=QString::number(m_model.array().size(j));
		}
		txt+=");   ";
	}
	
	statusBar()->showMessage(txt,0);
	
	
	Complex val=0;
	real absval=0;
	
	if (m_model.selectedRect().x()<0) {
		val=m_model.get();
		absval=abs(m_model.get());
		txt="  ";
		txt+=tr("Value %1;   ").arg(toString(val));
		txt+=tr("Abs %1  ").arg(absval);
	}
	else {
		QRect SR=m_model.selectedRect();
		int ct=0;
		Complex sum=0;
		real abssum=0;
		for (int x=SR.x(); x<=SR.x()+SR.width(); x++) 
			for (int y=SR.y(); y<=SR.y()+SR.height(); y++) {
				sum=sum+m_model.get(x,y);
				abssum=abssum+abs(m_model.get(x,y));
				ct++;
			}
		if (ct>0) {
			val=sum/ct;
			absval=abssum/ct;
		}
		txt="  ";
		txt+=tr("Avg %1;   ").arg(toString(val));
		txt+=tr("Sum %1;   ").arg(toString(sum));
		txt+=tr("Avg Abs %1  ").arg(absval);
	}
	
	
	m_value_label->setText(txt);
}

void ViewmdaWidget::update_dim_boxes() {
	bool hold_do_change_dimensions=m_do_change_dimensions;
	m_do_change_dimensions=false;
	
	m_dim1_box->clear();
	m_dim2_box->clear();
	m_dim3_box->clear();
	for (int j=0; j<m_model.array().dimCount(); j++) {
		qint32 N=m_model.array().size(j);
		QString txt;
		QString hold="   ";
		if (j==m_model.D1()) hold="X: ";
		else if (j==m_model.D2()) hold="Y: ";
		else if (j==m_model.D3()) hold="Z: ";
		txt=QString("%1%2 (%3)").arg(hold).arg(j+1).arg(N);
		m_dim1_box->addItem(txt);
		m_dim2_box->addItem(txt);
		m_dim3_box->addItem(txt);
		
	}
	if (m_model.D1()>=0) {
		if (m_model.D1()!=m_dim1_box->currentIndex())
			m_dim1_box->setCurrentIndex(m_model.D1());
	}
	if (m_model.D2()>=0) {
		if (m_model.D2()!=m_dim2_box->currentIndex())
			m_dim2_box->setCurrentIndex(m_model.D2());
	}
	if (m_model.D3()>=0) {
		if (m_model.D3()!=m_dim3_box->currentIndex())
			m_dim3_box->setCurrentIndex(m_model.D3());
	}
	m_do_change_dimensions=hold_do_change_dimensions;
}

void ViewmdaWidget::slot_dim_box_changed() {
	if (!m_do_change_dimensions) {
		return;
	}
	if (sender()==m_dim1_box) {
		if (m_dim1_box->currentIndex()<0) {
			return; //important
		}
		m_model.setD1(m_dim1_box->currentIndex());
	}
	else if (sender()==m_dim2_box) {
		if (m_dim2_box->currentIndex()<0) {
			return; //important
		}
		m_model.setD2(m_dim2_box->currentIndex());
	}
	else if (sender()==m_dim3_box) {
		if (m_dim3_box->currentIndex()<0) {
			return; //important
		}
		m_model.setD3(m_dim3_box->currentIndex());
	}
}

void ViewmdaWidget::slot_dimension_button_clicked() {
	if (m_dim1_box->isVisible())
		show_dimension_boxes(false);
	else
		show_dimension_boxes(true);
}

void ViewmdaWidget::slot_brightness_button_clicked() {
	m_brightness_tool->show();
}

void ViewmdaWidget::show_dimension_boxes(bool visible) {
	m_dim1_box_action->setVisible(visible);
	m_dim1_box->setEnabled(visible);
	m_dim2_box_action->setVisible(visible);
	m_dim2_box->setEnabled(visible);
	m_dim3_box_action->setVisible(visible);
	m_dim3_box->setEnabled(visible);
}

void ViewmdaWidget::slot_brightness_levels_changed(float minlevel,float maxlevel) {
	m_model.setWindows(minlevel,maxlevel);
}

void ViewmdaWidget::slot_lighter() {
	double wmin=m_model.windowMin();
	double wmax=m_model.windowMax();
	wmax=wmin+(wmax-wmin)/1.1;
	m_model.setWindows(wmin,wmax);
}
void ViewmdaWidget::slot_darker() {
	double wmin=m_model.windowMin();
	double wmax=m_model.windowMax();
	wmax=wmin+(wmax-wmin)*1.1;
	m_model.setWindows(wmin,wmax);
}

void ViewmdaWidget::slot_save_as() {
	QString fname=QFileDialog::getSaveFileName(this,"Save array as...",QDir::currentPath());		
	if (!fname.isEmpty()) {
		m_model.array().write(fname.toAscii().data());
	}
}
void ViewmdaWidget::slot_open() {
	QString fname=QFileDialog::getOpenFileName(this,"Open array...",QDir::currentPath());		
	if (!fname.isEmpty()) {
		Mda A;
		A.read(fname.toAscii().data());
		m_model.setArray(A);
	}
}

void ViewmdaWidget::slot_export_imageJ() {
	export_to_imageJ(m_model.array(),m_model.D1(),m_model.D2(),m_model.D3());
}

void ViewmdaWidget::slot_1d_fft() {
	Mda X=m_model.array();
	apply_1d_fft(X,false,m_model.D1());
	m_model.setArray(X);
}
void ViewmdaWidget::slot_2d_fft() {
	Mda X=m_model.array();
	apply_2d_fft(X,false,m_model.D1(),m_model.D2());
	m_model.setArray(X);
}
void ViewmdaWidget::slot_3d_fft() {
	Mda X=m_model.array();
	apply_3d_fft(X,false,m_model.D1(),m_model.D2(),m_model.D3());
	m_model.setArray(X);
}
void ViewmdaWidget::slot_1d_ifft() {
	Mda X=m_model.array();
	apply_1d_fft(X,true,m_model.D1());
	m_model.setArray(X);
}
void ViewmdaWidget::slot_2d_ifft() {
	Mda X=m_model.array();
	apply_2d_fft(X,true,m_model.D1(),m_model.D2());
	m_model.setArray(X);
}
void ViewmdaWidget::slot_3d_ifft() {
	Mda X=m_model.array();
	apply_3d_fft(X,true,m_model.D1(),m_model.D2(),m_model.D3());
	m_model.setArray(X);
}


