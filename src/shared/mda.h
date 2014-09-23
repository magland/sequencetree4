#ifndef mda_H
#define mda_H

#include <stdlib.h>
#include <stdio.h>
#include <QHash>

#define MAX_MDA_DIMS 50
#define MDA_COMMAND_MASK -3

#define MDA_TYPE_COMPLEX -1
#define MDA_TYPE_BYTE -2
#define MDA_TYPE_REAL -3
#define MDA_TYPE_SHORT -4
#define MDA_TYPE_INT32 -5
#define MDA_SPARSE -1001

#define VALID_WIDGET(X) (qApp->allWidgets().contains(X))

#include "complex.h"
typedef int integer;

/*#ifdef __LP64__
	typedef int int32;
#else
	typedef qint32 int32;
#endif*/

//make sure it doesn't try to import/export stuff
#define CHAINLINK_CORE_INTERFACE_EXPORT



class CHAINLINK_CORE_INTERFACE_EXPORT MdaElement : public AbstractComplex {
public:
	void *m_data;
	int m_data_type;
	
	real re() const;
	real im() const;
	void setRe(real val);
	void setIm(real val);
	
	void operator=(const MdaElement &X) {setRe(X.re()); setIm(X.im());}
	void operator=(const Complex &X) {setRe(X.re()); setIm(X.im());}
	void operator=(real val) {setRe(val); setIm(0);}

	
};

class Mda;
typedef Mda Vector;
typedef Mda Matrix;
typedef Mda RealMda;
typedef real NonnegativeReal;

struct complex_struct {
	real re,im;
};


class CHAINLINK_CORE_INTERFACE_EXPORT Mda {
	friend class MdaElement;
public:
	bool all_indices;
	Mda();
	Mda(const Mda &X);
	virtual ~Mda();
	void operator=(real val);
	void operator=(const AbstractComplex &val);
	void operator=(const Mda &X);
	void allocate(int datatype,qint32 S1,qint32 S2=-1,qint32 S3=-1,qint32 S4=-1,qint32 S5=-1,qint32 S6=-1,qint32 S7=-1,qint32 S8=-1);
	void allocate(int datatype,int num_dimensions,qint32 *dims);
	void allocate_sparse(int datatype,qint32 S1,qint32 S2=-1,qint32 S3=-1,qint32 S4=-1,qint32 S5=-1,qint32 S6=-1,qint32 S7=-1,qint32 S8=-1);
	void allocate_sparse(int datatype,int num_dimensions,qint32 *dims);
	bool isSparse() const {return m_is_sparse;}
	void clear(bool allocate_one_by_one=true);
	void setAll(const AbstractComplex &val);
	void setAll(real val);
	bool inRange(qint64 ind1,qint32 ind2=-1,qint32 ind3=-1,qint32 ind4=-1,qint32 ind5=-1,qint32 ind6=-1,qint32 ind7=-1,qint32 ind8=-1);
	MdaElement operator[](qint64 index);
	MdaElement operator()(qint64 ind1,qint32 ind2=-1,qint32 ind3=-1,qint32 ind4=-1,qint32 ind5=-1,qint32 ind6=-1,qint32 ind7=-1,qint32 ind8=-1);
	MdaElement operator()(qint32 *index);
	Complex get(qint64 ind1,qint32 ind2=-1,qint32 ind3=-1,qint32 ind4=-1,qint32 ind5=-1,qint32 ind6=-1,qint32 ind7=-1,qint32 ind8=-1);
	Complex get(qint32 *index);
	unsigned char &mask(qint64 ind1,qint32 ind2=-1,qint32 ind3=-1,qint32 ind4=-1,qint32 ind5=-1,qint32 ind6=-1,qint32 ind7=-1,qint32 ind8=-1);
	unsigned char &mask(qint32 *index);
	unsigned char getmask(qint64 ind1,qint32 ind2=-1,qint32 ind3=-1,qint32 ind4=-1,qint32 ind5=-1,qint32 ind6=-1,qint32 ind7=-1,qint32 ind8=-1);
	unsigned char getmask(qint32 *index);
	int dimCount() const;
	qint32 *dims() {return m_dims;}
	qint64 size() const;
	qint32 size(int dim) const;
	int dataType() const;
	void convertToComplex();
	qint32 sparseCount() const;
	QList<qlonglong> sparseIndices() const;
	
	bool read(const char *filename);
	bool read(FILE *inf);
	bool write(const char *filename);
	bool write(FILE *outf);
	
	///Shift with wrapping
	void shift(qint32 *shifts);
	void squeeze();
public:
	/*! Pointer to the actual data if dataType()==MDA_TYPE_BYTE*/
	unsigned char *data_byte;
	/*! Pointer to the actual data if dataType()==MDA_TYPE_SHORT*/
	short int *data_short;
	/*! Pointer to the actual data if dataType()==MDA_TYPE_INT32*/
	qint32 *data_int32;
	/*! Pointer to the actual data if dataType()==MDA_TYPE_REAL*/
	real *data_real;
	/*! Pointer to the actual data if dataType()==MDA_TYPE_COMPLEX*/
	complex_struct *data_complex;
	/*! Pointer to mask data. The mask is simply an array of unsigned characters with the same dimensions as the mda.*/
	unsigned char *data_mask;
private:
	int m_dim_count;
	qint32 m_dims[MAX_MDA_DIMS];
	qint64 m_dimprod;
	int m_data_type;
	
	bool m_is_sparse;
	QHash<qlonglong,unsigned char> data_byte_sparse;
	QHash<qlonglong,short> data_short_sparse;
	QHash<qlonglong,qint32> data_int32_sparse;
	QHash<qlonglong,real> data_real_sparse;
	QHash<qlonglong,complex_struct> data_complex_sparse;
	QHash<qlonglong,unsigned char> data_mask_sparse;
	
	void initialize(int datatype);
	void allocate_mask();
	qint64 get_index(qint64 ind1,qint32 ind2=-1,qint32 ind3=-1,qint32 ind4=-1,qint32 ind5=-1,qint32 ind6=-1,qint32 ind7=-1,qint32 ind8=-1);
	qint64 get_index(qint32 *index);
	void copy_from(const Mda &X);
};



#endif
