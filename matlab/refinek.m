function fk = refinek(f,N,dim)
%REFINEK   Increase or decrease resolution of an FFT of real data.
%   REFINEK(F,N) returns F inflated to length N by padding the FFT of real
%   data with zeros, or truncated if N is smaller than the original
%   dimension of F.  For matrices, the refinement is applied to each
%   column. For N-D arrays, REFINEK operates on the first non-singleton
%   dimension.
%
%   Care is taken to ensure that the returned array is still the FFT of real
%   data.
%
%   REFINEK(F,N,DIM) applies the refinement operation across the dimension
%   DIM.
%
%   See also REFINEK2, REFINE, REFINE2.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 3
  % First non-singleton dimension
  dim = min(find(size(f) ~= 1));
  if isempty(dim), dim = 1; end
end

sz = size(f);
N0 = sz(dim);
if sz(dim) == N, fk = f; return; end
sz(dim) = N;
fk = zeros(sz);

kmin0 = floor(-(N0-1)/2); kmax0 = floor((N0-1)/2);
kmin = floor(-(N-1)/2); kmax = floor((N-1)/2);

% Create vector of indices along each dimension and store in cell.
numDims = ndims(f);
f = fftshift(f,dim);
ic = cell(1,numDims);
for k = 1:numDims, if k ~= dim, ic{k} = 1:size(f,k); end; end
if N0 < N
  %
  % Upsampling
  %
  ic{dim} = [kmin0:kmax0]-kmin+1;
  fk(ic{:}) = f;
  % When N0 is even, zero the old Nyquist mode.  This is better than
  % symmetrizing, since we want to guarantee that we've padded with N-N0
  % zeros.
  if ~mod(N0,2)
    ic{dim} = 1-kmin + kmin0;
    fk(ic{:}) = 0;
  end
else
  %
  % Downsampling
  %
  ic{dim} = [kmin:kmax]-kmin0+1;
  fk = f(ic{:});
  % When N is even, make the Nyquist mode real.
  if ~mod(N,2)
    ic{dim} = 1;
    fk(ic{:}) = real(fk(ic{:}));
  end
end

fk = (N/N0)*ifftshift(fk,dim);
