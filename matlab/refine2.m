function fp = refine2(f,N)
%REFINE2   Increase resolution of a 2D graph using FFT.
%   REFINE2(F,N) returns an N by N version of F, inflated
%   by taking the FFT of the data, padding with zeros, then
%   inverting using the IFFT.
%
%   See also REFINE.

N0 = size(f,1);

if N < N0
  error('No downsampling in refine2')
end

kmin0 = floor(-(N0-1)/2);
kmax0 = floor( (N0-1)/2);
kmin = floor(-(N-1)/2);
kmax = floor( (N-1)/2);
c = -kmin+1;
c0 = -kmin0+1;
k0 = [kmin0:kmax0];

fk0 = fftshift(fft2(f));
fk = zeros(N,N);

% Will this work with even N0?  Then we need to duplicate the Nyquist mode.
fk(c + k0,c + k0) = fk0(c0 + k0,c0 + k0);

fp = (N/N0)^2*ifft2(ifftshift(fk),'symmetric');
