#include "stglobalview.h"
#include <QMouseEvent>

STGlobalViewItem::STGlobalViewItem(STGlobalParameter *GP) {
	if (!GP) return;
	m_name=GP->name;
	m_value=GP->value;
	m_units=GP->units;
	setText(0,GP->name);
	setText(1,GP->value);
	setText(2,GP->units);
	setIcon(0,QIcon(":/images/greenball.bmp"));
	
}

STGlobalViewTree::STGlobalViewTree(QWidget *parent) : QTreeWidget(parent) {
}
STGlobalViewTree::~STGlobalViewTree() {
}
void STGlobalViewTree::initialize() {
	QStringList labels; labels << "Parameter" << "Value" << "Units";
	setHeaderLabels(labels);
}
void STGlobalViewTree::mousePressEvent ( QMouseEvent * event ) {
	QTreeWidget::mousePressEvent(event);
	if (event->button()==Qt::RightButton) {
		emit mouseRightClick(event->pos());
	}
}

STGlobalView::STGlobalView(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	ui.tree->initialize();
	m_sequence=0;
		
	connect(ui.tree,SIGNAL(itemActivated(QTreeWidgetItem *,int)),this,SLOT(slot_item_activated(QTreeWidgetItem *,int)));
	connect(ui.tree,SIGNAL(itemChanged(QTreeWidgetItem *,int)),this,SLOT(slot_item_changed(QTreeWidgetItem *,int)));
	connect(ui.tree,SIGNAL(mouseRightClick(QPoint)),this,SLOT(slot_mouse_right_click(QPoint)));
	
	connect(ui.delete_button,SIGNAL(clicked()),this,SLOT(slot_delete_global_parameter()));
	connect(ui.up_button,SIGNAL(clicked()),this,SLOT(slot_move_global_parameter_up()));
	connect(ui.down_button,SIGNAL(clicked()),this,SLOT(slot_move_global_parameter_down()));
	
	m_popup.addAction("Remove global parameter",this,SLOT(slot_delete_global_parameter()));
	m_edit_sequence_mode=true;
}

STGlobalView::~STGlobalView()
{
}

void STGlobalView::setSequence(STMetaSequence *Seq) {
	m_sequence=Seq;
}
void STGlobalView::updateGlobalParameters() {
	QString current_string;
	if (ui.tree->currentItem()) current_string=ui.tree->currentItem()->text(0);
	ui.tree->clear();
	if (!m_sequence) return;
	for (int j=0; j<m_sequence->globalParameterCount(); j++) {
		STGlobalParameter *GP=m_sequence->globalParameter(j);
		ui.tree->addTopLevelItem(new STGlobalViewItem(GP));
	}
	if (!current_string.isEmpty()) {
		for (int j=0; j<ui.tree->topLevelItemCount(); j++) {
			if (ui.tree->topLevelItem(j)->text(0)==current_string)
				ui.tree->setCurrentItem(ui.tree->topLevelItem(j));
		}
	}
	ui.tree->resizeColumnToContents(0);
}

void STGlobalView::slot_item_activated(QTreeWidgetItem *item,int col) {
	if ((col!=2)||(!m_edit_sequence_mode)) col=1;
	Qt::ItemFlags holdflags=item->flags();
	item->setFlags(holdflags|Qt::ItemIsEditable);
	ui.tree->editItem(item,col);
	item->setFlags(holdflags);
}

void STGlobalView::slot_item_changed(QTreeWidgetItem *item,int col) {
	STGlobalViewItem *item2=(STGlobalViewItem *)item;
	if (!item2) return;
	if (col==1) {
		QString compare1=item2->value(); compare1.remove(' ');
		QString compare2=item->text(1); compare2.remove(' ');
		if (compare1==compare2) return; //nothing has changed
		emit globalParameterValueChanged(item2->name(),item->text(1));
	}
	else if (col==2) {
		QString compare1=item2->units(); compare1.remove(' ');
		QString compare2=item->text(2); compare2.remove(' ');
		if (compare1==compare2) return; //nothing has changed
		emit globalParameterUnitsChanged(item2->name(),item->text(2));
	}
}

void STGlobalView::slot_delete_global_parameter() {
	STGlobalViewItem *item2=(STGlobalViewItem *)ui.tree->currentItem();
	if (!item2) return;
	emit deleteGlobalParameter(item2->name());
}
void STGlobalView::slot_move_global_parameter_up() {
	STGlobalViewItem *item2=(STGlobalViewItem *)ui.tree->currentItem();
	if (!item2) return;
	emit moveGlobalParameterUp(item2->name());
}
void STGlobalView::slot_move_global_parameter_down() {
	STGlobalViewItem *item2=(STGlobalViewItem *)ui.tree->currentItem();
	if (!item2) return;
	emit moveGlobalParameterDown(item2->name());
}


void STGlobalView::slot_mouse_right_click(QPoint pos) {
	if (m_edit_sequence_mode) m_popup.popup(mapToGlobal(pos));
}

void STGlobalView::setEditSequenceMode(bool val) {
	m_edit_sequence_mode=val;
	ui.button_frame->setVisible(val);
}

