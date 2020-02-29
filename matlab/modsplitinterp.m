function [xm,ym] = modsplitinterp(x,y,md,off)
%MODSPLITINTERP Split lists x and y into periodic segments in x.
%   [XM,YM] = MODSPLITINTERP(X,Y,MOD) splits the vectors X and Y
%   into cells XM and YM containing segments that do not wrap
%   around in the periodic variable X.  The periodicity in X
%   is given by MOD.  The function also interpolates the endpoints
%   of each segment.  MODSPLITINTERP(X,Y,MOD,OFF) adds a constant
%   offset OFF to XM.
%
%   See also MOD, MODSPLIT, PLOTMODX, PLOTMODY, PLOTMODXY.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 4
    % Offset.
    off = 0;
end

% Split lists.
[xm ym] = modsplit(x,y,md,off);

% The first point cannot be interpolated.
x0 = [];
y0 = [];

for i = 1:(length(xm)-1),
    %
    % Interpolate last point of segment.
    %
    n = length(xm{i});
    % Which way is the segment leaving the domain?
    sdx = sign(xm{i+1}(1) - xm{i}(n));
    if sdx ~= 0
        % The x-distance between the two points.
        % (mod always has the same sign as md.)
        dx = mod(xm{i+1}(1) - xm{i}(n),md);
        if sdx == 1
            dx = md - dx;
        end
        dy = ym{i+1}(1) - ym{i}(n);
        if (sdx == 1)
            x1 = 0;
            eps = xm{i}(n);
        else
            x1 = md;
            eps = md - xm{i}(n);
        end
        y1 = ym{i}(n) + (dy/dx)*eps;
        xm{i} = [x0;xm{i};x1];
        ym{i} = [y0;ym{i};y1];
    else
        % Point falls right on the edge.
        % No need to interpolate.
        x1 = [];
        y1 = [];
    end
    x0 = md - x1;
    y0 = y1;
end

% Prepend the point to the final segment.
xm{length(xm)} = [x0;xm{length(xm)}];
ym{length(xm)} = [y0;ym{length(xm)}];
