#ifndef VSPhantomView_H
#define VSPhantomView_H

#include "ui_vsphantomview.h"
#include "vsphantom.h"

class VSPhantomView : public QWidget {
	Q_OBJECT
private:
	Ui::VSPhantomView ui;
public:
	VSPhantomView(QWidget *parent=0) : QWidget(parent) {
		ui.setupUi(this);
		ui.property_editor->initialize();
		m_phantom=0;
		
		connect(ui.select_phantom_button,SIGNAL(clicked()),this,SIGNAL(selectPhantom()));
	}
	void setPhantom(VSPhantom *P);
	void updateView();
signals:
	void selectPhantom();
private:
	VSPhantom *m_phantom;
};

#endif
