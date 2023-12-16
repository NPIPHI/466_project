//
// Created by 16182 on 12/15/2023.
//

#include "star.h"
#include "exact.h"
#include "bound_exact.h"

AlignmentResult star_align(std::vector<std::string> const& sequences, ScoreMatrix const& matrix){
    NDarray<AlignmentResult, 2> pair_scores(sequences.size());

    for(int i = 0; i < sequences.size(); i++){
        for(int j = i+1; j < sequences.size(); j++){
            AlignmentResult res = bounded_exact_align({sequences[i], sequences[j]}, matrix, 40);
            pair_scores.at(i,j) = res;
            std::swap(res.alignment[0], res.alignment[1]);
            pair_scores.at(j,i) = res;
        }
    }

    int center_idx = 0;
    int center_score = INT_MIN;
    for(int i = 0; i < sequences.size(); i++){
        int score = 0;
        for(int j = 0; j < sequences.size(); j++){
            if(i != j){
                score += pair_scores.at(i,j).score;
            }
        }
        if(score > center_score){
            center_score = score;
            center_idx = i;
        }
    }

    std::vector<std::string> alignment;
    alignment.push_back(sequences[center_idx]);

    for(int i = 0; i < sequences.size(); i++){
        if(i != center_idx){
            std::string align_i = pair_scores.at(i,center_idx).alignment[0];
            std::string align_center = pair_scores.at(i,center_idx).alignment[1];
            const auto & ref_align = alignment[0];
            for(int i = 0; i < ref_align.size() && i < align_center.size() && i < align_i.size(); i++){
                if(ref_align[i] == '-' && align_center[i] != '-'){
                    align_center.insert(align_center.begin() + i, '-');
                    align_i.insert(align_i.begin() + i, '-');
                } else if(ref_align[i] != '-' && align_center[i] == '-'){
                    for(auto&s : alignment){
                        s.insert(s.begin() + i , '-');
                    }
                }
            }
            alignment.push_back(align_i);
        }
    }
    return {.score=score_alignment(matrix, alignment), .alignment=alignment};
}
