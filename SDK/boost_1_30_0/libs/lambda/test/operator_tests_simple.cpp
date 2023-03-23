//  operator_tests_simple.cpp  -- The Boost Lambda Library ---------------
//
// Copyright (C) 2000-2003 Jaakko J�rvi (jaakko.jarvi@cs.utu.fi)
// Copyright (C) 2000-2003 Gary Powell (powellg@amazon.com)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies. 
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice 
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty, 
// and with no claim as to its suitability for any purpose.
//
// For more information, see www.boost.org

// -----------------------------------------------------------------------


#define BOOST_INCLUDE_MAIN  // for testing, include rather than link
#include <boost/test/test_tools.hpp>    // see "Header Implementation Option"

#include "boost/lambda/lambda.hpp"

#include <vector>
#include <map>
#include <set>
#include <string>

#include <iostream>

#ifndef BOOST_NO_STRINGSTREAM
#include <sstream>
#endif

using namespace std;
using namespace boost;

using namespace boost::lambda;


class unary_plus_tester {};
unary_plus_tester operator+(const unary_plus_tester& a) { return a; } 

void cout_tests()
{
#ifndef BOOST_NO_STRINGSTREAM
  using std::cout;
  ostringstream os;
  int i = 10; 
  (os << _1)(i);

  (os << constant("FOO"))();

  BOOST_TEST(os.str() == std::string("10FOO"));
  

  istringstream is("ABC 1");
  std::string s;
  int k;

  is >> s;
  is >> k;

  BOOST_TEST(s == std::string("ABC"));
  BOOST_TEST(k == 1);
  // test for constant, constant_ref and var
  i = 5;
  constant_type<int>::type ci(constant(i));
  var_type<int>::type vi(var(i)); 

  (vi = _1)(make_const(100));
  BOOST_TEST((ci)() == 5);
  BOOST_TEST(i == 100);

  int a;
  constant_ref_type<int>::type cr(constant_ref(i));
  (++vi, var(a) = cr)();
  BOOST_TEST(i == 101);
#endif
}

void arithmetic_operators() {
  int i = 1; int j = 2; int k = 3;

  using namespace std;
  using namespace boost::lambda;
   
  BOOST_TEST((_1 + 1)(i)==2);
  BOOST_TEST(((_1 + 1) * _2)(i, j)==4);
  BOOST_TEST((_1 - 1)(i)==0);

  BOOST_TEST((_1 * 2)(j)==4);
  BOOST_TEST((_1 / 2)(j)==1);

  BOOST_TEST((_1 % 2)(k)==1);

  BOOST_TEST((-_1)(i) == -1);
  BOOST_TEST((+_1)(i) == 1);
  
  // test that unary plus really does something
  unary_plus_tester u;
  unary_plus_tester up = (+_1)(u);
}

void bitwise_operators() {
  unsigned int ui = 2;

  BOOST_TEST((_1 << 1)(ui)==(2 << 1));
  BOOST_TEST((_1 >> 1)(ui)==(2 >> 1));

  BOOST_TEST((_1 & 1)(ui)==(2 & 1));
  BOOST_TEST((_1 | 1)(ui)==(2 | 1));
  BOOST_TEST((_1 ^ 1)(ui)==(2 ^ 1));
  BOOST_TEST((~_1)(ui)==~2u);
}

void comparison_operators() {
  int i = 0, j = 1;

  BOOST_TEST((_1 < _2)(i, j) == true);
  BOOST_TEST((_1 <= _2)(i, j) == true);
  BOOST_TEST((_1 == _2)(i, j) == false);
  BOOST_TEST((_1 != _2)(i, j) == true);
  BOOST_TEST((_1 > _2)(i, j) == false);
  BOOST_TEST((_1 >= _2)(i, j) == false);

  BOOST_TEST((!(_1 < _2))(i, j) == false);
  BOOST_TEST((!(_1 <= _2))(i, j) == false);
  BOOST_TEST((!(_1 == _2))(i, j) == true);
  BOOST_TEST((!(_1 != _2))(i, j) == false);
  BOOST_TEST((!(_1 > _2))(i, j) == true);
  BOOST_TEST((!(_1 >= _2))(i, j) == true);
}

void logical_operators() {

  bool t = true, f = false;
  BOOST_TEST((_1 && _2)(t, t) == true); 
  BOOST_TEST((_1 && _2)(t, f) == false); 
  BOOST_TEST((_1 && _2)(f, t) == false); 
  BOOST_TEST((_1 && _2)(f, f) == false); 

  BOOST_TEST((_1 || _2)(t, t) == true); 
  BOOST_TEST((_1 || _2)(t, f) == true); 
  BOOST_TEST((_1 || _2)(f, t) == true); 
  BOOST_TEST((_1 || _2)(f, f) == false); 

  BOOST_TEST((!_1)(t) == false);
  BOOST_TEST((!_1)(f) == true);

  // test short circuiting
  int i=0;

  (false && ++_1)(i);
  BOOST_TEST(i==0); 
  i = 0;

  (true && ++_1)(i);
  BOOST_TEST(i==1); 
  i = 0;

  (false || ++_1)(i);
  BOOST_TEST(i==1); 
  i = 0;

  (true || ++_1)(i);
  BOOST_TEST(i==0); 
  i = 0;
}

void unary_incs_and_decs() {
  int i = 0;

  BOOST_TEST(_1++(i) == 0);
  BOOST_TEST(i == 1);
  i = 0;

  BOOST_TEST(_1--(i) == 0);
  BOOST_TEST(i == -1);
  i = 0;

  BOOST_TEST((++_1)(i) == 1);
  BOOST_TEST(i == 1);
  i = 0;

  BOOST_TEST((--_1)(i) == -1);
  BOOST_TEST(i == -1);
  i = 0;

  // the result of prefix -- and ++ are lvalues
  (++_1)(i) = 10;
  BOOST_TEST(i==10);
  i = 0;

  (--_1)(i) = 10;
  BOOST_TEST(i==10);
  i = 0;
}

void compound_operators() { 

  int i = 1; 

  // normal variable as the left operand
  (i += _1)(make_const(1));
  BOOST_TEST(i == 2);

  (i -= _1)(make_const(1));
  BOOST_TEST(i == 1);

  (i *= _1)(make_const(10));
  BOOST_TEST(i == 10);

  (i /= _1)(make_const(2));
  BOOST_TEST(i == 5);

  (i %= _1)(make_const(2));
  BOOST_TEST(i == 1);

  // lambda expression as a left operand
  (_1 += 1)(i);
  BOOST_TEST(i == 2);

  (_1 -= 1)(i);
  BOOST_TEST(i == 1);

  (_1 *= 10)(i);
  BOOST_TEST(i == 10);

  (_1 /= 2)(i);
  BOOST_TEST(i == 5);

  (_1 %= 2)(i);
  BOOST_TEST(i == 1);
  
  // shifts
  unsigned int ui = 2;
  (_1 <<= 1)(ui);
  BOOST_TEST(ui==(2 << 1));

  ui = 2;
  (_1 >>= 1)(ui);
  BOOST_TEST(ui==(2 >> 1));

  ui = 2;
  (ui <<= _1)(make_const(1));
  BOOST_TEST(ui==(2 << 1));

  ui = 2;
  (ui >>= _1)(make_const(1));
  BOOST_TEST(ui==(2 >> 1));

  // and, or, xor
  ui = 2;
  (_1 &= 1)(ui);
  BOOST_TEST(ui==(2 & 1));

  ui = 2;
  (_1 |= 1)(ui);
  BOOST_TEST(ui==(2 | 1));

  ui = 2;
  (_1 ^= 1)(ui);
  BOOST_TEST(ui==(2 ^ 1));

  ui = 2;
  (ui &= _1)(make_const(1));
  BOOST_TEST(ui==(2 & 1));

  ui = 2;
  (ui |= _1)(make_const(1));
  BOOST_TEST(ui==(2 | 1));

  ui = 2;
  (ui ^= _1)(make_const(1));
  BOOST_TEST(ui==(2 ^ 1));
    
}

void assignment_and_subscript() {

  // assignment and subscript need to be defined as member functions.
  // Hence, if you wish to use a normal variable as the left hand argument, 
  // you must wrap it with var to turn it into a lambda expression

  using std::string;
  string s;

  (_1 = "one")(s);
  BOOST_TEST(s == string("one"));

  (var(s) = "two")();
  BOOST_TEST(s == string("two"));

  BOOST_TEST((var(s)[_1])(make_const(2)) == 'o');
  BOOST_TEST((_1[2])(s) == 'o');
  BOOST_TEST((_1[_2])(s, make_const(2)) == 'o');

  // subscript returns lvalue
  (var(s)[_1])(make_const(1)) = 'o';
  BOOST_TEST(s == "too");
 
  (_1[1])(s) = 'a';
  BOOST_TEST(s == "tao");

  (_1[_2])(s, make_const(0)) = 'm';
  BOOST_TEST(s == "mao");

  // TODO: tests for vector, set, map, multimap
}

class A {};

void address_of_and_dereference() {
  
  A a; int i = 42;  
  
  BOOST_TEST((&_1)(a) == &a);
  BOOST_TEST((*&_1)(i) == 42);

  std::vector<int> vi; vi.push_back(1);
  std::vector<int>::iterator it = vi.begin();
  
  (*_1 = 7)(it);
  BOOST_TEST(vi[0] == 7); 

  // TODO: Add tests for more complex iterator types
}



void comma() {

  int i = 100;
  BOOST_TEST((_1 = 10, 2 * _1)(i) == 20);

  // TODO: that the return type is the exact type of the right argument
  // (that r/l valueness is preserved)

}

void pointer_arithmetic() {

  int ia[4] = { 1, 2, 3, 4 };
  int* ip = ia;
  int* ia_last = &ia[3];

  const int cia[4] = { 1, 2, 3, 4 };
  const int* cip = cia;
  const int* cia_last = &cia[3];

 
  // non-const array
  BOOST_TEST((*(_1 + 1))(ia) == 2);

  // non-const pointer
  BOOST_TEST((*(_1 + 1))(ip) == 2);

  BOOST_TEST((*(_1 - 1))(ia_last) == 3);

  // const array
  BOOST_TEST((*(_1 + 1))(cia) == 2);
  // const pointer
  BOOST_TEST((*(_1 + 1))(cip) == 2);
  BOOST_TEST((*(_1 - 1))(cia_last) == 3);
 
  // pointer arithmetic should not make non-consts const
    (*(_1 + 2))(ia) = 0;
    (*(_1 + 3))(ip) = 0;

  BOOST_TEST(ia[2] == 0);
  BOOST_TEST(ia[3] == 0);

  // pointer - pointer
  BOOST_TEST((_1 - _2)(ia_last, ia) == 3);
  BOOST_TEST((_1 - _2)(cia_last, cia) == 3);
  BOOST_TEST((ia_last - _1)(ia) == 3);
  BOOST_TEST((cia_last - _1)(cia) == 3);
  BOOST_TEST((cia_last - _1)(cip) == 3);

}

int test_main(int, char *[]) {

  arithmetic_operators();
  bitwise_operators();
  comparison_operators();
  logical_operators();
  unary_incs_and_decs();
  compound_operators();
  assignment_and_subscript();
  address_of_and_dereference();
  comma();
  pointer_arithmetic();
  cout_tests();
  return 0;
}






