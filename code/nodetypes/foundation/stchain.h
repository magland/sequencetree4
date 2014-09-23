/* BEGIN EXCLUDE */
/* END EXCLUDE */

#define ST_ALIGN_LEFT 1
#define ST_ALIGN_CENTER 2
#define ST_ALIGN_RIGHT 3
#define ST_ALIGN_RELATIVE 4
#define ST_ALIGN_ABSOLUTE 5

struct STChainAlignmentData {
	int alignment;
	double timing;
	int relative_index;
};

class STChain : public STNode {
public:
	STChain();
	virtual ~STChain() {
	}
	
	bool prepare();
	double duration() {return m_duration;}
	
	void addChild(STNode *obj);
	void removeChild(int index);
	void setAlignment(int index,int alignment,double timing=0,int relative_index=0);
	void setDuration(double val) {m_duration=val;}
	void alignChild(STNode *childptr,int alignment,double timing,int relative_index);
private:
	SList<STChainAlignmentData> m_alignment_data;
	double m_duration;
};

bool overlaps(STNode *C1,STNode *C2);

