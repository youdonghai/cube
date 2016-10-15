//
// Created by youdonghai on 14/10/16.
//

#ifndef CUBE_CUBIECUBE_H
#define CUBE_CUBIECUBE_H

#include <stdint.h>
#include "CubeDefine.h"


class CubieCube {
public:
    static void init();
    static void initMoveCube();
    static CubieCube *moveCube[6];

    CubieCube();
    uint16_t getTwist();
    void setTwist(uint16_t);
    uint16_t getFlip();
    void setFlip(uint16_t flip);
    uint16_t getUDSlicePosition();
    uint8_t getFRtoBRPermutations();
    uint16_t getURtoDBPermutations();

    uint16_t getFRtoBR();
    void setFRtoBR(uint16_t idx);
    uint32_t getURtoDB();
    void setURtoDB(uint32_t idx, bool over_write_FRtoBR=true);
    uint16_t getCornerPermutation();
    void setCornerPermutation(uint16_t idx);

    // only use in phase2
    uint16_t getURtoDBPermutation();
    void setURtoDBPermutation(uint16_t idx);

    void rotateLeft(Edge *ea, int l, int r);
    void rotateLeft(Corner *ca, int l, int r);
    void rotateRight(Edge *ea, int l, int r);
    void rotateRight(Corner *ca, int l, int r);

    int Cnk(int n, int k);

    int verify();
    uint8_t cornerParity();
    uint8_t edgeParity();

    /**
     * (A*B)(x).c=A(B(x).c).c
     * (A*B)(x).o=A(B(x).c).o+B(x).o
     */
    void cornerMultiply(CubieCube *c);
    void edgeMultiply(CubieCube *c);

    // 8 corner permutation and orientation
    Corner cp[8];
    uint8_t co[8];

    // 12 edge permutation adn orientation
    Edge ep[12];
    uint8_t eo[12];
};


#endif //CUBE_CUBIECUBE_H
