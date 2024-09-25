## 一、 Boost 的数值微分方法
数值微分方法 (使用finite_difference.hpp)
```cpp 
#include <iostream>
#include <boost/math/differentiation/finite_difference.hpp>

using namespace boost::math::differentiation;
using namespace std;


int main() {

    auto f = [](double x) { return std::exp(x); };
    double x = 1.7;
    double dfdx = finite_difference_derivative(f, x);

    cout << dfdx << endl;
}

```

自动求解微分(使用autodiff.hpp)
```cpp 
#include <boost/math/differentiation/autodiff.hpp>
#include <iostream>

template <typename T>
T fourth_power(T const& x) {
    T x4 = x * x;  // retval in operator*() uses x4's memory via NRVO.
    x4 *= x4;      // No copies of x4 are made within operator*=() even when squaring.
    return x4;     // x4 uses y's memory in main() via NRVO.
}

int main() {
    using namespace boost::math::differentiation;

    constexpr unsigned Order = 5;                  // Highest order derivative to be calculated.
    auto const x = make_fvar<double, Order>(2.0);  // Find derivatives at x=2.
    auto const y = fourth_power(x);
    for (unsigned i = 0; i <= Order; ++i)
        std::cout << "y.derivative(" << i << ") = " << y.derivative(i) << std::endl;
    return 0;
}
```

### [Example 2: Multi-variable mixed partial derivatives with multi-precision data type](https://www.boost.org/doc/libs/1_82_0/libs/math/doc/html/math_toolkit/autodiff.html#math_toolkit.autodiff.example-multiprecision)

#### [Calculate ![](https://www.boost.org/doc/libs/1_82_0/libs/math/doc/equations/autodiff/mixed12.svg) with a precision of about 50 decimal digits, where ![](https://www.boost.org/doc/libs/1_82_0/libs/math/doc/equations/autodiff/example2f.svg).](https://www.boost.org/doc/libs/1_82_0/libs/math/doc/html/math_toolkit/autodiff.html#math_toolkit.autodiff.calculate_autodiff_equation_mixe)

In this example, `make_ftuple<float50, Nw, Nx, Ny, Nz>(11, 12, 13, 14)` returns a `std::tuple` of 4 independent `fvar` variables, with values of 11, 12, 13, and 14, for which the maximum order derivative to be calculated for each are 3, 2, 4, 3, respectively. The order of the variables is important, as it is the same order used when calling `v.derivative(Nw, Nx, Ny, Nz)` in the example below.

```cpp 
#include <boost/math/differentiation/autodiff.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <iostream>

using namespace boost::math::differentiation;

template <typename W, typename X, typename Y, typename Z>
promote<W, X, Y, Z> f(const W& w, const X& x, const Y& y, const Z& z) {
  using namespace std;
  return exp(w * sin(x * log(y) / z) + sqrt(w * z / (x * y))) + w * w / tan(z);
}

int main() {
  using float50 = boost::multiprecision::cpp_bin_float_50;

  constexpr unsigned Nw = 3;  // Max order of derivative to calculate for w
  constexpr unsigned Nx = 2;  // Max order of derivative to calculate for x
  constexpr unsigned Ny = 4;  // Max order of derivative to calculate for y
  constexpr unsigned Nz = 3;  // Max order of derivative to calculate for z
  // Declare 4 independent variables together into a std::tuple.
  auto const variables = make_ftuple<float50, Nw, Nx, Ny, Nz>(11, 12, 13, 14);
  auto const& w = std::get<0>(variables);  // Up to Nw derivatives at w=11
  auto const& x = std::get<1>(variables);  // Up to Nx derivatives at x=12
  auto const& y = std::get<2>(variables);  // Up to Ny derivatives at y=13
  auto const& z = std::get<3>(variables);  // Up to Nz derivatives at z=14
  auto const v = f(w, x, y, z);
  // Calculated from Mathematica symbolic differentiation.
  float50 const answer("1976.319600747797717779881875290418720908121189218755");
  std::cout << std::setprecision(std::numeric_limits<float50>::digits10)
            << "mathematica   : " << answer << '\n'
            << "autodiff      : " << v.derivative(Nw, Nx, Ny, Nz) << '\n'
            << std::setprecision(3)
            << "relative error: " << (v.derivative(Nw, Nx, Ny, Nz) / answer - 1) << '\n';
  return 0;
}
/*
Output:
mathematica   : 1976.3196007477977177798818752904187209081211892188
autodiff      : 1976.3196007477977177798818752904187209081211892188
relative error: 2.67e-50
*/
```

### [Example 3: Black-Scholes Option Pricing with Greeks Automatically Calculated](https://www.boost.org/doc/libs/1_82_0/libs/math/doc/html/math_toolkit/autodiff.html#math_toolkit.autodiff.example-black_scholes)

#### [Calculate greeks directly from the Black-Scholes pricing function.](https://www.boost.org/doc/libs/1_82_0/libs/math/doc/html/math_toolkit/autodiff.html#math_toolkit.autodiff.calculate_greeks_directly_from_t)

Below is the standard Black-Scholes pricing function written as a function template, where the price, volatility (sigma), time to expiration (tau) and interes rate are template parameters. This means that any greek based on these 4 variables can be calculated using autodiff. The below example calculates delta and gamma where the variable of differentiation is only the price. For examples of more exotic greeks, see `example/black_scholes.cpp`.


```cpp 
#include <boost/math/differentiation/autodiff.hpp>
#include <iostream>

using namespace boost::math::constants;
using namespace boost::math::differentiation;

// Equations and function/variable names are from
// https://en.wikipedia.org/wiki/Greeks_(finance)#Formulas_for_European_option_Greeks

// Standard normal cumulative distribution function
template <typename X>
X Phi(X const& x) {
  return 0.5 * erfc(-one_div_root_two<X>() * x);
}

enum class CP { call, put };

// Assume zero annual dividend yield (q=0).
template <typename Price, typename Sigma, typename Tau, typename Rate>
promote<Price, Sigma, Tau, Rate> black_scholes_option_price(CP cp,
                                                            double K,
                                                            Price const& S,
                                                            Sigma const& sigma,
                                                            Tau const& tau,
                                                            Rate const& r) {
  using namespace std;
  auto const d1 = (log(S / K) + (r + sigma * sigma / 2) * tau) / (sigma * sqrt(tau));
  auto const d2 = (log(S / K) + (r - sigma * sigma / 2) * tau) / (sigma * sqrt(tau));
  switch (cp) {
    case CP::call:
      return S * Phi(d1) - exp(-r * tau) * K * Phi(d2);
    case CP::put:
      return exp(-r * tau) * K * Phi(-d2) - S * Phi(-d1);
  }
}

int main() {
  double const K = 100.0;                    // Strike price.
  auto const S = make_fvar<double, 2>(105);  // Stock price.
  double const sigma = 5;                    // Volatility.
  double const tau = 30.0 / 365;             // Time to expiration in years. (30 days).
  double const r = 1.25 / 100;               // Interest rate.
  auto const call_price = black_scholes_option_price(CP::call, K, S, sigma, tau, r);
  auto const put_price = black_scholes_option_price(CP::put, K, S, sigma, tau, r);

  std::cout << "black-scholes call price = " << call_price.derivative(0) << '\n'
            << "black-scholes put  price = " << put_price.derivative(0) << '\n'
            << "call delta = " << call_price.derivative(1) << '\n'
            << "put  delta = " << put_price.derivative(1) << '\n'
            << "call gamma = " << call_price.derivative(2) << '\n'
            << "put  gamma = " << put_price.derivative(2) << '\n';
  return 0;
}
/*
Output:
black-scholes call price = 56.5136
black-scholes put  price = 51.4109
call delta = 0.773818
put  delta = -0.226182
call gamma = 0.00199852
put  gamma = 0.00199852
*/
```

## 二、 使用Boost进行的高斯-勒让德积分

`auto integrator = gauss<double, 10>::integrate([&](double x) {return PhiFunc(x, &params); }, x_i, x_j);`

