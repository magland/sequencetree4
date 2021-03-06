/* BEGIN EXCLUDE */
/* END EXCLUDE */

class STArbGradient : public STNode {
public:
	/* BEGIN SEQUENCETREE -- this section generated by SequenceTree -- do not edit */
	STReal ramp_time_1;
	STReal plateau_time;
	STReal ramp_time_2;
	STVector3 kspace_offset;
	STReal peakamp;
	STReal peakslew;
	/* END SEQUENCETREE -- this section generated by SequenceTree -- do not edit */
	
	//events
	STScannerArbGradient *GX;
	STScannerArbGradient *GY;
	STScannerArbGradient *GZ;
	/////////////////////////////////////////////////////
	STArbGradient();
	virtual ~STArbGradient();
	
	virtual Vector3 gradientShape(double t); //t ranges from 0 to 1, accross the plateau, return k-space location
	Vector3 momentAt(double t); //t is between 0 and 1, along the plateau
	Vector3 ramp1Moment();
	
	virtual bool initialize();
	virtual bool prepare();
	virtual bool run();
	virtual double duration();
	virtual Vector3 terminalMoment();
	virtual Vector3 totalGradientMoment();
	virtual Vector3 gradientStartTimes(); 
	virtual Vector3 gradientEndTimes();
private:
	void delete_events();
	
	
	Vector3 m_total_moment;
};

