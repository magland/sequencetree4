#include "customclasses.h"

////// STSphericalLoop //////
#line 1 "source-of-STSphericalLoop"
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STSphericalLoop::STSphericalLoop() {
	ST_CLASS(STSphericalLoop,STLoop)
	/* ST_PARAMETERS */
	//ST_PARAMETER(STIterator,angle,0:1:0 check: 0;90;180;270,degrees)
	ST_PARAMETER(STIterator,projection,0:1:0,)
	ST_PARAMETER(STIterator,average,0:1:0,)
	//ST_PARAMETER(STIterator,PE,0:1:0,)
	ST_PARAMETER(STVector3,readout_dir1,(1,0,0),)
	ST_PARAMETER(STVector3,readout_dir2,(0,1,0),)
	//ST_PARAMETER(STVector3,readout_dir3,(0,0,1),)
	ST_PARAMETER(STVector3,gradient_amp,(0,0,5),)
	ST_PARAMETER(STInteger, num_projections, 5000, number of projections)
	//ST_PARAMETER(STReal, viewAngleStep, 1,)
	ST_PARAMETER(STInteger, num_averages, 1, number of averages)
	ST_PARAMETER(STInteger,RFS,0,0 or 1)
	ST_PARAMETER(STInteger, phi_0, 117, phi_0)
	ST_PARAMETER(STInteger,isMainLoop,1,0 or 1)
	ST_PARAMETER(STReal, Radius, 10,)
	m_reorderInfo = NULL;
	//ST_PARAMETER(STReal, hold_phi, 0, radians)
	/* ST_CHILDREN */
	ST_CHILD(STZTEBlock,Block)
}

STSphericalLoop::~STSphericalLoop()
{
     if (m_reorderInfo!=NULL) delete [] m_reorderInfo;
      m_reorderInfo = NULL;
}

int STSphericalLoop::iteration_number=0;
const double STSphericalLoop::PI = 3.14159265358979323846;

long STSphericalLoop::setKspaceLocations(int radius) {
     if (m_reorderInfo!=NULL) delete [] m_reorderInfo;
    // long lMaxProjectionsToMeasure = (2*radius+1)*(2*radius+1)*(2*radius+1);
     //long lMaxProjectionsToMeasure = (long) (4*3.2/3*(radius+1)*(radius+1)*(radius+1));
     long lMaxProjectionsToMeasure = (long) (3.2*(radius+1)*(radius+1));
     m_reorderInfo = new RawLineData [lMaxProjectionsToMeasure] ;    
      long proj=0;	
	for (int PE1=-radius;PE1<=radius;++PE1) {
		for (int PE2=-radius;PE2<=radius;++PE2) {
			//for (int PE3=-radius;PE3<=radius;++PE3) {
				if (((PE1*PE1+PE2*PE2)<radius*radius)&&(proj<lMaxProjectionsToMeasure)) {
                               m_reorderInfo[proj].lKx = ((double)PE1)/((double)radius);
                               m_reorderInfo[proj].lKy = ((double)PE2)/((double)radius);
                              // m_reorderInfo[proj].lKz = ((double)PE3)/((double)radius);				    
				          ++proj;				
				}			
                 //   }		
              }	
         }	
         return proj;					
//	printf("I am at setKspaceLocations!!!\n");
}


bool STSphericalLoop::prepare() {
	average.set(1,1,num_averages);
	// calculate the distributions 
	    if (isMainLoop) {
	        if (num_projections.isModified()) {
		    projection.set(0,1,num_projections-1);
		//    viewAngleStep=360.0/(static_cast<double>(num_projections))*PI/180.0;
	        }	    
           }
          else {
          	if (Radius.isModified()) {
           	 long num_projections_temp = setKspaceLocations((int)Radius);
           	 projection.set(0,1,num_projections_temp-1);             	
             }
            }		
	  return STLoop::prepare();
}

bool STSphericalLoop::loopRun() {
      // RF spoiling
      if (RFS) {
	long phase_shift_temp=static_cast<long>(phi_0*(double(iteration_number*(iteration_number+1)))/2);
	sequence()->phase_shift = phase_shift_temp%360;
	iteration_number++;
	}	
      if (isMainLoop) {
      //Wong's method MRM 1994	
	 //  double kZ = (2.0*projection-num_projections-1)/num_projections;
	  // double temp1 = sqrt(num_projections*PI)*asin(kZ);
	  // double temp2 = sqrt(1.0-kZ*kZ);
	  double viewAngle=360.0/(static_cast<double>(num_projections))*PI/180.0*projection;
          Block->kspace_dir=readout_dir1*cos(viewAngle)+readout_dir2*sin(viewAngle);     
	}
      else {
      	     int projection_temp = (int)(projection);
      	     double x =  m_reorderInfo[projection_temp].lKx;
      	     double y =  m_reorderInfo[projection_temp].lKy;
      	    // double z =  m_reorderInfo[projection_temp].lKz;
           Block->kspace_dir=readout_dir1*x+readout_dir2*y;//+ readout_dir3*z;  
       }
       Block->kspace_echo=0;
	return Block->run();
}


////// STZTEAcquire //////
#line 1 "source-of-STZTEAcquire"
/* BEGIN EXCLUDE */

/* END EXCLUDE */

STZTEAcquire::STZTEAcquire() {
	ST_CLASS(STZTEAcquire,STNode)
	/* ST_PARAMETERS */
	ST_PARAMETER(STVector3, echo_moment, (0,0,0), [uT/mm]-us)
	ST_PARAMETER(STVector3, moment_per_point, (0,0,0),[uT/mm]-us)
	ST_PARAMETER(STReal,readout_offset,0,microsec)
	ST_PARAMETER(STReal,RF_offset,0,microsec)
	/* ST_CHILDREN */
	ST_CHILD(STRF,RF)
	ST_CHILD(STGradientAmp,ReadoutGradient)
	ST_CHILD(STReadout,Readout)
	
	setReferenceChildIndex(0);
}
bool STZTEAcquire::prepare() {
	if (!sequence()) return false;
	if (!scanner()) return false;
	
	//Prepare RF
	if (RF->isModified())
           if(!RF->prepare()) return false;
      
      double RF_duration=RF->duration();
      //RF_duration=scanner()->rounduptime(RF_duration);
           
      //Prepare Readout           		
	if (Readout->isModified()) 
		if (!Readout->prepare()) return false;
		
	double readout_duration=Readout->duration();
	//readout_duration=scanner()->rounduptime(readout_duration);
	
	//Prepare Readout Gradient
	ReadoutGradient->amplitude=moment_per_point/Readout->dwell_time;
	ReadoutGradient->plateau_time=scanner()->rounduptime(RF_offset+RF_duration+readout_duration+readout_offset);
	if (ReadoutGradient->isModified()) 
		if (!ReadoutGradient->prepare()) return false;
	
      
	//set gradient information for RF
	RF->setGradientAmplitude(ReadoutGradient->amplitude);
	RF->setInitialTotalMoment(initialTotalMoment()+ReadoutGradient->amplitude*(ReadoutGradient->ramp_time_1/2+RF_offset));	
	
	//set gradient information for readout
	Readout->setGradientAmplitude(ReadoutGradient->amplitude);
	Readout->setInitialTotalMoment(initialTotalMoment()+ReadoutGradient->amplitude*(RF_duration*(1.0-RF->reference_fraction)+readout_offset));
	
	//Set start times
	
	ReadoutGradient->setRelativeStartTime(0);
	RF->setRelativeStartTime(ReadoutGradient->relativeStartTime()+ReadoutGradient->ramp_time_1+RF_offset);
	Readout->setRelativeStartTime(RF->relativeStartTime()+RF_duration+readout_offset/*+ReadoutGradient->ramp_time_1*/);
	
	setModified(false);
	
	return true;
}

Vector3 STZTEAcquire::terminalMoment() {
	if (!ReadoutGradient) return Vector3(0,0,0);
	if (!RF) return Vector3(0,0,0);
      double RF_duration=RF->duration();
      RF_duration=scanner()->rounduptime(RF_duration);
	double effective_time=ReadoutGradient->ramp_time_2/2.0+ReadoutGradient->plateau_time-RF_duration*RF->reference_fraction-RF_offset;
	return ReadoutGradient->amplitude*effective_time;
}

double STZTEAcquire::duration() {
	return ReadoutGradient->relativeStartTime()+ReadoutGradient->duration();
}


////// STRoot //////
#line 1 "source-of-STRoot"
STRoot::STRoot() {
	ST_CLASS(STRoot,STSequence)
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal,FOVx,50,mm)
	ST_PARAMETER(STReal,FOVy,50,mm)
	ST_PARAMETER(STReal,FOVz,50,mm)
	/* ST_CHILDREN */
	ST_CHILD(STSphericalLoop,PrepLoop);
	ST_CHILD(STSphericalLoop,ZTELoop);
	//ST_CHILD(STRadialLoop_2,SPILoop);
	ST_CHILD(STSphericalLoop,PETRALoop);
}

bool STRoot::prepare() {
	//sequence()->set.FOV=(FOVx,FOVy,FOVz);	
	FOV.setX(FOVx);FOV.setY(FOVy);FOV.setZ(FOVz);
	return STSequence::prepare();
}


////// STZTEBlock //////
#line 1 "source-of-STZTEBlock"
/* BEGIN EXCLUDE */
/* END EXCLUDE */

STZTEBlock::STZTEBlock() {
	ST_CLASS(STZTEBlock,STChain)
	/* ST_PARAMETERS */
	ST_PARAMETER(STReal,TE,4600,microsec)
	ST_PARAMETER(STReal,TR,50000,microsec)
	ST_PARAMETER(STVector3, kspace_dir, (1,0,0),)
	ST_PARAMETER(STVector3, kspace_echo, (0,0,0),)
	ST_PARAMETER(STReal, excite_time, 1000, microsec)
	//ST_PARAMETER(STReal,TEshift,20,)
	//ST_PARAMETER(STVector3,extra_moment,(0,0,0),mT/m-us)
	/* ST_CHILDREN */
	ST_CHILD(STZTEAcquire,Acquire);
	ST_CHILD(STAcquire,Acquire2);
       ST_CHILD(STEncode, Rewind)	
	
//	ST_DEFAULT(Excite->RF,STRF)
//	ST_DEFAULT(Excite->RF->flip_angle,15)
//	setReferenceChildIndex(0);
}
bool STZTEBlock::initialize() {
	STChain::initialize();
	return true;
}
bool STZTEBlock::prepare() {
	/* ST_ALIGNMENTS */
	ST_ALIGN(Acquire, ST_ALIGN_LEFT, excite_time, 0)
	ST_ALIGN(Acquire2, ST_ALIGN_LEFT, TE, 0)
      ST_ALIGN(Rewind, ST_ALIGN_LEFT, 0, 0)

	if (!sequence()) return false;
	if (TR.isModified()) setDuration(TR);
	Acquire->moment_per_point=sequence()->kspace2moment(kspace_dir);
	Acquire->echo_moment=sequence()->kspace2moment(kspace_echo);
	Acquire2->moment_per_point=sequence()->kspace2moment(kspace_dir*(-1.0));
	Acquire2->echo_moment=sequence()->kspace2moment(kspace_echo);
	return STChain::prepare();
}


