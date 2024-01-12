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
#include <omp.h>
#include <algorithm>
#include <thread>
//using namespace std;
//using namespace std::chrono;


// Number of Bots in game
constexpr int N = 2;

struct GameOutput {

    int turns;
    int errorCode;

    GameOutput() : turns(0), errorCode(1) {};

};  


class ArenaReferee {

public:
    GameOutput Play_Game(std::array<std::string,N> Bot_Names) {
        
        GameOutput output;

        map C=Generate_Map();
        vector<AI> Bot(Bot_Names.size());

        for(int i=0;i<Bot_Names.size();++i) {
            Bot[i].id=i;
            Bot[i].name=Bot_Names[i];
            StartProcess(Bot[i]);
            Bot[i].p.r=C[0];
            Bot[i].p.v=vec{0,0};
            Bot[i].p.lap=0;
            Bot[i].p.next=1;
            Bot[i].p.angle=Angle(C[1]-C[0]);//Could do better?
            Bot[i].p.time=100;
        }

        int turn{0};
        //Feed first turn inputs
        for (AI &b:Bot) {
            stringstream ss;
            ss << N_L << endl << C.size() << endl;
            for (const vec{
                ss << cp << endl;
            }
            b.Feed_Inputs(ss.str());
        }
        while(++turn>0 && !stop){
            for (int i=0;i<Bot.size();++i) {
                if (Bot[i].alive()){
                    //Feed turn inputs
                    stringstream ss;
                    for(int j=0;j<2;++j) {//Duplicate pod info
                        ss << Bot[i].p.r << " " << round(Bot[i].p.v.x) << " " << round(Bot[i].p.v.y) << " " << round(Bot[i].p.angle) << " " << Bot[i].p.next << endl;
                    }
                    for(int j=0;j<2;++j) {//Feed fake far away opponent position
                        ss << -10000+1000*j << " " << -10000 << " " << 0 << " " << 0 << " " << 0 << " " << 1 << endl;
                    }
                    try {
                        Bot[i].Feed_Inputs(ss.str());
                        string Move{GetMove(Bot[i],turn)};
                        string err_str{EmptyPipe(Bot[i].errPipe)};
                        if (Debug_AI){
                            ofstream err_out("log.txt",ios::app);
                            err_out << err_str << endl;
                        }
                        Make_Move(Move,Bot[i],C,turn);
                        if (Bot[i].p.lap==N_L){
                            output.turn = turn;
                        }
                        else if (Bot[i].p.time==0){
                            output.errorCode = -2;
                        }
                    }
                    catch(...){
                        std::cerr << "Loss by timeout" << std::endl;
                        output.errorCode = -1;
                    }
                }
                else {
                    output.errorCode = -2;
                }
            }
        }
        output.errorCode = -2;
        
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