#include "vsbasicphantoms.h"
#include <QStringList>

QString vector2_to_string(float x,float y) {
	return QString("(%1,%2)").arg(x).arg(y);
}
void string_to_vector2(QString val,float &x,float &y) {
	val.remove("(");
	val.remove(")");
	val.remove(" ");
	QStringList list=val.split(",");
	x=list.value(0).toFloat();
	y=list.value(1).toFloat();
}

QString vector3_to_string(float x,float y,float z) {
	return QString("(%1,%2,%3)").arg(x).arg(y).arg(z);
}
void string_to_vector3(QString val,float &x,float &y,float &z) {
	val.remove("(");
	val.remove(")");
	val.remove(" ");
	QStringList list=val.split(",");
	x=list.value(0).toFloat();
	y=list.value(1).toFloat();
	z=list.value(2).toFloat();
}
