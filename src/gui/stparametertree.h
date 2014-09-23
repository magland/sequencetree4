#ifndef stparametertree_H
#define stparametertree_H

#include <QTreeWidget>
#include <QMouseEvent>

class STParameterTree : public QTreeWidget {
	Q_OBJECT
public:
	STParameterTree(QWidget *parent) : QTreeWidget(parent) {}
protected:
	void mousePressEvent(QMouseEvent *event) {
		QTreeWidget::mousePressEvent(event);
		if (event->button()==Qt::RightButton) {
			emit itemRightClicked(currentItem(),event->pos());
		}
	}
signals:
	void itemRightClicked(QTreeWidgetItem *item,QPoint pos);
};


#endif
