function Fr = realify(F)
%REALIFY   Enforce reality symmetry of an FFT array.
%   FR = REALIFY(F) takes the complex array F and applies the
%   symmetries needed to enfore the reality condition on F.

% Inspired from the code for fftshift.m.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

numDims = ndims(F);

ic = cell(1,numDims);

for k = 1:numDims
  N = size(F,k);
  % ic{k} is the index array corresponding to the conjugate
  % elements in that dimension.
  ic{k} = [1 N:-1:2];
end

% Just add to the conjugate and divide by 2!
Fr = (F + conj(F(ic{:})))/2;
