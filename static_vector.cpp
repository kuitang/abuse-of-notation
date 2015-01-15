#include <algorithm>
#include <iostream>
#include <initializer_list>

template <typename T, int len>
struct Vector {
  T value[len];

  Vector<T, len>(std::initializer_list<T> ell) {
    std::copy(ell.begin(), ell.begin() + len, value);
  }

  Vector<T, len> &operator+=(const Vector<T, len> &rhs) {
    for (int i = 0; i < len; ++i) {
      value[i] += rhs.value[i];
    }

    return *this;
  }

  friend std::ostream &operator<<(std::ostream &out, const Vector<T, len> &obj) {
    out << "[";
    for (int i = 0; i < len - 1; ++i) {
      out << obj.value[i] << ", ";
    }
    out << obj.value[len - 1] << "]";

    return out;
  }
};

/*
template <typename T, int len>
std::ostream &operator<<(std::ostream &out, const Vector<T, len> &obj) {
}
*/

int main() {
  Vector<double, 4> v1{ 1, 2, 3, 4};
  Vector<double, 4> v2{ -1, -2, -3, -4 };

  v1 += v2;

  std::cout << "v1 + v2 = " << v1 << std::endl;

  return 0;
}
