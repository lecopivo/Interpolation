#pragma once

#include <cmath>
#include <type_traits>

#include "meta_utils.h"

template <int I>
auto InterpolateNthArgument = [](auto fun, auto interpolation) {
  return [=](auto... x) {
    static_assert(std::is_invocable_v<decltype(fun), decltype(x)...>,
                  "Invalid aguments");

    auto foo = [=](int i) mutable {
      get<I>(x...) = i;
      return fun(x...);
    };

    auto xI = get<I>(x...);
    return interpolation(foo)(xI);
  };
};

template <int I, class Fun, class... Interpolation>
auto InterpolationDimWise_impl(Fun fun, Interpolation... interpolation) {

  if constexpr (I == sizeof...(interpolation)) {
    return fun;
  } else {
    auto interpol = get<I>(interpolation...);
    return InterpolationDimWise_impl<I + 1>(
        InterpolateNthArgument<I>(fun, interpol), interpolation...);
  }
}

template <class... Interpolation>
auto InterpolationDimWise(Interpolation... interpolation) {
  return [=](auto fun) {
    return InterpolationDimWise_impl<0>(fun, interpolation...);
  };
}

auto ConstantInterpolation = [](auto fun) {
  return [=](auto x) mutable {
    int ix = (int)round(x);
    return fun(ix);
  };
};

auto LinearInterpolation = [](auto fun) {
  return [=](auto x) mutable {
    int ix = (int)floor(x);
    auto wx = x - ix;

    return (wx != 0 ? wx * fun(ix + 1) : 0) +
           (wx != 1 ? (1 - wx) * fun(ix) : 0);
  };
};

auto CubicInterpolation = [](auto fun) {
  return [=](auto x) mutable {
    // for explanation see https://en.wikipedia.org/wiki/Cubic_Hermite_spline
    int ix = (int)floor(x);
    auto wx = x - ix;

    if (wx == 0)
      return 1.0 * fun(ix);

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
};
