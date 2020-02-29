function fk = refinek2(f,M,N)
%REFINEK2   Increase or decrease resolution of an FFT of 2D real data.
%   REFINEK2(F,M,N) returns an M by N version of the 2D FFT array F,
%   inflated by padding with zeros, or truncated.
%
%   Care is taken to ensure that the returned array is still the FFT of real
%   data.
%
%   REFINEK2(F,N) returns an N by N array.
%
%   See also REFINE, REFINEK, REFINE2.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 3, N = M; end

% Apply refine to each dimension.  (See doc fft.)
fk = refinek(refinek(f,M,1),N,2);
