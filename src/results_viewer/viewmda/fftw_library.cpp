#include "fftw_library.h"
#include <QDebug>
#include "fftw3.h"

bool internal_fftshift(Mda &X) {
	qint32 shifts[MAX_MDA_DIMS];
	for (qint32 j=0; j<X.dimCount(); j++)
		shifts[j]=X.size(j)/2;
	X.shift(shifts);
	return true;
}

bool internal_ifftshift(Mda &X) {
	qint32 shifts[MAX_MDA_DIMS];
	for (qint32 j=0; j<X.dimCount(); j++)
		shifts[j]=(X.size(j)+1)/2;
	X.shift(shifts);
	return true;
}

bool internal_fftw(Mda &X, integer dim, integer do_inverse_transform) {
	int d1=(int)dim-1;
	qint64 N=X.size();
	if (!N) return false;

	if ((d1>=X.dimCount())||(d1<0)) {
		qWarning() << "Dimension out of range";
		return false;
	}

	fftw_complex *in, *out;
	fftw_plan p;

	in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

	int n[MAX_MDA_DIMS];
	qint32 j,k;
	n[0]=X.size(d1);
	int howmany=N/X.size(d1);
	int istride=1;
	int idist=X.size(d1);

	if (do_inverse_transform) {
		p = fftw_plan_many_dft(1, n, howmany,
							in, NULL,istride, idist,
							out, NULL,istride, idist,
							FFTW_BACKWARD, FFTW_ESTIMATE);
	}
	else {
		p = fftw_plan_many_dft(1, n, howmany,
							in, NULL,istride, idist,
							out, NULL,istride, idist,
							FFTW_FORWARD, FFTW_ESTIMATE);
	}

	qint32 ind[MAX_MDA_DIMS];
	bool done;
	qint32 ct;

	for (j=0; j<X.dimCount(); j++)
		ind[j]=0;
	done=false;
	ct=0;
	while (!done) {
		for (k=0; k<X.size(d1); k++) {
			ind[d1]=k;
			Complex val=X(ind);
			in[ct][0]=val.re();
			in[ct][1]=val.im();
			ct++;
		}

		//increment ind
		k=X.dimCount();
		do {
			k--;
			while (k==d1) k--;
			if ((k!=d1)&&(k>=0)) {
				ind[k]++;
				if (ind[k]>=X.size(k)) {
					ind[k]=0;
				}
			}
		} while ((k>=0)&&(ind[k]==0));
		if (k<0) done=true;
	} 

	//printf("fftw_execute...");
	fftw_execute(p);
	//printf("fftw_execute finished.");

	if (X.dataType()!=MDA_TYPE_COMPLEX) {
		//printf("Converting to complex...");
		X.convertToComplex();
		//printf("Done.");
	}

	for (j=0; j<X.dimCount(); j++)
		ind[j]=0;
	done=false;
	ct=0;
	while (!done) {
		for (k=0; k<X.size(d1); k++) {
			ind[d1]=k;
			X(ind)=Complex((real)out[ct][0],(real)out[ct][1]);
			ct++;
		}

		//increment ind
		k=X.dimCount();
		do {
			k--;
			while (k==d1) k--;
			if ((k!=d1)&&(k>=0)) {
				ind[k]++;
				if (ind[k]>=X.size(k)) {
					ind[k]=0;
				}
			}
		} while ((k>=0)&&(ind[k]==0));
		if (k<0) done=true;
	} 

	fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);

	if (do_inverse_transform) {
		real factor=1.0F/X.size(d1);
		for (qint64 j=0; j<X.size(); j++)
			X[j]=X[j]*factor;
	}
	return true;
}

bool fftshift(Mda &ret, Mda &X) {
	ret=X;
	internal_fftshift(ret);
	return true;
}

bool ifftshift(Mda &ret, Mda &X) {
	ret=X;
	internal_ifftshift(ret);
	return true;
}	
