#include "stcontrollermainwindow.h"
#include "stcontrollerconfigdlg.h"
#include <QDateTime>
#include <QTextCharFormat>
#include <QMessageBox>
#include "stcommon.h"
#include <QProcess>
#include <QDebug>
#include <QDir>
#include "../gui/distribute_raw_data.h"

bool write_text_file(const QString & fname,const QString &txt) {
	/*FILE *outf=fopen(fname.toAscii().data(),"w");
	if (!outf) return false;
	for (long j=0; j<txt.count(); j++) {
		char c=txt[j].toAscii();
		fputc(c,outf);
	}
	fclose(outf);
	return true;*/
	QFile file(fname);
	if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
		return false;
	file.write(txt.toAscii());
	file.close();
	return true;
}


void STControllerMainWindow::slot_configuration() {
	STControllerConfigDlg dlg;
	if (dlg.exec()==QDialog::Accepted) 
		update_configuration();
}

void STControllerMainWindow::update_configuration() {
	QSettings settings("Magland","ST4Controller");
	m_controller.setTimeInterval(settings.value("timer_interval",2).toDouble());
}

void STControllerMainWindow::slot_controller_message(QString msg,bool bold) {
	QTextCursor cursor=ui.log_text->textCursor();
	cursor.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
	QTextCharFormat chformat=cursor.charFormat();
	QFont fnt=chformat.font();
	fnt.setBold(bold);
	chformat.setFont(fnt);
	cursor.setCharFormat(chformat);
	cursor.insertText(QDateTime::currentDateTime().toString()+"\t"+msg+"\n");
	ui.log_text->setTextCursor(cursor);
	ui.log_text->ensureCursorVisible();
}

void STControllerMainWindow::slot_sequence_files_changed() {
	ui.sequences->clear();
	QStringList sequence_files=m_controller.sequenceFiles();
	foreach (QString fname,sequence_files) {
		ui.sequences->addSequence(fname);
	}
	ui.sequences->resizeColumnToContents(0);
	ui.sequences->resizeColumnToContents(1);
}


void STControllerMainWindow::slot_raw_data_files_changed() {
	ui.raw_data_files->clear();
	QStringList files=m_controller.rawDataFiles();
	foreach (QString fname,files) {
		ui.raw_data_files->addFile(fname);
	}
	ui.raw_data_files->resizeColumnToContents(0);
	ui.raw_data_files->resizeColumnToContents(1);
	ui.raw_data_files->resizeColumnToContents(2);
}


void STControllerMainWindow::slot_view_header() {
	QString fname=ui.raw_data_files->currentFile();
	if (fname.isEmpty()) return;
	QTextEdit *editor=new QTextEdit(0);
	editor->setAttribute(Qt::WA_DeleteOnClose);
	QString asc_fname=QFileInfo(fname).path()+"/"+QFileInfo(fname).baseName()+".asc";
	if (QFileInfo(asc_fname).exists()) {
		editor->setText(read_text_file(asc_fname));
	}
	else {
		editor->setText(extract_siemens_vb_header(fname));
	}
	editor->showNormal();
}

void erase_all_files_in_directory(QString dirname) {
	QDir dir(dirname);
	if (!dir.exists()) {
		qWarning() << __FUNCTION__ << "directory does not exist:" << dirname;
		return;
	}
	QStringList filelist=dir.entryList(QStringList("*"),QDir::Files|QDir::NoDotAndDotDot);
	foreach (QString fname,filelist) {
		if (!dir.remove(fname)) {
			qWarning() << __FUNCTION__ << "Unable to erase file:" << fname;
		}
	}
}

void STControllerMainWindow::slot_parse() {
	show_message("Parsing raw data file...");
	QString fname=ui.raw_data_files->currentFile();
	if (fname.isEmpty()) return;
	QString sequence_name=get_sequence_name(fname);
	int num_channels=get_num_channels(fname);
	if (sequence_name.isEmpty()) {	
		QMessageBox::information(this,"Cannot find sequence","Cannot find sequence name in header.");
		show_message("");
		return;
	}
	QString seq_bin_dirname=get_seq_bin_dir();
	QString seq_base_fname=seq_bin_dirname+"/"+sequence_name;
	QString sts_fname=seq_base_fname+".sts";
	QString exe_fname=seq_base_fname+".exe";
	QString param_fname=seq_base_fname+".st4parameters";
	if (!QFileInfo(sts_fname).exists()) {	
		QMessageBox::information(this,"Cannot find .sts file","Cannot find file: "+sts_fname);
		show_message("");
		return;
	}	
	if (!QFileInfo(exe_fname).exists()) {	
		QMessageBox::information(this,"Cannot find .exe file","Cannot find file: "+exe_fname);
		show_message("");
		return;
	}	
	if (!QFileInfo(param_fname).exists()) {	
		QMessageBox::information(this,"Cannot find parameters file","Cannot find file: "+param_fname);
		//show_message("");
		//return;
	}		
	#ifdef WIN32
	exe_fname.replace("/","\\");
	sts_fname.replace("/","\\");
	param_fname.replace("/","\\");
	#endif
	QString raw_template_fname=ST_TMP_DIR+"/raw_template.dat";
	{
		QString exec=exe_fname;
		QStringList args; 
		
		args << "raw_template" 
			 << param_fname
			 << ST_TMP_DIR+"/parameters.output.txt"
			 << raw_template_fname
			 << ST_TMP_DIR+"/results.output.txt"
			 << "-1" << "-1";
		#ifdef WIN32
		for (int j=0; j<args.count(); j++)
			args[j].replace("/","\\");
		#endif
			
		qDebug() << "Running: " << exec << args;
		show_message("Generating raw data template...");
		QProcess::execute(exec,args);
	}
	show_message("Parsing raw data file...");
	
	
	
	
	QDir(ST_TMP_DIR).mkdir("data");
	QString data_dir=ST_TMP_DIR+"/data";
	erase_all_files_in_directory(data_dir);
	
	{
		DistributeRawDataStruct X;
		X.data_fname=fname;
		X.template_fname=raw_template_fname;
		X.header_size=128;
		X.num_channels=num_channels;
		X.output_directory=data_dir;
		X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VB;
		distribute_raw_data(X);
		#ifdef WIN32
			QString exec=ST_BIN_DIR+"/results_viewer.exe";
		#else
			QString exec=ST_BIN_DIR+"/results_viewer";
		#endif
		QStringList args; args << data_dir;
		show_message("Parsing raw data file...");
		#ifdef WIN32
			QString batch_fname=ST_TMP_DIR+"/tmp_script.bat";
			QString batch_text;
			if (QDir(ST_ROOT_DIR+"/chainlink_st4").exists()) {
				batch_text+= QString("set CHAINLINK_DIR=%1\n").arg(ST_ROOT_DIR+"/chainlink_st4");
				batch_text+= QString("set PATH=%1;%2;%PATH%\n").arg(ST_ROOT_DIR+"/chainlink_st4/bin").arg(ST_BIN_DIR);
			}
			batch_text+= QString("%1 %2").arg(exec,data_dir);
			batch_text.replace("/","\\");
			write_text_file(batch_fname,batch_text);
			QProcess::startDetached(batch_fname);
		#else
			QProcess::startDetached(exec,args);
		#endif			
	}
	
	QFile::copy(fname,data_dir+"/"+QFileInfo(fname).fileName());
	QFile::copy(sts_fname,data_dir+"/"+QFileInfo(sts_fname).fileName());
	QFile::copy(exe_fname,data_dir+"/"+QFileInfo(exe_fname).fileName());
	QFile::copy(param_fname,data_dir+"/"+QFileInfo(param_fname).fileName());
	show_message("");
}	

void STControllerMainWindow::slot_open_sequence_folder() {
	QProcess::startDetached("explorer",QStringList(get_seq_bin_dir()));
}


void STControllerMainWindow::slot_open_raw_folder() {
	QProcess::startDetached("explorer",QStringList(get_raw_data_dir()));
}

void STControllerMainWindow::show_message(QString msg) {
	statusBar()->showMessage(msg);
	qApp->processEvents();	
}
