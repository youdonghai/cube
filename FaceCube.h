//
// Created by youdonghai on 14/10/16.
//

#ifndef CUBE_FACECUBE_H
#define CUBE_FACECUBE_H

#include <string>
#include "CubeDefine.h"
#include "CubieCube.h"

const int FACELET_LEN = 54;

class FaceCube {
public:
    FaceCube();
    FaceCube(std::string s);
    int fromString(std::string s);
    std::string toString();
    void toCubieCube(CubieCube *c);
    void fromCubieCube(CubieCube *c);
    std::string randomCube();

    Color f[FACELET_LEN];
    static const Facelet cornerFacelet[8][3];
    static const Facelet edgeFacelet[12][2];
    static const Color cornerColor[8][3];
    static const Color edgeColor[12][2];

};


#endif //CUBE_FACECUBE_H
