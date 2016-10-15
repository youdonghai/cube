//
// Created by youdonghai on 14/10/16.
//

#include <algorithm>
#include "CubieCube.h"

static Corner cpU[] = { UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB };
static uint8_t coU[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static Edge epU[] = { UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR };
static uint8_t eoU[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static Corner cpR[] = { DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR };
static uint8_t coR[] = { 2, 0, 0, 1, 1, 0, 0, 2 };
static Edge epR[] = { FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR };
static uint8_t eoR[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static Corner cpF[] = { UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB };
static uint8_t coF[] = { 1, 2, 0, 0, 2, 1, 0, 0 };
static Edge epF[] = { UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR };
static uint8_t eoF[] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 };

static Corner cpD[] = { URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR };
static uint8_t coD[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static Edge epD[] = { UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR };
static uint8_t eoD[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static Corner cpL[] = { URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB };
static uint8_t coL[] = { 0, 1, 2, 0, 0, 2, 1, 0 };
static Edge epL[] = { UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR };
static uint8_t eoL[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static Corner cpB[] = { URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL };
static uint8_t coB[] = { 0, 0, 1, 2, 0, 0, 2, 1 };
static Edge epB[] = { UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB };
static uint8_t eoB[] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 };

CubieCube *CubieCube::moveCube[6] = {};

void CubieCube::init() {
    initMoveCube();
}

void CubieCube::initMoveCube() {
    for (int i = 0; i < 6; ++i) {
        moveCube[i] = new CubieCube();
        if (i == U) {
            std::copy(cpU, cpU+8, moveCube[i]->cp);
            std::copy(coU, coU+8, moveCube[i]->co);
            std::copy(epU, epU+12, moveCube[i]->ep);
            std::copy(eoU, eoU+12, moveCube[i]->eo);
        }
        if (i == R) {
            std::copy(cpR, cpR+8, moveCube[i]->cp);
            std::copy(coR, coR+8, moveCube[i]->co);
            std::copy(epR, epR+12, moveCube[i]->ep);
            std::copy(eoR, eoR+12, moveCube[i]->eo);
        }
        if (i == F) {
            std::copy(cpF, cpF+8, moveCube[i]->cp);
            std::copy(coF, coF+8, moveCube[i]->co);
            std::copy(epF, epF+12, moveCube[i]->ep);
            std::copy(eoF, eoF+12, moveCube[i]->eo);
        }
        if (i == D) {
            std::copy(cpD, cpD+8, moveCube[i]->cp);
            std::copy(coD, coD+8, moveCube[i]->co);
            std::copy(epD, epD+12, moveCube[i]->ep);
            std::copy(eoD, eoD+12, moveCube[i]->eo);
        }
        if (i == L) {
            std::copy(cpL, cpL+8, moveCube[i]->cp);
            std::copy(coL, coL+8, moveCube[i]->co);
            std::copy(epL, epL+12, moveCube[i]->ep);
            std::copy(eoL, eoL+12, moveCube[i]->eo);
        }
        if (i == B) {
            std::copy(cpB, cpB+8, moveCube[i]->cp);
            std::copy(coB, coB+8, moveCube[i]->co);
            std::copy(epB, epB+12, moveCube[i]->ep);
            std::copy(eoB, eoB+12, moveCube[i]->eo);
        }
    }
}


CubieCube::CubieCube() {
}

uint16_t CubieCube::getTwist() {
    uint16_t ret = 0;
    for (int i = URF; i < DRB; i++)
        ret = (uint16_t) (3 * ret + co[i]);
    return ret;
}

void CubieCube::setTwist(uint16_t twist) {
    int twistParity = 0;
    for (int i = DRB - 1; i >= URF; i--) {
        co[i] = (uint8_t) (twist % 3);
        twistParity += co[i];
        twist /= 3;
    }
    co[DRB] = (uint8_t) ((3 - twistParity % 3) % 3);
}

uint16_t CubieCube::getFlip() {
    uint16_t ret = 0;
    for (int i = UR; i < BR; i++)
        ret = (uint16_t) (2 * ret + eo[i]);
    return ret;
}

void CubieCube::setFlip(uint16_t flip) {
    int flipParity = 0;
    for (int i = BR - 1; i >= UR; i--) {
        flipParity += eo[i] = (uint8_t) (flip % 2);
        flip /= 2;
    }
    eo[BR] = (uint8_t) ((2 - flipParity % 2) % 2);
}

int CubieCube::Cnk(int n, int k) {
    int i, j, s;
    if (n < k)
        return 0;
    if (k > n / 2)
        k = n - k;
    for (s = 1, i = n, j = 1; i != n - k; i--, j++) {
        s *= i;
        s /= j;
    }
    return s;
}

uint8_t CubieCube::cornerParity() {
    int s = 0;
    for (int i = DRB; i >= URF + 1; i--)
        for (int j = i - 1; j >= URF; j--)
            if (cp[j] > cp[i])
                s++;
    return (uint8_t) (s % 2);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Parity of the edges permutation. Parity of corners and edges are the same if the cube is solvable.
uint8_t CubieCube::edgeParity() {
    int s = 0;
    for (int i = BR; i >= UR + 1; i--)
        for (int j = i - 1; j >= UR; j--)
            if (ep[j] > ep[i])
                s++;
    return (uint8_t) (s % 2);
}


int CubieCube::verify() {
    int sum = 0;
    int edgeCount[12] = {};
    for (int i = 0; i <= BR; ++i) {
        edgeCount[ep[i]]++;
    }
    for (int i = 0; i < 12; i++)
        if (edgeCount[i] != 1)
            return -2;

    for (int i = 0; i < 12; i++)
        sum += eo[i];
    if (sum % 2 != 0)
        return -3;

    int cornerCount[8] = {};
    for (int i = 0; i <= DRB; ++i) {
        cornerCount[cp[i]]++;
    }
    for (int i = 0; i < 8; i++)
        if (cornerCount[i] != 1)
            return -4;// missing corners

    sum = 0;
    for (int i = 0; i < 8; i++)
        sum += co[i];
    if (sum % 3 != 0)
        return -5;// twisted corner

    if ((edgeParity() ^ cornerParity()) != 0)
        return -6;// parity error

    return 0;// cube ok
}

// http://kociemba.org/math/UDSliceCoord.htm
uint16_t CubieCube::getUDSlicePosition() {
    int c = 0;
    int x = 0;
    for (int i = BR; i >= UR; --i) {
        if (FR <= ep[i] && ep[i] <= BR) {
            c += Cnk(11 - i, ++x);
        }
    }
    return (uint16_t) c;
}

void CubieCube::rotateLeft(Edge *ea, int l, int r) {
    Edge temp = ea[l];
    for (int i = l; i < r; i++)
        ea[i] = ea[i + 1];
    ea[r] = temp;
}

uint16_t CubieCube::getCornerPermutation() {
    Corner perm[8] = {};
    int b = 0;
    for (int i = 0; i < 8; i++)
        perm[i] = cp[i];
    for (int j = 7; j > 0; j--)// compute the index b < 8! for the permutation in perm
    {
        int k = 0;
        while (perm[j] != (Corner)j) {
            rotateLeft(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (uint16_t) b;
}

void CubieCube::rotateLeft(Corner *ca, int l, int r) {
    Corner temp = ca[l];
    for (int i = l; i < r; i++)
        ca[i] = ca[i + 1];
    ca[r] = temp;
}

uint8_t CubieCube::getFRtoBRPermutations() {
    Edge perm[4] = {};
    int b = 0;
    int j = 0;
    for (int i = 0; i < 12; ++i) {
        if (ep[i] >= FR && ep[i] <= BR) {
            perm[j++] = ep[i];
        }
    }
    for (int j = 3; j > 0; j--)// compute the index b < 8! for the permutation in perm
    {
        int k = 0;
        while (perm[j] != (Edge)(j+8)) {
            rotateLeft(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (uint8_t) b;
}

uint16_t CubieCube::getURtoDBPermutations() {
    // max 40320
    int j = 0;
    Edge perm[8] = {};
    for (int i = 0; i < 12; ++i) {
        if (ep[i] >= UR && ep[i] <= DB) {
            perm[j++] = ep[i];
        }
    }
    int x = 0;
    for (int j = 7; j > 0; j--)// compute the index b < 8! for the permutation in perm
    {
        int k = 0;
        while (perm[j] != (Edge)j) {
            rotateLeft(perm, 0, j);
            k++;
        }
        x = (j + 1) * x + k;
    }
    return (uint16_t) x;
}

void CubieCube::cornerMultiply(CubieCube *c) {
    Corner cPerm[8] = {};
    uint8_t cOri[8] = {};
    for (int corn = 0; corn < 8; ++corn) {
        cPerm[corn] = cp[c->cp[corn]];

        uint8_t oriA = co[c->cp[corn]];
        uint8_t oriB = c->co[corn];
        uint8_t ori = 0;
        ori = (uint8_t) (oriA + oriB);
        if (ori >= 3)
            ori -= 3;
        cOri[corn] = ori;
    }
    for (int c = 0; c < 8; ++c) {
        cp[c] = cPerm[c];
        co[c] = cOri[c];
    }
}

void CubieCube::edgeMultiply(CubieCube *c) {
    Edge ePerm[12] = {};
    uint8_t eOri[12] = {};
    for (int edge = 0; edge < 12; ++edge) {
        ePerm[edge] = ep[c->ep[edge]];
        eOri[edge] = (uint8_t) ((c->eo[edge] + eo[c->ep[edge]]) % 2);
    }
    for (int e = 0; e < 12; ++e) {
        ep[e] = ePerm[e];
        eo[e] = eOri[e];
    }
}

uint16_t CubieCube::getFRtoBR() {
    int slice1 = getUDSlicePosition();
    int slice2 = getFRtoBRPermutations();
    return (uint16_t) (slice1 * 24 + slice2);
}

void CubieCube::setFRtoBR(uint16_t idx) {
    int x;
    Edge sliceEdge[] = { FR, FL, BL, BR };
    Edge otherEdge[] = { UR, UF, UL, UB, DR, DF, DL, DB };
    int b = idx % 24; // Permutation
    int a = idx / 24; // Combination
    for (int i = 0; i < 12; ++i) {
        ep[i] = DB;
    }

    for (int j = 1, k; j < 4; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight(sliceEdge, 0, j);
    }

    x = 3;// generate combination and set slice edges
    for (int j = UR; j <= BR; j++)
        if (a - Cnk(11 - j, x + 1) >= 0) {
            ep[j] = sliceEdge[3 - x];
            a -= Cnk(11 - j, x-- + 1);
        }
    x = 0; // set the remaining edges UR..DB
    for (int j = UR; j <= BR; j++)
        if (ep[j] == DB)
            ep[j] = otherEdge[x++];
}

uint32_t CubieCube::getURtoDB() {
    int slice1 = getUDSlicePosition();
    int slice2 = getURtoDBPermutation();
    // 8! == 40320
    return (uint32_t) (slice1 * 40320 + slice2);
}

void CubieCube::setURtoDB(uint32_t idx, bool over_write_FRtoBR) {
    // 8! == 40320
    int x;
    Edge sliceEdge[] = { UR, UF, UL, UB, DR, DF, DL, DB };
    Edge otherEdge[] = { FR, FL, BL, BR };
    int b = idx % 40320; // Permutation
    int a = idx / 40320; // Combination
    Edge tmp_ep[12] = {};
    for (int i = 0; i < 12; ++i) {
        tmp_ep[i] = DB;
    }

    for (int j = 1, k; j < 8; j++)// generate permutation from index b
    {
        k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0)
            rotateRight(sliceEdge, 0, j);
    }

    x = 3;
    for (int j = UR; j <= BR; j++)
        if (a - Cnk(11 - j, x + 1) >= 0) {
            tmp_ep[j] = otherEdge[3 - x];
            a -= Cnk(11 - j, x-- + 1);
        }
    x = 0;
    for (int j = UR; j <= BR; j++)
        if (tmp_ep[j] == DB)
            ep[j] = sliceEdge[x++];

    if (over_write_FRtoBR) {
        for (int j = UR; j <= BR; j++) {
            if (tmp_ep[j] >= FR && tmp_ep[j] <= BR) {
                ep[j] = tmp_ep[j];
            }
        }
    }
}

void CubieCube::rotateRight(Edge *ea, int l, int r) {
    Edge temp = ea[r];
    for (int i = r; i > l; i--)
        ea[i] = ea[i - 1];
    ea[l] = temp;
}

void CubieCube::rotateRight(Corner *ca, int l, int r) {
    Corner temp = ca[r];
    for (int i = r; i > l; i--)
        ca[i] = ca[i - 1];
    ca[l] = temp;
}

void CubieCube::setCornerPermutation(uint16_t idx) {
    Corner perm[] = { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
    int k;
    for (int j = 1; j < 8; j++) {
        k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            rotateRight(perm, 0, j);
    }
    int x = 7;// set corners
    for (int j = 7; j >= 0; j--)
        cp[j] = perm[x--];
}

uint16_t CubieCube::getURtoDBPermutation() {
    Edge perm[8] = {};
    int b = 0;
    int j = 0;
    for (int i = 0; i < 12; ++i) {
        if (ep[i] >= UR && ep[i] <= DB) {
            perm[j++] = ep[i];
        }
    }
    for (int j = 7; j > 0; j--)
    {
        int k = 0;
        while (perm[j] != (Edge)j) {
            rotateLeft(perm, 0, j);
            k++;
        }
        b = (j + 1) * b + k;
    }
    return (uint16_t) b;
}

void CubieCube::setURtoDBPermutation(uint16_t idx) {
    Edge perm[] = {UR, UF, UL, UB, DR, DF, DL, DB};
    int k;
    for (int j = 1; j < 8; j++) {
        k = idx % (j + 1);
        idx /= j + 1;
        while (k-- > 0)
            rotateRight(perm, 0, j);
    }
    int x = 7;
    for (int j = 7; j >= 0; j--)
        ep[j] = perm[x--];
}

