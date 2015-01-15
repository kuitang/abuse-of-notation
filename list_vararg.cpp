#include <iostream>

// Abusing the variadic template parameter as recursive list.

// Style: I follow Bartosz Milewski's style of using class instead of
// typename in template declarations. This is because
//
// 1. In template metaprogramming, I define all types as struct instead
//    of class--there is not really any need for private members since
//    everything is compile-time and static anyways.
//
// 2. Higher order functions _require_ the template<class> class f1
//    declaration.

// Put the general declaration first.
template<class... list> struct
count;

// Base case
template<> struct count<> { static const int res = 0; };
template<class head, class... tail> struct
count<head, tail...> {
  static const int res = 1 + count<tail...>::res;
};

// Higher order function example.
//
// DISUNIFICATION of functions: functions are still semantically first class,
// but no longer syntactically so:
//
// 1. Input function: the function argument must be template<class> class f
//    instead of just class f.
//
// 2. Output function: you cannot simply assign res to some other type via the
//    typedef idiom, but you must explicitly construct another struct to
//    forward the partial application (see Partial application example.).
//
// The type of a template class is described exactly by its structure.

template<template<class> class f1, template<class> class f2> struct
or_combinator {
  template<class T> struct
  lambda {
    static const bool res = f1<T>::res || f2<T>::res;
  };
};

template<class T> struct isPtr     { static const bool res = false; };
template<class T> struct isPtr<T*> { static const bool res = true;  };

template<class T> struct isConst          { static const bool res = false; };
template<class T> struct isConst<const T> { static const bool res = true;  };

// do nothing
struct X;

template<class... args> struct isPtrOrConst {
  static const auto res = or_combinator<isPtr, isConst>::lambda<args...>::res;
};


// Higher order functions on lists (on wrapped values)

template<int N> struct Integer {
  static const int res = N;
  typedef Integer<0> zero;
};

template<char N> struct Char {
  static const char res = N;
  typedef Char<0> zero;
};

// Add wrapped values.
template<class x, class y> struct Plus {
  static const auto res = x::res + y::res;
};

// _Declare_ the most general type first
template<template<class> class pred, class... list> struct all;

// _Define_ the base case
template<template<class> class pred> struct
all<pred> {
  static const bool res = true;
};

template<template<class, class> class f, class init, class... list> struct foldr;

template<template<class, class> class f, class init> struct
foldr<f, init> {
  static const auto res = init::res;
};

template<template<class, class> class f, class init, class head, class... tail> struct
foldr<f, init, head, tail...> {
  static const auto res = f<head, foldr<f, init, tail...>>::res;
};

// _Define_ the recursive case
template<
  template<class> class pred,
  class head,
  class... tail> struct
all<pred, head, tail...> {
  static const bool res = pred<head>::res && all<pred, tail...>::res;
};

// List comprehensions (now we are getting *very*) abusive
//
// This exploits C++'s interpretation of parameter unpacking.

// First, we make a singleton wrapper for all types:
template<class T> struct one {
  static const int res = 1;
};

// Now we abuse notation:
template<class... list> struct
count {
  static const int res = foldr<Plus, one<list>...>::res;
};

// What the hell is one<list>::res...? Well if list is
// one<int>, one<char>, one<long>
//
// Then one<list>... expands to
// one<int>, one<char>, one<long>

// Since we've implemented foldr, we should also implement map.
// But there's the rub: there is no way to "return" a variadic parameter pack
// (list) in TMP. We could instead write map in a continuation-passing style.
// So we can pass it a partially applied foldr to get a map-reduce pattern.
// (See isPtrOrConst for an example of partial application.)

// Again, this abuses the expansion of variadic pack with ...

template<template<class...> class cont,
         template<class> class f,
         class... list> struct
map_cont {
  static const auto res = cont<f<list>...>::res;
};

template <class T> struct Square {
  static const auto res = T::res * T::res;
};

// HOW TO DEFINE GENERIC ZERO?!

template <class... list> struct Sum;
template <>              struct Sum<> { }; // error
template <class head, class...tail> struct
Sum<head, tail...> {
  static const auto res = foldr<Plus, Integer<0>, head, tail...>::res;
};

template <class... list> struct SumSquares {
  static const auto res = map_cont<Sum, Square, list...>::res;
};

int main() {
  std::cout << "count<int, char, long, X>::res = " <<
               count<int, char, long, X>::res <<
               std::endl;

  // Partial application example.
  std::cout << "isPtrOrConst<const int> = " <<
               isPtrOrConst<const int>::res <<
               std::endl;

  std::cout << "all<isConst, const int, const char, const long>::res = " <<
               all<isConst, const int, const char, const long>::res << "\n" <<
               "all<isConst, const int, bool>::res = " <<
               all<isConst, const int, bool>::res <<
               std::endl;

  std::cout << "foldr<Plus, Integer<0>, Integer<1>, Integer<2>, Integer<3>>::res = " <<
               foldr<Plus, Integer<0>, Integer<1>, Integer<2>, Integer<3>>::res << "\n" <<
               "char(foldr<Plus, Char<'A'>, Char<1>, Char<2>>::res) = " <<
               char(foldr<Plus, Char<'A'>, Char<1>, Char<2>>::res) <<
               std::endl;

  std::cout << "count<int, long, char>::res = " <<
               count<int, long, char>::res <<
               std::endl;

  std::cout << "SumSquares<Integer<1>, Integer<2>, Integer<3>>::res = " <<
               SumSquares<Integer<1>, Integer<2>, Integer<3>>::res <<
               std::endl;
}
