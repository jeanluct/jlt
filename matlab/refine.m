function fr = refine(f,N,dim)
%REFINE   Increase or decrease resolution of a real array using FFT.
%   REFINE(F,N) returns F inflated to length N by taking the FFT of the
%   data, padding with zeros, then inverting using the IFFT.  For matrices,
%   the refine operation is applied to each column. For N-D arrays, refine
%   operates on the first non-singleton dimension.
%
%   REFINE(F,N,DIM) applies the refine operation across the dimension DIM.
%
%   See also REFINE2, REFINEK.

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

fr = ifft(refinek(fft(f,[],dim),N,dim),[],dim,'symmetric');
