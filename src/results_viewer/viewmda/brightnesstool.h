#ifndef brightnesstool_H
#define brightnesstool_H

#include "ui_brightnesstool.h"

class BrightnessTool : public QWidget {
	Q_OBJECT
	Ui::BrightnessTool ui;
public:
	BrightnessTool(QWidget *parent=0);
	virtual ~BrightnessTool();
	void setRange(float range_min,float range_max);
	void setLevels(float level_min,float level_max,bool emit_levels_changed);
	float levelMin();
	float levelMax();
signals:
	void levelsChanged(float level_min,float level_max);
private:
	float m_range_min,m_range_max;
	float m_level_min,m_level_max;
	bool m_do_emit;
	
	void update_controls();
private slots:
	void slot_minlevel_changed();
	void slot_maxlevel_changed();
};

#endif
