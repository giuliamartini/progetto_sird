#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sird.hpp"




//test di placePeople - ha senso 
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


//test di setInfected FUNZIONA - HA SENSO
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



/*TEST_CASE("Testing the stability of the population") {

    Sird::Population board(20); 
    board.placePeople(100, 3); //numero massimo di persone 
    board.setInfected({10, 10}); 
    Sird::State count = board.counts(3); 
    int frame = 0;
    board.Sird::Population::evolve(3, 0.8, 0.3, 0.1);
    board.move(3);
    ++frame;
    CHECK(count.S + count.I + count.R + count.D + count.V == 100);
}*/



//Non torna questo test
/*TEST_CASE("Testing evolve") {
    
    SUBCASE("Testing the beginning of the vaccination"){
    Sird::Population board(40);
    board.Sird::Population::placePeople(1000, 3);
    board.Sird::Population::setInfected({20, 20}); 
    board.Sird::Population::setInfected({20, 19});
    board.Sird::Population::setInfected({20, 18});
    board.Sird::Population::setInfected({19, 20});
    board.Sird::Population::setInfected({21, 20});
    int frame = 0;
    Sird::State count = board.counts(3);
    board.evolve(3, 0.8, 0.3, 0.1, 0.7);
    board.move(3);
    board.massVaccination(3, 20, 0.7);
    ++frame; 
    if(frame < 20){
        CHECK(count.V == 0);
    } else if(frame >= 20) { //forse invece può succedere che ci siano 0 vaccinati dopo? 
        CHECK(count.V == 0);
    }
    }
    }
*/

//test senza move per vedere se con empty intorno non si infetta 
//anche questo non ha senso 
/*TEST_CASE("Testing without move") {
    double beta = 0.8;
    double gamma = 0.3;
    double delta = 0.1;
    double epsilon = 0.7;
    int infectivity = 3;
    constexpr int board_size = 40;
    Sird::Population board(board_size);
    board.Sird::Population::placePeople(1, 3);
    int frame = 0;
    Sird::State count = board.counts(3);
    board.evolve(infectivity, beta, gamma, delta);
    ++frame;
    CHECK(count.S == 0);
    CHECK(count.I == 1);
}*/




