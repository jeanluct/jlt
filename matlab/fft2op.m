function Lk = fft2op(L)
%FFT2OP   Fourier transform an operator in 2D.
%   LK = FFT2OP(L) computes LK = F2*L*inv(F2), where F2 is the matrix
%   of the 2D FFT.  L must be a matrix of size M^2*N^2.  L acts on
%   column vectors of length N^2 obtained from PK(A), where A is an
%   N*N matrix.
%
%   See also IFFT2OP, PK, UNPK, FFTOP, IFFTOP.

% Note that F2 = F2.' , inv(F2) = inv(F2).', so the transform can be
% taken to the left or right.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

[M2,N2] = size(L);

FL = zeros(M2,N2);
Lk = zeros(M2,N2);

% FFT the columns
for jj = 1:N2
  ft = fft2(full(unpk(L(:,jj))));
  FL(:,jj) = ft(:);
end
% Then IFFT the rows
for ii = 1:M2
  ft = ifft2(full(unpk(FL(ii,:))));
  Lk(ii,:) = ft(:);
end
