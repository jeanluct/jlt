function fi = interpp(f,X)
%INTERPP  Interpolate a 1D periodic function.
%   FI = INTERPP(F,X) returns F(X) given the function F by
%   using linear interpolation. F is assumed period-1.
%
%   See also INTERPP2, INTERPK2.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

N = length(f);

% Find which f interval X lies in.
X = N*mod(X,1);
ix1 = floor(X)+1;
ix2 = ix1+1;

% Take periodicity into account.
ix2(find(ix1 == N)) = 1;

% Special case: if X is very small and negative, then
% N*mod(X,1) gives N, rather than N*(1+X), so the floor
% function fails to round down to N-1.
%% I can't reproduce this anymore... maybe it was a bug and it's gone?
ixNp1 = find(ix1 == N+1);
ix2(ixNp1) = 1;
ix1(ixNp1) = N;

% The function at the two endpoints of the interval.
f1 = reshape(f(ix1),size(X));
f2 = reshape(f(ix2),size(X));

fi = (f2-f1).*(X-ix1) + f2;
