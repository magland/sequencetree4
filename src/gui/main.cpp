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


#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QDir>

#include "stmainwindow.h"
#include "sstring.h"
#include "slist.h"
#include "stcommon.h"
//#include "update_code_from_web.h"



///////////////////////////////// ** ////////////////////////////////////
int main(int argc, char *argv[])
{
	
	QApplication app(argc, argv);
		
	QDir(ST_ROOT_DIR).mkdir("code");
	
	/*if (!QFile(ST_CODE_DIR+"/nodetypes/foundation/_filelist.txt").exists()) {
		qDebug() << "Updating code...";
		//update_code_from_web();
	}*/

	STMainWindow main_window;
	main_window.showMaximized();

	return app.exec();
}

