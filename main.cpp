#include <chrono>
#include <fstream> //file.csv
#include <thread> //sleep, chrono
#include "sird.hpp"

#include "sird.hpp"

void print_f(std::ostream& os,
             Sird::Population const& population,
             int infectivity)
{
  constexpr auto clear = "\033[2J";
  int const N = population.side();

  os << clear;
  os << "\t\t\t" << '+' << std::string(N * 3 - 2 - infectivity, '-') << "+\n";

  for (int r = infectivity; r < N - infectivity + 1; ++r) {
    os << "\t\t\t" << '|';
    for (int c = infectivity; c < 3 * N - infectivity + 1; ++c) {
      Sird::Person const& person = population.person(r, c);
      switch (person) {
        case Sird::Person::Infected:
          os << "\033[31m*\033[0m";
          break;
        case Sird::Person::Vaccinated:
          // os << '^';
          os << "\033[32m^\033[0m";
          break;
        case Sird::Person::Susceptible:
          os << "\033[36m+\033[0m";
          break;
        case Sird::Person::Recovered:
          os << "\033[33m-\033[0m";
          break;
        case Sird::Person::Dead:
          os << '.';
          break;
        case Sird::Person::Empty:
          os << ' ';
          break;
        default:
          std::cout << "Strange error in print() funcion: board_[" << r << " , "
                    << c << "] is an unknown State\n";
      }
    }
    os << "|\n";
  }
  os << "\t\t\t" << '+' << std::string(N * 3 - 2 - infectivity, '-') << "+\n";
}

int main()
{
  std::cout
      << "Please enter the value of beta, make sure it is between 0.0 and 1.0"
      << '\n';
  double beta;

  std::cin >> beta;
  if (beta < 0 || beta > 1) {
    throw std::runtime_error{"the value of beta is not allowed \n"};
  };
  std::cout
      << '\n'
      << "Please enter the value of gamma, make sure it is between 0.0 and 1.0"
      << '\n';
  double gamma;
  std::cin >> gamma;
  if (gamma < 0 || gamma > 1) {
    throw std::runtime_error{"the value of gamma is not allowed \n"};
  };
  std::cout
      << '\n'
      << "Please enter the value of delta, make sure it is between 0.0 and 1.0"
      << '\n';
  double delta;
  std::cin >> delta;
  if (delta < 0 || delta > 1) {
    throw std::runtime_error{"the value of delta is not allowed \n"};
  };
  std::cout << '\n'
            << "Please enter the value of epsilon, make sure it is between 0.0 "
               "and 1.0"
            << '\n';
  double epsilon;

  std::cin >> epsilon;
  if (epsilon < 0 || epsilon > 1) {
    throw std::runtime_error{"the value of epsilon is not allowed \n"};
  };
  std::cout << '\n'
            << "Please enter the dimension of the board (good choice between "
               "20 and 50)"
            << '\n';
  int n_;
  std::cin >> n_;
  std::cout << '\n'
            << "Please enter the value of infectivity (i), make sure it is "
               "less than 5"
            << '\n';
  int infectivity;
  std::cin >> infectivity;
  if (infectivity > 10) {
    throw std::runtime_error{"the value of infectivity is not allowed \n"};
  };

  std::cout << '\n'
            << "Please enter the initial number of susceptibles, make sure it "
               "is less than 3 * (n - i) * (n - i)"
            << '\n';
  int numberOfPeople;
  std::cin >> numberOfPeople;
  if (numberOfPeople > 3 * (n_ - infectivity) * (n_ - infectivity)) {
    throw std::runtime_error{"the number of people is not allowed \n"};
  };

  //  numero di persone massimo che si possono vaccinare in un giorno
  int toBeVaccinated = 20;

  Sird::Population board(n_);
  board.placePeople(numberOfPeople, infectivity);

  board.setInfected({n_ / 2, n_ / 2 * 3});  // pazienti 0 al centro della board
  board.setInfected({n_ / 2, n_ / 2 * 3 - 1});
  board.setInfected({n_ / 2, n_ / 2 * 3 + 1});
  board.setInfected({n_ / 2 + 1, n_ / 2 * 3});
  board.setInfected({n_ / 2 - 1, n_ / 2 * 3});
  print_f(std::cout, board, infectivity);
  int frame = 0;
  Sird::State initialSituation = board.counts();
  int init = initialSituation.S + initialSituation.I;

  // Grafico

  std::ofstream graph;
  graph << "Graph\n";
  graph.open("Graph.csv");
  graph << "Day,Susceptible,Infected,Recovered,Dead,Vaccinated\n";
  Sird::State count{0, 0, 0, 0, 0};

  while (board.counts().I != 0 && frame !=500) {
    count = board.counts();
    assert(count.S + count.I + count.R + count.D + count.V == init);
    print_f(std::cout, board, infectivity);
    std::cout << "S: "
              << count.S + count.V
              // tra i Susceptibles si contano anche i vaccinati
              << " - I: " << count.I << " - R: " << count.R
              << " - D: " << count.D << " - V: " << count.V << "\n\n";

    std::cout << "Day " << frame << '\n';
    graph << frame << "," << count.S << "," << count.I << "," << count.R << ","
          << count.D << "," << count.V << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    board.evolve(infectivity, beta, gamma, delta, epsilon);
    board.move(infectivity);
    ++frame;

    if (frame >= 20 &&
        count.V <=
            static_cast<int>(count.S *
                             epsilon)) {  // dopo 20 giorni partono i vaccini
      board.massVaccination(infectivity, toBeVaccinated, epsilon);
      if (frame >= 40) {
        board.susceptibleAgain(infectivity, 0.1, count);
      }  // dopo 40 giorni cominciano a tornare susceptible
    }
  }
  system("clear");
  graph.close();
  system("python ./graph.py");
}
