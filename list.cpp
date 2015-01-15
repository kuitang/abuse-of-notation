#include <iostream>
#include <cstdint>
#include "abuse.h"

TP <int N> ST Int {
  SC int res = N;
};

// Todo: figure out multiple dispatch in TP.
TP <TN X, TN Y> ST Plus { };
TP <TN X, TN Y> ST Times { };

TP <int N, int M> ST Plus<Int<N>, Int<M>> {
  TD TN Int<N + M>::res res;
};

TP <int N, int M> ST Times<Int<N>, Int<M>> {
  TD TN Int<N * M>::res res;
};

TP <TN X> ST Square { TD TN Times<X, X>::res res; };

TP <TN H, TN T> ST List {
  TD H Head;
  TD T Tail;
};

TP <TN F> ST Lambda {

};

ST Nil { };

TP <TN X> FN Length      { SC int res = 1 + Length<TN X::Tail>::res; };
TP <>     FN Length<Nil> { SC int res = 0; };

TP <TN F, TN X> FN Map {
  TD F<TN X::Head>::res fhead_res;
  TD TN List<fhead_res, Map<F, TN X::Tail>>::res res;
};
TP <TN F>       FN Map<F, Nil> { TD Nil res; };

TP <TN Accum, TN Op, TN X> FN FoldLeft {
  TD TN FoldLeft<Op<Accum, TN X::Head>::res, Op, TN X::Tail>::res res;
};

TP <TN Accum, TN Op>       FN FoldLeft<Accum, Op, Nil> {
  TD Accum res;
};

int main() {

  TD List<Int<1>, List<Int<2>, List<Int<3>, Nil>>> OneTwoThree;
  std::cout << "Length of OneTwoThree: " << Length<OneTwoThree>::res << std::endl;

  SC int sum   = FoldLeft<Int<0>, Plus, OneTwoThree>::res;
  SC int sumSq = FoldLeft<Int<0>, Plus, Map<Square, OneTwoThree>>::res;

  std::cout << "sum = " << sum << " and sumSq = " << sumSq << std::endl;

  return 0;
}
