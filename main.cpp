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
          os << '^';
          // os << "\033[36m^\033[0m";
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
  std::cout << "Please enter the value of beta" << '\n';
  double beta;
  //= 0.8;
  std::cin >> beta;
  // std::cout << '\n' << "Please enter the value of gamma " << '\n';
  double gamma = 0.3;
  // std::cin >> gamma;
  //  std::cout << '\n' << "Please enter the value of delta " << '\n';
  double delta = 0.05;
  //  std::cin >> delta;
  // std::cout << "Please enter the value of infectivity" << '\n';
  // std::cout << "Please enter epsilon" << '\n';
  double epsilon = 0.7;
  // std::cin >> epsilon;
  //= 0.7;
  int infectivity = 3;
  // std::cin >> infectivity;
  // std::cout << "Please enter the number of people, less than n*n*3" << '\n';
  int numberOfPeople

      = 4000;
  // std::cin >> numberOfPeople;
  // std::cout  << "Please enter the dimension of the board" << '\n';
  constexpr int n_ = 50;
  // std::cin >> n_;

  //  numero di persone massimo che si possono vaccinare in un giorno
  // std::cout << "Please enter the number numero massimo di vaccini al giorno"
  //         << '\n';
  int toBeVaccinated = 20;
  // std::cin >> toBeVaccinated;

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
              << " - I: " << count.I << " - R: " << count.R << "D: " << count.D
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
