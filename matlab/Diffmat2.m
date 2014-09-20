function [Dx,Dy,Dxx,Dyy] = Diffmat2(N,Lx,Ly)
%DIFFMAT2   Second-order Fourier differentiation matrices in 2D.
%   [DX,DY,DXX,DYY] = DIFFMAT2(N,L) returns the sparse Fourier
%   differentiation matrices for N gridpoints on a period-L domain in
%   2D (if omitted, L defaults to 2pi).  The packing and unpacking of
%   the 2D array into a vector is compatible with PACK and UNPACK.
%
%   [DX,DY,DXX,DYY] = DIFFMAT2(N,LX,LY) specifies the period in each
%   direction.
%
%   See also FOURDIF, PACK, UNPACK.

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 2
  Lx = 2*pi;
end

if nargin < 3
  Ly = Lx;
end

k1x = 2*pi/Lx;
k1y = 2*pi/Ly;

% Fourier differentiation matrices
[x0,D]  = fourdif(N,1);
[x0,D2] = fourdif(N,2);

D = sparse(D);
D2 = sparse(D2);

% Dx is an antisymmetric toeplitz matrix.
Dx = kron(k1x*D,speye(N));

% Dy just consists of the differentiation matrix D repeated along
% the diagonal.
Dy = kron(speye(N),k1y*D);

% Dxx is a symmetric toeplitz matrix.
Dxx = kron(k1x^2*D2,speye(N));

% Dyy just consists of the differentiation matrix D2 repeated along
% the diagonal.
Dyy = kron(speye(N),k1y^2*D2);

% Ditch Dxy: too full.
%Dxy = kron(D,D);
