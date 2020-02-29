%
% Copyright (c) 2004-2020 Jean-Luc Thiffeault <jeanluc@mailaps.org>
%
% See the file LICENSE for copying permission.
%

alpha = .234;

% Initial distribution.
nens = 10000;
x0 = rand(nens,2);

x = x0;
th = zeros(nens,1);
sz = 15;

th = sin(pi*x0(:,1));
%th = sin(pi*x0(:,1)).*sin(pi*x0(:,2));

figure(1)
scatter(x(:,1),x(:,2),sz,th)

nit = 5;
for n = 1:nit
   [x(:,1) x(:,2)] = baker(x(:,1),x(:,2),alpha);
end

figure(2)
%imagesc(x,y,th);
scatter(x(:,1),x(:,2),sz,th)
