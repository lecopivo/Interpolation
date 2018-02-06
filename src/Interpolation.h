#pragma once

#include <cmath>
#include <tuple>

#include "meta_utils.h"

template <class... Interpolation1D> class InterpolationDimWise {
public:
  constexpr static int Dim = sizeof...(Interpolation1D);

public:
  InterpolationDimWise(Interpolation1D... interpolation)
      : m_interpolation(interpolation...) {}

  template <class Fun, class... Real> auto operator()(Fun &&fun, Real... x) {
    return interpolate_impl<0>(fun, x...);
  }

private:
  template <int I, class Fun, class... Real>
  inline auto interpolate_impl(Fun &&fun, Real... x) {

    if constexpr (I == Dim) {
      return fun(x...);
    } else {
      auto y = get<I>(x...);
      auto &interpolation = std::get<I>(m_interpolation);
      auto funI = [x..., &fun,this](int i) mutable {
        get<I>(x...) = i;
        return interpolate_impl<I + 1>(fun, x...);
      };
      return interpolation(funI, y);
    }
  }

  std::tuple<Interpolation1D...> m_interpolation;
};

auto ConstantInterpolation = [](auto &&fun, auto x) {
  int ix = (int)round(x);

  return fun(ix);
};

// maybe the return type should be:
// decltype(fun(int())
// because arithemetics operation can create only an expression(for with Eigen)
auto LinearInterpolation = [](auto &&fun, auto x) {
  auto wx = x - floor(x);
  int ix = (int)floor(x);

  return (wx != 0 ? wx * fun(ix + 1) : 0) + (wx != 1 ? (1 - wx) * fun(ix) : 0);
};

auto CubicInterpolation = [](auto &&fun, auto x) {
  // for explanation see https://en.wikipedia.org/wiki/Cubic_Hermite_spline
  int ix = (int)floor(x);
  auto wx = x - ix;

  auto pm1 = fun(ix - 1);
  auto p0 = fun(ix);
  auto p1 = fun(ix + 1);
  auto p2 = fun(ix + 2);

  auto m0 = 0.5 * (p1 - pm1);
  auto m1 = 0.5 * (p2 - p0);

  auto t1 = wx;
  auto t2 = t1 * wx;
  auto t3 = t2 * wx;

  return (2.0 * t3 - 3.0 * t2 + 1.0) * p0 + (t3 - 2.0 * t2 + t1) * m0 +
         (-2.0 * t3 + 3.0 * t2) * p1 + (t3 - t2) * m1;
};
