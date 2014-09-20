%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

% Test Fourier differentiation matrices in 2D.

% Number of gridpoints
N = 60;
% Physical size of domain
L = 2*pi;

% The x-grid
x = L*(0:N-1)/N;
y = x';
[xx,yy] = meshgrid(x,y);

% The k-grid
k1 = 2*pi/L;
k = k1*[0 1:floor((N-1)/2) floor(-(N-1)/2):-1];
[kx,ky] = meshgrid(k,k');

% Pick some periodic function to differentiate
x1 = k1*xx; y1 = k1*yy;
phi = exp(cos(x1) + 2*sin(x1 + 2*y1));

% Derivatives of phi
phix = k1*(-sin(x1) + 2*cos(x1 + 2*y1))...
       .*exp(cos(x1) + 2*sin(x1 + 2*y1));
phiy = k1*(4*cos(x1 + 2*y1)).*exp(cos(x1) + 2*sin(x1 + 2*y1));
phixx = -k1^2*(cos(x1) + 2*sin(x1 + 2*y1) ...
	       - (sin(x1) - 2*cos(x1 + 2*y1)).^2)...
	.*exp(cos(x1) + 2*sin(x1 + 2*y1));
phiyy = 8*k1^2*(1 + cos(2*(x1 + 2*y1)) - sin(x1 + 2*y1))...
	.*exp(cos(x1) + 2*sin(x1 + 2*y1));
phixy = -4*k1^2*(cos(x1 + 2*y1).*(sin(x1) - 2*cos(x1+2*y1)) ...
		 + sin(x1 + 2*y1))...
	.*exp(cos(x1) + 2*sin(x1 + 2*y1));

% A simpler function.
%phi = cos(x1) + 2*sin(x1 + 2*y1);
%phix = k1*(-sin(x1) + 2*cos(x1 + 2*y1));
%phiy = k1*(4*cos(x1 + 2*y1));
%phixx = -k1^2*(cos(x1) + 2*sin(x1 + 2*y1));
%phiyy = -8*k1^2*(sin(x1 + 2*y1));
%phixy = -4*k1^2*(sin(x1 + 2*y1));

% Derivatives of phi using FFT: useful to check differentiation
% matrices.  The error in this case should be very small (1e-12)
% even for few modes.
%phik = fft2(phi);
%phixk  = i*kx.*phik;
%phixxk = -kx.*kx.*phik;
%phiyk  = i*ky.*phik;
%phiyyk = -ky.*ky.*phik;
%phixyk = -kx.*ky.*phik;
%phix  = ifft2(phixk ,'symmetric');
%phixx = ifft2(phixxk,'symmetric');
%phiy  = ifft2(phiyk ,'symmetric');
%phiyy = ifft2(phiyyk,'symmetric');
%phixy = ifft2(phixyk,'symmetric');

% Derivatives of phi using Fourier differentiation matrices
[x0,D] = fourdif(N,1);
[x0,D2] = fourdif(N,2);
D = k1*D;
D2 = k1^2*D2;

% Check with the differentiation matrices from Trefethen's book (p. 22).
h = 2*pi/N;
if rem(N,2) == 0
  column = [0 .5*(-1).^(1:N-1).*cot((1:N-1)*h/2)]';
  column2 = [-pi^2/3/h^2-1/6 .5*(-1).^(2:N).*csc((1:N-1)*h/2).^2]';
else
  % For odd N, cot -> csc.
  column = [0 .5*(-1).^(1:N-1).*csc((1:N-1)*h/2)]';
  % For odd N, csc^2 -> cot*csc, -1/6 -> 1/12.
  column2 = [-pi^2/3/h^2+1/12 ...
	    .5*(-1).^(2:N).*cot((1:N-1)*h/2).*csc((1:N-1)*h/2)]';
end
Dtref = k1*toeplitz(column,column([1 N:-1:2]));
D2tref = k1^2*toeplitz(column2,column2([1 N:-1:2]));
fprintf(1,'|D - Dtref| = %e\n',max(max(abs(D - Dtref))));
fprintf(1,'|D2 - D2tref| = %e\n',max(max(abs(D2 - D2tref))));

% Another way to get the Fourier differentiation matrix:
if N < 30
  % Don't do this if N is too large: requires dense matrices of
  % size N^2*N^2 and many FFT's!
  d = i*k;
  if rem(N,2) == 0
    % In even dimensions, there is a negative mode with no positive
    % analogue.  The reality symmetry condition implies that its
    % coefficient must be real.  Hence, when it gets multiplied by ik
    % it would become imaginary, which would violate the reality
    % symmetry, so the only cure is to set it to zero.
    d(N/2 + 1) = 0;
  end
  Dk = ifftop(diag(d));
  fprintf(1,'|D - ifftop(diag(i*k))| = %e\n',max(max(abs(D - Dk))));
end

% Check that the answers agree!
fprintf(1,'\nVerify Fourier differentiation matrices:\n\n');
% Right-multiply by -D for an x-derivative!
fprintf(1,'|  -phi*D  - phix | = %e\n',max(max(abs(  -phi*D  - phix ))));
fprintf(1,'| D*phi    - phiy | = %e\n',max(max(abs( D*phi    - phiy ))));
fprintf(1,'|   phi*D2 - phixx| = %e\n',max(max(abs(   phi*D2 - phixx))));
fprintf(1,'|D2*phi    - phiyy| = %e\n',max(max(abs(D2*phi    - phiyy))));
fprintf(1,'|-D*phi*D  - phixy| = %e\n',max(max(abs(-D*phi*D  - phixy))));

phiV = pk(phi);

% 2D Differentiation matrices acting on vector phiV.
tic
[Dx,Dy,Dxx,Dyy] = Diffmat2(N,L);
toc

fprintf(1,'\nVector version:\n\n');
fprintf(1,'| Dx*phiV - pk(phix) | = %e\n',...
	max(max(abs( Dx*phiV - pk(phix) ))));
fprintf(1,'| Dy*phiV - pk(phiy) | = %e\n',...
	max(max(abs( Dy*phiV - pk(phiy) ))));
fprintf(1,'|Dxx*phiV - pk(phixx)| = %e\n',...
	max(max(abs(Dxx*phiV - pk(phixx)))));
fprintf(1,'|Dyy*phiV - pk(phiyy)| = %e\n',...
	max(max(abs(Dyy*phiV - pk(phiyy)))));
% Using Dx*(Dy*phiV) instead of Dx*Dy*phiV makes a huge difference in
% execution space and time!  The reason is that Dx*Dy is a full
% matrix, so avoid computing it at all costs.
fprintf(1,'|Dxy*phiV - pk(phixy)| = %e\n',...
	max(max(abs(Dx*(Dy*phiV) - pk(phixy)))));
fprintf(1,'|Dyx*phiV - pk(phixy)| = %e\n',...
	max(max(abs(Dy*(Dx*phiV) - pk(phixy)))));

% Use finite-difference matrices as a check.

h = L/N;
% Order of the approximation.
ord = 5;
Dfd = centfd(1,N,ord,'periodic')/h;
D2fd = centfd(2,N,ord,'periodic')/h^2;

Dxfd = kron(Dfd,speye(N));
Dyfd = kron(speye(N),Dfd);
Dxxfd = kron(D2fd,speye(N));
Dyyfd = kron(speye(N),D2fd);

fprintf(1,'\nFinite-difference version:\n\n');
fprintf(1,'| Dxfd*phiV - pk(phix) | = %e\n',...
	max(max(abs( Dxfd*phiV - pk(phix) ))));
fprintf(1,'| Dyfd*phiV - pk(phiy) | = %e\n',...
	max(max(abs( Dyfd*phiV - pk(phiy) ))));
fprintf(1,'|Dxxfd*phiV - pk(phixx)| = %e\n',...
	max(max(abs(Dxxfd*phiV - pk(phixx)))));
fprintf(1,'|Dyyfd*phiV - pk(phiyy)| = %e\n',...
	max(max(abs(Dyyfd*phiV - pk(phiyy)))));
fprintf(1,'|Dxyfd*phiV - pk(phixy)| = %e\n',...
	max(max(abs(Dxfd*(Dyfd*phiV) - pk(phixy)))));
fprintf(1,'|Dyxfd*phiV - pk(phixy)| = %e\n',...
	max(max(abs(Dyfd*(Dxfd*phiV) - pk(phixy)))));
