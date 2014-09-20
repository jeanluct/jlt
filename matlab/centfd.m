function D = centfd(deriv,N,ord,flag)
%CENTFD   Central finite-difference differentiation matrix.
%   D = CENTFD(DERIV,N,ORD,FLAG) returns the 2*ORDth order central
%   finite-difference matrix for the DERIVth derivative on N
%   gridpoints.  FLAG is used to specify the boundary conditions, and
%   can be 'zero' (default) or 'periodic'.  ORD defaults to 2.
%
%   See also DIFF, UFDWT.

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 4
  flag = 'zero';
end

if nargin < 3
  ord = 2;
end

pts = 2*ord+1;

w = ufdwt(1,pts,deriv);
w = w(ord+1,:);

switch flag
 case 'zero'
  w = repmat(w,[N 1]);
  D = spdiags(w,[-ord:ord],N,N);
 case 'periodic'
  w = [w(end-(ord-1):end) w w(1:ord)];
  w = repmat(w,[N 1]);
  D = spdiags(w,[-N+1:-N+ord -ord:ord N-ord:N-1],N,N);
 otherwise
  error('Unknown option.')
end
