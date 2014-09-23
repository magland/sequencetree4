#include "rawdatafiletree.h"
#include "stcontroller.h"
#include <QDebug>

RawDataFileTree::RawDataFileTree(QWidget *parent) : QTreeWidget(parent) 
{
}

RawDataFileTree::~RawDataFileTree()
{
}

void RawDataFileTree::initialize() {
	setColumnCount(5);
	QStringList labels; labels << "File" << "Date" << "Size" << "Sequence" << "# Channels";
	setHeaderLabels(labels);
}

QString file_size_string(long num_bytes) {
	if (num_bytes<10000) {
		return QString("%1 B").arg(num_bytes);
	}
	else if (num_bytes/1000<10000) {
		return QString("%1 KB").arg(num_bytes/1000);
	}
	else {
		return QString("%1 MB").arg(num_bytes/1000000);
	}
}

QString read_text_file(QString fname) {
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
		return "";
	QString ret=file.readAll();
	file.close();
	return ret;
}


QString get_sequence_name(QString raw_fname) {
	QString asc_fname=QFileInfo(raw_fname).path()+"/"+QFileInfo(raw_fname).baseName()+".asc";
	if (QFileInfo(asc_fname).exists()) {
		QString header_text=read_text_file(asc_fname);
		long ind=header_text.indexOf("tProtocolName");
		if (ind<0) return "";
		long ind2=header_text.indexOf("\"",ind);
		if (ind2<0) return "";
		long ind3=header_text.indexOf("\"",ind2+1);
		if (ind3<0) return "";
		return header_text.mid(ind2+1,ind3-ind2-1);
	}
	else {
		QString header_text=extract_siemens_vb_header(raw_fname);
		long ind1=header_text.indexOf("\"SequenceDescription\"");
		if (ind1>=0) {
			long ind2=header_text.indexOf("\"",ind1+22);
			if (ind2>=0) {
				long ind3=header_text.indexOf("\"",ind2+1);
				if (ind3>=0) {
					return header_text.mid(ind2+1,ind3-ind2-1);
				}
			}
		}
	}
	return "";
}

int get_num_channels(QString raw_fname) {
	FILE *inf=fopen(raw_fname.toAscii().data(),"rb");
	if (!inf) return 0;
	quint32 header_size;
	fread(&header_size,sizeof(quint32),1,inf);
	fseek(inf,header_size+30,SEEK_SET);
	quint16 num_channels;
	fread(&num_channels,sizeof(quint16),1,inf);
	fclose(inf);
	return num_channels;
}

