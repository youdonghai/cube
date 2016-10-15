//
// Created by youdonghai on 15/10/16.
//

#ifndef CUBE_SEARCH_H
#define CUBE_SEARCH_H


#include <cstddef>
#include <string>
#include <mutex>

#include "CubieCube.h"
#include "CoordCube.h"

class Search {
public:
    Search(){};

    int init(std::string cs);
    void phase1(uint64_t start, uint64_t end);
    static std::string printResult();
private:
    CoordCube coordc;
    uint8_t moves[31];
    uint8_t move_len;

    bool isPhase1Done();
    bool checkPhase1Done();

    static std::mutex tlk;
    static int phase1_done;
    static int phase2_done;
    static uint8_t result_moves[31];
    static uint8_t result_move_len;

};


#endif //CUBE_SEARCH_H
