function plotmodxy(x,y,mdx,mdy,attrib,offx,offy)
%PLOTMODXY Plot with periodic x and y axes.
%   PLOTMODXY(X,Y,MOD) plots Y vs X, first taking the mod
%   by MOD of the coordinates X and Y.
%   PLOTMODXY(X,Y,MODX,MODY) uses values MODX and MODY
%   to mod the two axes.
%   PLOTMODXY(X,Y,MODX,MODY,ATTRIB) uses ATTRIB as plot
%   attributes (see PLOT).
%   PLOTMODXY(X,Y,MODX,MODY,ATTRIB,OFFX) adds an offset
%   OFFX to X.
%   PLOTMODXY(X,Y,MODX,MODY,ATTRIB,OFFX,OFFY) adds offsets
%   OFFX and OFFY to X and Y.
%
%   See also MOD, PLOT, PLOTMODX, PLOTMODY, MODSPLITINTERP.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 4
    mdy = mdx;
end

if nargin < 5
    attrib = '';
end

if nargin < 6
    offx = 0;
end

if nargin < 7
    offy = 0;
end

% Mod in the x coordinate.
[xm ym] = modsplitinterp(x,y,mdx,offx);

% Mod in the y coordinate.
for i = 1:length(xm)
    [ymm{i} xmm{i}] = modsplitinterp(ym{i},xm{i},mdy,offy);
end

% Hold plot if needed.
savehold = ishold;
if savehold == 0
    newplot
    hold on
end

% Plot each line segment.
for i = 1:length(xm)
    for j = 1:length(xmm{i})
        plot(xmm{i}{j},ymm{i}{j},attrib);
    end
end

% Restore hold properties.
if savehold == 0
    hold off
end
