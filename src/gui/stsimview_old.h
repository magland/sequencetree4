#ifndef stsimview_H
#define stsimview_H

#include <QAbstractScrollArea>
#include "stmetasequence.h"
#include "stsimscanner.h"

class STSimView : public QAbstractScrollArea {
	Q_OBJECT
public:
	STSimView(QWidget *par=0);
	virtual ~STSimView();
	void setSequence(STMetaSequence *S);
	long currentBlockNumber();
	void setCurrentBlockNumber(long val);
	bool isCompiled() {return m_is_compiled;}
	void setCompiled(bool val) {m_is_compiled=val;}
private:
	STMetaSequence *m_sequence;
	STSimScanner m_scanner;
	long m_current_block_number;
	bool m_is_compiled;
	
	void update_plots();
	
	bool simulate();
};

#endif
