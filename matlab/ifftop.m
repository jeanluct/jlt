function L = ifftop(Lk)
%IFFTOP   Inverse Fourier transform an operator.
%   L = IFFTOP(LK) computes L = inv(F)*Lk*F, where F is the matrix of
%   the 1D FFT.
%
%   See also FFTOP, FFT2OP, IFFT2OP.

% Note that F = F.' , inv(F) = inv(F).', so the transform can be taken
% to the left or right.

[M,N] = size(Lk);

FiLk = zeros(M,N);
L = zeros(M,N);

% IFFT the columns
for jj = 1:N
  ift = ifft(Lk(:,jj));
  FiLk(:,jj) = ift;
end
% Then FFT the rows
for ii = 1:M
  ift = fft(FiLk(ii,:));
  L(ii,:) = ift;
end
