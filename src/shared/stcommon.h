#ifndef stcommon_H
#define stcommon_H

#include <QApplication>

#define ST_ROOT_DIR (qApp->applicationDirPath()+"/..")
#define ST_BIN_DIR (qApp->applicationDirPath())
#define ST_FOUNDATION_DIR (qApp->applicationDirPath()+"/../code/nodetypes/foundation")
#define ST_USER_DIR (qApp->applicationDirPath()+"/../code/nodetypes/user")
#define ST_TMP_DIR (qApp->applicationDirPath()+"/../tmp")
#define ST_SIMULATOR_DIR (qApp->applicationDirPath()+"/../simulator")
#define ST_TEMPLATES_DIR (qApp->applicationDirPath()+"/../templates")
#define ST_EXPORT_DIR (qApp->applicationDirPath()+"/../export")
#define ST_SRC_DIR (qApp->applicationDirPath()+"/../src")
#define ST_FRAMEWORK_DIR (qApp->applicationDirPath()+"/../code/framework")
#define ST_CODE_DIR (qApp->applicationDirPath()+"/../code")

#endif
