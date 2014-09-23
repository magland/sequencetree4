/* BEGIN EXCLUDE */
#ifndef stsequence_H
#define stsequence_H

#include "stloop.h"
#include "stresources.h"
/* END EXCLUDE */

struct STSequenceGlobalParameter {
	SString name;
	SList<SString> connections;
};

class ST_EXPORT STSequence : public STLoop {
public:
	STVector3 FOV;
	STReal maxamp;
	STReal ramprate;
	STReal gamma;
	STVector3 FOV_shift;
	STReal phase_shift;
	STVector3 FOV_shift_offset;
	
	STSequence();
	Vector3 kspace2moment(Vector3 kspace);
	
	STLoop *loop(int index);
	
	bool loopInitialize();
	bool loopStep();
	bool loopRun();
	
	bool initialize();
	void setTreeIndices() {int ind=0; STNode::setTreeIndices(ind);}
	void setADCIndices() {int ind=0; STNode::setADCIndices(ind);}
	
	bool run();
	bool check();
	bool checkButDontRun();
	bool computeStatistics();
	virtual void setup() {}

	void loadParametersFromFile(SString file_name);	
	void loadParametersFromText(const SString &txt);	
	void writeParametersToFile(SString file_name);
	
	bool checkMode() {return m_check_mode;}
	bool statisticsMode() {return m_statistics_mode;}
	
	void incrementStatBlockCount(long val) {m_stat_block_count+=val;}
	void incrementStatDuration(double val) {m_stat_duration+=val;}
	void incrementStatSAR(double val) {m_stat_SAR+=val;}
	long statBlockCount() {return m_stat_block_count;}
	double statDuration() {return m_stat_duration;}
	double statSAR() {return m_stat_SAR;}
	
	double getFOVShiftX() {return FOV_shift.x()+FOV_shift_offset.x();}
	double getFOVShiftY() {return FOV_shift.y()+FOV_shift_offset.y();}
	double getFOVShiftZ() {return FOV_shift.z()+FOV_shift_offset.z();}
	
	double phaseAdjustment();
	bool checkButDontRunMode() {return m_check_but_dont_run_mode;}
	
	STNode *findNodeFromString(SString str);
	STParameter *findParameterFromString(SString str);

	STResources *resources();
	STResource *resource(SString name);
	
	void addGlobalParameterConnection(SString name,SString connection);
	double globalParameter(SString name);
	void setGlobalParameter(SString name,double val);
private:	
	int m_current_loop_index;
	bool m_check_mode;
	bool m_check_but_dont_run_mode;
	bool m_statistics_mode;
	
	long m_stat_block_count;
	double m_stat_duration;
	double m_stat_SAR;

	STResources m_resources;
	
	SList<STSequenceGlobalParameter> m_global_parameters;
	
	void write_parameters_to_text(SString &txt,STNode *ptr=0);
};

/* BEGIN EXCLUDE */
#endif
/* END EXCLUDE */
