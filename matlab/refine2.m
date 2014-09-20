function fr = refine2(f,M,N)
%REFINE2   Increase or decrease resolution of a 2D real array using FFT.
%   REFINE2(F,M,N) returns an M by N version of the 2D array F, inflated by
%   taking the FFT of the data, padding with zeros, then inverting using the
%   IFFT.
%
%   REFINE2(F,N) returns an N by N array.
%
%   See also REFINE, REFINEK, REFINEK2.

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 3, N = M; end

% Apply refine to each dimension.  (See doc fft.)
fr = refine(refine(f,M,1),N,2);
