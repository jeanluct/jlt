function [xn,yn] = baker(x,y,al)
%BAKER   Baker's map.
%   [XN,YN] = BAKER(X,Y) returns the image of (X,Y) under the
%   action of the baker's map.  BAKER(X,Y,ALPHA) returns
%   the generalized baker's map, where ALPHA <= 0.5.
%   Both X and Y are in the interval [0,1].
%

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 3
  % Default if the uniform baker's map.
  al = 0.5;
end

if al > 0.5
  error('Bakers map requires alpha < 0.5.')
end

be = 1 - al;

if length(x) == 1
  if length(y) ~= 1
    error('Both x and y must be scalars or vectors.')
  end
  %
  % Act on scalars.
  %
  if y < al
    xn = al*x;
    yn = y/al;
  else
    xn = 1 - be*x;
    yn = (1 - y)/be;
  end
  return
else
  if length(y) ~= length(x)
    error('x and y must have the same length.')
  end
  %
  % Act on vectors.
  %
  % Allocate arrays.
  xn = zeros(size(x));
  yn = zeros(size(y));
  % Formula for y <= alpha.
  ila = find(y <= al);
  xn(ila) = al*x(ila);
  yn(ila) = y(ila)/al;
  % Formula for y > alpha.
  iga = find(y >  al);
  xn(iga) = 1 - be*x(iga);
  yn(iga) = (1 - y(iga))/be;
end
