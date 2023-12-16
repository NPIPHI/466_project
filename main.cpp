#include <iostream>
#include "exact.h"
#include "bound_exact.h"
#include "Synthetic.h"
#include "star.h"
#include "tree.h"
#include "mem.h"
#include<chrono>

int main(int argc, char** argv) {
    if(argc != 6){
        std::cerr << "wrong argc\n";
        return 1;
    }
    std::string algo = argv[1];
    int count = std::stoi(argv[2]);
    int len = std::stoi(argv[3]);
    double insert = std::stod(argv[4]);
    double mutate = std::stod(argv[5]);
    auto seqs = synth_data(count, len, insert, mutate);

    ScoreMatrix mat;
    auto t1 = std::chrono::steady_clock::now();
    AlignmentResult res;
    if(algo == "exact"){
        res = exact_align(seqs, mat);
    }
    if(algo == "bound"){
        res = bounded_exact_align(seqs, mat, len / 40 + 2);
    }
    if(algo == "star"){
        res = star_align(seqs, mat);
    }
    if(algo == "tree"){
        res = tree_align(seqs, mat);
    }
    auto t2 = std::chrono::steady_clock::now();

    std::cout << res.score << '\n';
    std::cout << (t2-t1)/std::chrono::microseconds (1) << '\n';
    std::cout << get_mem() << '\n';
    return 0;
}
