/****************************************************************************
** Generated by HaiQ
**
** Project name:  VS
** Project date:  Tue May 27 2008
** Author:
****************************************************************************/
#include <QApplication>
#include <QWidget>
#include "vsmainwindow.h"


///////////////////////////////// ** ////////////////////////////////////
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(":/images/vscanner.png"));

	VSMainWindow W;
	W.showNormal();

	if (argc-1>=1) {
		W.openSequence(argv[1]);
	}

	return app.exec();
}

