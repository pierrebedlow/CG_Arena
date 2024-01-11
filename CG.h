#pragma once
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <vector>
#include <cmath>
#include <array>
#include <fstream>
#include <random>
#include <chrono>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <poll.h>
//#include <omp.h>
#include <algorithm>
#include <thread>
//using namespace std;
//using namespace std::chrono;


// Number of Bots in game
constexpr int N = 2;

struct GameOutput {

 int turns;


};  


class GameReferee {

public:
    GameOutput Play_Game(std::array<std::string,N> Bot_Names) {
        
        GameOutput output;

        output.turns = 0;
        
        return output;
    };

    bool IsValidMove(const std::string &Move){
        std::stringstream ss(Move);
        /*
        for(int i=0;i<2;++i){
            string line;
            getline(ss,line);
            stringstream ss2(line);
            action PodMove;
            if(!(ss2 >> PodMove.t >> PodMove.thrust)){
                return false;
            }
        }
        */
        return true;
    }

};