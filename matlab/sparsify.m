function As = sparsify(A,zcutoff)
%SPARSIFY   Trim near-zero elements.
%   AS = SPARSIFY(A,ZCUTOFF) makes a sparse matrix AS from A by
%   dropping elements that are less that ZCUTOFF in absolute
%   value.  If omitted, ZCUTOFF defaults to 1e-8 * max(max(abs(A))).

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 2
  zcutoff = 1e-8 * full(max(max(abs(A))));
end

% Get the nonzero elements.
[i,j,v] = find(A);
% Find the elements with magnitude above cutoff.
ii = find(abs(v) > zcutoff);
As = sparse(i(ii),j(ii),v(ii),size(A,1),size(A,2));
