/* BEGIN EXCLUDE */
#ifndef stloop_H
#define stloop_H

#include "stnode.h"
/* END EXCLUDE */

class ST_EXPORT STLoop : public STNode {
public:
	STLoop();
	bool isLoop() {return true;}
	
	virtual bool loopInitialize();
	virtual bool loopStep();
	virtual bool loopRun();
	
	bool run();
private:
	SList<STIterator *> m_iterators;
};

/* BEGIN EXCLUDE */
#endif
/* END EXCLUDE */
