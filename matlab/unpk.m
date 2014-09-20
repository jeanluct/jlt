function A = unpk(V,sz)
%UNPK   Unpack a vector into an array.
%   A = UNPK(V) returns a square matrix of size N*N corresponding
%   to the vector V of length N^2.
%
%   A = UNPK(V,SIZE), where SIZE is a vector of integers, unpacks
%   the vector V of length prod(SIZE) into an array with dimensions
%   SIZE.
%
%   See also PK.

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 2
  % Assume square matrix.
  M = sqrt(length(V));
  if (M ~= floor(M))
    error('Specify size with UNPK(V,SIZE), or use V of length N^2.')
  end
  sz = [M,M];
end

A = zeros(sz);

A(:) = V(:);
