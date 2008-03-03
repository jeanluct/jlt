function fs = smoothk(f,cutoff,pfilt)
%SMOOTHK   Smooth data by filtering Fourier spectrum.
%   FS = SMOOTH(F,CUTOFF,PFILT) returns smoothed FS given a
%   function F.  CUTOFF (default 10) is the cutoff wavenumber, and
%   PFILT is the 'power' (flatness) of the filter (default 2).
%
%   See also SMOOTH3.

% $Id: smoothk.m,v 1.2 2008/03/02 19:52:30 jeanluc Exp $

if nargin < 3
  % Power (flatness) of filter
  pfilt = 2;
end

if nargin < 2
  % Cutoff wavenumber
  cutoff = 10;
end

% Apply filter to every dimension.  Not tested for numDims>2.

numDims = ndims(f);

fs = f;
sz = size(f);

for d = 1:numDims
  N = sz(d);
  kmin = floor(-(N-1)/2);
  kmax = floor( (N-1)/2);
  k = [0:kmax kmin:-1];
  %
  % Smooth
  %
  fk = fft(fs,[],d);
  % The filter
  filtk = exp(-(abs(k).^pfilt)/cutoff^pfilt);
  % Complicated rigmarole to replicate the filter along dimension d.
  % The problem is the singleton dimension of filtk.  I think
  % reshaping does the trick, but I haven't tested it for dimensions
  % greater than 2.
  szrep = sz; szrep(d) = 1;
  % Apply filter along dimension d.
  fkfilt = fk.*reshape(repmat(filtk,szrep),sz);
  fs = ifft(fkfilt,[],d,'symmetric');
end

% Renormalise so that max is at same level.
maxf = f; maxfs = fs;
for d = 1:numDims
  maxf = max(maxf);
  maxfs = max(maxfs);
end
fs = fs*maxf/maxfs;
