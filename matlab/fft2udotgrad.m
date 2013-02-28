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
% difficult to vectorize.
Ak = fft2udotgrad_helper(N,uxknz,uyknz,nzx,nzy,L);

if false
  % Doublecheck (only feasible for smallish N, say N=21).
  Ak2 = sparse(N*N,N*N,nnz(Ak));
  fac = (2*pi/L)/(N*N);
  kmin = floor(-(N-1)/2); kmax = floor( (N-1)/2); k = [0:kmax kmin:-1];
  for ikx = 1:N
    for iky = 1:N
      for ilx = 1:N
	for ily = 1:N
	  K = ikx+N*(iky-1);
	  L = ilx+N*(ily-1);
	  kx = k(ikx); ky = k(iky); lx = k(ilx); ly = k(ily);
	  lkx = lx-kx; lky = ly-ky;
	  ilkx = find(k == lkx); ilky = find(k == lky);
	  if ilkx >= 1 & ilkx <= N & ilky >= 1 & ilky <= N
	    Ak2(K,L) = -fac*1i*(kx*uxk(ilkx,ilky) + ky*uyk(ilkx,ilky));
	  end
	end
      end
    end
  end
  % Check if sparse matrices are equal.  This is shockingly tricky.  For
  % instance, (Ak-Ak2)*w doesn't work.  Something's not right.
  w = rand(N*N,1);
  fprintf('Check difference = %g\n',max(max(abs(Ak*w - Ak2*w))))
end
