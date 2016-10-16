//
// Created by youdonghai on 16/10/16.
//

#ifndef CUBE_SEARCH2_H
#define CUBE_SEARCH2_H

#include <string>
#include <vector>
#include "CubieCube.h"
#include "CoordCube.h"


class Search2 {
public:
    Search2(std::string c);

    int go();

    int doPhase1Search(CoordCube start, int max_depth, int search_depth);
    int doPhase2Search(CoordCube start, int max_depth, int search_depth);
    int phase1();
    int phase2();
    void printResult();
    void printNormalResult(std::vector<int> ax, std::vector<int> po);

private:
    std::vector<uint8_t> vec_phase1_moves;
    std::vector<uint8_t> vec_phase1_depth;
    uint8_t phase1depth;
    CoordCube init_coordc;
    std::vector<uint8_t> vec_phase2_moves;
    std::vector<uint8_t> vec_phase2_depth;
    uint8_t phase2depth;
    CoordCube phase2_init_coordc;

    std::string cube_string;
};


#endif //CUBE_SEARCH2_H
