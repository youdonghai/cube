//
// Created by youdonghai on 16/10/16.
//

#include <iostream>
#include "Search2.h"
#include "FaceCube.h"

Search2::Search2(std::string c) {
    vec_phase1_moves.resize(12);
    vec_phase1_depth.resize(12);
    phase1depth = 0;

    vec_phase2_moves.resize(18);
    vec_phase2_depth.resize(18);
    phase2depth = 0;

    cube_string = c;
}

int Search2::go() {

    FaceCube fc;
    CubieCube cc;
    fc.fromString(cube_string);
    fc.toCubieCube(&cc);
    if (cc.verify() != 0) {
        std::cerr << "Cube verify error" << std::endl;
        return -1;
    }
    std::cout << fc.toString() << std::endl;
    init_coordc.fromCubieCube(&cc);

    if (init_coordc.isClean()) {
        return 0;
    }

    phase1();
    printResult();
    return 0;
}

int Search2::phase1() {
    for (int i = 1; i < 13; ++i) {
        int result = doPhase1Search(init_coordc, i, 0);
        if (0 == result) {
            return 0;
        }
    }
    return -1;
}

int Search2::phase2() {
    phase2_init_coordc = init_coordc;
    for (int i = 0; i < phase1depth; ++i) {
        phase2_init_coordc.move(vec_phase1_moves[i]);
    }
    for (int i = 1; i < 19; ++i) {
        int result = doPhase2Search(phase2_init_coordc, i, 0);
        if (0 == result) {
            return 0;
        }
    }
    return -1;
}

int Search2::doPhase1Search(CoordCube start, int max_depth, int search_depth) {
    if (search_depth > max_depth-1) {
        return -1;
    }
    for (int i = 0; i < 18; ++i) {
        CoordCube tmp_coordc = start;
        tmp_coordc.move(i);
        int depth1 = CoordCube::lb_UDSliceFlip[tmp_coordc.flip*CoordCube::N_SLICE1 + tmp_coordc.FRtoBR/24];
        int depth2 = CoordCube::lb_UDSliceTwist[tmp_coordc.twist*CoordCube::N_SLICE1 + tmp_coordc.FRtoBR/24];
        int depth = std::max(depth1, depth2);
        if (depth == 0) {
            vec_phase1_moves[search_depth] = (uint8_t) i;
            phase1depth = (uint8_t) (search_depth + 1);
            if(0 == phase2()) {
                return 0;
            }
            continue;
        }
        int skip = 0;
        if (depth > max_depth-search_depth) {
            skip = 1;
        }
        if (skip) {
            continue;
        }
        vec_phase1_depth[search_depth] = (uint8_t) depth;
        vec_phase1_moves[search_depth] = (uint8_t) i;
        if (0 == this->doPhase1Search(tmp_coordc, max_depth, search_depth+1)) {
            return 0;
        }
    }
    return -1;
}

int Search2::doPhase2Search(CoordCube start, int max_depth, int search_depth) {
    if (search_depth > max_depth-1) {
        return -1;
    }

    for (int i = 0; i < 18; ++i) {
        if (3 == i || 5 == i || 6 == i || 8 == i || 12 == i || 14 == i || 15 == i || 17 == i) {
            continue;
        }
        CoordCube tmp_coordc = start;
        tmp_coordc.move(i);
        int depth1 = CoordCube::lb_UDSliceCorner[
                (CoordCube::N_SLICE2 * tmp_coordc.cornerPermutation + tmp_coordc.FRtoBR) * 2 +
                        tmp_coordc.parity];
        int depth2 = CoordCube::lb_UDSliceEdge[
                (CoordCube::N_SLICE2 * tmp_coordc.URtoDB + tmp_coordc.FRtoBR) * 2 +
                        tmp_coordc.parity];
        int depth = std::max(depth1, depth2);
        if (depth == 0) {
            vec_phase2_moves[search_depth] = (uint8_t) i;
            phase2depth = (uint8_t) (search_depth + 1);
            return 0;
        }
        int skip = 0;
        if (depth > max_depth-search_depth) {
            skip = 1;
        }
        if (skip) {
            continue;
        }
        vec_phase2_depth[search_depth] = (uint8_t) depth;
        vec_phase2_moves[search_depth] = (uint8_t) i;
        if (0 == this->doPhase2Search(tmp_coordc, max_depth, search_depth+1)) {
            return 0;
        }
    }
    return -1;
}

void Search2::printResult() {
    if (phase1depth == 0 && phase2depth == 0) {
        std::cout << "It's a clean cube." << std::endl;
        return;
    }
    for (int i = 0; i < phase1depth; ++i) {
        std::cout << (int)vec_phase1_moves[i] << ", ";
    }
    for (int i = 0; i < phase2depth; ++i) {
        std::cout << (int)vec_phase2_moves[i];
        if (i != phase2depth-1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::vector<int> ax;
    std::vector<int> po;
    for (int i = 0; i < phase1depth; ++i) {
        ax.push_back(vec_phase1_moves[i]/3);
        po.push_back(vec_phase1_moves[i]%3);
    }
    for (int i = 0; i < phase2depth; ++i) {
        ax.push_back(vec_phase2_moves[i]/3);
        po.push_back(vec_phase2_moves[i]%3);
    }
    printNormalResult(ax, po);
}

void Search2::printNormalResult(std::vector<int> ax, std::vector<int> po) {
    std::string r;
    for (size_t i = 0; i < ax.size(); ++i) {
        switch (ax[i]) {
            case 0:
                r += 'U';
                break;
            case 1:
                r += 'R';
                break;
            case 2:
                r += 'F';
                break;
            case 3:
                r += 'D';
                break;
            case 4:
                r += 'L';
                break;
            case 5:
                r += 'B';
                break;
        }
        switch (po[i]) {
            case 0:
                break;
            case 1:
                r += '2';
                break;
            case 2:
                r += "'";
                break;
        }
        r += " ";
    }
    r = r.substr(0, r.size()-1);
    std::cout << r << std::endl;
}

