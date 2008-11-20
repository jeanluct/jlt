function V = pk(A)
%PK   Pack an array into a vector.
%   V = PK(A) returns the linear vector corresponding to the
%   array A.  This is equivalent to A(:).
%
%   See also UNPK.

% Just use Matlab's default linear numbering of arrays.
V = A(:);
