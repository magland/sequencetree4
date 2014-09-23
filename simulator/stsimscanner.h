
#ifndef stsimscanner_H
#define stsimscanner_H

#include "stscanner.h"
#include <stdio.h>
#include "stsimscannerblocklist.h"

class ST_EXPORT STSimScanner : public STScanner {
public:
	STSimScanner() {
		m_min_block=m_max_block=-1;
		m_raw_template_file=0;
		m_wait_for_trigger=false;
	}
	virtual ~STSimScanner() {
		clear();
	}
	//virtual from STScanner ////////////////////////////////
	void beginSequence();
	void endSequence();
	void beginBlock(double duration);
	void endBlock(STNode *block_node);
	bool inBlockRange() {
		if (m_min_block<0) return true;
		return ((m_min_block<=m_current_block_number)&&(m_current_block_number<=m_max_block));
	}
	bool doneScanning() {
		if (m_max_block<0) return false;
		if (blockCount()>=m_max_block+1) return true;
		return false;
	}
	void iteratorInitialized(int iterator_index);
	void iteratorStepped(int iterator_index);
	void iteratorReset(int iterator_index);
	int scannerCommand(SString command_name,void *data1=0,void *data2=0,void *data3=0,void *data4=0);
	////////////////////////////////////////////////////////////////////
	
	void clear() {
		m_blocks.deleteAndClear(); 
		m_current_block_number=0;
	}
	long blockCount() {return m_blocks.count();}
	STSimScannerBlock *block(long index) {return m_blocks.block(index);}
	void addBlock(STSimScannerBlock *B) {
		m_blocks.addBlock(B);
	}
	void writeBlocks(SString file_name) {m_blocks.write(file_name);}
	void readBlocks(SString file_name) {m_blocks.read(file_name);}
	
	void setBlockRange(long min_block,long max_block) {
		m_min_block=min_block;
		m_max_block=max_block;
	}
	
	void setRawTemplateFile(FILE *raw_template_file) {
		m_raw_template_file=raw_template_file;
	}
	void setReadoutIndex(int ADC_index,int iterator_index,int indx);
	
private:
	STSimScannerBlockList m_blocks;
	
	long m_min_block,m_max_block;
	long m_current_block_number;
	FILE *m_raw_template_file;
	bool m_wait_for_trigger;
	
	void add_block_event(STSimScannerBlock *B,STScannerEvent *E,STScannerEventInfo start_time);
};

#endif
