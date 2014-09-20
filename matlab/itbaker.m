%
% Copyright (c) 2004-2014 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

% Number of iterations - 1 (first slot reserved for initial condition).
nit = 50;
% Number of initial conditions.
nens = 200;
dy = 1/(nens+1);

alpha = .234;
beta = 1 - alpha;

x0 = .1*alpha;
% Avoid boundaries: special points with large stretching.
y0 = [dy:dy:1-dy];

L = zeros(nens,nit);

for j = 1:nens
  x = x0;
  y = y0(j);
  L(j,1) = 1;
  for i = 2:nit
    if y < alpha
      L(j,i) = L(j,i-1)/alpha;
    else
      L(j,i) = L(j,i-1)/beta;
    end
    [x y] = baker(x,y,alpha);
  end
end

T = zeros(nens,nit);
lyap = zeros(nens,nit);
for j = 1:nens
  T(j,:) = cumtrapz(L(j,:).^2);
  lyap(j,:) = log(L(j,:))./[1:nit];
end

kappa = .001;
avg = zeros(1,nit);
for i = 1:nit
  avg(i) = trapz(exp(-kappa*T(:,i)));
end
