function V = pk(A)
%PK   Pack an array into a vector.
%   V = PK(A) returns the linear vector corresponding to the
%   array A.  This is equivalent to A(:).
%
%   See also UNPK.

% $Id: pk.m,v 1.2 2007/05/19 09:25:03 jeanluc Exp $

% Just use Matlab's default linear numbering of arrays.
V = A(:);
