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
    CoordCube(CoordCube *c);
    bool isClean();
    void fromCubieCube(CubieCube *c);
    void toCubiecube(CubieCube *c);

    static const uint16_t N_TWIST = 2187;// 3^7 角块的方向, 3^7种可能
    static const uint16_t N_FLIP = 2048;// 2^11 中间块的方向, 2^11种可能
    static const uint16_t N_SLICE1 = 495;// FR,FL,BL,BR 四个中间块可能的位置，忽略顺序
    static const uint16_t N_SLICE2 = 24;// FR,FL,BL,BR 四个中间块可能的排列，忽略位置
    static const uint16_t N_PARITY = 2; // 奇偶性
    static const uint16_t N_FRtoBR = 11880; // FR,FL,BL,BR 所有的可能 = N_SLICE1*N_SLICE2

    // phase2
    static const int N_URFtoDRB = 40320;// 所有角块可能的排列 8!
    static const int N_URtoDBPERMUTATIONS = 40320;// 前八个中间块可能的排列 8!
    static const int N_URtoDB = 19958400; // 12!/(12-8)! 前八个中间块的排列，包括位置和顺序

    static const uint16_t N_MOVE = 18; // 18种转动方式

    // phase1 & phase2
    static uint16_t twistMove[N_TWIST][N_MOVE];
    static uint16_t flipMove[N_FLIP][N_MOVE];
    static uint8_t parityMove[N_PARITY][N_MOVE];
    static uint16_t FRtoBRMove[N_FRtoBR][N_MOVE];

    // phase2
    static uint16_t URFtoDRBMove[N_URFtoDRB][N_MOVE]; // corners
    static uint32_t URtoDBMove[N_URtoDB][N_MOVE]; // first 8 edges

    // 反查表
    static const int N_UDSLICEFLIP = N_SLICE1*N_FLIP;
    static uint8_t lb_UDSliceFlip[N_UDSLICEFLIP];
    static const int N_UDSLICETWIST = N_SLICE1 * N_TWIST;
    static uint8_t lb_UDSliceTwist[N_UDSLICETWIST];

    static uint8_t lb_UDSliceCorner[N_SLICE2*N_URFtoDRB*N_PARITY];
    static uint8_t lb_UDSliceEdge[N_SLICE2*N_URtoDBPERMUTATIONS*N_PARITY];

    static void init();
    static void initMoveTable();

    static const char * MOVE_TABLE_FILE;
    static const int FILE_MAGIC_NUMBER;
    static bool loadMoveTable();
    static void writeMoveTable();

    void move(uint8_t m);

    // 以下六个向量可以表示任何一种魔方
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
