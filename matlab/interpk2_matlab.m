function fx = interpk2_matlab(fk,x)
%INTERPK2   Interpolate given a 2D Fourier representation.
%   FX = INTERPK2(FK,[X Y]) returns F(X,Y) given the 2D Fourier
%   representation FK of the 2D-periodic real function F.  The array
%   FK is assumed to be in 'meshgrid' style, where the rows are Y and
%   the columns X.
%
%   See also FFT2, IFFT2.

% TODO: use real symmetry to cut down execution time.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

N = size(fk,1);
kmin = floor(-(N-1)/2);
kmax = floor( (N-1)/2);
k = [0:kmax kmin:-1];

ex = exp(2*pi*i * k' * x(2))*exp(2*pi*i * k * x(1));

% Now its just pointwise multiplication of the two arrays, which
% can be done in the same way in any dimensions.
fx = real(fk(:).'*ex(:))/N^2;
