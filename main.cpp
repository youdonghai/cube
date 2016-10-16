#include <iostream>
#include <ctime>
#include <algorithm>
#include <getopt.h>
#include <sstream>
#include "Search.h"
#include "FaceCube.h"
#include "CoordCube.h"
#include "Search2.h"

std::vector<int> toMoveList(std::string solution) {
    std::vector<std::string> str_moves;
    std::stringstream ss;
    ss.str(solution);
    std::string item;
    while (std::getline(ss, item, ',')) {
        str_moves.push_back(item);
    }
    std::vector<int> moves;
    for (size_t i = 0; i < str_moves.size(); ++i) {
        moves.push_back(std::atoi(str_moves[i].c_str()));
    }
    return moves;
}

int verify(std::string cube, std::string solution) {
    FaceCube fc(cube);
    std::cout << fc.toString() << std::endl;
    CubieCube cc;
    fc.toCubieCube(&cc);

    int v = cc.verify();
    if (0 != v) {
        std::cout << "verify error" << std::endl;
        return -1;
    }

    std::vector<int> moves = toMoveList(solution);
    CoordCube coordc(&cc);
    for(size_t i=0; i<moves.size();++i) {
        coordc.move(moves[i]);
    }
    if (coordc.isClean()) {
        std::cout << "solution ok." << std::endl;
        return 0;
    }
    std::cout << "can not restore the cube." << std::endl;
    return 1;
}

void usage(char *argv1) {
    printf("%s [options]\n", argv1);
    printf("Options:\n  no option, generate a random cube and restore it.\n");
    printf("  -g: generate a random cube string.\n");
    printf("  -c <cube string>: restore cube.\n");
    printf("  -c <cube string> -s <solution>: verify a solution.\n");
}

int main(int argc, char **argv) {
    std::srand((unsigned int) std::time(0));

    int gflag = 0;
    int cflag = 0;
    char *cube_string = NULL;
    int sflag = 0;
    char *solution = NULL;

    int c;
    while ((c = getopt(argc, argv, "hgc:s:")) != -1) {
        switch (c) {
            case 'h':
                usage(argv[0]);
                exit(0);
            case 'g':
                gflag = 1;
                break;
            case 'c':
                cflag = 1;
                cube_string = optarg;
                break;
            case 's':
                sflag = 1;
                solution = optarg;
        }
    }

    if (cflag && sflag) {
        CubieCube::init();
        CoordCube::init();
        return verify(cube_string, solution);
    }

    if (cflag) {
        CubieCube::init();
        CoordCube::init();
        Search2 s(cube_string);
        return s.go();
    }

    if (gflag) {
        FaceCube fc;
        std::cout << fc.randomCube() << std::endl;
        return 0;
    }

    // no options
    FaceCube fc;
    std::string cs = fc.randomCube();
    CubieCube::init();
    CoordCube::init();
    Search2 s(cs);
    return s.go();
}