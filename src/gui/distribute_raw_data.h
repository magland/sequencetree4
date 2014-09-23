#ifndef distribute_raw_data_H
#define distribute_raw_data_H

#include <QString>
#include "mda.h"
#include <QList>

#define RAW_DATA_FORMAT_SIEMENS_VA 1
#define RAW_DATA_FORMAT_SIEMENS_VB 2

struct DistributeRawDataStruct {
	QString output_directory;
	QString template_fname;
	QString data_fname;
	int num_channels;
	int raw_data_format;
	int header_size;
};

struct RawFileStruct {
	int ADC_index;
	QList<long> effective_dimensions;
	Mda array;
	long max_num_points;
	
	QList<long> effective_current_index;
	QList<long> current_index_override;
	QList<long> last_real_index;
};

struct RawTemplateRecord {
	int code;
	//5: readout -- ADC_index, num_points
	//10: initialize -- iterator_index
	//15: step -- iterator_index
	//20: reset -- iterator_index
	//25: set_index -- ADC_index, iterator_index, index
	int ADC_index;
	int num_points;
	int iterator_index;
	int index;
};

struct RawReadoutRecord {
	int ADC_index;
	int num_points;
	long array_index;
};

void distribute_raw_data(DistributeRawDataStruct &X);

void read_template_records(
					QList<RawTemplateRecord> &ret,
					QString template_fname);
void determine_raw_file_dimensions(
					QList<RawFileStruct> &raw_file_structs,
					QList<long> &real_index,
					QList<RawTemplateRecord> &template_records);
void allocate_raw_file_arrays(
					QList<RawFileStruct> &raw_file_structs,
					int num_channels);
void define_raw_readout_records(
					QList<RawReadoutRecord> &raw_readout_records,
					QList<RawFileStruct> &raw_file_structs,
					QList<RawTemplateRecord> &template_records,
					int num_channels,
					QList<long> &real_index);


#endif
