#include <iostream>
#include <jlt/polynomial.hpp>

using namespace jlt;

int main()
{
  using std::cout;
  using std::endl;

  polynomial<double> p(0,0), q(2);

  p.add_to_coeff(1,.5);
  p.add_to_coeff(2,.25);
  p.add_to_coeff(-1,.33);
  p.add_to_coeff(3,-.66);
  p.add_to_coeff(3,-.33);
  p.add_to_coeff(-3,-.11);
  p.add_to_coeff(0,-.1111);
  p.set_coeff(0,32);

  p.printFancy(cout) << endl;
  q.set_coeff(5,2);
  q.printFancy(cout) << endl;

  p.multiply_by_monomial(-1,2);

  p.printFancy(cout) << endl;

  p.add_polynomial_X_monomial(q,-2,-.5);

  p.printFancy(cout) << endl;

  polynomial<double> pp(1), qq(1);
  pp.set_coeff(1,1);
  qq.set_coeff(-1,1);
  pp.printFancy(cout) << endl;
  qq.printFancy(cout) << endl;
  pp.multiply_by_polynomial(qq);
  pp.printFancy(cout,"t") << endl;

  cout << endl;

  pp.printFancy(cout) << endl;
  qq.printFancy(cout) << endl;
  cout << pp*qq;

  cout << pp(2) << endl;
  cout << pp[10] << endl;

  cout << pp << endl;
  pp[10] = .65;
  pp[-10] = .65;
  cout << pp << endl;

  cout << endl;

  polynomial<double> c(1);

  c[1] = 2;
  c[2] = 3;
  c[3] = 4;
  c[4] = 5;
  c[5] = 6;
  c.print(cout) << endl;
  c[0] -= 1;
  c[1] -= 2;
  c[2] -= 3;
  c[5] -= 6;
  c.print(cout) << endl;
  c.compact();
  c.print(cout) << endl;

  c = q;
  c.print(cout) << endl;
  q.print(cout) << endl;
  if (c == q) cout << "Equal\n";
  c.printFancy(cout) << endl;
  q.printFancy(cout) << endl;
  c[1] = 1;
  c.printFancy(cout) << endl;
  q.printFancy(cout) << endl;

  cout << endl;

  c = 3;
  c.printFancy(cout) << endl;
  if (c != q) cout << "Inequal\n";

  cout << endl;

  q *= 3;
  q.printFancy(cout) << endl;
  q *= q;
  q.printFancy(cout) << endl;
  q /= 6;
  q.printFancy(cout) << endl;

  cout << endl;

  qq.printFancy(cout) << endl;
  q -= qq;
  q.printFancy(cout) << endl;
  q += qq;
  q -= 3;
  q.printFancy(cout) << endl;

  cout << "Derivative of " << p << endl << " is " << p.derive() << endl;

  return 0;
}
