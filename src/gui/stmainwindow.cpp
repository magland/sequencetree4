/****************************************************************************
 This file is part of SequenceTree
 Copyright (C) 2005-2008 Jeremy Magland (Jeremy.Magland@gmail.com)

 SequenceTree is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 SequenceTree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SequenceTree; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/


#include "stmainwindow.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include "stpastedialog.h"
#include "stconfigurationdlg.h"
#include <QDesktopServices>
#include <QInputDialog>
#include <QUrl>
#include "exporttoscannerdlg.h"
#include "openfilefromwebdlg.h"
#include "httpdownloader.h"
#include "stcommon.h"
//#include "update_code_from_web.h"
#define MSG_DELAY 3000
#include "readrawdatadlg.h"

STMainWindow::STMainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	
	ui.tabWidget->clear();
	
	ui.actionExportToScanner->setEnabled(exports_are_enabled());
	
	//ui.actionOpenSequenceFromWeb->setEnabled(false);
	
	connect(ui.actionNewSequence,SIGNAL(triggered()),this,SLOT(slot_new_sequence()));
	connect(ui.actionOpenSequence,SIGNAL(triggered()),this,SLOT(slot_open_sequence()));
	//connect(ui.actionOpenSequenceFromWeb,SIGNAL(triggered()),this,SLOT(slot_open_sequence_from_web()));
	connect(ui.actionCloseSequence,SIGNAL(triggered()),this,SLOT(slot_close_sequence()));
	connect(ui.actionSaveSequence,SIGNAL(triggered()),this,SLOT(slot_save_sequence()));
	connect(ui.actionSaveSequenceAs,SIGNAL(triggered()),this,SLOT(slot_save_sequence_as()));
	connect(ui.actionUploadSequenceToWeb,SIGNAL(triggered()),this,SLOT(slot_upload_sequence_to_web()));
	
	connect(ui.actionCopyNode,SIGNAL(triggered()),this,SLOT(slot_copy_node()));
	connect(ui.actionPasteNode,SIGNAL(triggered()),this,SLOT(slot_paste_node()));
	
	connect(ui.actionCompile,SIGNAL(triggered()),this,SLOT(slot_compile()));
	
	connect(ui.actionAddResource,SIGNAL(triggered()),this,SLOT(slot_add_resource()));
	connect(ui.actionImportRFPulseWaveform,SIGNAL(triggered()),this,SLOT(slot_import_rf_pulse_waveform()));
	
	connect(ui.actionConfiguration,SIGNAL(triggered()),this,SLOT(slot_configuration()));
	connect(ui.actionExportToScanner,SIGNAL(triggered()),this,SLOT(slot_export_to_scanner()));
	connect(ui.actionUpdateExportToScanner,SIGNAL(triggered()),this,SLOT(slot_update_export_to_scanner()));
	connect(ui.actionExportToVirtualScanner,SIGNAL(triggered()),this,SLOT(slot_export_to_virtual_scanner()));
	connect(ui.actionReadRawData,SIGNAL(triggered()),this,SLOT(slot_read_raw_data()));
	connect(ui.actionCreateParameterFile,SIGNAL(triggered()),this,SLOT(slot_create_parameter_file()));
	//connect(ui.actionUpdateCodeFromWeb,SIGNAL(triggered()),this,SLOT(slot_update_code_from_web()));
	ui.actionUpdateCodeFromWeb->setEnabled(false);
	
	connect(ui.actionChainLink,SIGNAL(triggered()),this,SLOT(slot_chainlink()));
	connect(ui.actionRTST,SIGNAL(triggered()),this,SLOT(slot_rtst()));
	connect(ui.actionMRPulseTool,SIGNAL(triggered()),this,SLOT(slot_mr_pulse_tool()));
	
	connect(ui.actionPreferences,SIGNAL(triggered()),this,SLOT(slot_preferences()));
	
	connect(ui.actionAboutSequenceTree,SIGNAL(triggered()),this,SLOT(slot_about_sequencetree()));
	connect(ui.actionSequenceTreeWiki,SIGNAL(triggered()),this,SLOT(slot_sequencetree_wiki()));
	
	connect(ui.tabWidget,SIGNAL(currentChanged(int)),this,SLOT(slot_current_changed()));
	
	#ifndef QT_WEB_KIT
	ui.actionOpenSequenceFromWeb->setEnabled(false);
	#endif
	
	ui.actionNewSequence->setShortcut(tr("Ctrl+N"));
	ui.actionOpenSequence->setShortcut(tr("Ctrl+O"));
	ui.actionSaveSequence->setShortcut(tr("Ctrl+S"));
	
	ui.actionCopyNode->setShortcut(tr("Ctrl+C"));
	ui.actionPasteNode->setShortcut(tr("Ctrl+V"));
	
	ui.actionCompile->setShortcut(tr("F9"));
	
	QToolButton *close_button=new QToolButton;
	close_button->setIcon(QIcon(":/images/cross.png"));
	connect(close_button,SIGNAL(clicked()),this,SLOT(slot_close_sequence()));
	ui.tabWidget->setCornerWidget(close_button);
	
	update_recent_file_menu();
	update_actions();
	
}

void STMainWindow::openSequence(QString sequence_fname) {
	if (sequenceView(sequence_fname)) {
		ui.tabWidget->setCurrentWidget(sequenceView(sequence_fname));
	}
	else {
		STSequenceView *SV=new STSequenceView;
		connect_sequence_view(SV);
		STMetaSequence *Seq=new STMetaSequence;
		Seq->read(sequence_fname);
		SV->setSequence(Seq);
		SV->setFileName(sequence_fname);
		Seq->setFileName(sequence_fname);	
		ui.tabWidget->addTab(SV,"");
		ui.tabWidget->setCurrentWidget(SV);
		update_tab_labels();
		update_window_title();
		add_to_recent_files(sequence_fname);
		qApp->processEvents();
		SV->setPreferredSizes();
	}
	update_actions();
}

void STMainWindow::openSequenceFromText(const QString fname, const QString &txt) {
	if (sequenceView(fname)) {
		ui.tabWidget->setCurrentWidget(sequenceView(fname));
	}
	else {
		STSequenceView *SV=new STSequenceView;
		connect_sequence_view(SV);
		STMetaSequence *Seq=new STMetaSequence;
		QStringList text_lines=txt.split('\n');
		long index=0;
		Seq->loadFromText(text_lines,index);
		SV->setSequence(Seq);
		SV->setFileName(fname);
		Seq->setFileName(fname);	
		ui.tabWidget->addTab(SV,"");
		ui.tabWidget->setCurrentWidget(SV);
		update_tab_labels();
		update_window_title();
		//add_to_recent_files(sequence_fname);
		qApp->processEvents();
		SV->setPreferredSizes();
	}
	update_actions();
}


STSequenceView *STMainWindow::sequenceView(QString fname) {
	for (int j=0; j<sequenceViewCount(); j++)
		if (sequenceView(j)->fileName()==fname)
			return sequenceView(j);
	return 0;
}

void STMainWindow::slot_new_sequence() {
	STSequenceView *SV=new STSequenceView;
	connect_sequence_view(SV);
	STMetaSequence *Seq=new STMetaSequence;
	STMetaClass *root_class=new STMetaClass;
	root_class->setClassName("STRoot");
	root_class->setCode(read_text_file(ST_TEMPLATES_DIR+"/stroot.h"),read_text_file(ST_TEMPLATES_DIR+"/stroot.cpp"));
	Seq->addMetaClass(root_class);
	Seq->loadFoundationClasses(ST_FOUNDATION_DIR,false);
	Seq->loadFoundationClasses(ST_USER_DIR,true);
	SV->setSequence(Seq);
	ui.tabWidget->addTab(SV,"[untitled]");
	ui.tabWidget->setCurrentWidget(SV);
	update_window_title();
	qApp->processEvents();
	SV->setPreferredSizes();
	update_actions();
}
void STMainWindow::slot_open_sequence() {
	QSettings settings("Magland","SequenceTree4");
	QString holddir=settings.value("sequence_directory").toString();
	QString fname=QFileDialog::getOpenFileName(this,"Open sequence",holddir,"*.sts");
	if (fname.isEmpty()) return;
	settings.setValue("sequence_directory",QFileInfo(fname).path());
	statusBar()->showMessage(tr("Opening sequence: %1").arg(fname),MSG_DELAY);
	openSequence(fname);
	statusBar()->showMessage(tr("Sequence opened: %1").arg(fname),MSG_DELAY);
	update_actions();
}

bool STMainWindow::slot_close_sequence() {
	if (!currentSequenceView()) return false;
	QString fname=QFileInfo(currentSequenceView()->fileName()).fileName();
	if (currentSequenceView()->isModified()) {
		if (fname.isEmpty()) fname="[untitled]";
		QMessageBox::StandardButton ret=QMessageBox::question(this,"Save changes?",tr("Save changes to %1?").arg(fname),
				QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
		if (ret==QMessageBox::Save) {
			slot_save_sequence();
			if (!currentSequenceView()->isModified()) {
				return slot_close_sequence();
			}
		}
		else if (ret==QMessageBox::Cancel) {
			return false;
		}
	}
	statusBar()->showMessage(tr("Closing sequence: %1").arg(fname),MSG_DELAY);
	delete currentSequenceView();
	statusBar()->showMessage(tr("Sequence closed: %1").arg(fname),MSG_DELAY);
	update_window_title();
	update_actions();
	return true;
}
void STMainWindow::slot_save_sequence() {
	if (!currentSequence()) return;
	QString fname=currentSequenceView()->fileName();
	if ((fname.isEmpty())||(fname.mid(0,4)=="web:")) {
		slot_save_sequence_as();
		return;
	}	
	statusBar()->showMessage(tr("Saving sequence: %1").arg(currentSequenceView()->fileName()),MSG_DELAY);
	currentSequenceView()->updateBeforeSave();
	currentSequence()->write(currentSequenceView()->fileName());
	statusBar()->showMessage(tr("Sequence saved: %1").arg(currentSequenceView()->fileName()),MSG_DELAY);
	currentSequenceView()->setModified(false);
	update_actions();
}
void STMainWindow::slot_save_sequence_as() {
	if (!currentSequence()) return;
	QSettings settings("Magland","SequenceTree4");
	QString holddir=settings.value("sequence_directory").toString();
	QString fname=QFileDialog::getSaveFileName(this,"Save sequence as",holddir,"*.sts");
	if (fname.isEmpty()) return;
	settings.setValue("sequence_directory",QFileInfo(fname).path());
	currentSequenceView()->setFileName(fname);
	currentSequence()->setFileName(fname);
	statusBar()->showMessage(tr("Saving sequence: %1").arg(fname),MSG_DELAY);
	currentSequenceView()->updateBeforeSave();
	currentSequence()->write(fname);
	statusBar()->showMessage(tr("Sequence saved: %1").arg(fname),MSG_DELAY);
	update_tab_labels();
	currentSequenceView()->setModified(false);
	add_to_recent_files(fname);
	update_actions();
}

void STMainWindow::slot_open_recent_file() {
	QString fname=sender()->property("recent_file_name").toString();
	if (fname.isEmpty()) return;
	statusBar()->showMessage(tr("Opening sequence: %1").arg(fname),MSG_DELAY);
	openSequence(fname);
	statusBar()->showMessage(tr("Sequence opened: %1").arg(fname),MSG_DELAY);
	update_actions();
}

void STMainWindow::slot_compile() {
	if (!currentSequenceView()) return;
	currentSequenceView()->clearCompileLog();
	statusBar()->showMessage(tr("Compiling sequence..."),MSG_DELAY);
	currentSequenceView()->slot_compile();
	update_actions();
}

void STMainWindow::update_tab_labels() {
	for (int j=0; j<sequenceViewCount(); j++) {
		QString str=sequenceView(j)->fileName();
		if (str.mid(0,4)!="web:") {
			str=QFileInfo(str).fileName();
		}
		if (str.isEmpty()) str="[untitled]";
		ui.tabWidget->setTabText(j,str);
	}
}

void STMainWindow::connect_sequence_view(STSequenceView *SV) {
	connect(SV,SIGNAL(modificationChanged()),this,SLOT(slot_modification_changed()));
	connect(SV,SIGNAL(compiledChanged()),this,SLOT(slot_compiled_changed()));
	connect(SV,SIGNAL(statusMessage(QString)),this,SLOT(slot_status_message(QString)));
	connect(SV,SIGNAL(runMRPulseTool(QString)),this,SLOT(slot_run_mr_pulse_tool(QString)));
	SV->updatePreferences();
}

void STMainWindow::slot_modification_changed() {
	update_window_title();
	update_actions();
}
void STMainWindow::slot_compiled_changed() {
	update_actions();
}


void STMainWindow::update_window_title() {
	QString txt;
	if (currentSequenceView()) {
		txt="SequenceTree v4 - "+currentSequenceView()->fileName();
		if (currentSequenceView()->isModified())
			txt+=" *";
	}
	else {
		txt="SequenceTree v4";
	}
	setWindowTitle(txt);
		
}

void STMainWindow::slot_current_changed() {
	update_window_title();
	update_actions();
}

void STMainWindow::closeEvent(QCloseEvent *event) {
	while (currentSequenceView()) {
		if (!slot_close_sequence()) {
			event->setAccepted(false);
			return;
		}
	}
}

void STMainWindow::update_recent_file_menu() {
	QList<QAction *> actionlist=ui.menuFile->actions();
	foreach (QAction *A,actionlist) {
		if (!A->property("recent_file_name").toString().isEmpty()) {
			ui.menuFile->removeAction(A);
			delete A;
		}
	}
	ui.menuFile->addSeparator();
	
	QSettings settings("Magland","SequenceTree4");
	QStringList recent_files=settings.value("recent_files").toStringList();
	foreach (QString fname,recent_files) {
		QAction *newact=new QAction(fname,this);
		newact->setProperty("recent_file_name",fname);
		connect(newact,SIGNAL(triggered()),this,SLOT(slot_open_recent_file()));
		ui.menuFile->addAction(newact);
	}
}

void STMainWindow::add_to_recent_files(QString fname) {
	static int max_recent_files=12;
	QSettings settings("Magland","SequenceTree4");
	QStringList recent_files=settings.value("recent_files").toStringList();
	recent_files.removeAll(fname);
	recent_files.insert(0,fname);
	while (recent_files.count()>max_recent_files)
		recent_files.removeAt(recent_files.count()-1);
	settings.setValue("recent_files",recent_files);
	update_recent_file_menu();
}

void STMainWindow::slot_status_message(QString msg) {
	statusBar()->showMessage(msg);
}

void STMainWindow::slot_copy_node() {
	STMetaSequence *S=currentSequence();
	STSequenceView *SV=currentSequenceView();
	if (!SV) return; if (!S) return;
	STMetaNode *N=SV->currentNode();
	if (!N) return;
	m_clipboard_node.copyFrom(*N);
}
void STMainWindow::slot_paste_node() {
	STMetaSequence *S=currentSequence();
	STSequenceView *SV=currentSequenceView();
	if (!SV) return; if (!S) return;
	STMetaNode *N=SV->currentNode();
	if (!N) return;
	
	STPasteDialog dlg;
	if (dlg.exec()==QDialog::Accepted) {
		if (dlg.pasteOption()==ST_PASTE_OPTION_VALUES) {
			N->pasteValuesFrom(m_clipboard_node);
			SV->updateAllViews(false);
		}
		else if (dlg.pasteOption()==ST_PASTE_OPTION_STRUCTURE) {
			if (N->className()!=m_clipboard_node.className()) {
				QMessageBox::information(this,"Cannot copy node","Cannot copy structure, inconsistent node types. You must first change the node type of the destination node. This may require customizing the parent node.");
			}
			else {
				N->pasteStructureFrom(m_clipboard_node);
				SV->updateAllViews(true);
			}
		}
		
	}
	
}

void STMainWindow::slot_configuration() {
	STConfigurationDlg dlg;
	dlg.exec();	
}

void STMainWindow::slot_export_to_scanner() {
	if (!currentSequence()) return;
	if (!currentSequenceView()->isCompiled()) {
		QMessageBox::information(this,"Cannot export","Cannot export sequence. You must compile the sequence first.");
		return;
	}
	ExportToScannerDlg dlg;
	dlg.setSequence(currentSequence(),currentSequenceView()->executableFileName());
	dlg.exec();
}
void STMainWindow::slot_update_export_to_scanner() {
	QDesktopServices::openUrl(QUrl("http://www.thesouthpoles.com/sequencetree/download_export_module.php"));
}
void STMainWindow::slot_export_to_virtual_scanner() {
	if (!currentSequence()) return;
	if (currentSequenceView()->isModified()) {
		QMessageBox::information(this,"Save changes first...","Please save changes to your sequence before exporting to the virtual scanner.");
		return;
	}
	QString fname=currentSequence()->fileName();
	if (fname.mid(0,4)=="web:") {
		QMessageBox::information(this,"Save to disk first...","Please save sequence to disk before exporting to the virtual scanner.");
		return;
	}
	QString exec=ST_BIN_DIR+"/VS";
	QStringList args; args << fname;
	QProcess::startDetached(exec,args);
}

void STMainWindow::slot_read_raw_data() {
	if (!currentSequenceView()) return;
	if (!currentSequence()) return;
	if (!currentSequenceView()->isCompiled()) {
		QMessageBox::information(this,"Cannot read raw data","Cannot read raw data. You must compile the sequence first.");
		return;
	}
	QString raw_template_fname=ST_TMP_DIR+QString("/raw_template_sts.dat");
	currentSequenceView()->writeRawTemplateFile(raw_template_fname);	
	ReadRawDataDlg dlg;
	dlg.setRawDataTemplate(raw_template_fname);
	if (dlg.exec()==QDialog::Accepted) {
		STMetaSequence *Seq=currentSequence();
		QString data_dir=ST_TMP_DIR+"/data";
		Seq->write(data_dir+"/"+QFileInfo(Seq->fileName()).fileName());
		write_text_file(data_dir+"/reconstruction.ch",Seq->reconstructionCode());		
		
		#ifdef WIN32
		QString exec=ST_BIN_DIR+"/results_viewer.exe";
		#else
		QString exec=ST_BIN_DIR+"/results_viewer";
		#endif
		QStringList args; args << data_dir;
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
	
	
}

/*void STMainWindow::slot_update_code_from_web() {
	bool static busy=false;
	if (busy) return;
	busy=true;
	QMessageBox::information(this,"Updating code from web...","Updating code from web. See console window for status on the update.");
	qDebug()  << "Updating code...";	
	QDir(ST_ROOT_DIR).mkdir("code");
	//update_code_from_web();
	qDebug()  << "Done updating.";
	busy=false;
}*/

#ifdef QT_WEB_KIT
#include "staboutbox.h"
void STMainWindow::slot_about_sequencetree() {
	STAboutBox dlg;
	dlg.exec();
}
#else
void STMainWindow::slot_about_sequencetree() {
	QMessageBox::about(this,"About SequenceTree",QString("SequenceTree %1\nJeremy.Magland@gmail.com").arg(get_ST_version()));
}
#endif
void STMainWindow::slot_sequencetree_wiki() {
	QString url="http://www.thesouthpoles.com/sequencetree/wiki";
	QDesktopServices::openUrl(QUrl(url));
}

bool exports_are_enabled() {
	QString fname=ST_EXPORT_DIR+"/_exports_enabled";
	return (QFileInfo(fname).exists());
}

/*void STMainWindow::slot_open_sequence_from_web() {
	#ifdef QT_WEB_KIT
	OpenFileFromWebDlg dlg(this);
	dlg.setWindowTitle("Open sequence from web...");
	dlg.setUrl("http://thesouthpoles.com/sequencetree/wiki/index.php?title=Pulse_Sequences");
	dlg.setFileExtension(".sts");
	if (dlg.exec()==QDialog::Accepted) {
		QString file_text=dlg.fileText();
		QString file_name=dlg.fileName();
		if (file_text.isEmpty()) return;
		openSequenceFromText("web: "+file_name,file_text);		
	}
	#endif
}*/

void STMainWindow::slot_upload_sequence_to_web() {
	QSettings settings("Magland","SequenceTree4");
	QString default_val=settings.value("username").toString();
	QString username=QInputDialog::getText(this,"Upload sequence","Please select a user name:",QLineEdit::Normal,default_val);
	if (username.isEmpty()) return;
	settings.setValue("username",username);
	QDesktopServices::openUrl(QUrl(QString("http://www.thesouthpoles.com/sequencetree/upload_sts.php?username=")+username));
}

QString chainlink_dir() {
	QString ret="";
	if (QDir(ST_ROOT_DIR+"/chainlink_st4").exists())
		ret=ST_ROOT_DIR+"/chainlink_st4";
	else
		ret=getenv("CHAINLINK_DIR");
	#ifdef WIN32
	ret.replace("/","\\");
	#endif
	return ret;
}

void STMainWindow::slot_chainlink() {
	#ifdef WIN32
		QString st_bin_dir=ST_BIN_DIR;
		QString batch_fname=ST_TMP_DIR+"/tmp_script.bat";
		QString batch_text;
		batch_text+=QString("set CHAINLINK_DIR=%1\n").arg(chainlink_dir());
		batch_text+=QString("set PATH=%1;%2;%PATH%\n").arg(chainlink_dir()+"\\bin").arg(st_bin_dir);
		batch_text+= QString("%1\\bin\\chainlink").arg(chainlink_dir());
		batch_text.replace("/","\\");
		write_text_file(batch_fname,batch_text);
		QProcess::startDetached(batch_fname);
	#else
		QProcess::startDetached(chainlink_dir()+"/bin/chainlink");
	#endif		
}

void STMainWindow::slot_rtst() {
	#ifdef WIN32
		QString st_bin_dir=ST_BIN_DIR;
		QString batch_fname=ST_TMP_DIR+"/tmp_script.bat";
		QString batch_text;
		batch_text+=QString("set CHAINLINK_DIR=%1\n").arg(chainlink_dir());
		batch_text+=QString("set PATH=%1;%2;%PATH%\n").arg(chainlink_dir()+"\\bin").arg(st_bin_dir);
		batch_text+= QString("%1\\rtst.exe").arg(st_bin_dir);
		batch_text.replace("/","\\");
		write_text_file(batch_fname,batch_text);
		QProcess::startDetached(batch_fname);
	#else
		QString st_bin_dir=ST_BIN_DIR;
		QProcess::startDetached(st_bin_dir+"/rtst");
	#endif		
}

void STMainWindow::slot_mr_pulse_tool() {
	#ifdef WIN32
		QString st_bin_dir=ST_BIN_DIR;
		QString batch_fname=ST_TMP_DIR+"/tmp_script.bat";
		QString batch_text;
		batch_text+=QString("set CHAINLINK_DIR=%1\n").arg(chainlink_dir());
		batch_text+=QString("set PATH=%1;%2;%PATH%\n").arg(chainlink_dir()+"\\bin").arg(st_bin_dir);
		batch_text+= QString("%1\\bin\\mrpulsetool").arg(chainlink_dir());
		batch_text.replace("/","\\");
		write_text_file(batch_fname,batch_text);
		QProcess::startDetached(batch_fname);
	#else
		QProcess::startDetached(chainlink_dir()+"/bin/mrpulsetool");
	#endif		
}
void STMainWindow::slot_run_mr_pulse_tool(QString fname) {
	#ifdef WIN32
		QString st_bin_dir=ST_BIN_DIR;
		QString batch_fname=ST_TMP_DIR+"/tmp_script.bat";
		QString batch_text;
		batch_text+=QString("set CHAINLINK_DIR=%1\n").arg(chainlink_dir());
		batch_text+=QString("set PATH=%1;%2;%PATH%\n").arg(chainlink_dir()+"\\bin").arg(st_bin_dir);
		batch_text+= QString("%1\\bin\\mrpulsetool %2").arg(chainlink_dir()).arg(fname);
		batch_text.replace("/","\\");
		write_text_file(batch_fname,batch_text);
		QProcess::startDetached(batch_fname);
	#else
		QString exec=chainlink_dir()+"/bin/mrpulsetool";
		QStringList args; args << fname;
		QProcess::startDetached(exec,args);
	#endif		
}
void STMainWindow::slot_preferences() {
	STPreferencesDlg dlg;
	if (dlg.exec()==QDialog::Accepted) {
		updatePreferences();
	}
}

void STMainWindow::slot_add_resource() {
	if (!currentSequence()) return;
	QString resource_name=QInputDialog::getText(this,"Add Resource","Resource name:");
	if (resource_name.isEmpty()) return;
	if (currentSequence()->resource(resource_name)) {
		QMessageBox::information(this,"Cannot add resource","A resource with this name already exists.");
		return;
	}
	STResource *R=new STResource(sstr(resource_name));
	currentSequence()->addResource(R);
	currentSequenceView()->setModified(true);
	currentSequenceView()->updateResources();	
}

void STMainWindow::slot_import_rf_pulse_waveform() {
	if (!currentSequence()) return;
	QSettings settings("Magland","SequenceTree4");
	QString holddir=settings.value("rf_pulse_directory").toString();
	QString fname=QFileDialog::getOpenFileName(this,"Import RF Pulse Waveform",holddir,"*.mrp");
	if (fname.isEmpty()) return;
	settings.setValue("rf_pulse_directory",QFileInfo(fname).path());
	QSettings pulsefile(fname,QSettings::IniFormat);
	
	QStringList data_real_str=pulsefile.value("data_real").toStringList();
	QStringList data_imag_str=pulsefile.value("data_imag").toStringList();
	double timestep=pulsefile.value("timestep").toDouble()*1000;
	double rephase_time=pulsefile.value("rephase_time").toDouble()*1000;
	double bandwidth=pulsefile.value("bandwidth").toDouble();
	bool has_error=false;
	if (data_real_str.count()<=1) has_error=true;
	if (data_real_str.count()!=data_imag_str.count()) has_error=true;
	if (timestep<=0) has_error=true;
	if (bandwidth<=0) has_error=true;
	if (has_error) {
		QMessageBox::information(this,"Error importing pulse.","Error importing pulse.");
		return;
	}
	
	double *data_real=(double *)malloc(sizeof(double)*data_real_str.count());
	double *data_imag=(double *)malloc(sizeof(double)*data_real_str.count());
	for (long j=0; j<data_real_str.count(); j++) {
		data_real[j]=data_real_str[j].toDouble();
		data_imag[j]=data_imag_str[j].toDouble();
	}
	
	STMetaSequence *Seq=currentSequence();
	
	QString shortfname=QFileInfo(fname).fileName();
	STResource *R;
	R=new STResource(sstr(shortfname+"->data_real"));
	R->setDoubleList(data_real_str.count(),data_real);
	Seq->addResource(R);
	R=new STResource(sstr(shortfname+"->data_imag"));
	R->setDoubleList(data_real_str.count(),data_imag);
	Seq->addResource(R);
	R=new STResource(sstr(shortfname+"->timestep"));
	R->setDouble(timestep);
	Seq->addResource(R);
	R=new STResource(sstr(shortfname+"->rephase_time"));
	R->setDouble(rephase_time);
	Seq->addResource(R);
	R=new STResource(sstr(shortfname+"->bandwidth"));
	R->setDouble(bandwidth);
	Seq->addResource(R);

	
	free(data_real);
	free(data_imag);
	
	currentSequenceView()->setModified(true);
	currentSequenceView()->updateResources();
}

void STMainWindow::updatePreferences() {
	for (int j=0; j<ui.tabWidget->count(); j++) {
		STSequenceView *SV=qobject_cast<STSequenceView *>(ui.tabWidget->widget(j));
		if (SV) SV->updatePreferences();
	}
}

void STMainWindow::update_actions() {
	bool sequence_open=(currentSequence()!=0);
	bool sequence_compiled=false;
	if (sequence_open) {
		sequence_compiled=currentSequenceView()->isCompiled();
	}
	ui.actionAddResource->setEnabled(sequence_open);	
	ui.actionCloseSequence->setEnabled(sequence_open);
	ui.actionCompile->setEnabled(sequence_open);
	ui.actionCopyNode->setEnabled(sequence_open);
	ui.actionExportToScanner->setEnabled(sequence_open);
	ui.actionExportToVirtualScanner->setEnabled(sequence_open);
	ui.actionImportRFPulseWaveform->setEnabled(sequence_open);
	ui.actionPasteNode->setEnabled(sequence_open);
	ui.actionReadRawData->setEnabled(sequence_compiled);
	ui.actionSaveSequence->setEnabled(sequence_open);
	ui.actionSaveSequenceAs->setEnabled(sequence_open);
}

void STMainWindow::slot_create_parameter_file() {
	if (!currentSequence()) return;	
	QString fname=QFileDialog::getSaveFileName(this,"Save parameter file as","","*.st4parameters");	
	if (fname.isEmpty()) return;
	currentSequence()->writeParametersToFile(fname);		
}

