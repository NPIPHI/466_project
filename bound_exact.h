//
// Created by 16182 on 12/14/2023.
//

#ifndef ALGO_PROJECT_BOUND_EXACT_H
#define ALGO_PROJECT_BOUND_EXACT_H

#include<string>
#include<vector>
#include<span>
#include"algo.h"

AlignmentResult bounded_exact_align(std::vector<std::string> const& sequences, ScoreMatrix const& matrix, int bound);


#endif //ALGO_PROJECT_BOUND_EXACT_H
