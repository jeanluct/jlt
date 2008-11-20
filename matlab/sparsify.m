function As = sparsify(A,zcutoff)
%SPARSIFY   Trim near-zero elements.
%   AS = SPARSIFY(A,ZCUTOFF) makes a sparse matrix AS from A by
%   dropping elements that are less that ZCUTOFF in absolute
%   value.  If omitted, ZCUTOFF defaults to 1e-8 * max(max(abs(A))).

if nargin < 2
  zcutoff = 1e-8 * max(max(abs(A)));
end

% Find the nonzero elements.
idx = find(abs(A) > zcutoff);

% Allocate storage for sparse matrix.
sz = size(A);
As = spalloc(sz(1),sz(2),length(idx));

As(idx) = sparse(A(idx));
