#include "exporttoscannerdlg.h"
#include "stcommon.h"
#include <QDir>
#include <QSettings>
#include <QMessageBox>
#include <QProcess>
#include "readrawdatadlg.h"



void ExportToScannerDlg::setSequence(STMetaSequence *S,QString executable_fname) {
	m_sequence=S;
	m_executable_fname=executable_fname;
	initialize_controls();
}

void ExportToScannerDlg::initialize_controls() {
	QString recommended_sequence_name="";
	if (m_sequence) {
		recommended_sequence_name=QFileInfo(m_sequence->fileName()).baseName();
		recommended_sequence_name.remove(" ");
	}
	ui.sequence_name->setText(recommended_sequence_name);
	QString default_export_directory=QFileInfo(ST_TMP_DIR).canonicalFilePath();
	QSettings settings("Magland","SequenceTree4");
	ui.export_directory->setText(settings.value("export_directory",default_export_directory).toString());
	ui.scanner->clear();
	QString export_dir=export_directory();
	QDir dir(export_dir);
	QStringList folderlist=dir.entryList(QStringList("*"),QDir::Dirs|QDir::NoDotAndDotDot);
	folderlist.removeAll("siemens_common");
	foreach (QString folder,folderlist) {
		ui.scanner->addItem(folder,folder);
	}
	int current_index=folderlist.indexOf(settings.value("export_scanner").toString());
	if (current_index>=0)
		ui.scanner->setCurrentIndex(current_index);
}

QString ExportToScannerDlg::export_directory() {
	return ST_EXPORT_DIR;
}


void ExportToScannerDlg::accept() {
	QSettings settings("Magland","SequenceTree4");
	
	
	if (ui.sequence_name->text().isEmpty()) {
		QMessageBox::information(this,"Cannot export","Cannot export: you must specify a sequence name");
		return;
	}
	
	QString scanner_name=ui.scanner->currentText();
	if (scanner_name.indexOf("siemens_")==0) {
		if (!export_to_siemens(m_sequence,scanner_name,ui.export_directory->text()+"/"+ui.sequence_name->text(),m_executable_fname)) {
			QMessageBox::information(this,"Cannot export","Cannot export: an error occurred during export.");
			return;
		}
	}
	
	settings.setValue("export_directory",ui.export_directory->text());	
	settings.setValue("export_scanner",ui.scanner->currentText());
	
	QDialog::accept();
}

bool copy_and_overwrite(QString srcfile,QString dstfile) {
	QFile::remove(dstfile);
	return QFile::copy(srcfile,dstfile);
}

bool files_are_identical(QString fname1,QString fname2) {
	QFile F1(fname1);
	QFile F2(fname2);
	if (!F1.open(QIODevice::ReadOnly)) {
		return false;
	}
	if (!F2.open(QIODevice::ReadOnly)) {
		F1.close();
		return false;
	}
	QByteArray contents1=F1.readAll();
	QByteArray contents2=F2.readAll();
	F1.close();
	F2.close();
	return (contents1==contents2);
}

void touch_file(QString fname) {
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly)) 
		return;
	QByteArray txt=file.readAll();
	file.close();
	if (!file.open(QIODevice::WriteOnly)) 
		return;
	file.write(txt);
	file.close();
}

bool copy_all_files_between_directories(QString src_dirname,QString dst_dirname,bool only_copy_if_needed,bool touch_files) {
	QDir src_dir(src_dirname);
	QDir dst_dir(dst_dirname);
	QStringList list=src_dir.entryList(QStringList("*"),QDir::Files|QDir::NoDotAndDotDot);
	foreach (QString fname,list) {
		QString srcfile=src_dirname+"/"+fname;
		QString dstfile=dst_dirname+"/"+fname;
		if ((only_copy_if_needed)&&(files_are_identical(srcfile,dstfile))) {
			qDebug()  << "No changes detected in file: " << srcfile;
		}
		else {
			if (only_copy_if_needed) {
				qDebug()  << "Changes detected in file: " << srcfile;
			}
			if (!copy_and_overwrite(srcfile,dstfile)) {
				qWarning() << "Cannot copy file:" << fname;
				return false;
			}
			else {
				if (touch_files) {
					touch_file(dstfile);
				}
			}
		}
	}
	return true;
}

void replace_string_in_file(QString fname,char *str1,char *str2) {

	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly)) 
		return;
	QByteArray txt=file.readAll();
	file.close();
	if (txt.contains(str1)) {
		txt.replace(str1,str2);
		if (!file.open(QIODevice::WriteOnly)) 
			return;
		file.write(txt);
		file.close();
	}
}

void replace_string_in_files(QString dirname,QString before,QString after) {
	QDir dir(dirname);
	QStringList list=dir.entryList(QStringList("*"),QDir::Files|QDir::NoDotAndDotDot);
	foreach (QString fname,list) {
		replace_string_in_file(dirname+"/"+fname,before.toAscii().data(),after.toAscii().data());
	}
}

void replace_string_in_file_names(QString dirname,QString before,QString after) {
	QDir dir(dirname);
	QStringList list=dir.entryList(QStringList("*"),QDir::Files|QDir::NoDotAndDotDot);
	foreach (QString str,list) {
		QString fname=dirname+"/"+str;
		if (fname.contains(before)) {
			QString fname2=fname;
			fname2.replace(before,after);
			if (copy_and_overwrite(fname,fname2))
				if (fname!=fname2)
					QFile::remove(fname);
		}
	}
}

void insert_snippet_in_file(QString fname,QString keyword,QString code) {
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly)) 
		return;
	QString txt=file.readAll();
	file.close();
	if (txt.contains("CUSTOM-CODE-"+keyword)) {
		QString txt2;
		QStringList lines=txt.split("\n");
		foreach (QString line,lines) {
			txt2+=line+"\n";
			if (line.contains("CUSTOM-CODE-"+keyword)) {
				txt2+=code;
			}
		}
		if (!file.open(QIODevice::WriteOnly)) 
			return;
		file.write(txt2.toAscii());
		file.close();
	}
}

void insert_snippet_in_files(QString dirname,QString keyword,QString code) {
	QDir dir(dirname);
	QStringList list=dir.entryList(QStringList("*"),QDir::Files|QDir::NoDotAndDotDot);
	foreach (QString fname,list) {
		insert_snippet_in_file(dirname+"/"+fname,keyword,code);
	}
}

struct CustomCodeSnippet {
	QString keyword;
	QString code;
};

QList<CustomCodeSnippet> get_custom_code_snippets(STMetaSequence *Seq) {
	QList<CustomCodeSnippet> ret;
	QString notes=Seq->notes();
	QStringList lines=notes.split("\n");
	for (int j=0; j<lines.count(); j++) {
		if (lines[j].contains("CUSTOM-CODE-")) {
			int ind=lines[j].indexOf("CUSTOM-CODE-");
			int ind2=lines[j].indexOf(" ",ind);
			int ind3=lines[j].indexOf("=",ind);
			if ((ind3>=0)&&((ind3<ind2)||(ind2<0))) ind2=ind3;
			if (ind2>=0) {
				QString keyword=lines[j].mid(ind+12,ind2-ind-12);
				QString code;
				int k=j+1;
				bool done=false;
				while (!done) {
					if (k>=lines.count()) done=true;
					else if (lines[k].mid(0,1)=="=") done=true;
					else {
						code+=lines[k]+"\n";
						k++;
					}
				}
				if ((!keyword.isEmpty())&&(!code.isEmpty())) {
					CustomCodeSnippet S;
					S.code=code;
					S.keyword=keyword;
					ret << S;
				}
			}
		}
	}
	return ret;
}

bool export_to_siemens(STMetaSequence *Seq,QString scanner_version,QString sequence_directory,QString executable_fname) {
	/*QString exec=ST_BIN_DIR+"/export_to_siemens.exe";
	if (!QFileInfo(exec).exists()) {	
		QMessageBox::information(0,"Cannot export",QString("Cannot export: file does not exist: %1").arg(exec));
		return false;
	}*/	
	
	if (sequence_directory.contains(" ")) {
		QMessageBox::information(0,"Export to Scanner","Please choose a sequence name with no spaces.");
		return false;
	}
	
	if (QFileInfo(sequence_directory).exists()) {	
		if (QMessageBox::question(0,"Overwrite sequence?","A folder of this name already exists. Overwrite?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes) {
			/*QDir seq_dir(sequence_directory);
			QStringList fnames=seq_dir.entryList(QStringList("*"),QDir::Files);
			foreach (QString fname,fnames) {
				if (!QFile::remove(sequence_directory+"/"+fname)) {
					QMessageBox::information(0,"Cannot export",QString("Cannot export: unable to remove file: %1").arg(fname));
					return false;
				}
			}*/
		}
		else return false;
	}	
	QDir seq_parent_dir=QFileInfo(sequence_directory).path();
	QString sequence_name=QFileInfo(sequence_directory).fileName();
	seq_parent_dir.mkdir(sequence_name);
	if (!QFileInfo(sequence_directory).exists()) {
		QMessageBox::information(0,"Cannot export",QString("Cannot export: unable to create folder %1").arg(sequence_directory));
		return false;
	}
	
	
	Seq->writeFoundationClassesHeader(ST_SIMULATOR_DIR+"/foundationclasses.h");
	Seq->writeFoundationClassesSource(ST_SIMULATOR_DIR+"/foundationclasses.cpp");
	Seq->writeCustomClassesHeader(ST_SIMULATOR_DIR+"/customclasses.h");
	Seq->writeCustomClassesSource(ST_SIMULATOR_DIR+"/customclasses.cpp");
	Seq->writeCreateSequenceSource(ST_SIMULATOR_DIR+"/createsequence.cpp",sequence_name);
	Seq->writeLoadParametersFileSource(ST_SIMULATOR_DIR+"/loadparametersfile.cpp");
	
	QString seqname=QFileInfo(sequence_directory).baseName();
	Seq->write(ST_SIMULATOR_DIR+"/"+seqname+".sts");
	
	//QStringList args; args << sequence_directory << ST_ROOT_DIR << scanner_version;
	//QProcess::execute(exec,args);
	
	QString simdir=ST_SIMULATOR_DIR;
	QString frameworkdir=ST_ROOT_DIR+"/code/framework";
	QString scanner_directory=ST_ROOT_DIR+"/export/"+scanner_version;
	QString scanner_common_directory=ST_ROOT_DIR+"/export/siemens_common";
	
	QString tmp_sequence_dirname=ST_TMP_DIR+"/tmp_sequence_export";
	QDir(ST_TMP_DIR).mkdir("tmp_sequence_export");
	erase_all_files_in_directory(tmp_sequence_dirname);

	if (!QFile::copy(simdir+"/foundationclasses.h",tmp_sequence_dirname+"/foundationclasses.h")) {
		qWarning() << "Cannot copy file:" << simdir+"/foundationclasses.h" << "to" << tmp_sequence_dirname+"/foundationclasses.h";
		return false;
	}
	if (!QFile::copy(simdir+"/foundationclasses.cpp",tmp_sequence_dirname+"/foundationclasses.cpp")) {
		qWarning() << "Cannot copy file:" << simdir+"/foundationclasses.cpp" << "to" << tmp_sequence_dirname+"/foundationclasses.cpp";
		return false;
	}
	if (!QFile::copy(simdir+"/customclasses.h",tmp_sequence_dirname+"/customclasses.h")) {
		qWarning() << "Cannot copy file:" << simdir+"/customclasses.h" << "to" << tmp_sequence_dirname+"/customclasses.h";
		return false;
	}
	if (!QFile::copy(simdir+"/customclasses.cpp",tmp_sequence_dirname+"/customclasses.cpp")) {
		qWarning() << "Cannot copy file:" << simdir+"/customclasses.cpp" << "to" << tmp_sequence_dirname+"/customclasses.cpp";
		return false;
	}
	if (!QFile::copy(simdir+"/createsequence.h",tmp_sequence_dirname+"/createsequence.h")) {
		qWarning() << "Cannot copy file...";
		return false;
	}
	if (!QFile::copy(simdir+"/createsequence.cpp",tmp_sequence_dirname+"/createsequence.cpp")) {
		qWarning() << "Cannot copy file...";
		return false;
	}
	if (!QFile::copy(simdir+"/loadparametersfile.h",tmp_sequence_dirname+"/loadparametersfile.h")) {
		qWarning() << "Cannot copy file...";
		return false;
	}
	if (!QFile::copy(simdir+"/loadparametersfile.cpp",tmp_sequence_dirname+"/loadparametersfile.cpp")) {
		qWarning() << "Cannot copy file...";
		return false;
	}
	
	if (!copy_all_files_between_directories(frameworkdir,tmp_sequence_dirname,false,false)) return false;
	if (!copy_all_files_between_directories(scanner_directory,tmp_sequence_dirname,false,false)) return false;
	if (!copy_all_files_between_directories(scanner_common_directory,tmp_sequence_dirname,false,false)) return false;
	
	if (!copy_and_overwrite(simdir+"/"+seqname+".sts",tmp_sequence_dirname+"/"+seqname+".sts")) {
		qWarning() << "Problem copying file" << simdir+"/"+seqname+".sts" << "to" << tmp_sequence_dirname+"/"+seqname+".sts";
		return false;
	}
	replace_string_in_files(tmp_sequence_dirname,"$SequenceName$",seqname);
	replace_string_in_file_names(tmp_sequence_dirname,"$SequenceName$",seqname);
	QList<CustomCodeSnippet> snippets=get_custom_code_snippets(Seq);
	for (int j=0; j<snippets.count(); j++) {
		insert_snippet_in_files(tmp_sequence_dirname,snippets[j].keyword,snippets[j].code);
	}
	
	
	if (!copy_all_files_between_directories(tmp_sequence_dirname,sequence_directory,true,true)) return false;
	
	//do this to ensure that st4 framework is always recompiled (because st4.cpp includes other sources)
	qDebug()  << "Copying files from" << frameworkdir << "to" << sequence_directory;
	if (!copy_all_files_between_directories(frameworkdir,sequence_directory,false,true)) return false;
	
	qDebug()  << "Copying" << executable_fname;
	QFile::remove(sequence_directory+"/"+sequence_name+".exe");
	QFile::copy(executable_fname,sequence_directory+"/"+sequence_name+".exe");
	QFile::remove(sequence_directory+"/"+sequence_name+".exe.manifest");
	QFile::copy(executable_fname+".manifest",sequence_directory+"/"+sequence_name+".exe.manifest");
	
	return true;	
}
