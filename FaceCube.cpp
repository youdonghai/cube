//
// Created by youdonghai on 14/10/16.
//

#include <iostream>
#include <algorithm>
#include "FaceCube.h"
#include "CoordCube.h"

// 每个角块在FaceCube中对应的位置
const Facelet FaceCube::cornerFacelet[8][3] = { { U9, R1, F3 }, { U7, F1, L3 }, { U1, L1, B3 }, { U3, B1, R3 },
                                                { D3, F9, R7 }, { D1, L9, F7 }, { D7, B9, L7 }, { D9, R9, B7 } };

// 每个中间块在FaceCube中对应的位置
const Facelet FaceCube::edgeFacelet[12][2] = { { U6, R2 }, { U8, F2 }, { U4, L2 }, { U2, B2 }, { D6, R8 }, { D2, F8 },
                                               { D4, L8 }, { D8, B8 }, { F6, R4 }, { F4, L6 }, { B6, L4 }, { B4, R6 } };

const Color FaceCube::cornerColor[8][3] = { { U, R, F }, { U, F, L }, { U, L, B }, { U, B, R },
                                            { D, F, R }, { D, L, F }, { D, B, L }, { D, R, B } };

const Color FaceCube::edgeColor[12][2] = { { U, R }, { U, F }, { U, L }, { U, B }, { D, R }, { D, F },
                                           { D, L }, { D, B }, { F, R }, { F, L }, { B, L }, { B, R } };

FaceCube::FaceCube() {
    fromString("UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB");
}

FaceCube::FaceCube(std::string s) {
    fromString(s);
}

int FaceCube::fromString(std::string sc) {
    std::string s;
    for (size_t i = 0; i < sc.size(); ++i) {
        if (sc.at(i) == ' ') {
            continue;
        }
        s += sc.at(i);
    }
    for (size_t i = 0; i < s.length(); i++) {
        switch(s.at(i)){
            case 'U':
                f[i] = U;
                break;
            case 'R':
                f[i] = R;
                break;
            case 'F':
                f[i] = F;
                break;
            case 'D':
                f[i] = D;
                break;
            case 'L':
                f[i] = L;
                break;
            case 'B':
                f[i] = B;
                break;
            case ' ':
                break;
            default:
                return -1;
        }
    }
    return 0;
}

std::string FaceCube::toString() {
    std::string s;
    s.reserve(FACELET_LEN+1);
    for (int i = 0; i < FACELET_LEN; ++i) {
        if (i > 0 && i%9 == 0) {
            s.append(" ");
        }
        switch(f[i]){
            case U:
                s.append("U");
                break;
            case R:
                s.append("R");
                break;
            case F:
                s.append("F");
                break;
            case D:
                s.append("D");
                break;
            case L:
                s.append("L");
                break;
            case B:
                s.append("B");
                break;
        }
    }
    return s;
}

void FaceCube::toCubieCube(CubieCube *ccRet) {
    uint8_t ori = 0;
    for (int i = 0; i < 8; i++)
        ccRet->cp[i] = URF;// invalidate corners
    for (int i = 0; i < 12; i++)
        ccRet->ep[i] = UR;// and edges
    Color col1, col2;
    for (int i = URF; i <= DRB; ++i) {
        // get the colors of the cubie at corner i, starting with U/D
        for (ori = 0; ori < 3; ori++)
            if (f[cornerFacelet[i][ori]] == U || f[cornerFacelet[i][ori]] == D)
                break;
        col1 = f[cornerFacelet[i][(ori + 1) % 3]];
        col2 = f[cornerFacelet[i][(ori + 2) % 3]];

        for (int j = URF; j <= DRB; ++j) {
            if (col1 == cornerColor[j][1] && col2 == cornerColor[j][2]) {
                // in cornerposition i we have cornercubie j
                ccRet->cp[i] = (Corner) j;
                ccRet->co[i] = (uint8_t) (ori % 3);
                break;
            }
        }
    }
    for (int i = UR; i <= BR; ++i) {
        for (int j = UR; j <= BR; ++j) {
            if (f[edgeFacelet[i][0]] == edgeColor[j][0]
                && f[edgeFacelet[i][1]] == edgeColor[j][1]) {
                ccRet->ep[i] = (Edge) j;
                ccRet->eo[i] = 0;
                break;
            }
            if (f[edgeFacelet[i][0]] == edgeColor[j][1]
                && f[edgeFacelet[i][1]] == edgeColor[j][0]) {
                ccRet->ep[i] = (Edge) j;
                ccRet->eo[i] = 1;
                break;
            }
        }
    }
}

std::string FaceCube::randomCube() {
    CubieCube cc;
    cc.setFlip((uint16_t)(std::rand() % CoordCube::N_FLIP));
    cc.setTwist((uint16_t)(std::rand() % CoordCube::N_TWIST));
    for (int i = 0; i <= DRB; ++i) {
        cc.cp[i] = (Corner)i;
    }
    for (int j = 0; j <= BR; ++j) {
        cc.ep[j] = (Edge)j;
    }
    do {
        Corner *cnr_begin = &(cc.cp[0]);
        Corner *cnr_end = cnr_begin + 8;
        std::random_shuffle(cnr_begin, cnr_end);

        Edge *edg_begin = &(cc.ep[0]);
        Edge *edg_end = edg_begin + 12;
        std::random_shuffle(edg_begin, edg_end);
    } while((cc.edgeParity()^cc.cornerParity())!=0);
    fromCubieCube(&cc);
    return toString();
}

void FaceCube::fromCubieCube(CubieCube *c) {
    for (int i = 0; i <= DRB; ++i) {
        Corner j = c->cp[i];
        uint8_t  ori = c->co[i]; // Orientation of this cubie
        for (int n = 0; n < 3; n++)
            f[cornerFacelet[i][(n + ori) % 3]] = cornerColor[j][n];
    }
    for (int i = 0; i <= BR; ++i) {
        Edge j = c->ep[i];// edgecubie with index j is at edgeposition
        uint8_t ori = c->eo[i];// Orientation of this cubie
        for (int n = 0; n < 2; n++)
            f[edgeFacelet[i][(n + ori) % 2]] = edgeColor[j][n];
    }
}

