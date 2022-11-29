#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <thread>
#include "sird.hpp"

void print_f(std::ostream& os,
             Sird::Population const& population,
             int infectivity)
{
  constexpr auto clear = "\033[2J";
  int const N = population.side();

  os << clear;
  os << "\t\t\t" << '+' << std::string(N * 3 - 2 * infectivity, '-') << "+\n";

  for (int r = 0; r < N; ++r) {
    os << "\t\t\t" << '|';
    for (int c = 0; c < 3 * N; ++c) {
      Sird::Person const& person = population.person(r, c);
      switch (person) {
        case Sird::Person::Infected:
          os << "\033[31m*\033[0m";
          break;
        case Sird::Person::Vaccinated:
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
  os << "\t\t\t" << '+' << std::string(N * 3, '-') << "+\n";
}

int main()
{
  std::cout << "Please enter the value of beta, make sure it is between 0.0 and 1.0" << '\n';
  double beta;
  std::cin >> beta;
  std::cout << '\n' << "Please enter the value of gamma, make sure it is between 0.0 and 1.0" << '\n';
  double gamma;
  std::cin >> gamma;
  std::cout << '\n' << "Please enter the value of delta, make sure it is between 0.0 and 1.0" << '\n';
  double delta;
  std::cin >> delta;
  std::cout << '\n' << "Please enter the value of epsilon, make sure it is between 0.0 and 1.0" << '\n';
  double epsilon;
  std::cin >> epsilon;
  std::cout << '\n' << "Please enter the value of infectivity (i)" << '\n';
  int infectivity; 
  std::cin >> infectivity;
  std::cout  << '\n' << "Please enter the dimension of the board" << '\n';
  int n_;
  std::cin >> n_;
  std::cout << '\n' << "Please enter the initial number of susceptibles, make sure it is less than 3 * (n - i) * (n - i)" << '\n';
  int numberOfPeople;
  std::cin >> numberOfPeople;
  
  //  numero di persone massimo che si possono vaccinare in un giorno
  int toBeVaccinated = 20;

  // constexpr int board_size = n_;
  Sird::Population board(n_);
  board.Sird::Population::placePeople(numberOfPeople, infectivity);

  board.Sird::Population::setInfected(
      {n_ / 2, n_ / 2 * 3});  // pazienti 0 al centro della board
  board.Sird::Population::setInfected({n_ / 2, n_ / 2 * 3 - 1});
  board.Sird::Population::setInfected({n_ / 2, n_ / 2 * 3 + 1});
  board.Sird::Population::setInfected({n_ / 2 + 1, n_ / 2 * 3});
  board.Sird::Population::setInfected({n_ / 2 - 1, n_ / 2 * 3});
  print_f(std::cout, board, infectivity);
  int frame = 0;
  Sird::State initialSituation = board.counts(infectivity);
  int init = initialSituation.S + initialSituation.I;
  // definizione grafico
  std::ofstream graph;
  graph << "Graph\n";
  graph.open("Graph.csv");
  graph << "Day,Susceptible,Infected,Recovered,Dead,Vaccinated\n";

  // int infects = board.Sird::Population::count(infectivity).I;
  Sird::State count{0, 0, 0, 0, 0};

  while (board.Sird::Population::counts(infectivity).I != 0) {
    count = board.counts(infectivity);
    assert(count.S + count.I + count.R + count.D + count.V == init);
    print_f(std::cout, board, infectivity);
    std::cout << "S: "
              << count.S + count.V
              // tra i suscettibili conto anche i vaccinati
              << " - I: " << count.I << " - R: " << count.R << " - D: " << count.D
              << " - V: " << count.V << "\n\n";

    std::cout << "Day " << frame << '\n';
    graph << frame << "," << count.S << "," << count.I << "," << count.R << ","
          << count.D << "," << count.V << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //  std::chrono_literals::std::this_thread::sleep_for(10ms);

    board.evolve(infectivity, beta, gamma, delta, epsilon);
    board.move(infectivity);
    ++frame;

    if (frame >= 20 &&
        count.V <=
            static_cast<int>(count.S *
                             epsilon)) {  // dopo 20 giorni partono i vaccini
      board.massVaccination(infectivity, toBeVaccinated, epsilon);
      if (frame >= 40) {
        board.resusceptible(infectivity, 0.1, count);
      }  // dopo 40 giorni cominciano a tornare susceptible
    }
    /*if (frame >= 50) {  // dopo 50 giorni i vaccini non fanno più effetto
      board.vaccinationOver(infectivity, gamma, count);
    }*/
  }
  system("clear");
  graph.close();
  system("python ./graph.py");
}
