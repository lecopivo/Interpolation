#pragma once

#include "meta_utils.h"

template <typename... Interpolation1D>
class InterpolationDimWise {
public:
  template <int I>
  using Interpolation = getType<I, Interpolation1D...>;

  constexpr int Dim = sizeof...(Interpolation1D);

  template <typename Fun, typename Real>
  static auto interpolate(Fun const &fun, Real... x) {

    static_assert(sizeof...(x) == Dim, "Invalid number of input numbers.");

    return interpolate_impl<0>(fun, x...);
  }

private:
  template <int I, typename Fun, typename... Real>
  auto interpolate_impl(Fun const &fun, Real... x) {

    if constexpr (I == Dim) {
      return fun(x...);
    } else {
      return Interpolation<I>::interpolate([x..., &fun, this](int i) {
        auto &y = get<I>(x...);
        y       = i;
        return interpolate_impl<I + 1>(fun, x...);
      });
    }
  }
};

template <typename Interpolation, int N>
using InterpolationDimWiseN =
    typename ApplyNTimes<N>::template apply<InterpolationDimWise,
                                            Interpolation>;

class ConstantInterpolation {
public:
  template <typename Fun, typename Real>
  static std::invoke_result_t<Fun, int> interpolate(Fun const &fun,
                                                    Real       x) const {

    int ix = (int)round(x);

    return fun(ix);
  }
};

class LinearInterpolation {
public:
  template <typename Fun, typename Real>
  static std::invoke_result_t<Fun, int> interpolate(Fun const &fun,
                                                    Real       x) const {

    Real wx = x - floor(x);
    int  ix = (int)floor(x);

    return (wx != 0 ? wx * fun(ix + 1) : 0) +
           (wx != 1 ? (1 - wx) * fun(ix) : 0);
  }
};

class CubicInterpolation {
public:
  template <typename Fun, typename Real>
  static std::invoke_result_t<Fun, int> interpolate(Fun const &fun,
                                                    Real       x) const {
    // for explanation see https://en.wikipedia.org/wiki/Cubic_Hermite_spline
    int  ix = (int)floor(x);
    Real wx = x - ix;

    auto pm1 = fun(ix - 1);
    auto p0  = fun(ix);
    auto p1  = fun(ix + 1);
    auto p2  = fun(ix + 2);

    auto m0 = 0.5 * (p1 - pm1);
    auto m1 = 0.5 * (p2 - p0);

    Real t1 = wx;
    Real t2 = t1 * wx;
    Real t3 = t2 * wx;

    return (2.0 * t3 - 3.0 * t2 + 1.0) * p0 + (t3 - 2.0 * t2 + t1) * m0 +
           (-2.0 * t3 + 3.0 * t2) * p1 + (t3 - t2) * m1;
  }
};

template <typename... Interpolation1D>
class InterpolationDimWise {
public:
  template <int I>
  using Interpolation = getType<I, Interpolation1D...>;

  constexpr int Dim = sizeof...(Interpolation1D);

  template <typename Fun, typename Real>
  static auto interpolate(Fun const &fun, std::array<Real, Dim> x) {

    return interpolate_impl<0>(fun, x);
  }

  template <typename Fun>
  static auto createInterpolation(Fun const &fun) {
    return [&fun](auto... x) { return interpolate(fun, {x...}); };
  }

private:
  template <int I, typename Fun, typename Real>
  auto interpolate_impl(Fun const &fun, std::array<Real, Dim> x) {

    if constexpr (I == Dim) {
      return fun(x);
    } else {
      return Interpolation<I>::interpolate([x, &fun, this](int i) {
        x[I] = i;
        return interpolate_impl<I + 1>(fun, x);
      });
    }
  }
};

double c_style_interpolate(double fm1, double f0, double fp1, double fp2,
                           double x) {
  return f0;
}

double call_c_style(Fun const &fun, int center, double x, Int... ids) {
  return c_style_interpolate(fun(center + ids)..., x);
}
