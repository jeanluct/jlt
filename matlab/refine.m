function fp = refine(f,N)
%REFINE   Increase resolution of an array using FFT.
%   REFINE(F,N) returns F inflated to length N by taking the FFT of
%   the data, padding with zeros, then inverting using the IFFT.
%
%   See also REFINE2.

N0 = length(f);

if N < N0
  error('No downsampling in refine')
end

kmin0 = floor(-(N0-1)/2);
kmax0 = floor( (N0-1)/2);
kmin = floor(-(N-1)/2);
kmax = floor( (N-1)/2);
c = -kmin+1;
c0 = -kmin0+1;
k0 = [kmin0:kmax0];

fk0 = fftshift(fft(f));
fk = zeros(N,1);

% Will this work with even N0?  Then we need to duplicate the Nyquist mode.
fk(c + k0) = fk0(c0 + k0);

fp = (N/N0)*ifft(ifftshift(fk),'symmetric');
