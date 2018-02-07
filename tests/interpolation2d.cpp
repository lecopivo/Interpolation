#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Interpolation.h"

char const *help_message =
    "This program takes function f(x,y)= cos(3*r)/(r+1), where r "
    "=sqrt(x^2+y^2), and "
    "samples it on square [-2,2]^2 with 10x10 samples. "
    "Then it preforms different kinds of "
    "interpolations: piecewise constant, linear and "
    "cubic. The interpolation is done at 100x100 points.\n\nTo run this "
    "program, use the provided script `interpolation2d.sh` which runs this "
    "program and displays the result with gnuplot";

using namespace std;

void save_data(std::vector<double> x, std::vector<double> y,
               std::vector<double> data, std::string name) {
  ofstream file;
  file.open(name.append(".dat"));
  for (int i = 0; i < x.size(); i++) {
    file << x[i] << " " << y[i] << " " << data[i] << std::endl;
  }
  file.close();
}

int main() {

  std::cout << help_message << std::endl;

  int N = 10;
  int M = 100;
  double xmin = -2.0;
  double xmax = 2.0;
  double dx = (xmax - xmin) / (N - 1);
  double dy = (xmax - xmin) / (M - 1);
  std::vector<double> data_y(N * N);

  auto fun = [](double x, double y) {
    double r = sqrt(x * x + y * y);
    return cos(5 * r) / (r + 1);
  };

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      double x = xmin + i * dx;
      double y = xmin + j * dx;

      data_y[i * N + j] = fun(x, y);
    }
  }

  // define behavior outside of data range
  auto foo = [&](int i, int j) {

    i = min(max(i, 0), N - 1);
    j = min(max(j, 0), N - 1);

    return data_y[i * N + j];
  };

  std::vector<double> interpol_x(M * M);
  std::vector<double> interpol_y(M * M);
  std::vector<double> exact_y(M * M);
  std::vector<double> constant_y(M * M);
  std::vector<double> linear_y(M * M);
  std::vector<double> cubic_y(M * M);

  auto constant_foo =
      InterpolationDimWise(ConstantInterpolation, ConstantInterpolation)(foo);
  auto linear_foo =
      InterpolationDimWise(LinearInterpolation, LinearInterpolation)(foo);
  auto cubic_foo =
      InterpolationDimWise(CubicInterpolation, CubicInterpolation)(foo);

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      double x = xmin + i * dy;
      double y = xmin + j * dy;
      double ix = i * dy / dx;
      double iy = j * dy / dx;

      interpol_x[i * M + j] = x;
      interpol_y[i * M + j] = y;

      exact_y[i * M + j] = fun(x, y);
      constant_y[i * M + j] = constant_foo(ix, iy);
      linear_y[i * M + j] = linear_foo(ix, iy);
      cubic_y[i * M + j] = cubic_foo(ix, iy);
    }
  }

  save_data(interpol_x, interpol_y, exact_y, "exact2d");
  save_data(interpol_x, interpol_y, constant_y, "constant2d");
  save_data(interpol_x, interpol_y, linear_y, "linear2d");
  save_data(interpol_x, interpol_y, cubic_y, "cubic2d");

  return 0;
}
