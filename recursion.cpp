#include <iostream>
#include <cstdint>
#include "abuse.h"

TP <int64_t N> FN Fac    { SC int64_t res = N * Fac<N-1>::res; };
TP <>      FN Fac<0> { SC int64_t res = 1; };

// Euclid's algorithm
TP <int64_t A, int64_t B> FN GCD       { SC int64_t res = GCD<B, A % B>::res; };
TP <int64_t A>        FN GCD<A, 0> { SC int64_t res = A; };

TP <int64_t N, int64_t D> ST Rat {
  SC int64_t num = N;
  SC int64_t den = D;
};

TP <TN X> FN Simplify {
  SC int64_t gcd = GCD<X::num, X::den>::res;
  TD Rat<X::num / gcd, X::den / gcd> res;
};

TP <TN X, TN Y> FN RatMult {
  TD Rat<X::num * Y::num, X::den * Y::den> tmp;
  TD TN Simplify<tmp>::res res;
};

TP <TN X, TN Y> FN RatPlus {
  TD Rat<X::num*Y::den + X::den*Y::num, X::den*Y::den> tmp;
  TD TN Simplify<tmp>::res res;
};

TP <int64_t N> FN ETaylor {
  TD Rat<1, Fac<N>::res>  this_term;
  TD TN ETaylor<N-1>::res rest_terms;
  TD TN RatPlus<this_term, rest_terms>::res res;
};

TP <> FN ETaylor<0> { TD Rat<1, 1> res; };

int main() {
  std::cout << Fac<5>::res << std::endl;

  TD Rat<2,3> TwoThirds;
  TD Rat<1,4> OneFourth;
  TD RatMult<TwoThirds, OneFourth>::res OneSixth;

  std::cout << OneSixth::num << "/" << OneSixth::den << std::endl;


  TD ETaylor<13>::res e13;
  std::cout << "13th order Taylor approximation of e: " <<
            (1.0 * e13::num / e13::den) << std::endl;

  return 0;
}

