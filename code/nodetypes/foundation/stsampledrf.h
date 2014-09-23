class STSampledRF : public STRF {
public:
	/* BEGIN SEQUENCETREE -- this section generated by SequenceTree -- do not edit */
	STString pulse_shape;
	/* END SEQUENCETREE -- this section generated by SequenceTree -- do not edit */
	STSampledRF();
	bool prepare();
	void pulseShape(double cycles,double &re,double &im);
	
	void setSamples(long N, const double *data_real, const double *data_imag, double reference_center_time /*us*/, double reference_time_step /*us*/, double reference_bandwidth /*Hz*/);
private:
	long m_num_samples;
	double m_reference_center_time; //us
	double m_reference_timestep; //us
	double m_reference_bandwidth; //Hz
	SList<double> m_data_real;
	SList<double> m_data_imag;
	
	
};
