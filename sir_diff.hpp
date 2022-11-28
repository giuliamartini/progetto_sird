#include <iostream>
#ifndef SIR_DIFF_HPP
#define SIR_DIFF_HPP

#include <cassert>
#include <cmath>  // for std::round
#include <iostream>
#include <vector>
namespace Sir {
struct State
{
  int S;
  int I;
  int R;
  double b;
  double g;
};

class Epidemic
{
  State e_initial_state;

 public:
  Epidemic(State initial_state) : e_initial_state{initial_state}
  {
  }

  std::vector<State> evolve(int T);
};
}  // namespace Sir
#endif