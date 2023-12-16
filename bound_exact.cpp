//
// Created by 16182 on 11/27/2023.
//

#include "bound_exact.h"
#include "mem.h"
template<int N>
void bounded_fill_score(std::array<char, N> charset, ScoreMatrix const& matrix, std::span<int, N> indices, BoundedNDarray<int, N> & M, BoundedNDarray<int, N> & dirs) {
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
        int sc = M[idx] == INT_MIN ? INT_MIN : score<N>(matrix, chars) + M[idx];
        if(best_score < sc){
            best_score = sc;
            best_dir = dir;
        }
    }
    M[indices] = best_score == INT_MIN ? 0 : best_score; //0 if top left
    dirs[indices] = best_dir;
}

template<int N, int NN>
void bounded_exact_alignN(std::vector<std::string> const& sequences, ScoreMatrix const& matrix, std::span<int, NN> indices, BoundedNDarray<int, NN> & M, BoundedNDarray<int, NN> & dirs, int bound){
    if constexpr(N == 0){
        std::array<char, NN> charset{};
        for(int i = 0; i < NN; i++){
            charset[i] = sequences[i][indices[i]];
        }
        bounded_fill_score<NN>(charset, matrix, indices, M, dirs);
    } else {
        for(int i = 0; i < sequences[NN-N].size(); i++){
            if constexpr(NN-N != 0){
                if(std::abs(indices[0] - i) >= bound) continue; //skip rows that are outside the band
            }
            indices[NN-N] = i;
            bounded_exact_alignN<N-1, NN>(sequences, matrix, indices, M, dirs, bound);
        }
    }
}

template<int N>
std::vector<std::string> bounded_traceback(std::vector<std::string> const& sequences, BoundedNDarray<int, N> & dirs){
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

    int dash_count = 0;
    while(std::all_of(res.begin(), res.end(), [](auto&a){return a.size() > 0 && a.back() == '-';})){
        dash_count++;
        for(auto&s : res){
            s.pop_back();
        }
    }

    return res;
}

template<int N>
AlignmentResult bounded_pick_align_algo(std::vector<std::string> const& sequences, ScoreMatrix const& matrix, int bound){
    if constexpr(N == 0){
        assert(false && "Invalid sequence count");
        throw;
    } else {
        if(sequences.size() == N){
            size_t max_size = std::max_element(sequences.begin(),  sequences.end(), [](auto const&a, auto const&b){return a.size() < b.size();})->size();
            BoundedNDarray<int, N> M(max_size, bound);
            M.fill(INT_MIN);
            BoundedNDarray<int, N> dirs(max_size, bound);
            std::array<int, N> indices{};
            bounded_exact_alignN<N, N>(sequences, matrix, indices, M, dirs, bound);
            auto alignment = bounded_traceback(sequences, dirs);
            return AlignmentResult{.score=M[indices], .alignment=alignment};
        } else {
            return bounded_pick_align_algo<N-1>(sequences, matrix, bound);
        }
    }
}

AlignmentResult bounded_exact_align(std::vector<std::string> const& sequences, ScoreMatrix const& matrix, int bound){
    auto seqs = sequences;
    for(auto&s : seqs){
        s.insert(s.begin(), '-');
    }
    //pad strings so end of alignment is within band
    size_t max_size = std::max_element(seqs.begin(),  seqs.end(), [](auto const&a, auto const&b){return a.size() < b.size();})->size();
    for(auto&s : seqs){
        while(s.size() < max_size){
            s.push_back('-');
        }
    }

    return bounded_pick_align_algo<10>(seqs, matrix, bound);
}