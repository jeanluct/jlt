function [xm,ym] = modsplit(x,y,md,off)
%MODSPLIT Split lists x and y into periodic segments in x.
%   [XM,YM] = MODSPLIT(X,Y,MOD) splits the vectors X and Y into
%   cells XM and YM containing segments that do not wrap
%   around in the periodic variable X.  The periodicity in X
%   is given by MOD.  MODSPLIT(X,Y,MOD,OFF) adds a constant
%   offset OFF to XM.
%
%   See also MOD, MODSPLITINTERP, PLOTMODX, PLOTMODY, PLOTMODXY.

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 4
    % Offset.
    off = 0;
end

% The quotient "region" of the coordinate.
quot = floor(x/md);

cur = 1; i = 1;

while (i <= length(quot)),
    qui = quot(i);
    k = 1;
    while (i+k <= length(quot) & quot(i+k) == qui),
        k = k + 1;
    end
    ran = i:(i+k-1);
    xm{cur} = mod(x(ran),md);
    ym{cur} = y(ran);
    cur = cur + 1; i = i + k;
end
