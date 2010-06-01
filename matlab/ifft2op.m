function L = ifft2op(Lk)
%IFFT2OP   Inverse Fourier transform an operator in 2D.
%   L = IFFT2OP(LK) computes L = inv(F2)*LK*F2, where F2 is the matrix
%   of the 2D FFT.  LK must be a matrix of size M^2*N^2.  L acts on
%   column vectors of length N^2 obtained from PK(A), where A is an
%   N*N matrix.
%
%   See also FFT2OP, PK, UNPK, FFTOP, IFFTOP.

% Note that F2 = F2.' , inv(F2) = inv(F2).', so the transform can be
% taken to the left or right.

[M2,N2] = size(Lk);

FiLk = zeros(M2,N2);
L = zeros(M2,N2);

% IFFT the columns
for jj = 1:N2
  ift = ifft2(full(unpk(Lk(:,jj))));
  FiLk(:,jj) = ift(:);
end
% Then FFT the rows
for ii = 1:M2
  ift = fft2(full(unpk(FiLk(ii,:))));
  L(ii,:) = ift(:);
end
