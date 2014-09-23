#include "vspropertyeditor.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QDebug>

VSPropertyEditor::VSPropertyEditor(QWidget *parent) : QTreeWidget(parent) 
{
	connect(this,SIGNAL(itemActivated(QTreeWidgetItem *,int)),this,SLOT(slot_item_activated(QTreeWidgetItem *,int)));
	connect(this,SIGNAL(itemChanged(QTreeWidgetItem *,int)),this,SLOT(slot_item_changed(QTreeWidgetItem *,int)));
	m_object=0;
}
void VSPropertyEditor::initialize() {
	QStringList labels; labels << "Property" << "Value";
	setHeaderLabels(labels);
}

VSPropertyEditor::~VSPropertyEditor()
{
}

void VSPropertyEditor::setObject(QObject *obj) {
	clear();
	m_object=obj;
	if (!obj) return;
	const QMetaObject *meta=obj->metaObject();
	for (int j=0; j<meta->propertyCount(); j++) {
		QString name=meta->property(j).name();
		//QVariant value=obj->property(name.toAscii().data());
		if (name!="objectName")
			addTopLevelItem(new VSPropertyEditorItem(m_object,name));
	}
	QList<QObject *> chlist=m_object->children();
	for (int j=0; j<chlist.count(); j++) {
		QObject *ch=chlist[j];
		QTreeWidgetItem *chitem=new QTreeWidgetItem;
		chitem->setText(0,ch->objectName());
		addTopLevelItem(chitem);
		const QMetaObject *chmeta=ch->metaObject();
		for (int j=0; j<chmeta->propertyCount(); j++) {
			QString name=chmeta->property(j).name();
			//QVariant value=ch->property(name.toAscii().data());
			if (name!="objectName")
				chitem->addChild(new VSPropertyEditorItem(ch,name));
		}
		chitem->setExpanded(true);
	}
	resizeColumnToContents(0);
	resizeColumnToContents(1);
}

void VSPropertyEditor::slot_item_activated(QTreeWidgetItem *item,int col) {
	col=1;
	Qt::ItemFlags holdflags=item->flags();
	item->setFlags(holdflags|Qt::ItemIsEditable);
	editItem(item,col);
	item->setFlags(holdflags);
}

void VSPropertyEditor::slot_item_changed(QTreeWidgetItem *item,int col) {
	if (!m_object) return;
	VSPropertyEditorItem *item2=(VSPropertyEditorItem *)item;
	if (!item2) return;
	if (!item2->object()) return;
	item2->object()->setProperty(item2->name().toAscii().data(),item2->value());
	update_all_values();
}

void VSPropertyEditor::update_all_values(QTreeWidgetItem *item) {
	if (!item) {
		for (int j=0; j<topLevelItemCount(); j++) {
			update_all_values(topLevelItem(j));
		}
	}
	else {
		if (item->childCount()>0) {
			for (int j=0; j<item->childCount(); j++) {
				update_all_values(item->child(j));
			}
		}
		else {
			VSPropertyEditorItem *item2=(VSPropertyEditorItem *)item;
			item2->reset_value();
		}
	}
}

