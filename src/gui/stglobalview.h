#ifndef stglobalview_H
#define stglobalview_H

#include <QTreeWidget>
#include <QMenu>
#include "stmetasequence.h"

class STGlobalViewItem : public QTreeWidgetItem {
public:
	STGlobalViewItem(STGlobalParameter *GP);
	QString name() {return m_name;}
	QString value() {return m_value;}
	QString units() {return m_units;}
private:
	QString m_name,m_value,m_units;
};

class STGlobalViewTree : public QTreeWidget {
	Q_OBJECT
public:
	STGlobalViewTree(QWidget *parent);
	virtual ~STGlobalViewTree();
	void initialize();
signals:
	void mouseRightClick(QPoint pos);
protected:
	void mousePressEvent ( QMouseEvent * event );
};

#include "ui_stglobalview.h"
class STGlobalView : public QWidget {
	Q_OBJECT
private:
	Ui::STGlobalView ui;
public:
	STGlobalView(QWidget *parent);
	virtual ~STGlobalView();
	void setSequence(STMetaSequence *Seq);
	void updateGlobalParameters();
	void setEditSequenceMode(bool val);
signals:
	void globalParameterValueChanged(QString name,QString value);
	void globalParameterUnitsChanged(QString name,QString units);
	void deleteGlobalParameter(QString name);
	void moveGlobalParameterUp(QString name);
	void moveGlobalParameterDown(QString name);
private slots:
	void slot_item_activated(QTreeWidgetItem *item,int col);
	void slot_item_changed(QTreeWidgetItem *item,int col);
	void slot_delete_global_parameter();
	void slot_mouse_right_click(QPoint pos);
	void slot_move_global_parameter_up();
	void slot_move_global_parameter_down();
private:
	STMetaSequence *m_sequence;
	QMenu m_popup;
	bool m_edit_sequence_mode;
};

#endif
