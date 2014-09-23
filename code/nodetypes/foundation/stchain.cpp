/* BEGIN EXCLUDE */
/* END EXCLUDE */

STChain::STChain() {
	ST_CLASS(STChain,STNode)
	/* ST_PARAMETERS */
	/* ST_CHILDREN */
}

bool overlaps(STNode *C1,STNode *C2) {
	Vector3 vec1=C1->gradientEndTimes();
	Vector3 vec2=C2->gradientStartTimes();
	double rel1=C1->relativeStartTime();
	double rel2=C2->relativeStartTime();
	if (vec1.x()+rel1>vec2.x()+rel2) return true;
	if (vec1.y()+rel1>vec2.y()+rel2) return true;
	if (vec1.z()+rel1>vec2.z()+rel2) return true;
	return false;
}

bool STChain::prepare() {
	//prepare children and set initial moments
	if (childCount()>0) {
		child(0)->setInitialMoment(initialMoment());
		child(0)->setInitialTotalMoment(initialTotalMoment());
	}
	{for (int j=0; j<childCount(); j++) {
		if (child(j)->isModified()) {
			if (!child(j)->prepare()) return false;
			if (j+1<childCount()) {
				child(j+1)->setInitialMoment(child(j)->terminalMoment());
				child(j+1)->setInitialTotalMoment(child(j)->initialTotalMoment()+child(j)->totalGradientMoment());
			}
		}
	}}
	
	SList<int> has_been_positioned;
	for (int j=0; j<childCount(); j++)
		has_been_positioned.append(0);
	
	//set timing
	bool something_changed=true;
	while (something_changed) {
		something_changed=false;
		for (int j=0; j<childCount(); j++) {
			if (has_been_positioned[j]==0) {
				if (m_alignment_data[j].alignment==ST_ALIGN_LEFT) {
					if (j==0) {
						child(j)->setRelativeStartTime(m_alignment_data[j].timing);
						something_changed=true;
						has_been_positioned[j]=1;
					}
					else if (has_been_positioned[j-1]) {
						Vector3 hold1=child(j-1)->gradientEndTimes();
						Vector3 hold2=child(j)->gradientStartTimes();
						double reltime1=child(j-1)->relativeStartTime();
						double timex=reltime1+hold1.x()-hold2.x();
						double timey=reltime1+hold1.y()-hold2.y();
						double timez=reltime1+hold1.z()-hold2.z();
						double reltime2=timex; if (timey>reltime2) reltime2=timey; if (timez>reltime2) reltime2=timez;
						child(j)->setRelativeStartTime(reltime2+m_alignment_data[j].timing);
						something_changed=true;
						has_been_positioned[j]=1;
					}
				}
				else if (m_alignment_data[j].alignment==ST_ALIGN_RIGHT) {
					if (j==childCount()-1) {
						child(j)->setRelativeStartTime(duration()-child(j)->duration()+m_alignment_data[j].timing);
						something_changed=true;
						has_been_positioned[j]=1;
					}
					else if (has_been_positioned[j+1]) {
						Vector3 hold1=child(j+1)->gradientStartTimes();
						Vector3 hold2=child(j)->gradientEndTimes();
						double reltime1=child(j+1)->relativeStartTime();
						double timex=reltime1-hold2.x()+hold1.x();
						double timey=reltime1-hold2.y()+hold1.y();
						double timez=reltime1-hold2.z()+hold1.z();
						double reltime2=timex; if (timey<reltime2) reltime2=timey; if (timez<reltime2) reltime2=timez;
						child(j)->setRelativeStartTime(reltime2);
						something_changed=true;
						has_been_positioned[j]=1;
					}
				}
				else if (m_alignment_data[j].alignment==ST_ALIGN_RELATIVE) {
					int index=m_alignment_data[j].relative_index;
					if ((index>=0)&&(index<childCount())) {
						if (has_been_positioned[index]) {
							child(j)->setRelativeStartTime(child(index)->relativeStartTime()+child(index)->referenceTime()-child(j)->referenceTime()+m_alignment_data[j].timing);
							something_changed=true;
							has_been_positioned[j]=1;
						}
					}
				}
				else if (m_alignment_data[j].alignment==ST_ALIGN_ABSOLUTE) {
					child(j)->setRelativeStartTime(m_alignment_data[j].timing-child(j)->referenceTime());
					something_changed=true;
					has_been_positioned[j]=1;
				}
				
			}
		}
	}
	
	//check for overlaps
	{for (int j=0; j<childCount(); j++) {
		if ((j==0)&&(child(j)->relativeStartTime()<0)) {
			SString str=SString("Error in ")+name()+", start time for "+child(j)->name()+" is negative";
			reportError(str);
			return false;
		}
		if ((j>0)&&(overlaps(child(j-1),child(j)))) {
			SString str=SString("Error in ")+name()+", "+child(j-1)->name()+" overlaps "+child(j)->name();
			reportError(str);
			return false;
		}
		if (j==childCount()-1) {
			if (child(j)->relativeStartTime()+child(j)->duration()>m_duration) {
				SString str=SString("Error in ")+name()+", end time for "+child(j)->name()+" overlaps end of chain";
				reportError(str);
				return false;
			}
		}
	}}
	
	setModified(false);
	
	
	return true;
}

void STChain::addChild(STNode *obj) {
	STNode::addChild(obj);
	STChainAlignmentData data;
	data.alignment=ST_ALIGN_LEFT;
	data.timing=0;
	data.relative_index=0;
	m_alignment_data.append(data);
}
void STChain::removeChild(int index) {
	STNode::removeChild(index);
	m_alignment_data.removeAt(index);
}
void STChain::setAlignment(int index,int alignment,double timing,int relative_index) {
	bool something_changed=false;
	if (m_alignment_data[index].alignment!=alignment) {
		m_alignment_data[index].alignment=alignment;
		something_changed=true;
	}
	if (m_alignment_data[index].timing!=timing) {
		m_alignment_data[index].timing=timing;
		something_changed=true;
	}
	if (m_alignment_data[index].relative_index!=relative_index) {
		m_alignment_data[index].relative_index=relative_index;
		something_changed=true;
	}
	if (something_changed) setModified(true);
}

void STChain::alignChild(STNode *childptr,int alignment,double timing,int relative_index) {
	for (int j=0; j<childCount(); j++) 
		if (child(j)==childptr) {
			if (j<m_alignment_data.count()) {
				m_alignment_data[j].alignment=alignment;
				m_alignment_data[j].timing=timing;
				m_alignment_data[j].relative_index=relative_index;
				return;
			}
		}
	printf("Error setting alignment for %s",childptr->name().data());
}

