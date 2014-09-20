function plotmody(x,y,mdy,attrib,off)
%PLOTMODY Plot with periodic y axis.
%   PLOTMODY(X,Y,MODY) plots Y vs X, first taking the mod
%   by MODY of the coordinate Y.  PLOTMODY(X,Y,MODY,ATTRIB)
%   uses ATTRIB as plot attributes (see PLOT).
%   PLOTMODY(X,Y,MODY,ATTRIB,OFF) adds an offset OFF to Y.
%
%   See also MOD, PLOT, PLOTMODX, PLOTMODXY, MODSPLITINTERP.

%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 4
    attrib = '';
end

if nargin < 5
    off = 0;
end

[ym xm] = modsplitinterp(y,x,mdy,off);

savehold = ishold;
if savehold == 0
    newplot
    hold on
end

for i = 1:length(ym)
    plot(xm{i},ym{i},attrib);
end

if savehold == 0
    hold off
end
