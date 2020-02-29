function a = iszero(M)
%ISZERO   True if a matrix is the zero matrix.
%   A = ISZERO(M) returns true if a matrix is the zero matrix,
%   false otherwise.
%
%   See also ISEMPTY.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

a = isempty(find(M ~= 0));
