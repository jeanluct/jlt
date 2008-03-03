function a = iszero(M)
%ISZERO   True if a matrix is the zero matrix.
%   A = ISZERO(M) returns true if a matrix is the zero matrix,
%   false otherwise.
%
%   See also ISEMPTY.

% $Id: iszero.m,v 1.1 2007/03/18 12:31:11 jeanluc Exp $

a = isempty(find(M ~= 0));
