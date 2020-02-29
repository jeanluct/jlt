function Lk = fftop(L)
%FFTOP   Fourier transform an operator.
%   LK = FFTOP(L) computes LK = F*L*inv(F), where F is the matrix of
%   the 1D FFT.
%
%   See also IFFTOP, FFT2OP, IFFT2OP.

% Note that F = F.' , inv(F) = inv(F).', so the transform can be taken
% to the left or right.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

[M,N] = size(L);

FL = zeros(M,N);
Lk = zeros(M,N);

% FFT the columns
for jj = 1:N
  ft = fft(L(:,jj));
  FL(:,jj) = ft;
end
% Then IFFT the rows
for ii = 1:M
  ft = ifft(FL(ii,:));
  Lk(ii,:) = ft;
end
