function fp = refine(f,N,dim)
%REFINE   Increase resolution of an array using FFT.
%   REFINE(F,N) returns F inflated to length N by taking the FFT of the
%   data, padding with zeros, then inverting using the IFFT.  For matrices,
%   the refine operation is applied to each column. For N-D arrays, refine
%   operates on the first non-singleton dimension.
%
%   REFINE(F,N,DIM) applies the refine operation across the dimension DIM.
%
%   See also REFINE2.

% TODO: Downsampling.

if nargin < 3
  % First non-singleton dimension
  dim = min(find(size(f) ~= 1));
  if isempty(dim), dim = 1; end
end

sz = size(f);
N0 = sz(dim);
if sz(dim) > N
  error('No downsampling in refine');
elseif sz(dim) == N
  fp = f; return
end
sz(dim) = N;
fk = zeros(sz);

kmin0 = floor(-(N0-1)/2); kmax0 = floor( (N0-1)/2);
kmin = floor(-(N-1)/2); k0 = [kmin0:kmax0];

% Create vector of indices along each dimension and store in cell.
numDims = ndims(f);
ic = cell(1,numDims);
for k = 1:numDims
  nd = size(fk,k);
  if k ~= dim
    ic{k} = 1:nd;
  else
    ic{k} = k0-kmin+1;   % The dimension we operate on.
  end
end

fk(ic{:}) = fftshift(fft(f,[],dim),dim);

% When N0 is even, need to copy the Nyquist mode.
if ~mod(N0,2)
   ic{dim} = 1-kmin - kmin0;
   ic2 = ic; ic2{dim} = 1-kmin + kmin0;
   fk(ic{:}) = conj(fk(ic2{:}));
end

fp = (N/N0)*ifft(ifftshift(fk,dim),[],dim,'symmetric');
