//
// Created by 16182 on 12/15/2023.
//

#include "tree.h"
#include "exact.h"
#include "bound_exact.h"
#include <map>


AlignmentResult tree_align(std::vector<std::string> const& sequences, ScoreMatrix const& matrix){
    NDarray<AlignmentResult, 2> pair_scores(sequences.size());

    for(int i = 0; i < sequences.size(); i++){
        for(int j = i + 1; j < sequences.size(); j++){
            AlignmentResult res = bounded_exact_align({sequences[i], sequences[j]}, matrix,40);
            pair_scores.at(i,j) = res;
            std::swap(res.alignment[0], res.alignment[1]);
            pair_scores.at(j,i) = res;
        }
    }

    std::map<int, std::string> alignment;
    alignment[0] = sequences[0];

    while(alignment.size() != sequences.size()){
        int best_score = INT_MIN;
        int best_idx = 0;
        int best_src = 0;
        for(auto [k,v] : alignment){
            for(int i = 0; i < sequences.size(); i++){
                if(!alignment.contains(i)){
                    if(pair_scores.at(i,k).score > best_score){
                        best_score = pair_scores.at(i,k).score;
                        best_idx = i;
                        best_src = k;
                    }
                }
            }
        }

        std::string align_i = pair_scores.at(best_idx,best_src).alignment[0];
        std::string align_center = pair_scores.at(best_idx,best_src).alignment[1];
        const auto & ref_align = alignment[best_src];
        for(int i = 0; i < ref_align.size() && i < align_center.size() && i < align_i.size(); i++){
            if(ref_align[i] == '-' && align_center[i] != '-'){
                align_center.insert(align_center.begin() + i, '-');
                align_i.insert(align_i.begin() + i, '-');
            } else if(ref_align[i] != '-' && align_center[i] == '-'){
                for(auto&[_,s] : alignment){
                    s.insert(s.begin() + i , '-');
                }
            }
        }
        alignment[best_idx] = align_i;
    }

    std::vector<std::string> a;
    for(int i = 0; i < sequences.size(); i++){
        a.push_back(alignment[i]);
    }
    return {.score=score_alignment(matrix, a), .alignment=a};
}
