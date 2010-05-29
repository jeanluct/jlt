function fp = refine2(f,M,N)
%REFINE2   Increase resolution of a 2D graph using FFT.
%   REFINE2(F,M,N) returns an M by N version of the 2D array F, inflated by
%   taking the FFT of the data, padding with zeros, then inverting using the
%   IFFT.
%
%   REFINE2(F,N) returns an N by N array.
%
%   See also REFINE.

if nargin < 3, N = M; end

% Apply refine to each dimension.  (See doc fft.)
fp = refine(refine(f,M,1).',N,1).';
