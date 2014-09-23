#ifndef fftw_library_H
#define fftw_library_H

#include "mda.h"

/* ChainLink commands go here
CHAINLINK FUNCTION []=internal_fftshift(Mda X)
CHAINLINK FUNCTION []=internal_ifftshift(Mda X)
CHAINLINK FUNCTION []=internal_fftw(Mda X, integer dim, integer [do_inverse_transform])
CHAINLINK FUNCTION [Mda ret]=fftshift(Mda X)
CHAINLINK FUNCTION [Mda ret]=ifftshift(Mda X)
*/

bool internal_fftshift(Mda &X);
bool internal_ifftshift(Mda &X);
bool internal_fftw(Mda &X, integer dim, integer do_inverse_transform=false);
bool fftshift(Mda &ret, Mda &X);
bool ifftshift(Mda &ret, Mda &X);

#endif
