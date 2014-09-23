#include "vsmainwindow.h"
#include "vsktermscanner.h"
#include "run_virtual_scan.h"
#include "distribute_raw_data.h"
#include "stringchooserdlg.h"
#include <QProcess>
#include "stconfigurationdlg.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QPluginLoader>
#include "vsphantomplugin.h"
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QTextEdit>

VSMainWindow::VSMainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	
	m_scanner=new VSKtermScanner;	
	connect(m_scanner,SIGNAL(progress(long,long,QString)),this,SLOT(slot_scan_progress(long,long,QString)),Qt::DirectConnection);

	ui.vsplitter->setStretchFactor(0,1);
	ui.vsplitter->setStretchFactor(1,0);
	ui.hsplitter->setStretchFactor(0,1);
	ui.hsplitter->setStretchFactor(1,1);
	ui.hsplitter->setStretchFactor(2,0);
	
	m_phantom=0;
	
	connect(ui.actionOpenSequence,SIGNAL(triggered()),this,SLOT(slot_open_sequence()));
	connect(ui.actionSelectPhantom,SIGNAL(triggered()),this,SLOT(slot_select_phantom()));
	connect(ui.actionScan,SIGNAL(triggered()),this,SLOT(slot_scan()));
	connect(ui.actionConfiguration,SIGNAL(triggered()),this,SLOT(slot_configuration()));
	
	connect(&m_simulator,SIGNAL(compilationMessage(QString)),this,SLOT(slot_compilation_message(QString)));
	connect(&m_simulator,SIGNAL(simulationMessage(QString)),this,SLOT(slot_simulation_message(QString)));
	
	connect(ui.sequence_view,SIGNAL(clearSimulateLog()),this,SLOT(slot_clear_simulate_log()));
	
	connect(ui.phantom_view,SIGNAL(selectPhantom()),this,SLOT(slot_select_phantom()));
	
	QString bin_dir=ST_BIN_DIR;
	QString tmp_dir=ST_TMP_DIR;
	QString src_dir=ST_SIMULATOR_DIR;
	#ifdef WIN32
	bin_dir.replace("/","\\");
	tmp_dir.replace("/","\\");
	src_dir.replace("/","\\");
	#endif
	m_simulator.setDirectories(bin_dir,tmp_dir,src_dir);

	load_phantom_plugins();
	
	//VSPhantom *hold;
	//hold=new VSPhantom; hold->addSubphantom(new VSCylinderPhantom,"Cylinder"); hold->setName("Cylinder");
	//m_phantom_choices << hold;
	/*m_phantom_choices << new VSCubeMinusCylinderPhantom;
	m_phantom_choices << new VSCylinderPhantom;		
	m_phantom_choices << new VSCubePhantom;	
	m_phantom_choices << new VSSpherePhantom;
	m_phantom_choices << new VSAnnulusPhantom;		
	m_phantom_choices << new VSDynamicImpulsePhantom;
	m_phantom_choices << new VSSingleMotionPhantom;*/
	
	m_phantom=m_phantom_choices.value(0);
	ui.phantom_view->setPhantom(m_phantom);	
	
	restore_phantom_settings();	
	QSettings settings("Magland","VirtualScanner");
	ui.b0_inhomogeneity->setText(settings.value("b0_inhomogeneity").toString());
	if (ui.b0_inhomogeneity->text().isEmpty()) ui.b0_inhomogeneity->setText("(0,0,0)");
	ui.realtime_output->setText(settings.value("realtime_output").toString());
	ui.noise_level->setText(settings.value("noise_level").toString());
	
	m_progress_dialog=new QProgressDialog(this);
	m_progress_dialog->setMinimumDuration(0); //this is important so progress bar won't pop up after the scan erroneously
	m_cancel_button=new QPushButton("Cancel");
	m_progress_dialog->setCancelButton(m_cancel_button);
	
	connect(m_cancel_button,SIGNAL(clicked()),this,SLOT(slot_cancel_scan()));
}
VSMainWindow::~VSMainWindow() {
	save_phantom_settings();
	QSettings settings("Magland","VirtualScanner");
	settings.setValue("b0_inhomogeneity",ui.b0_inhomogeneity->text());
	settings.setValue("realtime_output",ui.realtime_output->text());
	settings.setValue("noise_level",ui.noise_level->text());
	delete m_scanner;
	qDeleteAll(m_phantom_choices);
}
void VSMainWindow::openSequence(QString fname) {
	fname=fname.trimmed();
	if (fname.isEmpty()) return;
	m_sequence.read(fname);
	m_sequence.setFileName(fname);
	m_simulator.setMetaSequence(&m_sequence);
	statusBar()->showMessage(tr("Compiling sequence: %1").arg(fname));
	ui.compile_log->clear();
	m_simulator.compile(true);
	statusBar()->showMessage(tr("Done compiling: %1").arg(fname));
	ui.sequence_view->setSimulator(&m_simulator);
	ui.simulate_log->clear();
}
void VSMainWindow::slot_open_sequence() {
	QSettings settings("Magland","SequenceTree4");
	QString holddir=settings.value("sequence_directory").toString();
	QString fname=QFileDialog::getOpenFileName(this,"Open sequence",holddir,"*.sts");
	if (fname.isEmpty()) return;
	settings.setValue("sequence_directory",QFileInfo(fname).path());
	statusBar()->showMessage(tr("Opening sequence: %1").arg(fname),MSG_DELAY);
	openSequence(fname);
	statusBar()->showMessage(tr("Sequence opened: %1").arg(fname),MSG_DELAY);	
}
void VSMainWindow::slot_select_phantom() {
	QStringList list;
	foreach (VSPhantom *P,m_phantom_choices) {	
		list << P->name();
	}	
	QString phantom_name=stringchooserdlg::choose_string(this,list,"Virtual Scanner","Select a phantom:");
	if (phantom_name.isEmpty()) return;
	foreach (VSPhantom *P,m_phantom_choices) {	
		if (P->name()==phantom_name) {
			m_phantom=P;
			ui.phantom_view->setPhantom(P);
		}
	}	
}

void VSMainWindow::slot_compilation_message(QString msg) {
	ui.logwidget->setCurrentWidget(ui.compile_tab);
	ui.compile_log->addMessage(msg);		
}
void VSMainWindow::slot_simulation_message(QString msg) {
	ui.logwidget->setCurrentWidget(ui.simulate_tab);
	ui.simulate_log->addItem(msg);		
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


void multiply33(matrix33 &ret,matrix33 &R1,matrix33 &R2) {
	for (int i1=0; i1<3; i1++)
	for (int i2=0; i2<3; i2++) {
		double hold=0;
		for (int k=0; k<3; k++) {
			hold+=R1[i1][k]*R2[k][i2];
		}
		ret[i1][i2]=hold;
	}
}

void define_rotation(matrix33 &ret,double Rxz,double Ryz,double Rxy) {
	Rxz*=PI/180;
	Ryz*=PI/180;
	Rxy*=PI/180;
	double R1[3][3];
	double R2[3][3];
	double R3[3][3];
	//set identity
	for (int i1=0; i1<3; i1++)
	for (int i2=0; i2<3; i2++) {
		double val=0;
		if (i1==i2) val=1;
		R1[i1][i2]=R2[i1][i2]=R3[i1][i2]=val;
	}
	R1[0][0]=cos(Rxz); R1[0][2]=-sin(Rxz);
	R1[2][0]=sin(Rxz); R1[2][2]=cos(Rxz);
	
	R2[1][1]=cos(Ryz); R2[1][2]=-sin(Ryz);
	R2[2][1]=sin(Ryz); R2[2][2]=cos(Ryz);
	
	R3[0][0]=cos(Rxy); R3[0][1]=-sin(Rxy);
	R3[1][0]=sin(Rxy); R3[1][1]=cos(Rxy);
	
	double hold[3][3];
	multiply33(hold,R2,R1);
	multiply33(ret,R3,hold);
}

void VSMainWindow::slot_scan() {
	if (!m_phantom) return;
	if (!m_simulator.metaSequence()) return;
	if (m_scanner->isRunning()) return;
	
	QDir(ST_TMP_DIR).mkdir("data");
	QString data_dir=ST_TMP_DIR+"/data";
	erase_all_files_in_directory(data_dir);
	
	m_progress_dialog->show();
	m_progress_dialog->setRange(0,100);
	m_progress_dialog->setValue(0);
	m_progress_dialog->setWindowTitle("ST Virtual Scanner");
	m_progress_dialog->setLabelText("Running Sequence...");
	qApp->processEvents();
	QString raw_template_fname=ST_TMP_DIR+"/vs_raw_template.dat";
	STMetaNodeParameter P=m_simulator.metaSequence()->parameter("FOV_shift_offset");
	P.value=ui.FOV_shift->text();
	m_simulator.metaSequence()->setParameter("FOV_shift_offset",P);
	m_simulator.runEntireSequence(true);
	m_progress_dialog->setLabelText("Writing template file...");
	qApp->processEvents();
	m_simulator.writeRawTemplateFile(raw_template_fname);
	STSimScannerBlockList list;
	m_progress_dialog->setLabelText("Reading simulation file...");
	qApp->processEvents();
	list.read(sstr(ST_TMP_DIR+"/simulation_output.dat"));
	m_scanner->setPhantom(m_phantom);
	m_scanner->setMinTermSize(ui.min_term_size->text().toDouble());
	m_scanner->setMaxNumTerms(ui.max_num_terms->text().toInt());	
	m_scanner->setNoiseLevel(ui.noise_level->text().toDouble());
	m_scanner->setB1Scale(ui.b1_scale->text().toDouble());
	QString inhomstr=ui.b0_inhomogeneity->text();
	inhomstr.remove("(");
	inhomstr.remove(")");
	QStringList inhomlist=inhomstr.split(",");
	if (inhomlist.count()==3) {
		m_scanner->setB0Inhomogeneity(inhomlist[0].toDouble(),inhomlist[1].toDouble(),inhomlist[2].toDouble());
	}
	else m_scanner->setB0Inhomogeneity(0,0,0);
	
	//QString holdstr=ui.object_shift->text();
	//holdstr.remove(" "); holdstr.remove("("); holdstr.remove(")");
	//QStringList object_shift_list=holdstr.split(",");
	//m_scanner->setObjectShift(object_shift_list.value(0).toDouble(),object_shift_list.value(1).toDouble(),object_shift_list.value(2).toDouble());
	
	QString sim_output_fname=ST_TMP_DIR+"/vs_output.dat";
	m_progress_dialog->setLabelText("Running virtual scan...");
	qApp->processEvents();
	if (run_virtual_scan(list,m_scanner,sim_output_fname,ui.realtime_output->text(),QFileInfo(m_sequence.fileName()).baseName())) {
		DistributeRawDataStruct X;
		X.data_fname=sim_output_fname;
		X.template_fname=raw_template_fname;
		X.header_size=128;
		X.num_channels=1;
		X.output_directory=data_dir;
		X.raw_data_format=RAW_DATA_FORMAT_SIEMENS_VA;
		m_progress_dialog->setRange(0,100);
		m_progress_dialog->setValue(0);
		m_progress_dialog->setLabelText("Distributing raw data...");
		qApp->processEvents();
		distribute_raw_data(X);
		m_progress_dialog->hide();
	
		m_simulator.metaSequence()->write(data_dir+"/"+QFileInfo(m_simulator.metaSequence()->fileName()).fileName());
		write_text_file(data_dir+"/reconstruction.ch",m_simulator.metaSequence()->reconstructionCode());
	
		if (ui.realtime_output->text().isEmpty()) {
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
		m_progress_dialog->hide();
	}
	else {
		m_progress_dialog->hide();
	}
}

void VSMainWindow::slot_clear_simulate_log() {
	ui.simulate_log->clear();
}


void VSMainWindow::slot_scan_progress(long prog,long total,QString msg) {
	if (!m_progress_dialog->isVisible()) return;
	m_progress_dialog->setLabelText(msg);	
	m_progress_dialog->setRange(0,total);
	m_progress_dialog->setValue(prog);
	qApp->processEvents();
}

void VSMainWindow::slot_cancel_scan() {
	m_scanner->cancelScan();
	if (QMessageBox::question(this,"Scan Cancelled","Scan cancelled. Would you like to view the current kspace terms?",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes) {
		QString txt=m_scanner->getComponentsText();
		QTextEdit *edit=new QTextEdit(0);
		edit->setAttribute(Qt::WA_DeleteOnClose);
		edit->setPlainText(txt);
		edit->showNormal();		
	}
}

void VSMainWindow::slot_configuration() {
	STConfigurationDlg dlg;
	dlg.exec();
}

void VSMainWindow::restore_phantom_settings() {
	QSettings settings("Magland","VirtualScanner");
	foreach (VSPhantom *P,m_phantom_choices) {
		const QMetaObject *mobj=P->metaObject();
		for (int j=0; j<mobj->propertyCount(); j++) {
			QMetaProperty prop=mobj->property(j);
			QString propname=prop.name();
			QVariant propval=settings.value(P->name()+"---"+propname);
			P->setProperty(prop.name(),propval);
		}
		for (int k=0; k<P->subphantomCount(); k++) {
			const QMetaObject *mobj2=P->subphantom(k)->metaObject();
			for (int j=0; j<mobj2->propertyCount(); j++) {
				QMetaProperty prop=mobj2->property(j);
				QString propname=prop.name();
				QVariant propval=settings.value(P->name()+"---"+P->subphantom(k)->name()+"---"+propname);
				P->subphantom(k)->setProperty(prop.name(),propval);
			}	
		}
	}
	QString current_phantom_name=settings.value("current_phantom").toString();
	if (!current_phantom_name.isEmpty()) {
		foreach (VSPhantom *P,m_phantom_choices) {
			if (P->name()==current_phantom_name) {
				m_phantom=P;
				ui.phantom_view->setPhantom(P);
			}
		}
	}
}
void VSMainWindow::save_phantom_settings() {
	QSettings settings("Magland","VirtualScanner");
	foreach (VSPhantom *P,m_phantom_choices) {
		const QMetaObject *mobj=P->metaObject();
		for (int j=0; j<mobj->propertyCount(); j++) {
			QMetaProperty prop=mobj->property(j);
			QString propname=prop.name();
			QVariant propval=P->property(prop.name());
			settings.setValue(P->name()+"---"+propname,propval);
		}
		for (int k=0; k<P->subphantomCount(); k++) {
			const QMetaObject *mobj2=P->subphantom(k)->metaObject();
			for (int j=0; j<mobj2->propertyCount(); j++) {
				QMetaProperty prop=mobj2->property(j);
				QString propname=prop.name();
				QVariant propval=P->subphantom(k)->property(prop.name());
				settings.setValue(P->name()+"---"+P->subphantom(k)->name()+"---"+propname,propval);
			}	
		}
	}
	if (m_phantom) settings.setValue("current_phantom",m_phantom->name());
}

void VSMainWindow::load_phantom_plugins() {
	QString search_path=ST_ROOT_DIR+"/vsplugins";
	QDir dir(search_path);
	QStringList list=dir.entryList(QDir::Files);
	foreach (QString fname,list) {
		QString path=search_path+"/"+fname;
		QPluginLoader *loader=new QPluginLoader(path);
		QObject *plugin=loader->instance();
		if (plugin) {
			qDebug()  << "Loading plugin:" << QFileInfo(path).fileName();
				
			if (qobject_cast<VSPhantomPlugin *>(plugin)) {
				VSPhantomPlugin *ptr=qobject_cast<VSPhantomPlugin *>(plugin);
				QList<VSPhantom *> phlist=ptr->phantomList();
				foreach (VSPhantom *P,phlist) {
					m_phantom_choices << P;
				}
			}
			else {
				qWarning() << "Unable to cast plugin to VSPhantomPlugin *.";
				loader->unload();
			}
		}
		else {
			if (QFileInfo(path).suffix()=="dll")
				qWarning() << "Unable to load plugin:" << QFileInfo(path).fileName() << loader->errorString();
		}
	}
}

