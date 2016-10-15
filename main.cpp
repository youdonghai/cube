#include <iostream>
#include <ctime>
#include <algorithm>
#include "Search.h"
#include "FaceCube.h"
#include "CoordCube.h"

//std::string s = "";
//std::vector<uint8_t> moves = {};
//std::string s = "UDULUFRRL UUBDRFBRB DFFRFBLLR FUDLDUULD BFFULBFDD LRRDBBLBR"; //F U R2 L' B L' U2 B' D2 R' F' . U L2 F2 U L2 F2 B2 D' L2 B2
//std::vector<uint8_t> moves = {6, 0, 4, 14, 15, 14, 1, 17, 10, 5, 8, 0, 13, 7, 0, 13, 7, 16, 11, 13, 16,};
std::string s = "LFDUUDLDB RBRDRFFUU FLUUFFBRU UDLBDFRBF DBDULRDRL FRBLBLRLB";
std::vector<uint8_t> moves = {9, 16, 14, 9, 17, 10, 4, 17, 10, 3, 0, 4, 2, 13, 16, 2, 13, 1, 13, 16,};

void test1() {
    FaceCube fc(s);
    std::cout << fc.toString() << std::endl;
    CubieCube cc;
    fc.toCubieCube(&cc);

    int v = cc.verify();
    if (0 != v) {
        std::cout << "1 verify error" << std::endl;
        return;
    }

    CoordCube coordc(&cc);
    for(size_t i=0; i<moves.size();++i) {
        coordc.move(moves[i]);
    }
    coordc.toCubiecube(&cc);
    if (cc.verify() != 0) {
        std::cout << "2 verify error" << std::endl;
    }
    fc.fromCubieCube(&cc);
    s = fc.toString();
    std::cout << s << std::endl;
}

void test2() {
    for (int i = 0; i < 18; ++i) {
        FaceCube fc;
        s = fc.toString();
        CubieCube cc;
        fc.toCubieCube(&cc);

        CoordCube coordc(&cc);
        coordc.move(i);

        coordc.toCubiecube(&cc);
        if (cc.verify() != 0) {
            std::cout << "verify error" << std::endl;
        }
        fc.fromCubieCube(&cc);
        s = fc.toString();
        std::cout << s << std::endl;
    }
}

void test3() {
    FaceCube fc;
    std::string s = fc.randomCube();
    std::cout << s << std::endl;
    fc.fromString(s);

    CubieCube cc;
    fc.toCubieCube(&cc);

    int v = cc.verify();
    if (0 != v) {
        std::cout << "1 verify error" << std::endl;
        return;
    }

    fc.fromCubieCube(&cc);
    std::cout << fc.toString() << std::endl;

}

int main() {
    std::srand((unsigned int) std::time(0));

    CubieCube::init();
    CoordCube::init();

    Search s;
    s.init("UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB");
    s.phase1(0, 9999999999);

    Search::printResult();
    //test1();
}