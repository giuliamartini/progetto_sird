#include <fstream>
#include "sir_diff.hpp"

void print(std::vector<Sir::State> const& states)
{
  std::cout
      << "+---------------+----------------+---------------+---------------+\n"
         "|      day      |        S       |       I       |       R       |\n"
         "+---------------+----------------+---------------+---------------+\n"
      << std::fixed;
  int T = states.size();
  for (int t = 0; t != T; ++t) {
    std::cout << "| " << '\t' << t + 1 << '\t' << " | " << '\t' << states[t].S
              << '\t' << " | " << '\t' << states[t].I << '\t' << " | " << '\t'
              << states[t].R << '\t' << " |\n";
  }
  std::cout
      << "+---------------+----------------+---------------+---------------+\n";
}
int main()
{
  std::ofstream graph;
  graph << "graph_diff\n";
  graph.open("graph_diff.csv");
  graph << "Day,Susceptible,Infected,Recovered\n";

  // int infects = board.Sird::Population::count(infectivity).I;
  int numberOfS = 300;
  int numberOfI = 4;
  double b = 0.8;
  double g = 0.5;
  int T = 200;

  Sir::State s0{};
  s0.S = numberOfS;
  s0.I = numberOfI;
  s0.R = 0;
  s0.b = b;
  s0.g = g;
  int frame = 0;

  Sir::Epidemic plague{s0};
  auto const states = plague.evolve(T);
  for (int i = 0, end = states.size(); i != end; ++i) {
    std::cout << "S: "
              << states[i].S
              // tra i suscettibili conto anche i vaccinati
              << " - I: " << states[i].I << " - R: " << states[i].R << "\n\n";

    std::cout << "Day " << frame << '\n';
    graph << frame << "," << states[i].S << "," << states[i].I << ","
          << states[i].R << '\n';
    ++frame;
  }
  print(states);

  graph.close();
  system("python ./graph_diff.py");
}
