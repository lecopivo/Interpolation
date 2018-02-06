#include <iostream>

#include "Interpolation.h"

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

  std::cout << "x:\t";
  for (int i = 0; i <= 10; i++) {
    double x = i / 10.0;
    std::cout << x << "\t";
  }

  std::cout << std::endl <<  "Const:\t";
  for (int i = 0; i <= 10; i++) {
    double x = i / 10.0;
    std::cout << ConstantInterpolation(fun1d, x) << "\t";
  }

  std::cout << std::endl <<  "Linear:\t";
  for (int i = 0; i <= 10; i++) {
    double x = i / 10.0;
    std::cout << LinearInterpolation(fun1d,x) << "\t";
  }

  std::cout << std::endl <<  "Cubic:\t";
  for (int i = 0; i <= 10; i++) {
    double x = i / 10.0;
    std::cout << CubicInterpolation(fun1d,x) << "\t";
  }

  //  ____    ____  _                          _
  // |___ \  |  _ \(_)_ __ ___   ___ _ __  ___(_) ___  _ __  ___
  //   __) | | | | | | '_ ` _ \ / _ \ '_ \/ __| |/ _ \| '_ \/ __|
  //  / __/  | |_| | | | | | | |  __/ | | \__ \ | (_) | | | \__ \
  // |_____| |____/|_|_| |_| |_|\___|_| |_|___/_|\___/|_| |_|___/
  
  std::cout << std::endl << std::endl << "Linear interpolation in 2D: " << std::endl;

  auto interpol2d =
      InterpolationDimWise(LinearInterpolation, LinearInterpolation);

  auto fun2d = [](int i, int j) {
    if (i <= 0 || j <= 0)
      return -1.0;
    return 1.0;
  };

  for (int i = 0; i <= 10; i++) {
    for (int j = 0; j <= 10; j++) {
      double x = i * 0.1;
      double y = j * 0.1;

      std::cout << interpol2d(fun2d, x, y) << "\t";
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

  auto CustomInterpolation = [](auto &&fun, auto x) {
    auto wx = x - floor(x);
    int ix = (int)floor(x);

    if (wx >= 0.3 && wx <= 0.7)
      return 0.0;

    return (wx != 0 ? wx * fun(ix + 1) : 0) +
           (wx != 1 ? (1 - wx) * fun(ix) : 0);
  };

  auto custom_interpol =
      InterpolationDimWise(CustomInterpolation, CustomInterpolation);

  for (int i = 0; i <= 10; i++) {
    for (int j = 0; j <= 10; j++) {
      double x = i * 0.1;
      double y = j * 0.1;

      std::cout << custom_interpol(fun2d, x, y) << "\t\e[0m";
    }
    std::cout << std::endl;
  }

  return 0;
}
