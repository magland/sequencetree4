#ifndef vspropertyeditor_H
#define vspropertyeditor_H
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class VSPropertyEditorItem : public QTreeWidgetItem {
public:
	VSPropertyEditorItem(QObject *object_in,QString name_in) {
		setText(0,name_in);
		m_object=object_in;
		m_name=name_in;
		reset_value();
	}
	QVariant value() {
		if (m_value.type()==QMetaType::Float)
			return text(1).toFloat();
		else return text(1);
	}
	QString name() {return m_name;}
	QObject *object() {return m_object;}
	void reset_value() {
		if (!m_object) return;
		m_value=m_object->property(m_name.toAscii().data());
		setText(1,m_value.toString());
	}
private:
	QObject *m_object;
	QString m_name;
	QVariant m_value;
	
};

class VSPropertyEditor : public QTreeWidget {
	Q_OBJECT
public:
	VSPropertyEditor(QWidget *parent=0);
	virtual ~VSPropertyEditor();
	void initialize();
	void setObject(QObject *obj);
private slots:
	void slot_item_activated(QTreeWidgetItem *,int);
	void slot_item_changed(QTreeWidgetItem *,int);
private:
	QObject *m_object;
	void update_all_values(QTreeWidgetItem *item=0);
};

#endif
