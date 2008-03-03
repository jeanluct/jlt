function [yb,Py] = pdfy(x,y,dydx,ny,intmethod,tol)
%PDFY   Construct PDF of a list of data.
%   [YB,PY] = PDFY(X,Y,DYDX,NY,INTMET,TOL) returns the
%   unnormalized probability distribution function (PDF)
%   of Y given the vectors (X,Y,DYDX), where DYDX is the
%   derivative of Y at X.  NY bins are used (default 50).
%   TOL is the tolerance used by the root-finding method
%   (default 1e-10), and INTMET is the interpolation method
%   to be used by interp1 (default 'cubic').
%
%   See also INTERPY, INTERP1, PDFFIT.

%   $Id: pdfy.m,v 1.9 2008/03/02 20:12:27 jeanluc Exp $

if nargin < 4
    % Number of y points to the PDF.
    ny = 50;
end

if nargin < 5
    % Interpolation method.
    intmethod = 'linear';
end

if nargin < 6
    % Tolerance for root finding.
    tol = 1.0e-10;
end

% Interpolation function for y.
yfunc = 'interpy';

%
% Make sure x is ordered properly
%

% Find the "direction" of the data.
i = 2; tau_sign = 0;
while tau_sign == 0,
    tau_sign = sign(x(i) - x(1));
    i = i + 1;
end

n = length(x);
z = zeros(1,n);		% Pre-allocate array as row-vector.
% Always keep first point.
z(1) = 1;
z(2:n) = tau_sign*(x(2:n) - x(1:(n-1)));

% Drop repeated or out-of-order points
x = x(find(z>0));
y = y(find(z>0));
dydx = dydx(find(z>0));
n = length(x);


eps = 0;			% How much to "skim" off the top and bottom.
ymin = min(y) + eps;
yrange = max(y) - ymin - 2*eps;

% The y "bins".
yb = ymin + yrange * [0:ny-1] / (ny-1);

Py = zeros(1,ny);		% Pre-allocate array as row-vector.
for il = 1:ny
    clear x0; nz = 0;
    Py(il) = 0;
    y1 = y(1) - yb(il);
    for ix = 2:n
	y2 = y(ix) - yb(il);
        % Check for sign change of y1 -> y2.
        if (y1*y2 <= 0)
            nz = nz + 1;
            %
            % Find the dydx where y=yb.
            %
            if (y2 == 0)
                % The point is dead on.
                dydx0 = dydx(ix);
            else
                if strcmp(intmethod,'linear')
                    % Use linear interpolation: very fast.
                    sl = (dydx(ix) - dydx(ix-1))/(y(ix) - y(ix-1));
                    dydx0 = sl*(yb(il) - y(ix-1)) + dydx(ix-1);
                else
                    [x0 y0] = fzero(yfunc,[x(ix-1) x(ix)], ...
                                  optimset('TolX',tol),x,y,yb(il),intmethod)
                    dydx0 = interp1(x,dydx,x0,intmethod);
                end
            end
            % Add up all the 1/|dydx| where y=yb to yield probability density.
            Py(il) = Py(il) + (1 / abs(dydx0));
        end
        y1 = y2;
    end
    % Output result.
    [il yb(il) Py(il)]
end

% Normalize the distribution.
Py = Py/trapz(yb,Py);

% A good estimate of the quality of the binning is how small the
% difference between the sample mean and the distribution mean is.
'Sample mean:'
trapz(x,y)/(x(n) - x(1))
'Distribution mean:'
trapz(yb,yb.*Py)
