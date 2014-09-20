function fx = interpk(fk,x,L)
%INTERPK   Interpolate given a Fourier representation.
%   FX = INTERPK(FK,X) returns F(X) given the Fourier representation
%   FK of the real periodic function F.
%
%   FX = INTERPK(FK,[X Y]) returns F(X,Y) given the 2D Fourier
%   representation FK of the 2D-periodic real function F.  The array
%   FK is assumed to be in 'meshgrid' style, where the rows are Y and
%   the columns X.
%
%   FX = INTERPK(FK,X,L) specifies the domain size L, which
%   defaults to 1.
%
%   FX = INTERPK(FK,[X Y],[LX LY]) specifies the domain sizes LX and
%   LY, which default to 1 and 1.  FX = INTERPK(FK,[X Y],L)
%   specifies LX = LY = L.
%
%   See also FFT, FFT2.

% TODO: use real symmetry to cut down execution time.

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

N = size(fk,1);
kmin = floor(-(N-1)/2);
kmax = floor( (N-1)/2);
k = [0:kmax kmin:-1];

nd = ndims(fk);
if nd == 2 & (size(fk,1) == 1 | size(fk,2) == 1)
  % NDIMS returns at least two, unfortunately, so adjust nd.
  nd = 1;
end

if nargin < 3
  if nd == 1
    L = 1;
  elseif nd == 2
    L(1) = 1;
    L(2) = 1;
  else
    error('Too many dimensions.')
  end
elseif nargin == 3
  if nd == 2 & max(size(L)) == 1
    % If only a number is given in 2D, make the periodicities the same.
    L(2) = L(1);
  end
end

if nd == 1
  ex = exp(2*pi*i/L * k' * x);
elseif ndims(fk) == 2
  % Note that the array fk is assumed to be in 'meshgrid' style,
  % where the rows are Y and the columns X.
  ex = exp(2*pi*i/L(2) * k' * x(2))*exp(2*pi*i/L(1) * k * x(1));
else
  error('Too many dimensions.')
end

% Now its just pointwise multiplication of the two arrays, which
% can be done in the same way in any dimensions.
fx = real(fk(:).'*ex(:))/N^nd;
