#include "sir_diff.hpp"
std::vector<Sir::State> Sir::Epidemic::evolve(int T)
{
  std::vector<State> result{e_initial_state};
  int const N = result.back().S + result.back().I + result.back().R;
  for (int t = 1; t != T; ++t) {
    State const& prev = result.back();
    State next;

    next.b = prev.b;

    next.S = std::round(prev.S - next.b * prev.S * prev.I / N);
    next.I =
        std::round(prev.I + next.b * prev.S * prev.I / N - prev.g * prev.I);
    next.R = N - next.S - next.I;
    next.g = prev.g;
    if (next.S == prev.S && next.I == prev.I && next.R == prev.R) {
      next.I = prev.I - 1;
      next.R = prev.R + 1;
    }
    assert(N == next.S + next.I + next.R);
    result.push_back(next);
    if (next.I == 0) {
      break;
    }
  }
  return result;
}