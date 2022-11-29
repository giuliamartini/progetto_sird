#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sird.hpp"




TEST_CASE("Testing board, placePeople") {

SUBCASE("Big board, max number of people") {
    Sird::Population board(40); 
    board.placePeople(1156, 3); //numero massimo di persone 
    Sird::State count = board.counts(3); 
    CHECK(count.S == 1156);
    CHECK(count.I == 0);
    CHECK(count.R == 0);
    CHECK(count.D == 0);
    CHECK(count.V == 0);
}

SUBCASE("Small board, max number of people") {
    Sird::Population board(20); 
    board.placePeople(196, 3); //numero massimo di persone 
    Sird::State count = board.counts(3); 
    CHECK(count.S == 196);
    CHECK(count.I == 0);
    CHECK(count.R == 0);
    CHECK(count.D == 0);
    CHECK(count.V == 0);
}
}



TEST_CASE("Testing setInfected") {

SUBCASE("One infected on the board") {
    Sird::Population board(40);
    board.setInfected({20, 20});  
    Sird::State count = board.counts(3);
    CHECK(count.I == 1);  
}


SUBCASE("Five infected on the board") {
    Sird::Population board(40);
    board.setInfected({20, 20}); 
    board.setInfected({20, 19}); 
    board.setInfected({20, 18});
    board.setInfected({19, 20});
    board.setInfected({18, 20});
    Sird::State count = board.counts(3);
    CHECK(count.I == 5);  
}
}



TEST_CASE("Testing the stability of the population") {
    Sird::Population board(20); 
    board.placePeople(250, 3); 
    board.setInfected({10, 10}); 
    board.setInfected({10, 9}); 
    board.setInfected({10, 8});
    board.setInfected({9, 10});
    board.setInfected({8, 10});
    Sird::State count = board.counts(3); 
    int numberOfPeople = count.I + count.S; 
        board.evolve(3, 0.8, 0.3, 0.1);
        board.move(3);
    count = board.counts(3);
    CHECK(count.S + count.I + count.R + count.D + count.V == numberOfPeople);
}




