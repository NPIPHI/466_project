//
// Created by 16182 on 11/27/2023.
//

#include "exact.h"
#include "mem.h"

template<int N>
void fill_score(std::array<char, N> charset, ScoreMatrix const& matrix, std::span<int, N> indices, NDarray<int, N> & M, NDarray<int, N> & dirs) {
    int best_score = INT_MIN;
    int best_dir = 0;
    for(int dir = 1; dir < (1 << N); dir++){
        auto chars = charset;
        std::array<int, N> idx{};
        bool oob = false;
        for(int j = 0; j < N; j++){
            if((dir & (1 << j)) == 0){
                chars[j] = '-';
                idx[j] = indices[j];
            } else {
                idx[j] = indices[j] - 1;
                if(idx[j] < 0){
                    oob = true;
                }
            }
        }
        if(oob) continue;
        int sc = score<N>(matrix, chars) + M[idx];
        if(best_score < sc){
            best_score = sc;
            best_dir = dir;
        }
    }

    M[indices] = best_score == INT_MIN ? 0 : best_score; //0 if top left
    dirs[indices] = best_dir;
}

template<int N, int NN>
void exact_alignN(std::vector<std::string> const& sequences, ScoreMatrix const& matrix, std::span<int, NN> indices, NDarray<int, NN> & M, NDarray<int, NN> & dirs){
    if constexpr(N == 0){
        std::array<char, NN> charset{};
        for(int i = 0; i < NN; i++){
            charset[i] = sequences[i][indices[i]];
        }
        fill_score<NN>(charset, matrix, indices, M, dirs);
    } else {
        for(int i = 0; i < sequences[NN-N].size(); i++){
            indices[NN-N] = i;
            exact_alignN<N-1, NN>(sequences, matrix, indices, M, dirs);
        }
    }
}

template<int N>
std::vector<std::string> traceback(std::vector<std::string> const& sequences, NDarray<int, N> & dirs){
    std::vector<std::string> res(sequences.size());
    std::array<int, N> indices;
    for(int i = 0; i < sequences.size(); i++){
        indices[i] = sequences[i].size() - 1;
    }
    while(indices != std::array<int, N>{}){
        int dir = dirs[indices];
        for(int i = 0; i < N; i++){
            if(dir & (1<<i)){
                res[i].push_back(sequences[i][indices[i]]);
                indices[i]--;
            } else {
                res[i].push_back('-');
            }
        }
    }

    for(auto&s: res){
        std::reverse(s.begin(), s.end());
    }

    return res;
}

template<int N>
AlignmentResult pick_align_algo(std::vector<std::string> const& sequences, ScoreMatrix const& matrix){
    if constexpr(N == 0){
        throw "";
    } else {
        if(sequences.size() == N){
            size_t max_size = std::max_element(sequences.begin(),  sequences.end(), [](auto const&a, auto const&b){return a.size() < b.size();})->size();
            NDarray<int, N> M(max_size);
            M.fill(INT_MIN);
            NDarray<int, N> dirs(max_size);
            std::array<int, N> indices{};
            exact_alignN<N, N>(sequences, matrix, indices, M, dirs);
            auto alignment = traceback(sequences, dirs);
            return AlignmentResult{.score=M[indices], .alignment=alignment};
        } else {
            return pick_align_algo<N-1>(sequences, matrix);
        }
    }
}

AlignmentResult exact_align(std::vector<std::string> const& sequences, ScoreMatrix const& matrix){
    auto seqs = sequences;
    for(auto&s : seqs){
        s.insert(s.begin(), '-');
    }
    return pick_align_algo<10>(seqs, matrix);
}