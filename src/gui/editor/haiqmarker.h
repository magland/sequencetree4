/****************************************************************************
 This file is part of HaiQ
 Copyright (C) 2007-2008 Jeremy Magland (Jeremy.Magland@gmail.com)

 HaiQ is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 HaiQ is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with HaiQ; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#ifndef haiq_marker_H
#define haiq_marker_H

#define MARKER_TYPE_TEMPORARY 1
#define MARKER_TYPE_FIND_STRING 2
#define MARKER_TYPE_BUILD_ERROR 3
#define MARKER_TYPE_BUILD_WARNING 4
#define MARKER_TYPE_BOOKMARK 5
#define MARKER_TYPE_BREAKPOINT 6
#define MARKER_TYPE_DEBUG_POSITION 7

#include <QString>

#include "haiqglobal.h"

struct haiq_marker {
	QString id;
	QString path;
	int line_number;
	int marker_type;
	QString description;
};

void set_random_id(haiq_marker &marker);

#endif
