function Fr = realify(F)
%REALIFY   Enforce reality symmetry of an FFT array.
%   FR = REALIFY(F) takes the complex array F and applies the
%   symmetries needed to enfore the reality condition on F.

% $Id: realify.m,v 1.4 2008/03/02 19:55:25 jeanluc Exp $

% Inspired by looking at the code for fftshift.m.

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
