//
// Created by youdonghai on 15/10/16.
//

#include <iostream>
#include <cmath>
#include "Search.h"

#include "FaceCube.h"
#include "CoordCube.h"

std::mutex Search::tlk;
int Search::phase1_done = 0;
int Search::phase2_done = 0;
uint8_t Search::result_moves[31] = {};
uint8_t Search::result_move_len = 0;


int Search::init(std::string cs) {
    FaceCube fc;
    CubieCube cc;
    fc.fromString(cs);
    fc.toCubieCube(&cc);
    if (cc.verify() != 0) {
        std::cerr << "Cube verify error" << std::endl;
        return -1;
    }
    coordc.fromCubieCube(&cc);
    isPhase1Done();
    return 0;
}


void Search::phase1(uint64_t start, uint64_t end) {
    uint64_t max = (uint64_t)powl(18, 12);
    uint64_t idx = 0;
    uint64_t processing_max_idx = 99999999999;
    for (idx = start; idx <= end; ++idx) {
        if (phase1_done) {
            return;
        }

        uint64_t tmp_idx = idx;
        for (int i = 0; i < 12; ++i) {
            moves[i] = (uint8_t) (tmp_idx % 18);
            tmp_idx = tmp_idx / 18;
            if (i == 4 && tmp_idx != processing_max_idx) {
                std::cout << "processing " << tmp_idx << std::endl;
                processing_max_idx = tmp_idx;
            }
        }
        if (isPhase1Done()) {
            return;
        }
    }
}

bool Search::isPhase1Done() {
    if (checkPhase1Done()) {
        if(tlk.try_lock()) {
            phase1_done = 1;
            result_move_len = move_len;
            for (int i = 0; i < move_len; ++i) {
                result_moves[i] = moves[i];
            }
        }
        return true;
    }
    return false;
}

bool Search::checkPhase1Done() {
    CoordCube tmp_coordc = coordc;
    CubieCube tmp_cc;
    tmp_cc.setFRtoBR(tmp_coordc.FRtoBR);
    if (0 == tmp_coordc.twist && 0 == tmp_coordc.flip && 0 == tmp_cc.getUDSlicePosition()) {
        move_len = 0;
        return true;
    }
    for (int i = 0; i < 12; ++i) {
        tmp_coordc.move(moves[i]);
        tmp_cc.setFRtoBR(tmp_coordc.FRtoBR);
        if (0 == tmp_coordc.twist && 0 == tmp_coordc.flip && 0 == tmp_cc.getUDSlicePosition()) {
            // get G1, done
            move_len = (uint8_t) (i + 1);
            return true;
        }
    }
    return false;
}

std::string Search::printResult() {
    std::cout << "result move len " << (int)result_move_len << std::endl;
    for (int i = 0; i < result_move_len; ++i) {
        std::cout << (int)result_moves[i] << ", ";
    }
    std::cout << std::endl;
    return std::string();
}

