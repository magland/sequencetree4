#include "rvfileview.h"
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

QList<long> read_mda_dims(QString mda_fname) {
	QList<long> ret;
	FILE *inf=fopen(mda_fname.toAscii().data(),"rb");
	if (!inf) return ret;

	bool is_sparse=false;
	qint32 hold_num_dims;
	if (fread(&hold_num_dims,sizeof(qint32),1,inf)<=0)
		return ret;
	qint32 data_type;
	if (hold_num_dims<0) {
		data_type=hold_num_dims;
		if (data_type==-1001) {
			is_sparse=true;
			fread(&data_type,sizeof(qint32),1,inf);
		}
		qint32 num_bytes;
		fread(&num_bytes,sizeof(qint32),1,inf);
		fread(&hold_num_dims,sizeof(qint32),1,inf);
	}
	for (int j=0; j<hold_num_dims; j++) {
		qint32 holdval;
		fread(&holdval,sizeof(qint32),1,inf);
		ret << holdval;
	}	
	
	fclose(inf);
	return ret;
}


RVFileViewItem::RVFileViewItem(QString fname) {
	m_file_name=fname;
	setText(0,QFileInfo(fname).fileName());
	if (QFileInfo(fname).suffix()=="mda") {
		QList<long> dims=read_mda_dims(fname);
		QString dims_string="";
		foreach (long dim,dims) {
			if (!dims_string.isEmpty()) dims_string+="x";
			dims_string+=QString::number(dim);
		}
		setText(1,dims_string);
	}
}

RVFileView::RVFileView(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);
	QStringList labels; labels << "File" << "Dimensions";
	ui.tree->setHeaderLabels(labels);
	
	
	connect(ui.reload_button,SIGNAL(clicked()),this,SLOT(slot_reload()));
	connect(ui.save_button,SIGNAL(clicked()),this,SLOT(slot_save()));
	connect(ui.open_button,SIGNAL(clicked()),this,SLOT(slot_open()));
	
	connect(ui.tree,SIGNAL(itemActivated(QTreeWidgetItem *,int)),this,SLOT(slot_item_activated(QTreeWidgetItem *)));
	
	QDir(ST_TMP_DIR).mkdir(ST_TMP_DIR+"/data");
	setDirectory(ST_TMP_DIR+"/data");
}

void RVFileView::setDirectory(QString directory) {
	if (directory.isEmpty()) return;
	if (!QDir(directory).exists()) return;
	m_directory=directory;
	ui.directory->setText(directory);
	refresh();
}
void RVFileView::refresh() {
	ui.tree->clear();
	QDir dir(m_directory);
	QStringList list=dir.entryList(QStringList("*"),QDir::Files);
	foreach (QString fname,list) {
		QString fname2=m_directory+"/"+fname;
		ui.tree->addTopLevelItem(new RVFileViewItem(fname2));
	}
	ui.tree->resizeColumnToContents(0);
	ui.tree->resizeColumnToContents(1);
	
}

void move_all_files_between_directories(QString src_dirname,QString dst_dirname) {
	QDir src_dir(src_dirname);
	QDir dst_dir(dst_dirname);
	QStringList list=src_dir.entryList(QStringList("*"),QDir::Files|QDir::NoDotAndDotDot);
	foreach (QString fname,list) {
		if (!QFile(src_dirname+"/"+fname).copy(dst_dirname+"/"+fname)) {
			qWarning() << "Cannot copy file:" << fname;
		}
		else {
			QFile::remove(src_dirname+"/"+fname);
		}
	}
}


void RVFileView::slot_reload() {
	setDirectory(ui.directory->text());
}
void RVFileView::slot_save() {
	QString expname=QInputDialog::getText(this,"Save Experiment","Name of experiment:");
	if (expname.isEmpty()) return;
	QString expfolder=experiments_folder();
	if (expfolder.isEmpty()) return;
	if (!QDir(expfolder).mkdir(expname)) {
		QMessageBox::information(this,"Unable to create directory","Unable to create directory: "+ expfolder+"/"+expname);
		return;
	}
	QString dirname=expfolder+"/"+expname;
	move_all_files_between_directories(ui.directory->text(),dirname);
	setDirectory(dirname);
}
void RVFileView::slot_open() {
	QString dirname=QFileDialog::getExistingDirectory(this,"View experiment in folder:",experiments_folder());
	if (dirname.isEmpty()) return;
	setDirectory(dirname);
}

QString RVFileView::experiments_folder() {
	QSettings settings("Magland","SequenceTree4");
	QString ret=settings.value("experiments_folder").toString();
	if (ret.isEmpty()) {
		ret=QFileDialog::getExistingDirectory(this,"Please choose a directory where all experiments will be saved:");
		settings.setValue("experiments_folder",ret);
	}
	return ret;
}

void RVFileView::configuration() {
	QSettings settings("Magland","SequenceTree4");
	QString newfolder=QFileDialog::getExistingDirectory(this,"Please choose a directory where all experiments will be saved:",settings.value("experiments_folder").toString());
	if (!newfolder.isEmpty())
		settings.setValue("experiments_folder",newfolder);
}

void RVFileView::slot_item_activated(QTreeWidgetItem *item) {
	RVFileViewItem *item2=(RVFileViewItem *)item;
	if (!item2) return;
	emit fileActivated(item2->fileName());
}


