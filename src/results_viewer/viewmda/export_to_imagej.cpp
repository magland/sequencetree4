#include "export_to_imagej.h"
#include <QFileDialog>

#include "mda.h"
#include <QStringList>
#include <QProcess>
#include <QSettings>
#include <QDebug>

bool export_to_imageJ(Mda &X, int d1,int d2,int d3) {
	QSettings settings("Magland", "ChainLink");
	QString imageJ_exec = settings.value("imageJ_exec", "").toString();
	if (!QFile(imageJ_exec).exists()) {
		QString default_path="";
		#ifdef WIN32
		default_path="c:\\progra~1\\imageJ\\imageJ.exe";
		#endif
		if (QFile(default_path).exists())
			imageJ_exec=default_path;
		else {
			imageJ_exec=QFileDialog::getOpenFileName(0,
						"Please select the imageJ executable file...",
						default_path,
						QString("*"));
			settings.setValue("imageJ_exec",imageJ_exec);
		}
	}
	#ifdef WIN32
	imageJ_exec.replace("Program Files","progra~1",Qt::CaseInsensitive);
	#endif
	if (!QFile(imageJ_exec).exists()) {
		qWarning() << "Unable to imageJ executable file:" << imageJ_exec;
		return false;
	}
	qDebug()  << "Using " << imageJ_exec << "...";
	
		
	QDir dir(QFileInfo(imageJ_exec).path());
	QStringList datlist=dir.entryList(QStringList("*.ch.dat"));
	for (int i=0; i<datlist.count(); i++)
		QFile::remove(dir.path()+"/"+datlist[i]);

	QString tmp_fname=QString("%1/temporary.ch.dat").arg(QFileInfo(imageJ_exec).path());
	QString macro_fname=QString("%1/macros/ImportChainLink.txt").arg(QFileInfo(imageJ_exec).path());
	
	#ifdef WIN32	
		tmp_fname.replace("/","\\");
		macro_fname.replace("/","\\");
	#endif
	
	qDebug()  << "Writing file: " << tmp_fname << "...";
	FILE *outf=fopen(tmp_fname.toAscii().data(),"wb");
	if (!outf) {
		qWarning() << "Unable to open temporary file in imageJ directory.";
		return false;
	}
	long N1=1,N2=1,N3=1;
	if (d1<X.dimCount()) N1=X.size(d1);
	if (d2<X.dimCount()) N2=X.size(d2);
	if (d3<X.dimCount()) N3=X.size(d3);
	qint32 inds[MAX_MDA_DIMS];
	for (int j=0; j<X.dimCount(); j++)
		inds[j]=0;
	for (long l=0; l<N3; l++) {
		inds[d3]=l;
		for (long k=0; k<N2; k++) {
			inds[d2]=k;
			for (long j=0; j<N1; j++) {
				inds[d1]=j;
				float val=abs(X.get(inds));
				fwrite(&val,sizeof(float),1,outf);
			}
		}
	}
	fclose(outf);

	//tmp_fname2 is same as tmp_fname except with "\" replaced by "\\"
	QString tmp_fname2=tmp_fname.replace("\\","\\\\");


	qDebug()  << "Writing file: " << macro_fname << "...";
	FILE *macroF=fopen(macro_fname.toAscii().data(),"w");
	if (!macroF) {
		qWarning() << "Unable to open file for writing:" << macro_fname;
		return false;
	}
	fprintf(macroF,"run(\"Raw...\", \"open=%s image=[32-bit Real] width=%d height=%d offset=0 number=%d gap=0 little-endian\");\n",tmp_fname2.toAscii().data(),N1,N2,N3);
	fclose(macroF);	
	
	QString exec=imageJ_exec;
	QStringList args; args << "-macro" << "ImportChainLink";
	
	qDebug()  << "Executing " << exec << args;	
	
	QProcess::startDetached(exec,args);
	return true;
}

