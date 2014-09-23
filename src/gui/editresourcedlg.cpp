#include "editresourcedlg.h"
#include "stmetanode.h"

void EditResourceDlg::setResource(STResource *R) {
	m_resource=R;
	ui.double_list->setText("");
	setWindowTitle("Edit Resource");
	if (!R) return;
	setWindowTitle(QString("Edit Resource - %1").arg(qstr(R->name())));
	QString txt;
	for (long j=0; j<m_resource->doubleListCount(); j++) {
		txt.append( QString("%1\n").arg(m_resource->getDoubleList(j),0,'g',12) );
	}
	ui.double_list->setText(txt);
}

void EditResourceDlg::accept() {
	if (m_resource) {
		QStringList list=ui.double_list->toPlainText().split("\n",QString::SkipEmptyParts);
		if (list.count()==0) {
			m_resource->setDoubleList(0,0);
		}
		else {
			double *hold_data=(double *)malloc(sizeof(double)*list.count());
			for (long j=0; j<list.count(); j++)
				hold_data[j]=list[j].toDouble();
			m_resource->setDoubleList(list.count(),hold_data);
			free(hold_data);
		}
	}
	QDialog::accept();
}

