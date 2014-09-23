#include "distribute_raw_data.h"

#include <QDebug>
#include <QList>
#include "mda.h"

void read_template_records(QList<RawTemplateRecord> &template_records,QString template_fname) {
	FILE *template_file=fopen(template_fname.toAscii().data(),"rb");
	if (!template_file) return;	
	while (!feof(template_file)) {	
		int code;
		if (fread(&code,sizeof(int),1,template_file)) {
			if (code==5) { //readout
				RawTemplateRecord R;
				R.code=code;
				fread(&R.ADC_index,sizeof(int),1,template_file);
				fread(&R.num_points,sizeof(int),1,template_file);
				template_records << R;
			}
			else if ((code==10)||(code==15)||(code==20)) { //initialize, step, reset
				RawTemplateRecord R;
				R.code=code;
				fread(&R.iterator_index,sizeof(int),1,template_file);
				template_records << R;
			}
			else if (code==25) {
				RawTemplateRecord R;
				R.code=code;
				int num_bytes;
				fread(&num_bytes,sizeof(int),1,template_file);
				if (num_bytes==sizeof(int)*3) {
					fread(&R.ADC_index,sizeof(int),1,template_file);
					fread(&R.iterator_index,sizeof(int),1,template_file);
					fread(&R.index,sizeof(int),1,template_file);
					template_records << R;
				}
				else {
					qWarning() << "Unexpected problem reading template file.";
					unsigned char dummy;
					for (int ct=0; ct<num_bytes; ct++)
						fread(&dummy,1,1,template_file);
				}				
			}
			else {
				qWarning() << "Unrecognized template code:" << code;
				int num_bytes;
				fread(&num_bytes,sizeof(int),1,template_file);
				unsigned char dummy;
				for (int ct=0; ct<num_bytes; ct++)
					fread(&dummy,1,1,template_file);
			}
		}
	}	
	fclose(template_file);
}

void determine_raw_file_dimensions(QList<RawFileStruct> &raw_file_structs,QList<long> &real_index,QList<RawTemplateRecord> &template_records) {
	foreach (RawTemplateRecord record,template_records) {
		if (record.code==5) { //readout
			int ADC_index=record.ADC_index;
			if (ADC_index<0) {
				qWarning() << "ADC_index is negative!";
			}
			else {
				while (ADC_index>=raw_file_structs.count()) {
					RawFileStruct S;
					S.ADC_index=raw_file_structs.count();
					S.max_num_points=1;
					raw_file_structs << S;
				}
				RawFileStruct *S=&raw_file_structs[ADC_index];
				while (S->last_real_index.count()<real_index.count()) {
					S->effective_current_index << 0;
					S->current_index_override << -1;
					S->effective_dimensions << 1;
					S->last_real_index << -1;
				}
				int last_iterator_index_that_changed=-1;
				for (int ct=0; ct<real_index.count(); ct++) {
					if ((real_index[ct]>S->last_real_index[ct])&&(S->last_real_index[ct]>=0))
						last_iterator_index_that_changed=ct;
				}
				if (last_iterator_index_that_changed>=0) {
					for (int ct=0; ct<last_iterator_index_that_changed; ct++) {
						S->effective_current_index[ct]=0;
						S->last_real_index[ct]=-1;
					}
					S->effective_current_index[last_iterator_index_that_changed]++;
					if (S->effective_current_index[last_iterator_index_that_changed]>=S->effective_dimensions[last_iterator_index_that_changed])
						S->effective_dimensions[last_iterator_index_that_changed]=S->effective_current_index[last_iterator_index_that_changed]+1;
				}
				if (S->max_num_points<record.num_points)
					S->max_num_points=record.num_points;
				S->last_real_index=real_index;
			}
		}
		else if (record.code==10) { //initialize
			int indx=record.iterator_index;
			while (indx>=real_index.count()) 
				real_index << 0;
			real_index[indx]=0;
		}
		else if (record.code==15) { //step
			int indx=record.iterator_index;
			if (indx<real_index.count())
				real_index[indx]++;
		}
		else if (record.code==20) { //reset
			int indx=record.iterator_index;
			if (indx<real_index.count())
				real_index[indx]=0;
		}
		else if (record.code==25) { //set index
			int ADC_index=record.ADC_index;
			int iterator_ind=record.iterator_index;
			int indx=record.index;
			///////////
			if (ADC_index<raw_file_structs.count()) {
				RawFileStruct *S=&raw_file_structs[ADC_index];
				if (iterator_ind<S->effective_dimensions.count()) {
					if (indx>=S->effective_dimensions[iterator_ind])
						S->effective_dimensions[iterator_ind]=indx+1;
				}
			}
		}
	}
}

void allocate_raw_file_arrays(QList<RawFileStruct> &raw_file_structs,int num_channels) {
	for (int j=0; j<raw_file_structs.count(); j++) {	
		RawFileStruct *S=&raw_file_structs[j];
		qint32 hold_dims[MAX_MDA_DIMS];
		int hold_num_dims=1;
		hold_dims[0]=S->max_num_points;
		for (int k=0; k<S->effective_dimensions.count(); k++)
			if (S->effective_dimensions[k]>1) {
				hold_dims[hold_num_dims]=S->effective_dimensions[k];
				hold_num_dims++;
			}
		if (num_channels>1) {
			hold_dims[hold_num_dims]=num_channels;
			hold_num_dims++;
		}
		while (hold_num_dims<2) {
			hold_dims[hold_num_dims]=1;
			hold_num_dims++;
		}
		S->array.allocate(MDA_TYPE_COMPLEX,hold_num_dims,hold_dims);
		S->array.setAll(0);
	}	
}

void define_raw_readout_records(QList<RawReadoutRecord> &raw_readout_records,
									QList<RawFileStruct> &raw_file_structs,
									QList<RawTemplateRecord> &template_records,
									int num_channels,
									QList<long> &real_index) {
	for (int j=0; j<real_index.count(); j++)
		real_index[j]=0;
	for (int j=0; j<raw_file_structs.count(); j++) {
		for (int k=0; k<raw_file_structs[j].last_real_index.count(); k++)
			raw_file_structs[j].last_real_index[k]=-1;
		raw_file_structs[j].effective_current_index=real_index;
		raw_file_structs[j].current_index_override.clear();
		for (int k=0; k<raw_file_structs[j].effective_current_index.count(); k++) {
			raw_file_structs[j].current_index_override << -1;
		}
	}
	foreach (RawTemplateRecord record,template_records) {
		if (record.code==5) { //readout
			int ADC_index=record.ADC_index;
			for (int channel_index=0; channel_index<num_channels; channel_index++) {
				if (ADC_index<0) {
					qWarning() << "ADC_index is negative!";
				}
				else if (ADC_index>=raw_file_structs.count()) {
					qWarning() << "ADC_index out of range! -- should be impossible";
				}
				else {
					RawFileStruct *S=&raw_file_structs[ADC_index];
					
					int last_iterator_index_that_changed=-1;
					for (int ct=0; ct<real_index.count(); ct++) {
						if ((real_index[ct]>S->last_real_index[ct])&&(S->last_real_index[ct]>=0))
							last_iterator_index_that_changed=ct;
					}
					if (last_iterator_index_that_changed>=0) {
						for (int ct=0; ct<last_iterator_index_that_changed; ct++) {
							S->effective_current_index[ct]=0;
							S->last_real_index[ct]=-1;
						}
						S->effective_current_index[last_iterator_index_that_changed]++;
					}					
					
					long index_1d=0;
					long factor=S->max_num_points;
					for (int ji=0; ji<S->effective_dimensions.count(); ji++) {
						if (S->current_index_override[ji]>=0)
							index_1d+=S->current_index_override[ji]*factor;
						else
							index_1d+=S->effective_current_index[ji]*factor;
						S->current_index_override[ji]=-1;
						factor*=S->effective_dimensions[ji];
					}
					index_1d+=channel_index*factor;
					RawReadoutRecord raw_readout_record;
					raw_readout_record.ADC_index=record.ADC_index;
					raw_readout_record.num_points=record.num_points;
					raw_readout_record.array_index=index_1d;
					raw_readout_records << raw_readout_record;
					
					S->last_real_index=real_index;
				}
			}
		}
		else if (record.code==10) { //initialize
			int indx=record.iterator_index;
			while (indx>=real_index.count()) 
				real_index << 0;
			real_index[indx]=0;
		}
		else if (record.code==15) { //step
			int indx=record.iterator_index;
			if (indx<real_index.count())
				real_index[indx]++;
		}
		else if (record.code==20) { //reset
			int indx=record.iterator_index;
			if (indx<real_index.count())
				real_index[indx]=0;
		}
		else if (record.code==25) { //set index
			int ADC_index=record.ADC_index;
			int iterator_ind=record.iterator_index;
			int indx=record.index;
			/////
			if (ADC_index<raw_file_structs.count()) {
				RawFileStruct *S=&raw_file_structs[ADC_index];
				if (record.iterator_index<S->current_index_override.count()) {
					S->current_index_override[iterator_ind]=indx;
				}
			}
		}
	}
}

void distribute_raw_data(DistributeRawDataStruct &X) {	
	//read template records
	QList<RawTemplateRecord> template_records;
	read_template_records(template_records,X.template_fname);
	
	//determine dimensions
	QList<long> real_index;
	QList<RawFileStruct> raw_file_structs;
	determine_raw_file_dimensions(raw_file_structs,real_index,template_records);
	
	//allocate arrays
	allocate_raw_file_arrays(raw_file_structs,X.num_channels);
	
	//define raw readout records
	QList<RawReadoutRecord> raw_readout_records;
	define_raw_readout_records(raw_readout_records,raw_file_structs,template_records,X.num_channels,real_index);
	
	//fill data
	//Open file and skip beginning header
	FILE *dataf=fopen(X.data_fname.toAscii().data(),"rb");
	if (!dataf) return;	
	if (X.raw_data_format==RAW_DATA_FORMAT_SIEMENS_VA) { //read header for VA
		for (int hi=0; hi<32;  hi++) {
			quint8 dummy;
			fread(&dummy,sizeof(quint8),1,dataf);
		}
	}
	else if (X.raw_data_format==RAW_DATA_FORMAT_SIEMENS_VB) { //read header for VB
		quint32 header_size;
		fread(&header_size,sizeof(quint32),1,dataf);
		for (int hi=0; hi<header_size-4;  hi++) {
			quint8 dummy;
			fread(&dummy,sizeof(quint8),1,dataf);
		}
	}
	foreach (RawReadoutRecord raw_readout_record,raw_readout_records) {
		int ADC_index=raw_readout_record.ADC_index;
		int num_points=raw_readout_record.num_points;
		long array_index=raw_readout_record.array_index;
		for (int hi=0; hi<X.header_size; hi++) {
			quint8 dummy;
			fread(&dummy,sizeof(quint8),1,dataf);
		}
		QList<float> readout_real,readout_imag;
		for (int hi=0; hi<num_points; hi++) {
			float hold=0;
			fread(&hold,4,1,dataf); //must be 4 bytes
			readout_real << hold;
			fread(&hold,4,1,dataf); //must be 4 bytes
			readout_imag << hold;
		}
		if (ADC_index<0) {
			qWarning() << "ADC_index is negative!";
		}
		else if (ADC_index>=raw_file_structs.count()) {
			qWarning() << "ADC_index out of range! -- should be impossible";
		}
		else {
			RawFileStruct *S=&raw_file_structs[ADC_index];
			for (int ji=0; ji<readout_real.count(); ji++) {
				if (array_index+ji<S->array.size()) {
					S->array[array_index+ji]=Complex(readout_real[ji],readout_imag[ji]);
				}
				else {
					qWarning() << "Index out of range: " << array_index+ji;
				}
			}
		}
	}

	//write arrays
	for (int j=0; j<raw_file_structs.count(); j++) {
		QString fname=QString("%1/ADC%2.mda").arg(X.output_directory).arg(j);
		#ifdef WIN32
			fname.replace("/","\\");
		#endif
		raw_file_structs[j].array.write(fname.toAscii().data());
	}
}
