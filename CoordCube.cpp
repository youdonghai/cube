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

uint8_t CoordCube::lb_UDSliceFlip[N_UDSLICEFLIP];
uint8_t CoordCube::lb_UDSliceTwist[N_SLICE1*N_TWIST];

uint8_t CoordCube::lb_UDSliceCorner[N_SLICE2*N_URFtoDRB*N_PARITY];
uint8_t CoordCube::lb_UDSliceEdge[N_SLICE2*N_URtoDBPERMUTATIONS*N_PARITY];

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

    for (int i = 0; i < N_UDSLICEFLIP; i++) {
        lb_UDSliceFlip[i] = 0xff;
    }
    int depth = 0;
    lb_UDSliceFlip[0] = 0;
    int done = 1;
    while (done != N_UDSLICEFLIP) {
        for (int i = 0; i < N_UDSLICEFLIP; ++i) {
            uint16_t flip = (uint16_t) (i / N_SLICE1);
            uint16_t slice = (uint16_t) (i % N_SLICE1);
            if (lb_UDSliceFlip[i] == depth) {
                for (int j = 0; j < 18; ++j) {
                    uint16_t new_slice = (uint16_t) (FRtoBRMove[slice * 24][j] / 24);
                    uint16_t new_flip = flipMove[flip][j];
                    if (lb_UDSliceFlip[N_SLICE1*new_flip+new_slice] == 0xff) {
                        lb_UDSliceFlip[N_SLICE1*new_flip+new_slice] = (uint8_t) (depth + 1);
                        ++done;
                    }
                }
            }
        }
        ++depth;
    }

    for (int i = 0; i < N_UDSLICETWIST; i++) {
        lb_UDSliceTwist[i] = 0xff;
    }
    depth = 0;
    lb_UDSliceTwist[0] = 0;
    done = 1;
    while (done != N_UDSLICETWIST) {
        for (int i = 0; i < N_UDSLICETWIST; i++) {
            int twist = i / N_SLICE1;
            int slice = i % N_SLICE1;
            if (lb_UDSliceTwist[i] == depth) {
                for (int j = 0; j < 18; j++) {
                    int newSlice = FRtoBRMove[slice * 24][j] / 24;
                    int newTwist = twistMove[twist][j];
                    if (lb_UDSliceTwist[N_SLICE1 * newTwist + newSlice] == 0xff) {
                        lb_UDSliceTwist[N_SLICE1 * newTwist + newSlice] = (uint8_t) (depth + 1);
                        done++;
                    }
                }
            }
        }
        depth++;
    }

    for (int i = 0; i < N_SLICE2 * N_URFtoDRB * N_PARITY; i++) {
        lb_UDSliceCorner[i] = 0xff;
    }
    depth = 0;
    lb_UDSliceCorner[0] = 0;
    done = 1;
    int pre_done_number = 0;
    while (done != N_SLICE2 * N_URFtoDRB * N_PARITY) {
        for (int i = 0; i < N_SLICE2 * N_URFtoDRB * N_PARITY; i++) {
            int parity = i % 2;
            int URFtoDRB = (i / 2) / N_SLICE2;
            int slice = (i / 2) % N_SLICE2;
            if (lb_UDSliceCorner[i] == depth) {
                for (int j = 0; j < 18; j++) {
                    switch (j) {
                        case 3:
                        case 5:
                        case 6:
                        case 8:
                        case 12:
                        case 14:
                        case 15:
                        case 17:
                            continue;
                        default:
                            int newSlice = FRtoBRMove[slice][j];
                            int newURFtoDRB = URFtoDRBMove[URFtoDRB][j];
                            int newParity = parityMove[parity][j];
                            if (lb_UDSliceCorner[(N_SLICE2 * newURFtoDRB + newSlice) * 2 + newParity] == 0xff) {
                                lb_UDSliceCorner[(N_SLICE2 * newURFtoDRB + newSlice) * 2 + newParity] =(uint8_t) (depth + 1);
                                done++;
                            }
                    }
                }
            }
        }
        if (pre_done_number == done) {
            // 阶段二限制了move类型，所以某些状态不能到达，这里跳出循环
            break;
        } else {
            pre_done_number = done;
        }
        depth++;
    }

    for (int i = 0; i < N_SLICE2 * N_URtoDBPERMUTATIONS * N_PARITY; i++) {
        lb_UDSliceEdge[i] = 0xff;
    }
    depth = 0;
    lb_UDSliceEdge[0] = 0;
    done = 1;
    pre_done_number = 0;
    while (done != N_SLICE2 * N_URtoDBPERMUTATIONS * N_PARITY) {
        for (int i = 0; i < N_SLICE2 * N_URtoDBPERMUTATIONS * N_PARITY; i++) {
            int parity = i % 2;
            int URtoDB = (i / 2) / N_SLICE2;
            int slice = (i / 2) % N_SLICE2;
            if (lb_UDSliceEdge[i] == depth) {
                for (int j = 0; j < 18; j++) {
                    switch (j) {
                        case 3:
                        case 5:
                        case 6:
                        case 8:
                        case 12:
                        case 14:
                        case 15:
                        case 17:
                            continue;
                        default:
                            int newSlice = FRtoBRMove[slice][j];
                            int newURtoDB = URtoDBMove[URtoDB][j];
                            int newParity = parityMove[parity][j];
                            if (lb_UDSliceEdge[(N_SLICE2 * newURtoDB + newSlice) * 2 + newParity] == 0xff) {
                                lb_UDSliceEdge[(N_SLICE2 * newURtoDB + newSlice) * 2 + newParity] = (uint8_t) (depth + 1);
                                done++;
                            }
                    }
                }
            }
        }
        if (pre_done_number == done) {
            // 同上
            break;
        } else {
            pre_done_number = done;
        }
        depth++;
    }

}

CoordCube::CoordCube(CubieCube *c) {
    fromCubieCube(c);
}

CoordCube::CoordCube(CoordCube *c) {
    this->twist = c->twist;
    this->flip = c->flip;
    this->FRtoBR = c->FRtoBR;
    this->cornerPermutation = c->cornerPermutation;
    this->URtoDB = c->URtoDB;
}

bool CoordCube::isClean() {
    return (0 == twist && 0 == flip && 0 == FRtoBR && 0 == cornerPermutation && 0 == URtoDB);
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
    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
       magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&lb_UDSliceFlip, sizeof(lb_UDSliceFlip), 1, f);
    } else {
        fclose(f);
        return false;
    }
    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
       magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&lb_UDSliceTwist, sizeof(lb_UDSliceTwist), 1, f);
    } else {
        fclose(f);
        return false;
    }
    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
       magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&lb_UDSliceCorner, sizeof(lb_UDSliceCorner), 1, f);
    } else {
        fclose(f);
        return false;
    }
    if(1 == fread(&magic_number, sizeof(magic_number), 1, f) &&
       magic_number == FILE_MAGIC_NUMBER)
    {
        fread(&lb_UDSliceEdge, sizeof(lb_UDSliceEdge), 1, f);
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

    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(lb_UDSliceFlip, sizeof(lb_UDSliceFlip), 1, f);
    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(lb_UDSliceTwist, sizeof(lb_UDSliceTwist), 1, f);
    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(lb_UDSliceCorner, sizeof(lb_UDSliceCorner), 1, f);
    fwrite(&FILE_MAGIC_NUMBER, sizeof(FILE_MAGIC_NUMBER), 1, f);
    fwrite(lb_UDSliceEdge, sizeof(lb_UDSliceEdge), 1, f);

    fclose(f);
}




