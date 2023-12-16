//
// Created by 16182 on 11/27/2023.
//

#ifndef ALGO_PROJECT_EXACT_H
#define ALGO_PROJECT_EXACT_H

#include<string>
#include<vector>
#include<span>
#include"algo.h"

AlignmentResult exact_align(std::vector<std::string> const& sequences, ScoreMatrix const& matrix);

#endif //ALGO_PROJECT_EXACT_H
