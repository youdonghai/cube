//
// Created by youdonghai on 14/10/16.
//

#ifndef CUBE_COORDCUBE_H
#define CUBE_COORDCUBE_H

#include <stdint-gcc.h>
#include "CubieCube.h"

class CoordCube {
public:
    CoordCube(){};
    CoordCube(CubieCube *c);
    void fromCubieCube(CubieCube *c);
    void toCubiecube(CubieCube *c);

    static const uint16_t N_TWIST = 2187;// 3^7 possible corner orientations
    static const uint16_t N_FLIP = 2048;// 2^11 possible edge flips
    static const uint16_t N_SLICE1 = 495;// 12 choose 4 possible positions of FR,FL,BL,BR edges
    static const uint16_t N_SLICE2 = 24;// 4! permutations of FR,FL,BL,BR edges in phase2
    static const uint16_t N_PARITY = 2; // 2 possible corner parities
    static const uint16_t N_FRtoBR = 11880; // 12!/(12-4)! permutation of FR,FL,BL,BR edges

    // phase2
    static const int N_URFtoDRB = 40320;// 8! permutations of the corners
    static const int N_URtoDB = 19958400; // 12!/(12-8)! permutations of UR, UF, UL, UB, DR, DF, DL, DB edges

    static const uint16_t N_MOVE = 18;

    // phase1 & phase2
    static uint16_t twistMove[N_TWIST][N_MOVE];
    static uint16_t flipMove[N_FLIP][N_MOVE];
    static uint8_t parityMove[N_PARITY][N_MOVE];
    static uint16_t FRtoBRMove[N_FRtoBR][N_MOVE];

    // phase2
    static uint16_t URFtoDRBMove[N_URFtoDRB][N_MOVE]; // corners
    static uint32_t URtoDBMove[N_URtoDB][N_MOVE]; // first 8 edges

    static void init();
    static void initMoveTable();

    static const char * MOVE_TABLE_FILE;
    static const int FILE_MAGIC_NUMBER;
    static bool loadMoveTable();
    static void writeMoveTable();

    void move(uint8_t m);

    // phase1
    uint16_t twist;
    uint16_t flip;
    uint16_t FRtoBR;

    // at phase2
    uint16_t cornerPermutation;
    uint32_t URtoDB;
    uint8_t parity;
};


#endif //CUBE_COORDCUBE_H
