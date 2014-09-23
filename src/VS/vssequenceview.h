#ifndef VSSequenceView_H
#define VSSequenceView_H

#include "ui_vssequenceview.h"
#include "stmetasequencesimulator.h"
#include "stglobalview.h"

class VSSequenceView : public QWidget {
	Q_OBJECT
private:
	Ui::VSSequenceView ui;
public:
	VSSequenceView(QWidget *parent=0) : QWidget(parent) {
		ui.setupUi(this);
		ui.global_view->setEditSequenceMode(false);
		m_simulator=0;
		
		connect(ui.global_view,SIGNAL(globalParameterValueChanged(QString,QString)),this,SLOT(slot_global_parameter_value_changed(QString,QString)));
	}
	void setSimulator(STMetaSequenceSimulator *S) {
		m_simulator=S;
		updateView();
	}
	void updateView() {
		ui.sequence_name->setText("");
		ui.global_view->setSequence(0);
		ui.duration->setText("");
		ui.num_blocks->setText("");
		if (!m_simulator) return;
		if (!m_simulator->metaSequence()) return;
		if (!m_simulator->isCompiled()) return;
		ui.global_view->setSequence(m_simulator->metaSequence());
		ui.global_view->updateGlobalParameters();
		
		STMetaSequence *Seq=m_simulator->metaSequence();
		ui.sequence_name->setText(Seq->fileName());
		
		m_simulator->computeStatistics(true);
		ui.duration->setText(QString("%1 seconds").arg(m_simulator->totalDuration()/1.0E6));
		ui.num_blocks->setText(QString::number(m_simulator->numBlocks()));
	}
signals:
	void clearSimulateLog();
private slots:
	void slot_global_parameter_value_changed(QString name,QString value);	
private:
	STMetaSequenceSimulator *m_simulator;
};

#endif
