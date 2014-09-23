#include "brightnesstool.h"
#include <QDebug>

BrightnessTool::BrightnessTool(QWidget *parent) : QWidget(parent) 
{
	ui.setupUi(this);
	m_range_min=0;
	m_range_max=255;
	m_level_min=0;
	m_level_max=255;
	ui.minimum->setRange(0,1000);
	ui.maximum->setRange(0,1000);
	setWindowFlags(windowFlags() | Qt::Tool);
	m_do_emit=true;
	
	connect(ui.minimum,SIGNAL(valueChanged(int)),this,SLOT(slot_minlevel_changed()),Qt::DirectConnection);
	connect(ui.maximum,SIGNAL(valueChanged(int)),this,SLOT(slot_maxlevel_changed()),Qt::DirectConnection);
}

BrightnessTool::~BrightnessTool()
{
}

void BrightnessTool::setRange(float range_min,float range_max) {
	m_range_min=range_min;
	m_range_max=range_max;
	if ((m_level_min<m_range_min)||(m_level_max>m_range_max)) {
		setLevels(qMax(m_level_min,m_range_min),qMin(m_level_max,m_range_max),false);
	}
	update_controls();
}
void BrightnessTool::setLevels(float level_min,float level_max,bool emit_levels_changed) {
	bool something_changed=false;
	if (level_min!=m_level_min) {
		m_level_min=level_min;
		something_changed=true;
	}
	if (level_max!=m_level_max) {
		m_level_max=level_max;
		something_changed=true;
	}
	if (something_changed) {
		if (emit_levels_changed) emit levelsChanged(m_level_min,m_level_max);
		update_controls();
	}
}
float BrightnessTool:: levelMin() {
	return m_level_min;
}
float BrightnessTool::levelMax() {
	return m_level_max;
}

void BrightnessTool::update_controls() {
	double minval=0;
	double maxval=0;
	if (m_range_max>m_range_min) {
		minval=(m_level_min-m_range_min)/(m_range_max-m_range_min);
		maxval=(m_level_max-m_range_min)/(m_range_max-m_range_min);
	}
	bool hold_do_emit=m_do_emit;
	m_do_emit=false;
	ui.minimum->setValue((int)(minval*1000));
	ui.maximum->setValue((int)(maxval*1000));
	ui.min_label->setText(QString("Minimum (%1)").arg(m_level_min));
	ui.max_label->setText(QString("Maximum (%2)").arg(m_level_max));
	m_do_emit=hold_do_emit;
}

void BrightnessTool::slot_minlevel_changed() {
	if (ui.maximum->value()<ui.minimum->value()) ui.maximum->setValue(ui.minimum->value());
	setLevels(m_range_min+(ui.minimum->value()*1.0/1000)*(m_range_max-m_range_min),m_range_min+(ui.maximum->value()*1.0/1000)*(m_range_max-m_range_min),m_do_emit);
}
void BrightnessTool::slot_maxlevel_changed() {
	if (ui.maximum->value()<ui.minimum->value()) ui.minimum->setValue(ui.maximum->value());
	setLevels(m_range_min+(ui.minimum->value()*1.0/1000)*(m_range_max-m_range_min),m_range_min+(ui.maximum->value()*1.0/1000)*(m_range_max-m_range_min),m_do_emit);
}


