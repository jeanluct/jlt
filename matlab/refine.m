function fp = refine(f,N,dim)
%REFINE   Increase or decrease resolution of a real array using FFT.
%   REFINE(F,N) returns F inflated to length N by taking the FFT of the
%   data, padding with zeros, then inverting using the IFFT.  For matrices,
%   the refine operation is applied to each column. For N-D arrays, refine
%   operates on the first non-singleton dimension.
%
%   REFINE(F,N,DIM) applies the refine operation across the dimension DIM.
%
%   See also REFINE2.

if nargin < 3
  % First non-singleton dimension
  dim = min(find(size(f) ~= 1));
  if isempty(dim), dim = 1; end
end

sz = size(f);
N0 = sz(dim);
if sz(dim) == N, fp = f; return; end
sz(dim) = N;
fk = zeros(sz);

kmin0 = floor(-(N0-1)/2); kmax0 = floor((N0-1)/2);
kmin = floor(-(N-1)/2); kmax = floor((N-1)/2);

% Create vector of indices along each dimension and store in cell.
numDims = ndims(f);
f = fftshift(fft(f,[],dim),dim);
ic = cell(1,numDims);
for k = 1:numDims, if k ~= dim, ic{k} = 1:size(f,k); end; end
if N0 < N
  %
  % Upsampling
  %
  ic{dim} = [kmin0:kmax0]-kmin+1;
  fk(ic{:}) = f;
  % When N0 is even, symmetrize the Nyquist mode.
  if ~mod(N0,2)
    ic{dim} = 1-kmin - kmin0;
    ic2 = ic; ic2{dim} = 1-kmin + kmin0;
    fk(ic{:}) = conj(fk(ic2{:}));
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

fp = (N/N0)*ifft(ifftshift(fk,dim),[],dim,'symmetric');
