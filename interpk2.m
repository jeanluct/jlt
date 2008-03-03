function fx = interpk2(fk,x)
%INTERPK2   Interpolate given a 2D Fourier representation.
%   FX = INTERPK2(FK,[X Y]) returns F(X,Y) given the 2D Fourier
%   representation FK of the 2D-periodic real function F.  The array
%   FK is assumed to be in 'meshgrid' style, where the rows are Y and
%   the columns X.
%
%   See also FFT2, IFFT2.

% $Id: interpk2.m,v 1.2 2006/02/25 15:34:06 jeanluc Exp $

% Implemented in C in interpk2.c.
