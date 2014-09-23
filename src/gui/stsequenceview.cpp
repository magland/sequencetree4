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


#include "stsequenceview.h"
#include <QMessageBox>
#include "stringchooserdlg.h"
#include <QInputDialog>
#include <QFile>
#include <QProcess>
#include "stsimview.h"
#include "stcommon.h"
#include <QSettings>
#include "editresourcedlg.h"


STSequenceView::STSequenceView(QWidget *parent) : QWidget(parent) {
	ui.setupUi(this);
	
	ui.reconstruction->setLineWrapMode(QTextEdit::NoWrap);
	ui.reconstruction->setTabStopWidth(10);	
	m_reconstruction_highlighter=new STReconstructionHighlighter(this);
	m_reconstruction_highlighter->setDocument(ui.reconstruction->document());
	
	ui.codeview->show();

	QString bin_dir=ST_BIN_DIR;
	QString tmp_dir=ST_TMP_DIR;
	QString src_dir=ST_SIMULATOR_DIR;
	#ifdef WIN32
	bin_dir.replace("/","\\");
	tmp_dir.replace("/","\\");
	src_dir.replace("/","\\");
	#endif
	m_simulator.setDirectories(bin_dir,tmp_dir,src_dir);
	
	ui.hsplitter->setStretchFactor(0,0);
	ui.hsplitter->setStretchFactor(1,1);
	ui.vsplitter->setStretchFactor(0,1);
	ui.vsplitter->setStretchFactor(1,0);
	ui.vsplitter2->setStretchFactor(0,1);
	ui.vsplitter2->setStretchFactor(1,0);
	
	connect(ui.nodeview,SIGNAL(parameterChanged(QString)),this,SLOT(slot_parameter_changed(QString)));
	connect(ui.nodeview,SIGNAL(editCode()),this,SLOT(slot_edit_code()));
	connect(ui.nodeview,SIGNAL(minimizeParameter(QString)),this,SLOT(slot_minimize_parameter(QString)));
	connect(ui.nodeview,SIGNAL(maximizeParameter(QString)),this,SLOT(slot_maximize_parameter(QString)));
	connect(ui.nodeview,SIGNAL(linkToGlobalParameter(QString)),this,SLOT(slot_link_to_global_parameter(QString)));
	connect(ui.nodeview,SIGNAL(unlinkGlobalParameter(QString)),this,SLOT(slot_unlink_global_parameter(QString)));
	
	connect(ui.globalview,SIGNAL(globalParameterValueChanged(QString,QString)),this,SLOT(slot_global_parameter_value_changed(QString,QString)));
	connect(ui.globalview,SIGNAL(globalParameterUnitsChanged(QString,QString)),this,SLOT(slot_global_parameter_units_changed(QString,QString)));
	connect(ui.globalview,SIGNAL(deleteGlobalParameter(QString)),this,SLOT(slot_delete_global_parameter(QString)));
	connect(ui.globalview,SIGNAL(moveGlobalParameterUp(QString)),this,SLOT(slot_move_global_parameter_up(QString)));
	connect(ui.globalview,SIGNAL(moveGlobalParameterDown(QString)),this,SLOT(slot_move_global_parameter_down(QString)));
	
	connect(ui.treeview,SIGNAL(currentNodeChanged(STMetaNode *)),this,SLOT(slot_current_node_changed(STMetaNode *)));
	connect(ui.treeview,SIGNAL(addChildNode(STMetaNode *)),this,SLOT(slot_add_child_node(STMetaNode *)));
	connect(ui.treeview,SIGNAL(customizeNode(STMetaNode *)),this,SLOT(slot_customize_node(STMetaNode *)));
	connect(ui.treeview,SIGNAL(promoteNode(STMetaNode *)),this,SLOT(slot_promote_node(STMetaNode *)));
	connect(ui.treeview,SIGNAL(deleteNode(STMetaNode *)),this,SLOT(slot_delete_node(STMetaNode *)));
	connect(ui.treeview,SIGNAL(renameNode(STMetaNode *)),this,SLOT(slot_rename_node(STMetaNode *)));
	connect(ui.treeview,SIGNAL(changeNodeType(STMetaNode *)),this,SLOT(slot_change_node_type(STMetaNode *)));
	connect(ui.treeview,SIGNAL(insertNode(STMetaNode *)),this,SLOT(slot_insert_node(STMetaNode *)));
	connect(ui.treeview,SIGNAL(nodeActivated(STMetaNode *)),this,SLOT(slot_node_activated(STMetaNode *)));
	connect(ui.treeview,SIGNAL(linkNodes(STMetaNode *,STMetaNode *)),this,SLOT(slot_link_nodes(STMetaNode *,STMetaNode *)));
	connect(ui.treeview,SIGNAL(unlinkNode(STMetaNode *)),this,SLOT(slot_unlink_node(STMetaNode *)));
	connect(ui.treeview,SIGNAL(resourceActivated(QString)),this,SLOT(slot_resource_activated(QString)));
	
	connect(ui.classview,SIGNAL(classActivated(QString)),this,SLOT(slot_class_activated(QString)));
	connect(ui.classview,SIGNAL(renameClass(QString)),this,SLOT(slot_rename_class(QString)));
	connect(ui.classview,SIGNAL(deleteClass(QString)),this,SLOT(slot_delete_class(QString)));
	connect(ui.classview,SIGNAL(exportClass(QString)),this,SLOT(slot_export_class(QString)));
	
	connect(ui.codeview,SIGNAL(changeClass(QString)),this,SLOT(slot_change_class(QString)));
	
	connect(&m_simulator,SIGNAL(compilationStarted()),this,SLOT(slot_compilation_started()),Qt::DirectConnection);
	connect(&m_simulator,SIGNAL(compilationMessage(QString)),this,SLOT(slot_compilation_message(QString)),Qt::DirectConnection);
	connect(&m_simulator,SIGNAL(compilationFinished()),this,SLOT(slot_compilation_finished()),Qt::DirectConnection);
	
	connect(&m_simulator,SIGNAL(simulationStarted()),this,SLOT(slot_simulation_started()));
	connect(&m_simulator,SIGNAL(simulationMessage(QString)),this,SLOT(slot_simulation_message(QString)));
	connect(&m_simulator,SIGNAL(statFinished(bool)),this,SLOT(slot_stat_finished(bool)));
	connect(&m_simulator,SIGNAL(runFinished(bool)),this,SLOT(slot_run_finished(bool)));
	
	connect(ui.simview,SIGNAL(blockChanged(STSimScannerBlock *)),this,SLOT(slot_block_changed(STSimScannerBlock *)));
	connect(ui.simview,SIGNAL(selectedEventsChanged(QSet<int>)),this,SLOT(slot_simview_selected_events_changed(QSet<int>)));
	
	connect(ui.event_list,SIGNAL(selectedEventsChanged(QSet<int>)),this,SLOT(slot_eventlist_selected_events_changed(QSet<int>)));
	connect(ui.event_list,SIGNAL(currentEventChanged(STSimScannerEvent)),this,SLOT(slot_current_event_changed(STSimScannerEvent)));
	
	connect(ui.compile_log,SIGNAL(gotoLine(QString,int)),this,SLOT(slot_goto_source_line(QString,int)));
	
	connect(ui.codeview,SIGNAL(classChanged()),this,SLOT(slot_class_changed()));
	connect(ui.codeview,SIGNAL(signalCompile()),this,SLOT(slot_compile()));
	
	m_is_modified=false;

	ui.tabWidget->setTabToolTip(0,"The <b>Parameters</b> tab shows the sequence parameters associated with the currently selected node in the tree view. Once the sequence has been compiled, green and red icons will appear next to parameters, indicating which can be modified by the user. Parameters with green icons can be modified by double clicking and editing. Parameters with red icons cannot be modified because they are set programmatically during sequence execution. As parameters are modified, the sequence events will be updated in real time in the simulation view.");
	ui.tabWidget->setTabToolTip(1,"The <b>Code</b> tab displays the C++ source code for foundation (built-in) SequenceTree node types, as well as for customized node types associated with the current sequence. To view the source code for a particular node, double click on the node in the tree view. Source code for foundation classes cannot be edited. To change the functionality of a particular sequence node, you must first customize the node by right-clicking on the node and selecting \"Customize node...\" Then the source code for this node will be editable. It is recommended that you only modifify source code for top level nodes (i.e. Loop and Block nodes).");
	ui.tabWidget->setTabToolTip(2,"The <b>Notes</b> tab shows notes (text) associated with the sequence as well as directives to the scanner, such as directives to the Siemens scanner console.");
	ui.tabWidget->setTabToolTip(3,"The <b>Reconstruction</b> tab contains image reconstruction and processing code in (Matlab/Chainlink syntax) associated with the current sequence.");	
	
	ui.browser_tab->setTabToolTip(0,"The <b>Tree</b> tab shows the pulse sequence as a hierarchy of nodes. The direct children of the root node are Loop nodes, which control the iterative structure of the sequence. Children of loop nodes are Block nodes, usually representing a single repetition in the pulse sequence. The lowest level nodes control individual scanner events such as gradients, RF pulses, and signal readouts.");
	ui.browser_tab->setTabToolTip(1,"The <b>Classes</b> tab lists all node types available, including foundation (built-in) node types and custom node types associated with the sequence.");
	ui.browser_tab->setTabToolTip(2,"The <b>Global</b> tab lists the global parameters associated with the sequence. Each global parameter is linked to a sequence parameter for some node. To modify a global parameter, double click and then edit the text.");

	ui.logtabwidget->setTabToolTip(0,"The <b>Compile</b> tab shows compiler messages. Press F9 to compile your sequence. Compiler errors will appear red. Double click on a compile error to go to the corresponding source location.");
	ui.logtabwidget->setTabToolTip(1,"The <b>Simulate</b> tab shows messages occuring during sequence simulation.");
	ui.logtabwidget->setTabToolTip(2,"The <b>Events</b> tab shows details about sequence events. Click on an event in the simulation view to highlight the corresponding event information.");
}

void STSequenceView::setPreferredSizes() {
	QList<int> hold_sizes;
	int sum_sizes;
	
	hold_sizes=ui.vsplitter2->sizes();
	sum_sizes=hold_sizes.value(0)+hold_sizes.value(1);
	hold_sizes[0]=sum_sizes-150; hold_sizes[1]=150;
	ui.vsplitter2->setSizes(hold_sizes);
	
	hold_sizes=ui.hsplitter->sizes();
	sum_sizes=hold_sizes.value(0)+hold_sizes.value(1);
	hold_sizes[0]=sum_sizes*1/3; hold_sizes[1]=sum_sizes*2/3;
	ui.hsplitter->setSizes(hold_sizes);
	
	hold_sizes=ui.vsplitter->sizes();
	sum_sizes=hold_sizes.value(0)+hold_sizes.value(1);
	hold_sizes[0]=sum_sizes*1/2; hold_sizes[1]=sum_sizes*1/2;
	ui.vsplitter->setSizes(hold_sizes);
}

STSequenceView::~STSequenceView() {
	//if (m_source_editor) delete m_source_editor;
}
void STSequenceView::setSequence(STMetaSequence *Seq) {
	m_sequence=Seq;
	m_simulator.setMetaSequence(m_sequence);
	ui.treeview->setSequence(Seq);
	ui.classview->setSequence(Seq);
	ui.globalview->setSequence(Seq);
	ui.simview->setSimulator(&m_simulator);
	ui.codeview->setSequence(Seq);
	ui.codeview->setClass(Seq->metaClass("STRoot"));
	ui.nodeview->setNode(0);
	ui.notes->setPlainText(Seq->notes());
	QString recon_code=Seq->reconstructionCode();
	if (recon_code.trimmed().isEmpty()) {recon_code="function reconstruction(num_channels)\nif (nargin<1) num_channels=1; end;\n\n";}
	ui.reconstruction->setPlainText(recon_code);
	m_is_modified=false;
	if ((Seq)&&(Seq->globalParameterCount()>0)) {
		ui.browser_tab->setCurrentWidget(ui.global_tab);
	}
	updateAllViews(false);
}

void STSequenceView::slot_current_node_changed(STMetaNode *node) {
	ui.nodeview->setNode(node);
}

void STSequenceView::slot_parameter_changed(QString name) {
	if (currentNode()) {
		m_sequence->handleLinksAfterNodeChanged(currentNode(),name);
	}
	m_simulator.computeStatistics();
	ui.simview->updatePlots(); //important to do this one first
	ui.nodeview->updateControls();
	ui.treeview->update();
	ui.globalview->updateGlobalParameters();
	setModified(true);
}

void STSequenceView::slot_edit_code() {
	if (!m_sequence) return;
	STMetaNode *N=ui.nodeview->node();
	if (!N) return;
	if (N->className().isEmpty()) return;
	show_source_for_class(N->className());
}
void STSequenceView::show_source_for_class(QString cname) {
	if (!m_sequence) return;
	STMetaClass *C=m_sequence->metaClass(cname);
	if (!C) return;
	ui.codeview->setClass(C);
	ui.tabWidget->setCurrentWidget(ui.code_tab);
	//m_source_editor->show();
	//m_source_editor->raise();
}

void STSequenceView::slot_class_changed() {
	setModified(true);
	updateAllViews(true);
}

void STSequenceView::get_info_for_new_child(STMetaNode *node,QString &child_class,QString &child_name) {
	if (!node) return;
	if (!m_sequence) return;
	STMetaClass *C=m_sequence->metaClass(node->className());
	if (!C) return;
	
	QString base_class_name="";
	QString exclude_base_class_name="";
	QString label="";
	QString title="";
	child_name="";
	if (m_sequence->inherits(node->className(),"STSequence")) {
		base_class_name="STLoop";
		exclude_base_class_name="STSequence";
		title="Add new loop";
		label="Add new loop of type:";
		child_name="Loop";
	}
	else if (m_sequence->inherits(node->className(),"STLoop")) {
		base_class_name="STChain";
		exclude_base_class_name="";
		title="Add new block";
		label="Add new block of type:";
		child_name="Block";
	}
	else {
		base_class_name="STNode";
		exclude_base_class_name="STLoop";
		title="Add new segment";
		label="Add new segment of type:";
		child_name="Block";
	}
	
	if (base_class_name.isEmpty()) {
		return;
	}
	
	QStringList classes=m_sequence->classesThatInherit(base_class_name,exclude_base_class_name);
	classes.removeAll("STLoop");
	qSort(classes);
	child_class=stringchooserdlg::choose_string(this,classes,title,label);
	if (!child_class.isEmpty()) {
		STMetaClass *C2=m_sequence->metaClass(child_class);
		if (!C2) return;
		
		bool done=false;
		int j=1;
		while (!done) {
			QString new_childname=child_name+QString::number(j);
			if (C->hasChild(new_childname)) {
				j++;
			}
			else {
				done=true;
				child_name=new_childname;
			}
		}
		child_name=QInputDialog::getText(this,"Add child","New child name:",QLineEdit::Normal,child_name);
		
	}
}

void STSequenceView::slot_add_child_node(STMetaNode *node) {
	if (!node) return;
	if (!m_sequence) return;
	STMetaClass *C=m_sequence->metaClass(node->className());
	
	if (!C) return;
	if (C->isFoundationClass()) return;

	QString child_class;
	QString child_name;
	get_info_for_new_child(node,child_class,child_name);
	if ((!child_class.isEmpty())&&(!child_name.isEmpty())) {
		C->addChild(child_class,child_name);
		slot_class_changed();
	}
	
	ui.treeview->expandNode(node);
}

void STSequenceView::slot_customize_node(STMetaNode *node) {
	if (!node) return;
	if (!m_sequence) return;
	STMetaNode *par=node->parent();
	if (!par) {
		QMessageBox::information(this,"Cannot customize node.","Cannot customize root node.");
		return;
	}
	STMetaClass *C=m_sequence->metaClass(node->className());
	STMetaClass *parC=m_sequence->metaClass(par->className());
	
	bool ok_to_customize=true;
	if (!C) {
		QMessageBox::information(this,"Cannot customize node.","Cannot customize node. An unexpected problem occurred (*).");
		ok_to_customize=false;
	}
	else if (!parC) {		
		QMessageBox::information(this,"Cannot customize node.","Cannot customize node. An unexpected problem occurred (**).");
		ok_to_customize=false;
	}
	if ((parC->isFoundationClass())&&(!m_sequence->isPromoted(node))) {
		QMessageBox::information(this,"Cannot customize node.","You must first customize the parent of this node.");
		ok_to_customize=false;
	}
	if (!ok_to_customize) return;
	
	if ((par)&&(par->parent())&&(par->parent()->parent())) {
		QString txt="It is highly recommended that you do not customize this node. In general, it is best to customize loop and block nodes, leaving lower level nodes as foundation types. Most tasks can be accomplished by customizing higher level nodes. If you know what you are doing, click OK. Otherwise, click cancel.";
		if (QMessageBox::question(this,"Customize node.",txt,QMessageBox::Ok|QMessageBox::Cancel)!=QMessageBox::Ok) {
			return;
		}
	}

	STMetaClass *C2=m_sequence->duplicateClass(C);
	if (!C2) {
		QMessageBox::information(this,"Cannot customize node.","Cannot customize node. An unexpected problem occurred (***).");
		return;
	}
	QString old_name=C2->className();
	QString new_class_name=QInputDialog::getText(this,"Customize node","Class name:",QLineEdit::Normal,C2->className());
	if (new_class_name!=C2->className()) {
		if (m_sequence->metaClass(new_class_name)) {
			QMessageBox::information(this,"Cannot customize","Cannot customize node: a class of this name already exists.");
			new_class_name="";
		}
	}
	if (new_class_name.isEmpty()) {
		m_sequence->removeClass(C2->className());
		return;
	}
	if (m_sequence->isPromoted(node)) {
		node->setClassName(old_name);
	}
	else {
		parC->changeChildType(node->name(),old_name);
	}
	m_sequence->renameClass(old_name,new_class_name);

	if (ui.codeview->currentClassName()==C->className()) {	
		ui.codeview->setClass(C2);
	}	
	
	slot_class_changed();
}

void STSequenceView::slot_promote_node(STMetaNode *node) {
	if (!node) return;
	if (!m_sequence) return;
	STMetaNode *par=node->parent();
	if (!par) return;
	STMetaClass *C=m_sequence->metaClass(node->className());
	STMetaClass *parC=m_sequence->metaClass(par->className());
	
	if ((!C)||(!parC)) return;
	
	STMetaClassChild chld=parC->child(node->name());
	if (chld.name!=node->name()) return;
	
	QStringList classes=m_sequence->classesThatInherit(chld.type);
	qSort(classes);
	QString class_name=stringchooserdlg::choose_string(this,classes,"Promote node","Node type:");	
	
	if (!class_name.isEmpty()) {
		node->setClassName(class_name);
		slot_class_changed();
	}
}


void STSequenceView::slot_delete_node(STMetaNode *node) {
	if (!node) return;
	if (!m_sequence) return;
	STMetaNode *par=node->parent();
	if (!par) return;
	STMetaClass *C=m_sequence->metaClass(node->className());
	STMetaClass *parC=m_sequence->metaClass(par->className());
	
	if (!C) return;
	if (!parC) return;
	if (parC->isFoundationClass()) return;
	
	parC->removeChild(node->name());
	slot_class_changed();
}

void STSequenceView::slot_insert_node(STMetaNode *node) {
	if (!node) return;
	if (!m_sequence) return;
	STMetaNode *par=node->parent();
	if (!par) return;
	STMetaClass *parC=m_sequence->metaClass(par->className());
	
	if (!parC) return;
	if (parC->isFoundationClass()) return;

	QString child_class;
	QString child_name;
	get_info_for_new_child(par,child_class,child_name);
	if ((!child_class.isEmpty())&&(!child_name.isEmpty())) {
		parC->insertChild(node->name(),child_class,child_name);
		slot_class_changed();
	}
}

void STSequenceView::slot_rename_node(STMetaNode *node) {
	if (!node) return;
	if (!m_sequence) return;
	STMetaNode *par=node->parent();
	if (!par) return;
	STMetaClass *C=m_sequence->metaClass(node->className());
	STMetaClass *parC=m_sequence->metaClass(par->className());
	
	if (!C) return;
	if (!parC) return;
	if (parC->isFoundationClass()) return;
	
	QString old_name=node->name();
	QString new_name=QInputDialog::getText(this,"Rename node","New name:",QLineEdit::Normal,old_name);
	if (!new_name.isEmpty()) {
		STTreeView *treeview=ui.treeview;
		for (int j=0; j<treeview->nodelist.count(); j++) {
			STMetaNode *N=treeview->nodelist[j];
			if (N->className()==parC->className()) {
				for (int k=0; k<N->childCount(); k++) {
					if (N->child(k)->name()==old_name) {
						N->child(k)->setName(new_name);
					}
				}
			}
		}
		parC->renameChild(old_name,new_name);
		slot_class_changed();	
	}
}

void STSequenceView::slot_change_node_type(STMetaNode *node) {
	if (!node) return;
	if (!m_sequence) return;
	STMetaNode *par=node->parent();
	if (!par) return;
	STMetaClass *C=m_sequence->metaClass(node->className());
	STMetaClass *parC=m_sequence->metaClass(par->className());
	
	if (!C) return;
	if (!parC) return;
	if (parC->isFoundationClass()) return;
	
	QString base_class_name;
	QString exclude_base_class_name;
	if (m_sequence->inherits(parC->className(),"STSequence")) {
		base_class_name="STLoop";
	}
	else if (m_sequence->inherits(parC->className(),"STLoop")) {
		base_class_name="STChain";
	}
	else {
		base_class_name="STNode";
		exclude_base_class_name="STLoop";
	}
	QStringList classnames=m_sequence->classesThatInherit(base_class_name,exclude_base_class_name);
	classnames.removeAll("STLoop");
	classnames.removeAll("STSequence");
	classnames.removeAll("STNode");
	classnames.removeAll("STChain");

	qSort(classnames);	
	QString new_class_name=
		stringchooserdlg::choose_string(this,classnames,"Change node type","New type:");
	if (!new_class_name.isEmpty()) {
		parC->changeChildType(node->name(),new_class_name);
		slot_class_changed();	
	}
}


void STSequenceView::slot_compile() {
	if (ui.codeview->isModified()) {
		ui.codeview->slotSaveChanges();
		qApp->processEvents();
	}
	m_sequence->update();
	m_simulator.compile();
	ui.tabWidget->setCurrentWidget(ui.parameters_tab);
	updateAllViews(false);
}
void STSequenceView::slot_compilation_started() {
	slot_compilation_message("Compiling...");
}
void STSequenceView::slot_compilation_message(QString msg) {
	ui.logtabwidget->setCurrentWidget(ui.compile_tab);
	ui.compile_log->addMessage(msg);
}
void STSequenceView::slot_compilation_finished() {
	m_simulator.computeStatistics();
	ui.simview->updatePlots();
	ui.nodeview->setCompiled(m_simulator.isCompiled());
	ui.treeview->update();
	ui.classview->updateClasses();
	update_status_info();
	emit compiledChanged();
}


void STSequenceView::add_simulate_message(QString msg,bool switch_tab) {
	if (switch_tab) ui.logtabwidget->setCurrentWidget(ui.simulate_tab);
	if (ui.simulate_log->count()<500) {
		QListWidgetItem *item=new QListWidgetItem(msg);
		ui.simulate_log->addItem(item);
		ui.simulate_log->scrollToBottom();
	}
	else {
		qDebug()  << msg;
	}
}


void STSequenceView::slot_simulation_started() {
	ui.simulate_log->clear();
	add_simulate_message("Simulating...",false);
}
void STSequenceView::slot_simulation_message(QString message) {
	QStringList messages=message.split("\n");
	foreach (QString message,messages) {
		message.remove(QChar(10));
		message.remove(QChar(13));
		message=message.trimmed();
		if (!message.isEmpty()) add_simulate_message(message,false);
	}
}

void STSequenceView::slot_block_changed(STSimScannerBlock *B) {
	ui.event_list->setBlock(B);
}

void STSequenceView::slot_simview_selected_events_changed(QSet<int> event_indices) {
	ui.logtabwidget->setCurrentWidget(ui.events_tab);
	ui.event_list->setSelectedEvents(event_indices);
	ui.event_list->setFocus();
}

void STSequenceView::slot_eventlist_selected_events_changed(QSet<int> event_indices) {
	ui.simview->setSelectedEvents(event_indices);
}

void STSequenceView::update_status_info() {
	QString msg;
	if (isCompiled())
		msg=QString("Duration = %1 seconds;  %2 blocks;  Rel. SAR = %3").arg(m_simulator.totalDuration()/1.0E6).arg(m_simulator.numBlocks()).arg(m_simulator.totalSAR());
	emit statusMessage(msg);
	if (isCompiled()) {
		slot_simulation_message(msg);
		ui.logtabwidget->setCurrentWidget(ui.simulate_tab);
	}
}

void STSequenceView::slot_current_event_changed(STSimScannerEvent event) {
	ui.treeview->setCurrentNode(event.tree_index);
}

void STSequenceView::slot_goto_source_line(QString fname,int line) {
	if (fname.indexOf("source-of-")==0) {
		QString cname=fname.mid(10);
		show_source_for_class(cname);
		ui.codeview->gotoSourceLine(line);
	}
	else if (fname.indexOf("header-of-")==0) {
		QString cname=fname.mid(10);
		show_source_for_class(cname);
		ui.codeview->gotoHeaderLine(line);
	}
}

void STSequenceView::slot_class_activated(QString class_name) {
	show_source_for_class(class_name);
}

void STSequenceView::slot_rename_class(QString class_name) {
	if (!m_sequence) return;
	QString new_name=QInputDialog::getText(this,"Rename Class","New class name:",QLineEdit::Normal,class_name);
	if (new_name.isEmpty()) return;
	if (new_name==class_name) return;
	m_sequence->renameClass(class_name,new_name);
	slot_class_changed();
}

void STSequenceView::slot_delete_class(QString class_name) {
	if (!m_sequence) return;
	if (ui.codeview->currentClassName()==class_name)
		ui.codeview->setClass(0);
	m_sequence->deleteClass(class_name);
	slot_class_changed();
}

void STSequenceView::slot_export_class(QString class_name) {
	if (!m_sequence) return;
	QString defaultstr=class_name.toLower()+".h";
	QString fname=QInputDialog::getText(this,"Export class",
						QString("Export %1 to file:").arg(class_name),QLineEdit::Normal,defaultstr);
	if (fname.isEmpty()) return;
	if (!m_sequence->exportClass(class_name,fname)) {
		QMessageBox::information(this,"Cannot export class","Cannot export class. Perhaps file with this name already exists.");
		return;
	}
	slot_class_changed();
}


void STSequenceView::slot_stat_finished(bool success) {
	Q_UNUSED(success);
	slot_simulation_message("Simulation finished.");
	update_status_info();
	ui.simview->updatePlots();
}

void STSequenceView::slot_run_finished(bool success) {
	Q_UNUSED(success);
	ui.nodeview->updateControls();
	slot_simulation_message("Simulation finished.");
	update_status_info();
}


void STSequenceView::slot_node_activated(STMetaNode *node) {
	if (!node) return;
	show_source_for_class(node->className());
}

void STSequenceView::slot_link_nodes(STMetaNode *N1,STMetaNode *N2) {
	if (!N1) return;
	if (!N2) return;
	if (!m_sequence) return;
	m_sequence->linkNodes(N1,N2);
	N2->pasteValuesFrom(*N1);
	ui.treeview->update();
}

void STSequenceView::slot_unlink_node(STMetaNode *N) {
	if (!N) return;
	if (!m_sequence) return;
	m_sequence->unlinkNode(N);
	ui.treeview->update();
}

void STSequenceView::slot_resource_activated(QString resource_name) {
	if (resource_name.isEmpty()) return;
	if (!m_sequence) return;
	if (resource_name.mid(resource_name.count()-4)==".mrp") {
		STResource *R_data_real=m_sequence->resource(resource_name+"->data_real");
		STResource *R_data_imag=m_sequence->resource(resource_name+"->data_imag");
		STResource *R_timestep=m_sequence->resource(resource_name+"->timestep");
		STResource *R_rephase_time=m_sequence->resource(resource_name+"->rephase_time");
		STResource *R_bandwidth=m_sequence->resource(resource_name+"->bandwidth");
		if ((!R_data_real)||(!R_data_imag)||(!R_timestep)||(!R_rephase_time)||(!R_bandwidth)) 
			return;
		if (R_data_real->doubleListCount()==0) return;
		if (R_data_real->doubleListCount()!=R_data_imag->doubleListCount()) return;		
		
		QStringList data_real_str;
		QStringList data_imag_str;
		for (long j=0; j<R_data_real->doubleListCount(); j++) {
			data_real_str << QString::number(R_data_real->getDoubleList(j));
			data_imag_str << QString::number(R_data_imag->getDoubleList(j));
		}			
		
		QString pulse_fname=ST_TMP_DIR+"/"+resource_name;
		QSettings pulse_file(pulse_fname,QSettings::IniFormat);
		pulse_file.clear();
		pulse_file.setValue("data_real",data_real_str);
		pulse_file.setValue("data_imag",data_imag_str);
		pulse_file.setValue("timestep",R_timestep->getDouble()/1000);
		pulse_file.setValue("rephase_time",R_rephase_time->getDouble()/1000);
		pulse_file.setValue("bandwidth",R_bandwidth->getDouble());
		emit runMRPulseTool(pulse_fname);
	}
	else {
		bool something_changed=false;
		STResource *R=m_sequence->resource(resource_name);
		if (!R) {
			R=new STResource(sstr(resource_name));
			m_sequence->addResource(R);
			something_changed=true;
		}
		EditResourceDlg dlg;
		dlg.setResource(R);
		if (dlg.exec()==QDialog::Accepted) {
			something_changed=true;
		}
		if (something_changed) {
			updateResources();			
			setModified(true);			
		}
	}
}

STMetaNode *STSequenceView::currentNode() {
	return ui.treeview->current_node();
}

void STSequenceView::updateAllViews(bool reset_compiled) {
	if (reset_compiled) {
		m_simulator.setCompiled(false);
		ui.nodeview->setCompiled(false);
	}
	m_sequence->update();
	ui.treeview->update();
	ui.classview->updateClasses();
	ui.codeview->update();
	ui.globalview->updateGlobalParameters();
	slot_current_node_changed(ui.treeview->current_node());
	m_simulator.computeStatistics();
	ui.simview->updatePlots();
	update_status_info();
}

void STSequenceView::updateBeforeSave() {
	if (ui.codeview->isModified()) {
		ui.codeview->slotSaveChanges();
		qApp->processEvents();
	}
	m_sequence->setNotes(ui.notes->toPlainText());
	m_sequence->setReconstructionCode(ui.reconstruction->toPlainText());
}

void STSequenceView::slot_minimize_parameter(QString pname) {
	m_simulator.minimizeParameter(ui.nodeview->node(),pname);
	slot_parameter_changed(pname);
}
void STSequenceView::slot_maximize_parameter(QString pname) {
	m_simulator.maximizeParameter(ui.nodeview->node(),pname);
	slot_parameter_changed(pname);
}

void STSequenceView::slot_link_to_global_parameter(QString pname) {
	if (!m_sequence) return;
	STMetaNode *N=ui.nodeview->node();
	if (!N) return;
	QString defaultval=pname;
	if (!N->parameter(pname).global_parameter.isEmpty()) defaultval=N->parameter(pname).global_parameter;
	bool ok;
	QString str=QInputDialog::getText(this,"Link to UI parameter","Name of UI parameter:",QLineEdit::Normal,defaultval,&ok);
	if (!ok) return;
	m_sequence->linkGlobalParameter(N,pname,str);
	setModified(true);
	updateAllViews(false);
	slot_parameter_changed(pname);
}

void STSequenceView::slot_unlink_global_parameter(QString pname) {
	if (!m_sequence) return;
	STMetaNode *N=ui.nodeview->node();
	if (!N) return;
	m_sequence->linkGlobalParameter(N,pname,"");
	setModified(true);
	updateAllViews(false);
	slot_parameter_changed(pname);
}

void STSequenceView::slot_global_parameter_value_changed(QString pname,QString pvalue) {
	if (!m_sequence) return;
	m_sequence->setGlobalParameterValue(pname,pvalue);
	setModified(true);
	updateAllViews(false);
}

void STSequenceView::slot_global_parameter_units_changed(QString pname,QString punits) {
	if (!m_sequence) return;
	m_sequence->setGlobalParameterUnits(pname,punits);
	setModified(true);
	ui.globalview->updateGlobalParameters();
}


void STSequenceView::slot_delete_global_parameter(QString pname) {
	if (!m_sequence) return;
	m_sequence->deleteGlobalParameter(pname);
	setModified(true);
	updateAllViews(false);
}

void STSequenceView::slot_move_global_parameter_up(QString pname) {
	m_sequence->moveGlobalParameterUp(pname);
	setModified(true);
	updateAllViews(false);
}
void STSequenceView::slot_move_global_parameter_down(QString pname) {
	m_sequence->moveGlobalParameterDown(pname);
	setModified(true);
	updateAllViews(false);
}

void STSequenceView::slot_change_class(QString cname) {
	show_source_for_class(cname);	
}


void STSequenceView::writeRawTemplateFile(QString fname) {
	m_simulator.writeRawTemplateFile(fname);
}

void STSequenceView::clearCompileLog() {
	ui.compile_log->clear();
}

void STSequenceView::updatePreferences() {
	ui.simview->updatePreferences();
}


