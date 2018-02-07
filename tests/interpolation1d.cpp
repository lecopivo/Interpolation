#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Interpolation.h"

char const *help_message =
    "This program takes function f(x)= sin(3*x)/x and "
    "samples it on interval [1,10] with 20 samples. "
    "Then it preforms different kinds of "
    "interpolations: piecewise constant, linear and "
    "cubic. The interpolation is done at 400 points.\n\nTo run this program, "
    "use the provided script `interpolation1d.sh` which runs this program and "
    "displays the result with gnuplot";

using namespace std;

void save_data(std::vector<double> x, std::vector<double> y, std::string name) {
  ofstream file;
  file.open(name.append(".dat"));
  for (int i = 0; i < x.size(); i++) {
    file << x[i] << " " << y[i] << std::endl;
  }
  file.close();
}

int main() {

  std::cout << help_message << std::endl;

  int N = 20;
  int M = 200;
  double xmin = 1.0;
  double xmax = 10.0;
  double dx = (xmax - xmin) / (N - 1);
  double dy = (xmax - xmin) / (M - 1);
  std::vector<double> data_y(N);
  auto fun = [](double x) { return sin(3.0 * x) / x; };

  for (int i = 0; i < N; i++) {
    double x = xmin + i * dx;

    data_y[i] = fun(x);
  }

  // define behavior outside of data range
  auto foo = [&](int i) { return data_y[min(max(i, 0), N - 1)]; };

  std::vector<double> interpol_x(M);
  std::vector<double> exact_y(M);
  std::vector<double> constant_y(M);
  std::vector<double> linear_y(M);
  std::vector<double> cubic_y(M);

  auto const_foo = ConstantInterpolation(foo);
  auto linear_foo = LinearInterpolation(foo);
  auto cubic_foo = CubicInterpolation(foo);

  for (int i = 0; i < M; i++) {
    double x = xmin + i * dy;
    double ix = i * dy / dx;

    interpol_x[i] = x;

    exact_y[i] = fun(x);
    constant_y[i] = const_foo(ix);
    linear_y[i] = linear_foo(ix);
    cubic_y[i] = cubic_foo(ix);
  }

  save_data(interpol_x, exact_y, "exact1d");
  save_data(interpol_x, constant_y, "constant1d");
  save_data(interpol_x, linear_y, "linear1d");
  save_data(interpol_x, cubic_y, "cubic1d");

  return 0;
}
