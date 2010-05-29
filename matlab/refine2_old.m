function fp = refine2(f,M,N)
%REFINE2   Increase resolution of a 2D graph using FFT.
%   REFINE2(F,M,N) returns an M by N version of the 2D array F, inflated by
%   taking the FFT of the data, padding with zeros, then inverting using the
%   IFFT.
%
%   REFINE2(F,N) returns an N by N array.
%
%   See also REFINE.

% TODO: Downsampling.

if nargin < 3, N = M; end

[M0 N0] = size(f);
fk = zeros([M N]);

if M < M0 || N < N0, error('No downsampling in refine'); end

Mkmin0 = floor(-(M0-1)/2); Mkmax0 = floor( (M0-1)/2);
Mkmin = floor(-(M-1)/2); Mk0 = [Mkmin0:Mkmax0];
Nkmin0 = floor(-(N0-1)/2); Nkmax0 = floor( (N0-1)/2);
Nkmin = floor(-(N-1)/2); Nk0 = [Nkmin0:Nkmax0];

fk0 = fftshift(fft2(f));

fk(Mk0-Mkmin+1,Nk0-Nkmin+1) = fk0;

% When M0 or N0 is even, need to copy the Nyquist mode(s).
%% THIS IS BROKEN.
if ~mod(M0,2)
  fk(1-Mkmin - Mkmin0,:) = conj(fk(1-Mkmin + Mkmin0,:));
end
if ~mod(N0,2)
  fk(:,1-Nkmin - Nkmin0) = conj(fk(:,1-Nkmin + Nkmin0));
end

fp = (M/M0*N/N0)*ifft2(ifftshift(fk),'symmetric');
