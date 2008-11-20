function fi = interpp2(f,XX)
%INTERPP2   Interpolate a 2D periodic function.
%   FI = INTERPP2(F,[X Y]) returns F(X,Y) given the function F by
%   using bilinear interpolation. F is assumed period-1 in both
%   directions, and to be in 'meshgrid' style, where the rows are Y
%   and the columns X.
%
%   See also INTERPP, INTERPK2.

Nx = size(f,2);
Ny = size(f,1);

if Nx == 1 || Ny == 1
  error('One-dimensional array in interpp2.')
end

% Find interval in each direction.
X = Nx*mod(XX(1),1);
Y = Ny*mod(XX(2),1);
ix = floor(X)+1;
iy = floor(Y)+1;

if ix == Nx
  % Take periodicity into account.
  ixp = 1;
elseif ix == Nx+1
  % Special case: if X(1) is very small and negative, then
  % Nx*mod(X(1),1) gives Nx, rather than Nx*(1+X(1)), so the floor
  % function fails to round down to Nx-1.
  ix = Nx;
  ixp = 1;
else
  ixp = ix+1;
end

if iy == Ny
  % Take periodicity into account.
  iyp = 1;
elseif iy == Ny+1
  % See ix==Nx+1 comment above.
  iy = Ny;
  iyp = 1;
else
  iyp = iy+1;
end

% The function at the four corners.
% Note how x and y are reversed, since f is assumed to be in
% meshgrid format (rows are y, columns are x).
f11 = f(iy ,ix );
f21 = f(iy ,ixp);
f12 = f(iyp,ix );
f22 = f(iyp,ixp);

X1 = X-(ix-1);
Y1 = Y-(iy-1);
X2 = 1-X1;
Y2 = 1-Y1;

fi = f11*X2*Y2 + f21*X1*Y2 + f12*X2*Y1 + f22*X1*Y1;
