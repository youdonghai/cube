//
// Created by youdonghai on 15/10/16.
//

#ifndef CUBE_SEARCH_H
#define CUBE_SEARCH_H


#include <cstddef>
#include <string>
#include <mutex>
#include <vector>

#include "CubieCube.h"
#include "CoordCube.h"

class Search {
public:
    Search(){
        now_local_phase = 1;
    };

    int init(std::string cs, int thread_index, int thread_size);
    void phase1();
    static std::string printResult();

    static void phase1Done(std::vector<uint8_t> *m, int len);

private:
    CoordCube coordc;

    int thread_index;
    int thread_size;

    int now_local_phase;

    bool isPhase1Done();
//    bool checkPhase1Done();

    static std::mutex tlk;
    static int phase1_done;
    static int phase2_done;
    static uint8_t result_moves[31];
    static uint8_t result_move_len;


    static const int population_size = 500;
    static const int survive_size = 100;
    static const int mutation_number = 1;
    static const int mutation_range = 3;

    std::vector<std::vector<uint8_t> > population;

    void survive();
    void mutation();

    uint8_t randPhase2gene();

};


#endif //CUBE_SEARCH_H
