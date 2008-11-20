function Ak = fft2udotgrad(ux,uy,L,zcutoff)
%FFT2UDOTGRAD   Compute the Fourier representation of u.grad.
%   AK = FFT2UDOTGRAD(UX,UY) return the Fourier representation of
%   the operator (UX UY).(d/dX d/dY).  The matrices UX and UY must
%   be N by N, and AK is returned as a sparse matrix of dimension
%   N^2 by N^2 that act on column vectors VK(:), where VK is the
%   2D FFT of the function V(:,:) discretized on an N by N grid.
%
%   AK = FFT2UDOTGRAD(UX,UY,L) specifies the domain size to be L (L
%   defaults to 2pi).  AK = FFT2UDOTGRAD(UX,UY,L,ZCUTOFF) also
%   specifies the cutoff for elements to be considered zero in the
%   sparsification of the FFT of U.

if nargin < 4
  % If the cutoff is too small, spurious nonzero elements occur.
  % 1e-8 seems like a sweet spot good for N=1000.
  zcutoff = 1e-8*max(max(max(abs(ux))),max(max(abs(uy))));
end

if nargin < 3
  L = 2*pi;
end

N = size(ux,1);

if size(ux,2) ~= N | size(uy,1) ~= N | size(uy,2) ~= N
  error('Matrices must be square and of the same dimension.')
end

kmin = floor(-(N-1)/2);
kmax = floor( (N-1)/2);

% FFT the velocity field, sparsifying with cutoff.
% Note that we use the transpose of the velocity field: the meshgrid
% format uses the -rows- as y, and the -columns- as x, but
% fft2udotgrad_helper assumes the usual (ix,jy) indexing.
uxk = sparsify(fft2(ux.'),zcutoff);
uyk = sparsify(fft2(uy.'),zcutoff);

% Make a list of the nonzero elements of uxk and uyk.
inz = find(abs(uxk) | abs(uyk));
uxknz = full(uxk(inz));
uyknz = full(uyk(inz));
% Row/columns of nonzero elements.
[nzx,nzy] = find(abs(uxk) | abs(uyk));

% The function that does the real work is written in C, since it is
% difficult to vectorise.
Ak = fft2udotgrad_helper(N,uxknz,uyknz,nzx,nzy,L);
