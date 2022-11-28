
#ifndef SIRD_HPP
#define SIRD_HPP
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

namespace Sird {
struct State
{
  int S;
  int I;
  int R;
  int D;
  int V;
};
enum class Person { Empty, Susceptible, Infected, Recovered, Dead, Vaccinated };
struct Desease
{
  int infectivity;
  double beta;
  double gamma;
};

struct Position
{
  int r;
  int c;
};

class Population  // diventa board, che tra person population e natin non si
                  // capisce niente

{
  int n_;
  using Row = std::vector<Person>;
  using Grid = std::vector<Row>;
  Grid nation;

 public:
  Population(int N) : n_{N}, nation(N, Row(N * 3, Person::Empty))
  {
  }

  // funzione che piazza le persone, creare anche empty case

  int side() const;

  Person& person(int r, int c);              // modificabile
  Person const& person(int r, int c) const;  // accesso
  State counts(int infectivity);

  void setInfected(Position p);
  void placePeople(int numberOfPeople, int infectivity);
  inline friend bool operator==(Population const& left, Population const& right)
  {
    return left.nation == right.nation;
  }  // friend perchè Grid nation è privato

  std::vector<int> interaction(int infectivity, int r, int c);

  bool infection(double beta, int infectivity, int r, int c);
  bool recovery(double gamma);
  bool dead(double delta);
  bool vaccination(double epsilon);
  void massVaccination(int infectivity, int toBeVaccinated,double epsilon);
  //void vaccinationOver(int infectivity, double gamma, Sird::State& state);
  void resusceptible(int infectivity, double beta, State& state);

  void evolve(
      int infectivity, double beta, double gamma, double delta, double epsilon);
  void move(int infectivity);
};

}  // namespace Sird
#endif

