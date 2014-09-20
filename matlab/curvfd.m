function k = curvfd(s,l)
%CURVFD   Find curvature of a line by finite differences.
%   K = CURVFD(S,L) returns the curvature of the line with
%   arc length S and unit tangent L, computed by
%   finite-differencing.
%

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

nl = size(l,1);
n = size(l,2);

i = [2:nl-1];
i2 = [2:nl];

% Allocate arrays for curvature.
% Lose endpoints.
k = zeros(nl-2,n);
delt = zeros(nl-1,1);
delt2 = zeros(nl-1,1);

delt(i2-1) = s(i2) - s(i2-1);
delt2 = delt.^2;

for j = 1:n
  % Central difference.  See Ferziger and Peric p. 46.
  k(i-1,j) = (l(i+1,j).*delt2(i-1) - l(i-1,j).*delt2(i) + ...
	      l(i,j).*(delt2(i) - delt2(i-1))) ./ ...
      (delt(i).*delt(i-1).*(delt(i-1) + delt(i)));
end
