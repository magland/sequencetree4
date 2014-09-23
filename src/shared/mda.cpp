#include "mda.h"
#include <stdio.h>
#include <QDebug>

real MdaElement::re() const {
	switch (m_data_type) {
		case MDA_TYPE_BYTE: return *((unsigned char *)m_data); break;
		case MDA_TYPE_SHORT: return *((short *)m_data); break;
		case MDA_TYPE_INT32: return *((qint32 *)m_data); break;
		case MDA_TYPE_REAL: return *((real *)m_data); break;
		case MDA_TYPE_COMPLEX: return ((complex_struct *)m_data)->re; break;
	};
	return 0;
}
real MdaElement::im() const {
	if (m_data_type==MDA_TYPE_COMPLEX) {
		return ((complex_struct *)m_data)->im;
	}
	else return 0;
}


void MdaElement::setRe(real val) {
	switch (m_data_type) {
		case MDA_TYPE_BYTE: *((unsigned char *)m_data)=(unsigned char)val; break;
		case MDA_TYPE_SHORT: *((short *)m_data)=(short)val; break;
		case MDA_TYPE_INT32: *((qint32 *)m_data)=(qint32)val; break;
		case MDA_TYPE_REAL: *((real *)m_data)=(real)val; break;
		case MDA_TYPE_COMPLEX: ((complex_struct *)m_data)->re=val; break;
	};
}

void MdaElement::setIm(real val) {
	if (m_data_type==MDA_TYPE_COMPLEX) {
		((complex_struct *)m_data)->im=val;
	};
}

void Mda::initialize(int datatype) {
	data_byte=0;
	data_real=0;
	data_short=0;
	data_int32=0;
	data_complex=0;
	data_mask=0;
	m_is_sparse=false;
	allocate(datatype,1,1);
}

Mda::Mda() 
{
	all_indices=false;
	initialize(MDA_TYPE_REAL);
}
Mda::Mda(const Mda &X) {
	all_indices=false;
	initialize(MDA_TYPE_REAL);
	copy_from(X);
}

void Mda::operator=(real val) {
	allocate(MDA_TYPE_REAL,1,1);
	data_real[0]=val;
}
void Mda::operator=(const AbstractComplex &val) {
	allocate(MDA_TYPE_COMPLEX,1,1);
	data_complex[0].re=val.re();
	data_complex[0].im=val.im();
}

void Mda::operator=(const Mda &X) {
	copy_from(X);
}

Mda::~Mda()
{
	clear(false);
}

void Mda::allocate(int datatype,int num_dimensions,qint32 *dims) {
	clear(false);
	m_is_sparse=false;
	m_data_type=datatype;
	m_dim_count=num_dimensions;
	m_dimprod=0;
	if (num_dimensions<=0) return;
	m_dimprod=1;
	for (int j=0; j<num_dimensions; j++) {
		m_dimprod*=dims[j];
		m_dims[j]=dims[j];
	}
	switch (datatype) {
		case MDA_TYPE_BYTE: 
			data_byte=(unsigned char *)malloc(sizeof(unsigned char)*m_dimprod);
			break;
		case MDA_TYPE_SHORT: 
			data_short=(short int *)malloc(sizeof(short int)*m_dimprod);
			break;
		case MDA_TYPE_INT32: 
			data_int32=(qint32 *)malloc(sizeof(qint32)*m_dimprod);
			break;
		case MDA_TYPE_REAL: 
			data_real=(real *)malloc(sizeof(real)*m_dimprod);
			break;
		case MDA_TYPE_COMPLEX: 
			data_complex=(complex_struct *)malloc(sizeof(complex_struct)*m_dimprod);
			break;
	}
}

void Mda::allocate(int datatype,qint32 S1,qint32 S2,qint32 S3,qint32 S4,qint32 S5,qint32 S6,qint32 S7,qint32 S8) {
	qint32 dims[MAX_MDA_DIMS];
	int num_dimensions=0;
	if (S1>=0) {
		num_dimensions++;
		dims[0]=S1;
	if (S2>=0) {
		num_dimensions++;
		dims[1]=S2;
	if (S3>=0) {
		num_dimensions++;
		dims[2]=S3;
	if (S4>=0) {
		num_dimensions++;
		dims[3]=S4;
	if (S5>=0) {
		num_dimensions++;
		dims[4]=S5;
	if (S6>=0) {
		num_dimensions++;
		dims[5]=S6;
	if (S7>=0) {
		num_dimensions++;
		dims[6]=S7;
	if (S8>=0) {
		num_dimensions++;
		dims[7]=S8;
	}}}}}}}}
	allocate(datatype,num_dimensions,dims);
}

void Mda::allocate_sparse(int datatype,int num_dimensions,qint32 *dims) {
	clear(false);
	m_is_sparse=true;
	m_data_type=datatype;
	m_dim_count=num_dimensions;
	m_dimprod=0;
	if (num_dimensions<=0) return;
	m_dimprod=1;
	for (int j=0; j<num_dimensions; j++) {
		m_dimprod*=dims[j];
		m_dims[j]=dims[j];
	}
}

void Mda::allocate_sparse(int datatype,qint32 S1,qint32 S2,qint32 S3,qint32 S4,qint32 S5,qint32 S6,qint32 S7,qint32 S8) {
	qint32 dims[MAX_MDA_DIMS];
	int num_dimensions=0;
	if (S1>=0) {
		num_dimensions++;
		dims[0]=S1;
	if (S2>=0) {
		num_dimensions++;
		dims[1]=S2;
	if (S3>=0) {
		num_dimensions++;
		dims[2]=S3;
	if (S4>=0) {
		num_dimensions++;
		dims[3]=S4;
	if (S5>=0) {
		num_dimensions++;
		dims[4]=S5;
	if (S6>=0) {
		num_dimensions++;
		dims[5]=S6;
	if (S7>=0) {
		num_dimensions++;
		dims[6]=S7;
	if (S8>=0) {
		num_dimensions++;
		dims[7]=S8;
	}}}}}}}}
	allocate_sparse(datatype,num_dimensions,dims);
}


void Mda::setAll(const AbstractComplex &val) {
	qint32 N=size();
	for (int j=0; j<N; j++)
		(*this)[j]=val;
}
void Mda::setAll(real val) {
	qint32 N=size();
	for (int j=0; j<N; j++)
		(*this)[j]=val;
}

void Mda::clear(bool allocate_one_by_one) {
	data_byte_sparse.clear();
	data_short_sparse.clear();
	data_int32_sparse.clear();
	data_real_sparse.clear();
	data_complex_sparse.clear();
	if (data_byte) free(data_byte); data_byte=0;
	if (data_short) free(data_short); data_short=0;
	if (data_int32) free(data_int32); data_int32=0;
	if (data_real) free(data_real); data_real=0;
	if (data_complex) free(data_complex); data_complex=0;
	if (data_mask) free(data_mask); data_mask=0;
	m_data_type=0;
	m_dim_count=0;
	m_dimprod=0;
	m_is_sparse=false;
	if (allocate_one_by_one) allocate(MDA_TYPE_REAL,1,1);
}



MdaElement Mda::operator[](const qint64 index) {
	if (index<0) {
		qWarning() << "WARNING!!! Index is negative" << __FUNCTION__ << index;
	}
	MdaElement ret;
	ret.m_data_type=m_data_type;
	if (m_is_sparse) {
		switch (m_data_type) {
			case MDA_TYPE_BYTE: ret.m_data=&data_byte_sparse[index]; break;
			case MDA_TYPE_SHORT: ret.m_data=&data_short_sparse[index]; break;
			case MDA_TYPE_INT32: ret.m_data=&data_int32_sparse[index]; break;
			case MDA_TYPE_REAL: ret.m_data=&data_real_sparse[index]; break;
			case MDA_TYPE_COMPLEX: ret.m_data=&data_complex_sparse[index]; break;
			default: ret.m_data=0;
		};
	}
	else {
		switch (m_data_type) {
			case MDA_TYPE_BYTE: ret.m_data=&data_byte[index]; break;
			case MDA_TYPE_SHORT: ret.m_data=&data_short[index]; break;
			case MDA_TYPE_INT32: ret.m_data=&data_int32[index]; break;
			case MDA_TYPE_REAL: ret.m_data=&data_real[index]; break;
			case MDA_TYPE_COMPLEX: ret.m_data=&data_complex[index]; break;
			default: ret.m_data=0;
		};
	}
	return ret;
}

qint64 Mda::get_index(qint32 *index) {
	qint64 ret=0;
	qint64 factor=1;
	for (int j=0; j<dimCount(); j++) {
		ret+=index[j]*factor;
		factor*=size(j);
	}
	return ret;
}

qint64 Mda::get_index (qint64 ind1,qint32 ind2,qint32 ind3,qint32 ind4,qint32 ind5,qint32 ind6,qint32 ind7,qint32 ind8) {
	qint64 index=ind1;
	qint64 factor=1;
	if ((ind2>=0)&&(m_dim_count>=2)) {
		factor*=m_dims[0];
		index+=ind2*factor;
	if ((ind3>=0)&&(m_dim_count>=3)) {
		factor*=m_dims[1];
		index+=ind3*factor;
	if ((ind4>=0)&&(m_dim_count>=4)) {
		factor*=m_dims[2];
		index+=ind4*factor;
	if ((ind5>=0)&&(m_dim_count>=5)) {
		factor*=m_dims[3];
		index+=ind5*factor;
	if ((ind6>=0)&&(m_dim_count>=6)) {
		factor*=m_dims[4];
		index+=ind6*factor;
	if ((ind7>=0)&&(m_dim_count>=7)) {
		factor*=m_dims[5];
		index+=ind7*factor;
	if ((ind8>=0)&&(m_dim_count>=8)) {
		factor*=m_dims[6];
		index+=ind8*factor;
	}}}}}}}
	return index;
}

bool Mda::inRange(qint64 ind1,qint32 ind2,qint32 ind3,qint32 ind4,qint32 ind5,qint32 ind6,qint32 ind7,qint32 ind8) {
	if (ind2<0) return ((0<=ind1)&&(ind1<m_dimprod));
	if (m_dim_count>=1) {
		if ((ind1<0)||(ind1>=m_dims[0])) return false;
	}
	else return true;
	if (m_dim_count>=2) {
		if ((ind2<0)||(ind2>=m_dims[1])) return false;
	}
	else return true;
	if (m_dim_count>=3) {
		if ((ind3<0)||(ind3>=m_dims[2])) return false;
	}
	else return true;
	if (m_dim_count>=4) {
		if ((ind4<0)||(ind4>=m_dims[3])) return false;
	}
	else return true;
	if (m_dim_count>=5) {
		if ((ind5<0)||(ind5>=m_dims[4])) return false;
	}
	else return true;
	if (m_dim_count>=6) {
		if ((ind6<0)||(ind6>=m_dims[5])) return false;
	}
	else return true;
	if (m_dim_count>=7) {
		if ((ind7<0)||(ind7>=m_dims[6])) return false;
	}
	else return true;
	if (m_dim_count>=8) {
		if ((ind8<0)||(ind8>=m_dims[7])) return false;
	}
	else return true;
	return false;
}

MdaElement Mda::operator()(qint64 ind1,qint32 ind2,qint32 ind3,qint32 ind4,qint32 ind5,qint32 ind6,qint32 ind7,qint32 ind8) {
	qint64 index=get_index(ind1,ind2,ind3,ind4,ind5,ind6,ind7,ind8);
	if (index<0) {
		qWarning() << "Index is negative" << __FUNCTION__ << ind1 << ind2 << ind3 << ind4;
	}
	return (*this)[index];
}
MdaElement Mda::operator()(qint32 *index_in) {
	qint64 index=get_index(index_in);
	if (index<0) {
		qWarning() << "Index is negative" << __FUNCTION__ << index_in[0] << index_in[1] << index_in[2] << index_in[3];
	}
	return (*this)[index];
}
Complex Mda::get(qint64 ind1,qint32 ind2,qint32 ind3,qint32 ind4,qint32 ind5,qint32 ind6,qint32 ind7,qint32 ind8) {
	qint64 index;
	if (ind2<0) index=ind1;
	else index=get_index(ind1,ind2,ind3,ind4,ind5,ind6,ind7,ind8);
	if (index<0) return Complex(0,0);
	if (index>=m_dimprod) return Complex(0,0);
	if (isSparse()) {
		switch (m_data_type) {
			case MDA_TYPE_BYTE: 
				if (data_byte_sparse.contains(index))
					return Complex((real)data_byte_sparse[index],0);
				else 
					return Complex(0,0);
				break;
			case MDA_TYPE_SHORT: 
				if (data_short_sparse.contains(index))
					return Complex((real)data_short_sparse[index],0);
				else 
					return Complex(0,0);
				break;
			case MDA_TYPE_INT32: 
				if (data_int32_sparse.contains(index))
					return Complex((real)data_int32_sparse[index],0);
				else 
					return Complex(0,0);
				break;
			case MDA_TYPE_REAL: 
				if (data_real_sparse.contains(index))
					return Complex((real)data_real_sparse[index],0);
				else 
					return Complex(0,0);
				break;
			case MDA_TYPE_COMPLEX: 
				if (data_complex_sparse.contains(index))
					return Complex(data_complex_sparse[index].re,data_complex_sparse[index].im);
				else 
					return Complex(0,0);
				break;
		}
	}
	else {
		MdaElement elmt=(*this)[index];
		return Complex(elmt.re(),elmt.im());
	}
	return Complex(0,0);
}
Complex Mda::get(qint32 *index) {
	qint64 index2=get_index(index);
	return get(index2);
}

unsigned char &Mda::mask(qint64 ind1,qint32 ind2,qint32 ind3,qint32 ind4,qint32 ind5,qint32 ind6,qint32 ind7,qint32 ind8) {
	qint64 index=get_index(ind1,ind2,ind3,ind4,ind5,ind6,ind7,ind8);
	if (m_is_sparse) {
		return data_mask_sparse[index];
	}
	else {
		if (!data_mask) allocate_mask();
		return data_mask[index];
	}
}
unsigned char &Mda::mask(qint32 *index_in) {
	qint64 index=get_index(index_in);
	if (m_is_sparse) {
		return data_mask_sparse[index];
	}
	else {
		if (!data_mask) allocate_mask();
		return data_mask[index];
	}
}

unsigned char Mda::getmask(qint64 ind1,qint32 ind2,qint32 ind3,qint32 ind4,qint32 ind5,qint32 ind6,qint32 ind7,qint32 ind8) {
	qint64 index=get_index(ind1,ind2,ind3,ind4,ind5,ind6,ind7,ind8);
	if (m_is_sparse) {
		if (data_mask_sparse.contains(index))
			return data_mask_sparse[index];
		else return 0;
	}
	else {
		if (!data_mask) return 0;
		return data_mask[index];
	}
}
unsigned char Mda::getmask(qint32 *index_in) {
	qint64 index=get_index(index_in);
	return getmask(index);
}


int Mda::dimCount() const {
	return m_dim_count;
}
qint64 Mda::size() const {
	return m_dimprod;
}
qint32 Mda::size(int dim) const {
	if (dim>=m_dim_count) return 1;
	return m_dims[dim];
}

int Mda::dataType() const {
	return m_data_type;
}

void Mda::allocate_mask() {
	if (m_is_sparse) return;
	if ((!data_mask)&&(m_dim_count>0)&&(m_dimprod>0)) {
		data_mask=(unsigned char *)malloc(m_dimprod*sizeof(unsigned char));
		for (int j=0; j<m_dimprod; j++)
			data_mask[j]=0;
	}
}

bool Mda::read(const char *filename) {
	clear(true);
	FILE *inf=fopen(filename,"rb");
	if (!inf) return false;
	bool ret=read(inf);
	fclose(inf);
	return ret;
}
bool Mda::read(FILE *inf) {
	clear(true);
	if (!inf) return false;
	bool is_sparse=false;
	qint32 hold_num_dims;
	qint32 hold_dims[MAX_MDA_DIMS];
	if (fread(&hold_num_dims,sizeof(qint32),1,inf)<=0)
		return false;
	qint32 data_type;
	if (hold_num_dims<0) {
		data_type=hold_num_dims;
		if (data_type==MDA_SPARSE) {
			is_sparse=true;
			fread(&data_type,sizeof(qint32),1,inf);
		}
		qint32 num_bytes;
		fread(&num_bytes,sizeof(qint32),1,inf);
		fread(&hold_num_dims,sizeof(qint32),1,inf);
	}
	else
		data_type=MDA_TYPE_COMPLEX;
	if (hold_num_dims>MAX_MDA_DIMS) return false;
	if (hold_num_dims<=0) return false;
	qint32 j;
	for (j=0; j<hold_num_dims; j++) {
		qint32 holdval;
		fread(&holdval,sizeof(qint32),1,inf);
		hold_dims[j]=holdval;
	}
	if (is_sparse) {
		allocate_sparse(data_type,hold_num_dims,hold_dims);
		qint32 index_count;
		fread(&index_count,sizeof(qint32),1,inf);
		for (qint32 j=0; j<index_count; j++) {
			qint64 index;
			real val_re=0;
			real val_im=0;
			unsigned char val_mask=0;
			fread(&index,sizeof(qlonglong),1,inf);
			fread(&val_re,sizeof(real),1,inf);
			if (dataType()==MDA_TYPE_COMPLEX)
				fread(&val_im,sizeof(real),1,inf);
			fread(&val_mask,sizeof(unsigned char),1,inf);
			(*this)[index]=Complex(val_re,val_im);
			mask(index)=val_mask;
		}
	}
	else {
		allocate(data_type,hold_num_dims,hold_dims);
		qint32 N=size();
		float re,im;
		for (j=0; j<N; j++) {
			if ((data_type==MDA_TYPE_COMPLEX)&&(data_complex)) {
				fread(&re,sizeof(float),1,inf);
				fread(&im,sizeof(float),1,inf);
				data_complex[j].re=re;
				data_complex[j].im=im;
			}
			else if ((data_type==MDA_TYPE_REAL)&&(data_real))
				fread(&data_real[j],4,1,inf);
			else if ((data_type==MDA_TYPE_SHORT)&&(data_short))
				fread(&data_short[j],2,1,inf);
			else if ((data_type==MDA_TYPE_INT32)&&(data_int32))
				fread(&data_int32[j],4,1,inf);
			else if ((data_type==MDA_TYPE_BYTE)&&(data_byte))
				fread(&data_byte[j],1,1,inf);
		}
		qint32 com;
		if (fread(&com,sizeof(qint32),1,inf)) {
			if (com==MDA_COMMAND_MASK) {
				allocate_mask();
				for (j=0; j<N; j++)
					fread(&data_mask[j],sizeof(char),1,inf);
			}
		}
	}
	return true;
}

bool Mda::write(const char *filename) {
	FILE *outf=fopen(filename,"wb");
	if (!outf) return false;
	bool ret=write(outf);
	fclose(outf);
	return ret;
}
bool Mda::write(FILE *outf) {
	if (!outf) return false;
	if (m_data_type!=MDA_TYPE_COMPLEX) {
		qint32 data_type=m_data_type;
		if (isSparse()) {
			qint32 hold_sparse=MDA_SPARSE;
			fwrite(&hold_sparse,sizeof(qint32),1,outf);
		}
		fwrite(&data_type,sizeof(qint32),1,outf);
		qint32 num_bytes=1;
		switch (data_type) {
			case MDA_TYPE_BYTE: num_bytes=1; break;
			case MDA_TYPE_SHORT: num_bytes=2; break;
			case MDA_TYPE_INT32: num_bytes=4; break;
			case MDA_TYPE_REAL: num_bytes=4; break;
			case MDA_TYPE_COMPLEX: num_bytes=8; break;
		}
		fwrite(&num_bytes,sizeof(qint32),1,outf);
	}
	qint32 num_dims=m_dim_count;
	fwrite(&num_dims,sizeof(qint32),1,outf);
	qint32 j;
	for (j=0; j<num_dims; j++) {
		qint32 holdval=m_dims[j];
		fwrite(&holdval,sizeof(qint32),1,outf);
	}
	if (isSparse()) {
		QList<qlonglong> indices=sparseIndices();
		qint32 index_count=indices.count();
		fwrite(&index_count,sizeof(qint32),1,outf);
		foreach (qint64 index,indices) {
			fwrite(&index,sizeof(qlonglong),1,outf);
			real hold=get(index).re();
			fwrite(&hold,sizeof(real),1,outf);
			if (dataType()==MDA_TYPE_COMPLEX) {
				hold=get(index).im();
				fwrite(&hold,sizeof(real),1,outf);
			}
			unsigned char hold_mask=getmask(index);
			fwrite(&hold_mask,sizeof(unsigned char),1,outf);
		}
	}
	else {
		qint32 N=size();
		float re,im;
		for (j=0; j<N; j++) {
			if ((m_data_type==MDA_TYPE_COMPLEX)&&(data_complex)) {
				re=data_complex[j].re;
				im=data_complex[j].im;
				fwrite(&re,sizeof(float),1,outf);
				fwrite(&im,sizeof(float),1,outf);
			}
			else if ((m_data_type==MDA_TYPE_REAL)&&(data_real))
				fwrite(&(data_real)[j],4,1,outf);
			else if ((m_data_type==MDA_TYPE_SHORT)&&(data_short))
				fwrite(&(data_short)[j],2,1,outf);
			else if ((m_data_type==MDA_TYPE_INT32)&&(data_int32))
				fwrite(&(data_int32)[j],2,1,outf);
			else if ((m_data_type==MDA_TYPE_BYTE)&&(data_byte))
				fwrite(&(data_byte)[j],1,1,outf);
		}
		if (data_mask) {
			qint32 com=MDA_COMMAND_MASK;
			fwrite(&com,sizeof(qint32),1,outf);
			for (j=0; j<N; j++)
				fwrite(&data_mask[j],sizeof(char),1,outf);
		}
		else {
			qint32 com=0;
			fwrite(&com,sizeof(qint32),1,outf);
		}
	}
	return true;
}

void Mda::convertToComplex() {
	if (dataType()==MDA_TYPE_COMPLEX) return;
	
	if (m_is_sparse) {
		QList<qlonglong> keys;
		switch (dataType()) {
			case MDA_TYPE_BYTE:
				keys=data_byte_sparse.keys();
				foreach (qint32 key,keys) {
					data_complex_sparse[key].re=(real)data_byte_sparse[key];
					data_complex_sparse[key].im=0;
				}
				data_byte_sparse.clear();
				break;
			case MDA_TYPE_SHORT:
				keys=data_short_sparse.keys();
				foreach (qint32 key,keys) {
					data_complex_sparse[key].re=(real)data_short_sparse[key];
					data_complex_sparse[key].im=0;
				}
				data_short_sparse.clear();
				break;
			case MDA_TYPE_INT32:
				keys=data_int32_sparse.keys();
				foreach (qint32 key,keys) {
					data_complex_sparse[key].re=(real)data_int32_sparse[key];
					data_complex_sparse[key].im=0;
				}
				data_int32_sparse.clear();
				break;
			case MDA_TYPE_REAL:
				keys=data_real_sparse.keys();
				foreach (qint32 key,keys) {
					data_complex_sparse[key].re=(real)data_real_sparse[key];
					data_complex_sparse[key].im=0;
				}
				data_real_sparse.clear();
				break;
			
		}
	}
	else {
		data_complex=(complex_struct *)malloc(sizeof(complex_struct)*m_dimprod);	
		if (dataType()==MDA_TYPE_REAL) {
			for (qint32 j=0; j<m_dimprod; j++) {
				data_complex[j].re=data_real[j];
				data_complex[j].im=0;
			}
			free(data_real);
			data_real=0;
		}
		else if (dataType()==MDA_TYPE_BYTE) {
			for (qint32 j=0; j<m_dimprod; j++) {
				data_complex[j].re=(real)data_byte[j];
				data_complex[j].im=0;
			}
			free(data_byte);
			data_byte=0;
		}
		else if (dataType()==MDA_TYPE_SHORT) {
			for (qint32 j=0; j<m_dimprod; j++) {
				data_complex[j].re=(real)data_short[j];
				data_complex[j].im=0;
			}
			free(data_short);
			data_short=0;
		}
		else if (dataType()==MDA_TYPE_INT32) {
			for (qint32 j=0; j<m_dimprod; j++) {
				data_complex[j].re=(real)data_int32[j];
				data_complex[j].im=0;
			}
			free(data_int32);
			data_int32=0;
		}
		else {
			free(data_complex);
			data_complex=0;
			qWarning() << "Unexpected problem:" << __FILE__ << __LINE__ << __FUNCTION__;
			return;
		}
	}
	m_data_type=MDA_TYPE_COMPLEX;
}

void Mda::copy_from(const Mda &X) {
	clear(true);
	qint32 hold_dims[MAX_MDA_DIMS];
	for (int j=0; j<X.dimCount(); j++)
		hold_dims[j]=X.size(j);
	if (X.isSparse()) {
		allocate_sparse(X.dataType(),X.dimCount(),hold_dims);
		switch (X.dataType()) {
			case MDA_TYPE_BYTE: data_byte_sparse=X.data_byte_sparse; break;
			case MDA_TYPE_SHORT: data_short_sparse=X.data_short_sparse; break;
			case MDA_TYPE_INT32: data_int32_sparse=X.data_int32_sparse; break;
			case MDA_TYPE_REAL: data_real_sparse=X.data_real_sparse; break;
			case MDA_TYPE_COMPLEX: data_complex_sparse=X.data_complex_sparse; break;
		}
		data_mask_sparse=X.data_mask_sparse;
	}
	else {
		allocate(X.dataType(),X.dimCount(),hold_dims);
		switch (X.dataType()) {
			case MDA_TYPE_BYTE: memcpy(data_byte,X.data_byte,sizeof(unsigned char)*X.size());
											break;
			case MDA_TYPE_SHORT: memcpy(data_short,X.data_short,sizeof(short)*X.size());
											break;
			case MDA_TYPE_INT32: memcpy(data_int32,X.data_int32,sizeof(qint32)*X.size());
											break;
			case MDA_TYPE_REAL: memcpy(data_real,X.data_real,sizeof(real)*X.size());
											break;
			case MDA_TYPE_COMPLEX: memcpy(data_complex,X.data_complex,sizeof(complex_struct)*X.size());
											break;
		}
		if (X.data_mask) {
			allocate_mask();
			memcpy(data_mask,X.data_mask,sizeof(unsigned char)*X.size());
		}
	}
}

void Mda::shift(qint32 *shifts) {
	if (isSparse()) {
		qWarning() << "Cannot shift sparse array yet.";
		return;
	}
	Mda hold;
	hold=(*this);
	qint32 inds[MAX_MDA_DIMS],inds2[MAX_MDA_DIMS];
	qint32 j;
	for (j=0; j<m_dim_count; j++) {
		inds[j]=0;
		if (size(j)==0) return;
		while (shifts[j]<0) shifts[j]+=size(j);
		inds2[j]=(inds[j]+shifts[j])%size(j);
	}
	

	qint32 k;
	bool done=false;
	while (!done) {
		hold(inds2)=(*this)(inds);

		//increment
		k=-1;
		do {
			k++;
			if (k<m_dim_count) {
				inds[k]++;
				if (inds[k]>=m_dims[k]) {
					inds[k]=0;
				}
				inds2[k]=(inds[k]+shifts[k])%size(k);
			}
		} while ((k<hold.dimCount())&&(inds[k]==0));
		if (k>=hold.dimCount()) done=true;
	}
	

	copy_from(hold);
	
}

void Mda::squeeze() {
	qint32 new_dims[MAX_MDA_DIMS];
	qint32 old_dim_count=m_dim_count;
	qint32 j,ct=0;
	for (j=0; j<m_dim_count; j++) {
		if (m_dims[j]>1) {
			new_dims[ct]=m_dims[j];
			ct++;
		}
	}
	m_dim_count=ct;
	for (j=0; j<m_dim_count; j++) {
		m_dims[j]=new_dims[j];
	}
	for (j=m_dim_count; j<old_dim_count; j++)
		m_dims[j]=1;
	if (m_dim_count<1) {
		m_dim_count=1;
		m_dims[0]=1;
	}
}

qint32 Mda::sparseCount() const {
	if (!isSparse()) return m_dimprod;
	switch (dataType()) {
		case MDA_TYPE_BYTE: return data_byte_sparse.count(); break;
		case MDA_TYPE_SHORT: return data_short_sparse.count(); break;
		case MDA_TYPE_INT32: return data_int32_sparse.count(); break;
		case MDA_TYPE_REAL: return data_real_sparse.count(); break;
		case MDA_TYPE_COMPLEX: return data_complex_sparse.count(); break;
	}
	return 0;
}

QList<qlonglong> Mda::sparseIndices() const {
	if (!isSparse()) return QList<qlonglong>();
	switch (dataType()) {
		case MDA_TYPE_BYTE: return data_byte_sparse.keys(); break;
		case MDA_TYPE_SHORT: return data_short_sparse.keys(); break;
		case MDA_TYPE_INT32: return data_int32_sparse.keys(); break;
		case MDA_TYPE_REAL: return data_real_sparse.keys(); break;
		case MDA_TYPE_COMPLEX: return data_complex_sparse.keys(); break;
	}
	return QList<qlonglong>();
}



