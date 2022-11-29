#include "sird.hpp"

int Sird::Population::side() const
{
  return nation.size();
}

Sird::Person& Sird::Population::person(int r, int c)  // modificabile
{
  int N = side();
  assert(r < N && r >= 0 && c < N * 3 && c >= 0);
  return nation[r][c];
}
Sird::Person const& Sird::Population::person(int r, int c) const  // accesso
{
  int N = side();
  return nation[r][c];
  assert(r < N && r >= 0 && c < N * 3 && c >= 0);
}
Sird::State Sird::Population::counts()
{
  State count = {0, 0, 0, 0, 0};
  for (int r = 0, end = side() - 1; r != end; ++r) {
    for (int c = 0, end = side() * 3 - 1; c != end; ++c) {
      if (person(r, c) == Sird::Person::Susceptible) {
        ++count.S;
      } else if (person(r, c) == Sird::Person::Vaccinated) {
        ++count.V;
      } else if (person(r, c) == Sird::Person::Infected) {
        ++count.I;
      } else if (person(r, c) == Sird::Person::Recovered) {
        ++count.R;
      } else if (person(r, c) == Sird::Person::Dead) {
        ++count.D;
      }
    }
  }

  return count;
}
void Sird::Population::setInfected(Position p)
{
  nation[p.r][p.c] = Sird::Person::Infected;
}

void Sird::Population::placePeople(int numberOfPeople, int infectivity)
{
  assert(numberOfPeople > 2 &&
         numberOfPeople < (n_ - infectivity) * 3 * (n_ - infectivity));

  std::random_device eng;
  std::vector<Position> emptyCells;
  for (int r = infectivity, end = n_ - infectivity; r != end; ++r) {
    for (int c = infectivity; c != 3 * n_ - infectivity; ++c) {
      if (nation[r][c] == Sird::Person::Empty) {
        emptyCells.push_back({r, c});
      }
    }
  }
  shuffle(emptyCells.begin(), emptyCells.end(), eng);
  for (int k = 0; k < numberOfPeople; ++k) {
    person(emptyCells[k].r, emptyCells[k].c) = Sird::Person::Susceptible;
  }
}

std::vector<int> Sird::Population::interaction(int infectivity,

                                               int r,
                                               int c)
{
  std::vector<int> numberOfInfected;

  // guarda in alto a destra
  for (int i = 1; i <= infectivity; ++i) {
    int count = 0;

    for (int j = 0; j < i; ++j) {
      if (person(r + i - j, c + j) == Sird::Person::Infected) {
        ++count;
      }

      if (person(r - j, c + i - j) == Sird::Person::Infected) {
        ++count;
      }

      if (person(r - i + j, c - j) == Sird::Person::Infected) {
        ++count;
      }

      if (person(r + j, c - i + j) == Sird::Person::Infected) {
        ++count;
      }
    }
    numberOfInfected.push_back(count);
  }
  return numberOfInfected;
}

bool Sird::Population::infection(double beta, int infectivity, int r, int c)
{
  std::vector<int> infectedAround = interaction(infectivity, r, c);
  bool result = false;
  for (int i = 1, end = infectedAround.size(); i != end; ++i) {
    for (int j = 0; j != infectedAround[i - 1]; ++j) {
      std::random_device eng;
      std::uniform_real_distribution<double> dist(0.0, 1.0);
      if (dist(eng) >= beta / i) {
        continue;
      } else if (dist(eng) < beta / i) {
        result = true;
        break;
      }
    }
  }
  return result;
}

bool Sird::Population::recovery(double gamma)
{
  bool result = false;

  std::random_device eng;
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  if (dist(eng) < gamma) {
    result = true;
  } else if (dist(eng) >= gamma) {
    result = false;
  }
  return result;
}

bool Sird::Population::death(double delta)
{
  bool result = false;

  std::random_device eng;
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  if (dist(eng) < delta) {  // muore
    result = true;
  } else if (dist(eng) >= delta) {
    result = false;  // vive
  }
  return result;
}

bool Sird::Population::vaccination(double epsilon)
{
  bool result = false;
  std::random_device eng;
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  if (dist(eng) < epsilon) {
    result = true;
  } else if (dist(eng) >= epsilon) {
    result = false;
  }
  return result;
}
void Sird::Population::massVaccination(
    int infectivity,  // funzione che rende vaccinate alcune persone , epsilon
                      // da il numero totale in percentuale di persone che si
                      // vaccineranno

    int toBeVaccinated,
    double epsilon)
{
  assert(toBeVaccinated < n_ * 3 * n_);

  std::random_device eng;
  std::vector<Position> toBeVaccinatedCells;
  for (int r = infectivity, end = n_ - infectivity; r != end; ++r) {
    for (int c = infectivity; c != n_ * 3 - infectivity; ++c) {
      if (nation[r][c] == Sird::Person::Susceptible) {
        toBeVaccinatedCells.push_back({r, c});
      }
    }
  }
  shuffle(toBeVaccinatedCells.begin(), toBeVaccinatedCells.end(), eng);
  for (int k = 0; k < toBeVaccinated; ++k) {
    bool vac = vaccination(epsilon);
    if (vac == true) {
      person(toBeVaccinatedCells[k].r, toBeVaccinatedCells[k].c) =
          Sird::Person::Vaccinated;
    } else if (vac == false) {
      continue;
    }
  }
}
void Sird::Population::susceptibleAgain(int infectivity,
                                        double sus_ag,
                                        Sird::State& state)
{
  int n_ = side();
  int recovered = static_cast<int>(sus_ag * state.R);
  assert(recovered < n_ * 3 * n_);

  std::random_device eng;
  std::vector<Position> recoveredCells;
  for (int r = infectivity, end = n_ - infectivity; r != end; ++r) {
    for (int c = infectivity; c != 3 * n_ - infectivity; ++c) {
      if (nation[r][c] == Sird::Person::Recovered) {
        recoveredCells.push_back({r, c});
      }
    }
  }
  shuffle(recoveredCells.begin(), recoveredCells.end(), eng);
  for (int k = 0; k < recovered; ++k) {
    person(recoveredCells[k].r, recoveredCells[k].c) =
        Sird::Person::Susceptible;
  }
}

void Sird::Population::evolve(
    // Sird::Population const& current,
    int infectivity,
    double beta,
    double gamma,
    double delta,
    double epsilon)
{
  int N = side();
  // current.side();
  // Sird::Population next = current;
  for (int r = infectivity; r < N - infectivity; ++r) {
    for (int c = infectivity; c < 3 * N - infectivity; ++c) {
      // non posso fare switch case perchè dovrei definire i bool all'inizio ma
      // non avrebbe senso, uso if

      if (person(r, c) == Sird::Person::Susceptible) {
        bool inf = infection(beta, infectivity, r, c);
        if (inf == true) {
          person(r, c) = Sird::Person::Infected;
        } else if (inf == false) {
          continue;
        }
      }

      else if (person(r, c) == Sird::Person::Vaccinated) {
        bool inf = infection(beta * epsilon, infectivity, r, c);
        if (inf == true) {
          person(r, c) = Sird::Person::Infected;
        } else if (inf == false) {
          continue;
        }
      } else if (person(r, c) == Sird::Person::Infected) {
        bool rec = recovery(gamma);
        if (rec == true) {
          person(r, c) = Sird::Person::Recovered;
        } else if (rec == false) {
          bool de = death(delta);
          if (de == true) {
            person(r, c) = Sird::Person::Dead;
          } else if (de == false) {
            person(r, c) = Sird::Person::Infected;
            // continue;
          }
        }
      } else if (person(r, c) == Sird::Person::Recovered) {
        person(r, c) = Person::Recovered;
      } else if (person(r, c) == Sird::Person::Dead) {
        continue;
      }
    }
  }
}

void Sird::Population::move(int const infectivity)
{
  std::random_device eng;
  std::uniform_int_distribution<int> dist(0, 4);  // 5 possibilità
  for (int r = infectivity, end = side() + 1 - infectivity; r != end; ++r) {
    for (int c = infectivity, end = side() * 3 + 1 - infectivity; c != end;
         ++c) {
      if (person(r, c) == Person::Susceptible ||
          person(r, c) == Person::Infected ||
          person(r, c) == Person::Recovered ||
          person(r, c) == Person::Vaccinated) {
        int direction = dist(eng);

        switch (direction) {
          case 0:  // su
                   // lo si fa rimbalzare sul bordo se è arrivato alla fine
            if (r == infectivity && person(r + 1, c) == Person::Empty) {
              person(r + 1, c) = person(r, c);
              person(r, c) = Person::Empty;
            } else if (r != infectivity && person(r - 1, c) == Person::Empty) {
              person(r - 1, c) = person(r, c);
              person(r, c) = Person::Empty;
            }

            break;
          case 1:  // giù
            if (r == side() - infectivity &&
                person(r - 1, c) == Person::Empty) {
              person(r - 1, c) = person(r, c);
              person(r, c) = Person::Empty;
            } else if (r != side() - infectivity &&
                       person(r + 1, c) == Person::Empty) {
              person(r + 1, c) = person(r, c);
              person(r, c) = Person::Empty;
            }
            break;
          case 2:  // sinistra
            if (c == infectivity && person(r, c + 1) == Person::Empty) {
              person(r, c + 1) = person(r, c);
              person(r, c) = Person::Empty;
            } else if (c != infectivity && person(r, c - 1) == Person::Empty) {
              person(r, c - 1) = person(r, c);
              person(r, c) = Person::Empty;
            }

            break;
          case 3:  // destra
            if (c == side() * 3 - infectivity &&
                person(r, c - 1) == Person::Empty) {
              person(r, c - 1) = person(r, c);
              person(r, c) = Person::Empty;
            } else if (c != side() * 3 - infectivity &&
                       person(r, c + 1) == Person::Empty) {
              person(r, c + 1) = person(r, c);
              person(r, c) = Person::Empty;
            }

            break;
          case 4:  // fermo
            break;
        }
      }
    }
  }
}
