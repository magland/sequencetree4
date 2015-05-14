#ifndef stcommon_H
#define stcommon_H

#include <QApplication>

inline QString application_dir_path() {
	QString ret=qApp->applicationDirPath();
	int ind0=ret.indexOf("st4.app");
	if (ind0>=1) {
		ret=ret.mid(0,ind0-1);
	}
	return ret;
}

#define ST_ROOT_DIR (application_dir_path()+"/..")
#define ST_BIN_DIR (application_dir_path())
#define ST_FOUNDATION_DIR (application_dir_path()+"/../code/nodetypes/foundation")
#define ST_USER_DIR (application_dir_path()+"/../code/nodetypes/user")
#define ST_TMP_DIR (application_dir_path()+"/../tmp")
#define ST_SIMULATOR_DIR (application_dir_path()+"/../simulator")
#define ST_TEMPLATES_DIR (application_dir_path()+"/../templates")
#define ST_EXPORT_DIR (application_dir_path()+"/../export")
#define ST_SRC_DIR (application_dir_path()+"/../src")
#define ST_FRAMEWORK_DIR (application_dir_path()+"/../code/framework")
#define ST_CODE_DIR (application_dir_path()+"/../code")

#endif
