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

#include "CG.h"

constexpr int PIPE_READ{0},PIPE_WRITE{1};
constexpr bool Debug_AI{false},Timeout{false};
constexpr double FirstTurnTime{1*(Timeout?1:10)},TimeLimit{0.15*(Timeout?1:10)};
bool stop{false};//Global flag to stop all arena threads when SIGTERM is received


struct AI{

    int id,pid,outPipe,errPipe,inPipe;
    std::string name;
    
    //pod p;
    inline void stop(){
        if(alive()){
            kill(pid,SIGTERM);
            int status;
            waitpid(pid,&status,0);//It is necessary to read the exit code for the process to stop
            if(!WIFEXITED(status)){//If not exited normally try to "kill -9" the process
                kill(pid,SIGKILL);
            }
        }
    }
    inline bool alive()const{
        return kill(pid,0)!=-1;//Check if process is still running
    }
    inline void Feed_Inputs(const std::string &inputs){
        if(write(inPipe,&inputs[0],inputs.size())!=inputs.size()){
            throw(5);
        }
    }
    inline ~AI(){
        close(errPipe);
        close(outPipe);
        close(inPipe);
        stop();
    }
};

void StartProcess(AI &Bot){
    int StdinPipe[2];
    int StdoutPipe[2];
    int StderrPipe[2];
    if(pipe(StdinPipe)<0){
        perror("allocating pipe for child input redirect");
    }
    if(pipe(StdoutPipe)<0){
        close(StdinPipe[PIPE_READ]);
        close(StdinPipe[PIPE_WRITE]);
        perror("allocating pipe for child output redirect");
    }
    if(pipe(StderrPipe)<0){
        close(StderrPipe[PIPE_READ]);
        close(StderrPipe[PIPE_WRITE]);
        perror("allocating pipe for child stderr redirect failed");
    }
    int nchild{fork()};
    if(nchild==0){//Child process
        if(dup2(StdinPipe[PIPE_READ],STDIN_FILENO)==-1){// redirect stdin
            perror("redirecting stdin");
            return;
        }
        if(dup2(StdoutPipe[PIPE_WRITE],STDOUT_FILENO)==-1){// redirect stdout
            perror("redirecting stdout");
            return;
        }
        if(dup2(StderrPipe[PIPE_WRITE],STDERR_FILENO)==-1){// redirect stderr
            perror("redirecting stderr");
            return;
        }
        close(StdinPipe[PIPE_READ]);
        close(StdinPipe[PIPE_WRITE]);
        close(StdoutPipe[PIPE_READ]);
        close(StdoutPipe[PIPE_WRITE]);
        close(StderrPipe[PIPE_READ]);
        close(StderrPipe[PIPE_WRITE]);
        execl(Bot.name.c_str(),Bot.name.c_str(),(char*)NULL);//(char*)Null is really important
        //If you get past the previous line its an error
        perror("exec of the child process");
    }
    else if(nchild>0){//Parent process
        close(StdinPipe[PIPE_READ]);//Parent does not read from stdin of child
        close(StdoutPipe[PIPE_WRITE]);//Parent does not write to stdout of child
        close(StderrPipe[PIPE_WRITE]);//Parent does not write to stderr of child
        Bot.inPipe=StdinPipe[PIPE_WRITE];
        Bot.outPipe=StdoutPipe[PIPE_READ];
        Bot.errPipe=StderrPipe[PIPE_READ];
        Bot.pid=nchild;
    }
    else{//failed to create child
        close(StdinPipe[PIPE_READ]);
        close(StdinPipe[PIPE_WRITE]);
        close(StdoutPipe[PIPE_READ]);
        close(StdoutPipe[PIPE_WRITE]);
        perror("Failed to create child process");
    }
}

inline std::string EmptyPipe(const int fd){
    int nbytes;
    if(ioctl(fd,FIONREAD,&nbytes)<0){
        throw(4);
    }
    std::string out;
    out.resize(nbytes);
    if(read(fd,&out[0],nbytes)<0){
        throw(4);
    }
    return out;
}


std::string GetMove(Game game, AI &Bot,const int turn){
    
    pollfd outpoll{Bot.outPipe,POLLIN};
    std::chrono::time_point<std::chrono::system_clock> Start_Time{std::chrono::system_clock::now()};
    std::string out;

    while(static_cast<std::chrono::duration<double>>(std::chrono::system_clock::now()-Start_Time).count()<(turn==1?FirstTurnTime:TimeLimit)){
        double TimeLeft{(turn==1?FirstTurnTime:TimeLimit)-static_cast<std::chrono::duration<double>>(std::chrono::system_clock::now()-Start_Time).count()};
        if(poll(&outpoll,1,TimeLeft)){
            out+=EmptyPipe(Bot.outPipe);
            if(game.IsValidMove(out)){
                return out;
            }
        }
    }
    throw(1);
}

void StopArena(const int signum){
    stop=true;
}


int main(int argc,char **argv){
    if(argc<2){
        std::cerr << "Program takes 1 input, the names of the AI to test" << std::endl;
        return 0;
    }
    int N_Threads{1};
    if(argc>=3){//Optional N_Threads parameter
        N_Threads=std::min(2*omp_get_num_procs(),std::max(1,atoi(argv[2])));
        std::cerr << "Running " << N_Threads << " arena threads" << std::endl;
    }

    std::array<std::string,N> Bot_Names;
    Game game;

    for(int i=0;i<N;++i){
        Bot_Names[i]=argv[i+1];
    }
    std::cout << "Testing AI " << Bot_Names[0] << std::endl;
    for(int i=0;i<Bot_Names.size();++i){
        std::ifstream Test{Bot_Names[i].c_str()};
        if(!Test){
            std::cerr << Bot_Names[i] << " couldn't be found" << std::endl;
            return 0;
        }
        Test.close();
    }
    signal(SIGTERM,StopArena);//Register SIGTERM signal handler so the arena can cleanup when you kill it
    signal(SIGPIPE,SIG_IGN);//Ignore SIGPIPE to avoid the arena crashing when an AI crashes
    int maps_done{0},maps_failed{0};
    double average_turns{0};

    #pragma omp parallel num_threads(N_Threads) shared(average_turns,maps_done,Bot_Names)
    
    while(!stop){
        
        GameOutput go{game.Play_Game(Bot_Names)};

        int turns{go.turns};
        if(turns>0){
            #pragma omp atomic
            average_turns+=(turns-average_turns)/(++maps_done);
        }
        else{
            ++maps_failed;
            //cerr << "Bot didn't complete map" << endl;
        }
        #pragma omp critical
        std::cerr << "Average turns: " << std::setprecision(5) << average_turns << " with " << maps_done << " maps done with " << std::setprecision(2) << 100.0*maps_failed/(maps_failed+maps_done) << "% maps failed" << std::endl;
    }
}
