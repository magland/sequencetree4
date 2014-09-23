#include "rvmainwindow.h"
#include "viewmdawidget.h"
#include "mda.h"
#include <QProcess>

void RVMainWindow::slot_open_experiment() {
	ui.file_view->openExperiment();
}
void RVMainWindow::slot_save_experiment() {
	ui.file_view->saveExperiment();
}
void RVMainWindow::slot_configuration() {
	ui.file_view->configuration();
}

void RVMainWindow::slot_file_activated(QString fname) {
	#ifdef WIN32
	fname.replace("/","\\");
	#endif
	
	QString suf=QFileInfo(fname).suffix();
	if (suf=="mda") {
		ViewmdaWidget *W=new ViewmdaWidget(this);
		Mda X;
		X.read(fname.toAscii().data());
		W->setArray(X);
		W->setProperty("fileName",fname);
		ui.tabWidget->addTab(W,QFileInfo(fname).fileName());
		ui.tabWidget->setCurrentWidget(W);
	}
	else if (suf=="ch") {
		QString exec=QString(getenv("CHAINLINK_DIR"))+"/bin/chainlink";
		QStringList args; args << fname << "-w" << QFileInfo(fname).path();
		QProcess::startDetached(exec,args);
	}
	
}

#ifdef CHAINLINK_FOUND
#include <QPluginLoader>
void RVMainWindow::set_chainlink_core() {
	if (m_chainlink_core) delete m_chainlink_core;
	m_chainlink_core=0;
	
	QString chainlink_dir=getenv("CHAINLINK_DIR");	
	
    	QPluginLoader loader;
    	QString plugin_fname=chainlink_dir+"/bin/chainlinkcoreplugin.dll";
    	qDebug()  << "Loading " << plugin_fname;
    	loader.setFileName(plugin_fname);
    	if (loader.load()) {
    		ChainlinkCorePluginInterface *ptr=qobject_cast<ChainlinkCorePluginInterface *>(loader.instance());
    		if (ptr) {
    			m_chainlink_core=ptr->getChainlinkCore(chainlink_dir);
    			if (!m_chainlink_core) {
    				qWarning()  << "Error, m_chainlink_core is NULL.";
    			}
    		}
    		else {
    			qWarning()  << "Error, ptr is NULL.";
    		}
    	}
    	else {
    		qWarning()  << "Unable to load plugin. " << loader.errorString() ;
    	}

}
#endif

#ifdef CHAINLINK_FOUND
void RVMainWindow::slot_reconstruction() {
	if (!m_chainlink_core) {
		qWarning() << "m_chainlink_core is NULL.";
		return;
	}
	m_chainlink_core->executionManager->setThreadMode(0);
	m_chainlink_core->settingsManager->setWorkingDirectory(ui.file_view->directory());
	qDebug()  << "Running reconstruction in directory:" << ui.file_view->directory();
	if (!m_chainlink_core->executionManager->executeCode("reconstruction(1);")) {
		qDebug()  << "Error runing script.";
	}
	ui.file_view->refresh();
}
#endif

void RVMainWindow::slot_close_tab() {
	if (current_widget()) {
		delete current_widget();
	}
}
