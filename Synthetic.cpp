//
// Created by 16182 on 11/28/2023.
//

#include "Synthetic.h"
#include <random>

std::vector<std::string> synth_data(int count, int len, double insert_rate, double mutate_rate){
    std::string base;

    std::mt19937 gen;
    std::uniform_real_distribution uniform(0.0,1.0);
    std::uniform_int_distribution<> letter_rand(0, 3);

    constexpr char letters[] = {'a','c','t','g'};

    auto rand_letter = [&](){
        return letters[letter_rand(gen)];
    };

    for(int i = 0; i < len; i++){
        base.push_back(rand_letter());
    }

    std::vector<std::string> seqs(count);

    for(auto& s: seqs){
        for(int i = 0; i < len; i++){
            if(uniform(gen) < insert_rate){
                s.push_back(rand_letter());
                i--;
            } else {
                if(uniform(gen) < mutate_rate){
                    s.push_back(rand_letter());
                } else {
                    s.push_back(base[i]);
                }
            }
        }
    }

    return seqs;
}