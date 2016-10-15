//
// Created by youdonghai on 14/10/16.
//

#include <stdio.h>
#include "CoordCube.h"


uint16_t CoordCube::twistMove[N_TWIST][N_MOVE];
uint16_t CoordCube::flipMove[N_FLIP][N_MOVE];
uint8_t CoordCube::parityMove[N_PARITY][N_MOVE] = { { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
                                                    { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 } };
uint16_t CoordCube::FRtoBRMove[N_FRtoBR][N_MOVE];
uint16_t CoordCube::URFtoDRBMove[N_URFtoDRB][N_MOVE];
uint32_t CoordCube::URtoDBMove[N_URtoDB][N_MOVE];

const char *CoordCube::MOVE_TABLE_FILE = "/tmp/_donghai_cube_move_table_.bin";
const int CoordCube::FILE_MAGIC_NUMBER = 666666;

void CoordCube::init() {
    if (loadMoveTable()) {
        return;
    }
    initMoveTable();
    writeMoveTable();
}

void CoordCube::initMoveTable() {
    CubieCube a;

    // init twist move table
    for (short i = 0; i < N_TWIST; i++) {
        a.setTwist(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.cornerMultiply(CubieCube::moveCube[j]);
                twistMove[i][3 * j + k] = a.getTwist();
            }
            a.cornerMultiply(CubieCube::moveCube[j]);// 4. faceturn restores
        }
    }

    for (short i = 0; i < N_FLIP; i++) {
        a.setFlip(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.edgeMultiply(CubieCube::moveCube[j]);
                flipMove[i][3 * j + k] = a.getFlip();
            }
            a.edgeMultiply(CubieCube::moveCube[j]);
        }
    }

    for (short i = 0; i < N_FRtoBR; i++) {
        a.setFRtoBR(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.edgeMultiply(CubieCube::moveCube[j]);
                FRtoBRMove[i][3 * j + k] = a.getFRtoBR();
            }
            a.edgeMultiply(CubieCube::moveCube[j]);
        }
    }

    for (int i = 0; i < N_URFtoDRB; ++i) {
        a.setCornerPermutation(i);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.cornerMultiply(CubieCube::moveCube[j]);
                URFtoDRBMove[i][3 * j + k] = a.getCornerPermutation();
            }
            a.cornerMultiply(CubieCube::moveCube[j]);
        }
    }

    for (int i = 0; i < N_URtoDB; ++i) {
        a.setURtoDB(i, true);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 3; k++) {
                a.edgeMultiply(CubieCube::moveCube[j]);
                URtoDBMove[i][3 * j + k] = a.getURtoDB();
            }
            a.edgeMultiply(CubieCube::moveCube[j]);
        }
    }
}

CoordCube::CoordCube(CubieCube *c) {
    fromCubieCube(c);
}

void CoordCube::fromCubieCube(CubieCube *c) {
    twist = c->getTwist();
    flip = c->getFlip();
    FRtoBR = c->getFRtoBR();

    cornerPermutation = c->getCornerPermutation();
    URtoDB = c->getURtoDB();
    parity = c->cornerParity();
}

void CoordCube::move(uint8_t m) {
    twist = twistMove[twist][m];
    flip = flipMove[flip][m];
    FRtoBR = FRtoBRMove[FRtoBR][m];

    cornerPermutation = URFtoDRBMove[cornerPermutation][m];
    URtoDB= URtoDBMove[URtoDB][m];
    parity = parityMove[parity][m];
}

void CoordCube::toCubiecube(CubieCube *c) {
    c->setTwist(twist);
    c->setFlip(flip);
    c->setFRtoBR(FRtoBR);
    c->setCornerPermutation(cornerPermutation);
    c->setURtoDB(URtoDB, false);
}

bool CoordCube::loadMoveTable() {
    FILE *f = fopen(MOVE_TABLE_FILE, "rb");
    if (f == NULL) {
        return false;
    }
    int magic_number = 0;

    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
            magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&twistMove, sizeof(twistMove), 1, f);
    } else {
        fclose(f);
        return false;
    }
    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
       magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&flipMove, sizeof(flipMove), 1, f);
    } else {
        fclose(f);
        return false;
    }
    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
       magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&FRtoBRMove, sizeof(FRtoBRMove), 1, f);
    } else {
        fclose(f);
        return false;
    }
    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
       magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&URFtoDRBMove, sizeof(URFtoDRBMove), 1, f);
    } else {
        fclose(f);
        return false;
    }
    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
       magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&URtoDBMove, sizeof(URtoDBMove), 1, f);
    } else {
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
}

void CoordCube::writeMoveTable() {
    FILE *f = fopen(MOVE_TABLE_FILE, "wb");
    if (f == NULL) {
        return;
    }
    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(twistMove, sizeof(twistMove), 1, f);
    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(flipMove, sizeof(flipMove), 1, f);
    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(FRtoBRMove, sizeof(FRtoBRMove), 1, f);
    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(URFtoDRBMove, sizeof(URFtoDRBMove), 1, f);
    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(URtoDBMove, sizeof(URtoDBMove), 1, f);
    fclose(f);
}



