function a = iszero(M)
%ISZERO   True if a matrix is the zero matrix.
%   A = ISZERO(M) returns true if a matrix is the zero matrix,
%   false otherwise.
%
%   See also ISEMPTY.

a = isempty(find(M ~= 0));
