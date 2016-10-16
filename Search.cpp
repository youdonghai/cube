//
// Created by youdonghai on 15/10/16.
//

#include <iostream>
#include <cmath>
#include <algorithm>
#include "Search.h"

#include "FaceCube.h"
#include "CoordCube.h"

std::mutex Search::tlk;
int Search::phase1_done = 0;
int Search::phase2_done = 0;
uint8_t Search::result_moves[31] = {};
uint8_t Search::result_move_len = 0;


int Search::init(std::string cs, int thread_index, int thread_size) {
    this->thread_index = thread_index;
    this->thread_size = thread_size;

    FaceCube fc;
    CubieCube cc;
    fc.fromString(cs);
    fc.toCubieCube(&cc);
    if (cc.verify() != 0) {
        std::cerr << "Cube verify error" << std::endl;
        return -1;
    }
    now_local_phase = 1;
    coordc.fromCubieCube(&cc);
    if (coordc.isClean()) {
        std::vector<uint8_t> tmp;
        phase1Done(&tmp, 0);
        now_local_phase = 3;
    }
    return 0;
}


void Search::phase1() {
    uint64_t max = (uint64_t) powl(18, 12) + 100;
    uint64_t part = max / (uint64_t)thread_size;
    uint64_t start = part * thread_index;

    population.resize(population_size);
    for (int i = 0; i < population_size; ++i) {
        population[i].resize(18);
        uint64_t rp = (uint64_t) rand();
        rp = (rp << 32) + rand();
        rp = start + (rp % part);
        uint64_t tmp_rp = rp;
        for (int j = 0; j < 12; ++j) {
            population[i][j] = (uint8_t) (tmp_rp % 18);
            tmp_rp = tmp_rp / 18;
        }
    }

    while (!isPhase1Done()) {
        survive();
        mutation();
    }
}

std::string Search::printResult() {
    std::cout << "result move len " << (int)result_move_len << std::endl;
    for (int i = 0; i < result_move_len; ++i) {
        std::cout << (int)result_moves[i] << ", ";
    }
    std::cout << std::endl;
    return std::string();
}


uint8_t Search::randPhase2gene() {
    static const uint8_t phase2_moves[10] = {0, 1, 2, 4, 7, 9, 10, 11, 13, 16};//{3, 5, 6, 8, 12, 14, 15, 17,};
    return phase2_moves[rand()%10];
}

void Search::mutation() {
    int gene_len = 12;
    bool phase2 = false;
    if (now_local_phase == 2) {
        gene_len = 18;
        phase2 = true;
    }
    uint8_t new_gene = 0;
    for (int i = 0; i < survive_size; ++i) {
        for (int j = 0; j < population_size / survive_size; ++j) {
            std::vector<uint8_t> tmp_gene = population[i];
            for (int k = 0; k < mutation_number; ++k) {
                int gene_index = rand() % gene_len;
                if (phase2) {
                    new_gene = randPhase2gene();
                } else {
                    new_gene = (uint8_t) (rand() % 18);
                }
                tmp_gene[gene_index] = new_gene;
            }
            population.push_back(tmp_gene);
        }
    }
}

bool Search::isPhase1Done() {
    tlk.lock();
    if (phase1_done == 0) {
        tlk.unlock();
        return false;
    }
    tlk.unlock();
    return true;
}

void Search::phase1Done(std::vector<uint8_t> *m, int len) {
    tlk.lock();
    if (1 == phase1_done) {
        // find another solution, skip
        tlk.unlock();
        return;
    }
    phase1_done = 1;
    result_move_len = (uint8_t) len;
    for (int i = 0; i < len; ++i) {
        result_moves[i] = m->at(i);
    }
    tlk.unlock();
}

int getPhase1Fitness(CoordCube *c, std::vector<uint8_t> *moves, int len) {
    static int now_min_fitness = INT32_MAX;
    CoordCube tmpc = *c;
    int min_fitness = INT32_MAX;
    for (int i = 0; i < len; ++i) {
        tmpc.move(moves->at(i));
        int fitness = tmpc.twist + tmpc.flip + tmpc.FRtoBR/24;
        if (0 == fitness) {
            Search::phase1Done(moves, i+1);
            return 0;
        }
        if (fitness < now_min_fitness) {
            now_min_fitness = fitness;
            std::cout << "min fitness " << now_min_fitness << std::endl;
        }
        if (i > 3 && fitness < min_fitness) {
            min_fitness = fitness;
        }
    }
    return min_fitness;
}

int getPhase2Fitness(CoordCube *c, std::vector<uint8_t> *moves, int len) {
    CoordCube tmpc = *c;
    int min_fitness = INT32_MAX;
    for (int i = 0; i < len; ++i) {
        tmpc.move(moves->at(i));
        int fitness = tmpc.cornerPermutation + tmpc.FRtoBR + tmpc.URtoDB;
        if (0 == fitness) {
            return 0;
        }
        if (i > 4 && fitness < min_fitness) {
            min_fitness = fitness;
        }
    }
    return min_fitness;
}

bool pairCompare(const std::pair<int, int>& firstElem, const std::pair<int, int>& secondElem) {
    return firstElem.second < secondElem.second;
}

void Search::survive() {
    std::vector<std::pair<int, int> > vec_fitness;
    vec_fitness.resize(population_size);
    int fitness = 0;
    for (int i = 0; i < population_size; ++i) {
        if (now_local_phase == 1) {
            fitness = getPhase1Fitness(&coordc, &population[i], 12);
        } else {
            fitness = getPhase2Fitness(&coordc, &population[i], 18);
        }
        vec_fitness[i] = std::make_pair(i, fitness);
    }

    std::sort(vec_fitness.begin(), vec_fitness.end(), pairCompare);

    std::vector<std::vector<uint8_t> > survived;
    for (int i = 0; i < survive_size; ++i) {
        survived.push_back(population[vec_fitness[i].first]);
    }
    population.swap(survived);
}

