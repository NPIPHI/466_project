//
// Created by 16182 on 11/27/2023.
//

#ifndef ALGO_PROJECT_ALGO_H
#define ALGO_PROJECT_ALGO_H

#include<cassert>
#include<span>

struct ScoreMatrix {
    int scores[256][256]{};

    ScoreMatrix() {
        for(int i = 0; i < 256; i++){
            for(int j = 0; j < 256; j++){
                scores[i][j] = i == j ? 1 : -1;
            }
        }
        scores['-']['-'] = 0;
    }

    [[nodiscard]] int score(char a, char b) const {
        return scores[a][b];
    }
};

template<int N>
inline int score(ScoreMatrix const& m, std::span<char, N> chars){
    int score = 0;
    for(int i = 0; i < N; i++){
        for(int j = i + 1; j < N; j++){
            score += m.score(chars[i], chars[j]);
        }
    }
    return score;
}
inline int score_alignment(ScoreMatrix const& m, std::vector<std::string> const& alignment){
    int score = 0;
    for(int c = 0; c < alignment[0].size(); c++){
        for(int i = 0; i < alignment.size(); i++){
            for(int j = i + 1; j < alignment.size(); j++){
                score += m.score(alignment[i][c], alignment[j][c]);
            }
        }
    }
    return score;
}

#include<iostream>
struct AlignmentResult {
    int score;
    std::vector<std::string> alignment;
    void print(){
        for(const auto&s : alignment){
            std::cout << s << '\n';
        }
        std::cout << "score: " << score << std::endl;
    }
};

constexpr int pow(int b, int e){
    int res = 1;
    for(int i = 0; i < e; i++){
        res *= b;
    }
    return res;
}

template<typename T, int N>
struct NDarray {
    T* backing;
    int len;
    explicit NDarray(int len): len(len){
        backing = new T[pow(len, N)];
    }

    T& at(int i, int j){
        static_assert(N == 2);
        return backing[i + j * len];
    }

    T& operator[](std::span<int, N> idx){
        int off = 0;
        int pow = 1;
        for(int i = 0; i < N; i++){
            off += pow * idx[i];
            pow *= len;
        }
        return backing[off];
    }

    void fill(T value){
        std::fill(backing, backing + pow(len, N), value);
    }
};

template<typename T, int N>
struct BoundedNDarray {
    T* backing;
    int len;
    int step_size;
    int bound_radius;
    explicit BoundedNDarray(int len, int bound_radius): len(len), step_size(pow(bound_radius*2+1, N-1)), bound_radius(bound_radius){
        backing = new T[len * step_size];
    }

    T& operator[](std::span<const int, N> idx){
        int off = idx[0] * step_size;
        int pow = 1;
        for(int i = 1; i < N; i++){
            assert(std::abs(idx[i] - idx[0]) <= bound_radius);
            off += pow * ((idx[i] - idx[0]) + bound_radius);
            pow *= (bound_radius*2+1);
        }
        assert(off >= 0 && off < len * step_size);
        return backing[off];
    }

    void fill(T value){
        std::fill(backing, backing + len * step_size, value);
    }
};

#endif //ALGO_PROJECT_ALGO_H
