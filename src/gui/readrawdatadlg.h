#ifndef ReadRawDataDlg_H
#define ReadRawDataDlg_H

#include "ui_readrawdatadlg.h"
#include <QSettings>
#include "stcommon.h"
#include <QDir>
#include <QFileDialog>
#include "distribute_raw_data.h"
#include <QProcess>
#include <QFile>
#include <QDir>

#include "stmetaclass.h"

void erase_all_files_in_directory(QString dirname);

class ReadRawDataDlg : public QDialog {
	Q_OBJECT
private:
	Ui::ReadRawDataDlg ui;
public:
	ReadRawDataDlg(QWidget *parent=0) : QDialog(parent) {
		ui.setupUi(this);
	}
	void setRawDataTemplate(QString fname) {
		ui.raw_data_template->setText(fname);
		QSettings settings("Magland","SequenceTree4");
        //ui.num_channels->setText(settings.value("num_channels","1").toString());
		ui.raw_data->setText(settings.value("raw_data").toString());
		ui.siemens_va_format->setChecked(settings.value("raw_data_format").toString()=="siemens_va");
		ui.siemens_vb_format->setChecked(settings.value("raw_data_format").toString()=="siemens_vb");		
		ui.siemens_vd_format->setChecked(settings.value("raw_data_format").toString()=="siemens_vd");	
		
		connect(ui.browse_raw_data_button,SIGNAL(clicked()),this,SLOT(slot_browse_raw_data()));
		slot_browse_raw_data();
	}
	void accept() {
		QSettings settings("Magland","SequenceTree4");
    //	settings.setValue("num_channels",ui.num_channels->text());
		settings.setValue("raw_data",ui.raw_data->text());
		
		QDir(ST_TMP_DIR).mkdir("data");
		
		QDir(ST_TMP_DIR).mkdir("data");
		QString data_dir=ST_TMP_DIR+"/data";
		erase_all_files_in_directory(data_dir);		
		
		
		DistributeRawDataStruct X;
		X.output_directory=ST_TMP_DIR+"/data";
		X.template_fname=ui.raw_data_template->text();
		X.data_fname=ui.raw_data->text();
        FILE *dataf=fopen(X.data_fname.toLatin1().data(),"rb");
        if (!dataf) return;
        //X.num_channels=ui.num_channels->text().toInt();
        X.num_channels=1;
		if (ui.siemens_va_format->isChecked()) {
			X.header_size=128;
			X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VA;
			settings.setValue("raw_data_format","siemens_va");
            fseek(dataf,30,SEEK_CUR);
            quint16 usedChannel;
            fread(&usedChannel,sizeof(quint16),1,dataf);
            X.num_channels=usedChannel;
		}
		else if (ui.siemens_vb_format->isChecked()) {
			X.header_size=128;
			X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VB;
			settings.setValue("raw_data_format","siemens_vb");
            quint32 header_size;
            fread(&header_size,sizeof(quint32),1,dataf);
            fseek(dataf,header_size-4,SEEK_CUR);
            fseek(dataf,30,SEEK_CUR);
            quint16 usedChannel;
            fread(&usedChannel,sizeof(quint16),1,dataf);
            X.num_channels=usedChannel;
		}
		else if (ui.siemens_vd_format->isChecked()) {
            X.header_size=192;
			X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VD;
			settings.setValue("raw_data_format","siemens_vd");
            fseek(dataf,4,SEEK_SET);
            quint32 nMeas;
            fread(&nMeas,sizeof(quint32),1,dataf);
            quint64 measLen=0;
            for(quint32 i=0;i<nMeas-1;++i) {
                fseek(dataf,16,SEEK_CUR);
                quint64 tmp;
                fread(&tmp,sizeof(quint64),1,dataf);
                measLen+=tmp%512?(tmp/512+1)*512:tmp;
                fseek(dataf,152-24,SEEK_CUR);
            }
            fseek(dataf,2*4+152*64+126*4+measLen,SEEK_SET);
            quint32 header_size;
            fread(&header_size,sizeof(quint32),1,dataf);
            fseek(dataf,header_size-4,SEEK_CUR);
            fseek(dataf,50,SEEK_CUR);
            quint16 usedChannel;
            fread(&usedChannel,sizeof(quint16),1,dataf);
            X.num_channels=usedChannel;
		}
		else {
			X.header_size=128;
			X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VA;
		}
        fclose(dataf);
		//the following was added on 1/6/2014 at request of michael
		QSettings settings0("Magland","SequenceTree4"); 
		settings0.setValue("experiments_folder",QFileInfo(X.data_fname).path());
		
		distribute_raw_data(X);
		
		QDialog::accept();
	}
private slots:
	void slot_browse_raw_data() {
		QString fname=QFileDialog::getOpenFileName(this,"Choose raw data file:",ui.raw_data->text());
		if (!fname.isEmpty())
			ui.raw_data->setText(fname);
	}
	
};

#endif
