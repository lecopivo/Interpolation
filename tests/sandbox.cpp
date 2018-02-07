#include <iostream>

#include "Interpolation2.h"

int main() {

  //  _   ____  _                          _
  // / | |  _ \(_)_ __ ___   ___ _ __  ___(_) ___  _ __
  // | | | | | | | '_ ` _ \ / _ \ '_ \/ __| |/ _ \| '_ \ 
  // | | | |_| | | | | | | |  __/ | | \__ \ | (_) | | | |
  // |_| |____/|_|_| |_| |_|\___|_| |_|___/_|\___/|_| |_|

  auto fun1d = [](int i) {
    if (i <= 0)
      return -1.0;
    return 1.0;
  };

  auto run_1d_test = [](auto &&fun, auto &&interpol,
                        char const *interpol_name) {
    std::cout << interpol_name << "\t";
    auto ifun = interpol(fun);
    for (double x = 0.0; x <= 1.0; x += 0.1) {
      std::cout << ifun(x) << "\t";
    }
    std::cout << std::endl;
  };

  run_1d_test(fun1d, [](auto &&fun) { return [](auto x) { return x; }; }, "x:");
  run_1d_test(fun1d, ConstantInterpolation, "const:");
  run_1d_test(fun1d, LinearInterpolation, "linear:");
  run_1d_test(fun1d, CubicInterpolation, "cubic:");

  //  ____    ____  _                          _
  // |___ \  |  _ \(_)_ __ ___   ___ _ __  ___(_) ___  _ __  ___
  //   __) | | | | | | '_ ` _ \ / _ \ '_ \/ __| |/ _ \| '_ \/ __|
  //  / __/  | |_| | | | | | | |  __/ | | \__ \ | (_) | | | \__ \
  // |_____| |____/|_|_| |_| |_|\___|_| |_|___/_|\___/|_| |_|___/

  std::cout << std::endl
            << std::endl
            << "Linear interpolation in 2D: " << std::endl;

  auto fun2d = [](int i, int j) {
    if (i <= 0 || j <= 0)
      return -1.0;
    return 1.0;
  };

  auto linear_fun2d =
      InterpolationDimWise(fun2d, LinearInterpolation, LinearInterpolation);

  for (int i = 0; i <= 10; i++) {
    for (int j = 0; j <= 10; j++) {
      double x = i * 0.1;
      double y = j * 0.1;

      std::cout << linear_fun2d(x, y) << "\t";
    }
    std::cout << std::endl;
  }

  //                 _                    _       _                        _
  //   ___ _   _ ___| |_ ___  _ __ ___   (_)_ __ | |_ ___ _ __ _ __   ___ | |
  //  / __| | | / __| __/ _ \| '_ ` _ \  | | '_ \| __/ _ \ '__| '_ \ / _ \| |
  // | (__| |_| \__ \ || (_) | | | | | | | | | | | ||  __/ |  | |_) | (_) | |
  //  \___|\__,_|___/\__\___/|_| |_| |_| |_|_| |_|\__\___|_|  | .__/ \___/|_|
  //                                                          |_|

  std::cout << std::endl << "Custom Interpolation: " << std::endl;

  auto CustomInterpolation = [](auto fun) {
    return [=](auto x) mutable {
      auto wx = x - floor(x);
      int ix = (int)floor(x);

      if (wx >= 0.3 && wx <= 0.7)
        return 0.0;

      return (wx != 0 ? wx * fun(ix + 1) : 0) +
             (wx != 1 ? (1 - wx) * fun(ix) : 0);
	   };
  };

  auto custom_fun2d =
      InterpolationDimWise(fun2d, CustomInterpolation, CustomInterpolation);

  for (int i = 0; i <= 10; i++) {
    for (int j = 0; j <= 10; j++) {
      double x = i * 0.1;
      double y = j * 0.1;

      std::cout << custom_fun2d(x, y) << "\t\e[0m";
    }
    std::cout << std::endl;
  }

  return 0;
}
