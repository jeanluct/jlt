function Xi = interpline(X,dX)
%INTERPLINE   Interpolate a material line to a given resolution.
%   XI = INTERPLINE(X,DX), where X is an array with vectors as rows
%   representing points on a continuous line, returns an interpolated
%   vector XI, where the maximum distance between interpolated points is DX.

Xi = interpline_helper(X,dX);
