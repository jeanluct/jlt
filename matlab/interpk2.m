function fx = interpk2(fk,x)
%INTERPK2   Interpolate given a 2D Fourier representation.
%   FX = INTERPK2(FK,[X Y]) returns F(X,Y) given the 2D Fourier
%   representation FK of the 2D-periodic real function F.  The array
%   FK is assumed to be in 'meshgrid' style, where the rows are Y and
%   the columns X.
%
%   See also FFT2, IFFT2.

% Implemented in C in interpk2.c.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%
