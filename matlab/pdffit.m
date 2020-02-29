function [a,b,nfit] = pdffit(xb,n,ndrop)
%PDFFIT   Fit a power law through a PDF.
%   [A,B,NFIT] = PDFFIT(XB,N,NDROP) returns a fit
%   of the data [XB,N] to a power law of the form
%   NFIT = B * XB.^A .  Bins with N<=NDROP points
%   are considered statistically insignificant and
%   are not used in the fit (defautl NDROP=3).
%
%   See also PDFY, POLYFIT.

%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

if nargin < 3
    % Drop bins with less than 3 points.
    ndrop = 3;
end

% Drop statistically insignificant bins.
lxb = log(xb(find(n>ndrop)));
ln = log(n(find(n>ndrop)));

% Linear fit
ab = polyfit(lxb,ln,1);

a = ab(1,1);
b = exp(ab(1,2));

nfit = b*xb.^a;
