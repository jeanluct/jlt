function V = pk(A)
%PK   Pack an array into a vector.
%   V = PK(A) returns the linear vector corresponding to the
%   array A.  This is equivalent to A(:).
%
%   See also UNPK.

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

% Just use Matlab's default linear numbering of arrays.
V = A(:);
