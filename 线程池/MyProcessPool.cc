#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define Max 4

typedef void (*task_t)(); // task_t 函数指针

void Task1(){ std::cout << "亚瑟使用一技能！" << std::endl; }

void Task2(){ std::cout << "亚瑟使用二技能！" << std::endl; }

void Task3(){ std::cout << "亚瑟使用三技能！" << std::endl; }

void Task4(){ std::cout << "亚瑟使用闪现！" << std::endl; }

task_t tasks[Max];

void LoadTask(){
    srand(time(0) & getpid() & 7273462);

    tasks[0] = Task1;
    tasks[1] = Task2;
    tasks[2] = Task3;
    tasks[3] = Task4;
}

class Channel{
public:
    Channel(int wfd, pid_t _processid, std::string name)
        : _wfd(wfd)
        , _processid(_processid)
        , _name(name)
    {}

    int GetFd(){ return _wfd; }
    pid_t GetPid() { return _processid; }
    std::string GetName() { return _name; }

    void ClosePipe(){ close(_wfd); }
    void wait(){
        int rid = waitpid(_processid, 0, 0);
        if(rid > 0) { std::cout << "wait " << _processid << " success!" << std::endl; }
    }

private:
    int _wfd;
    pid_t _processid;
    std::string _name;
};

void task(int rfd){
    while(1){
        int taskid = 0;
        ssize_t n = read(rfd, &taskid, sizeof(taskid));

        if(n == sizeof(int)){
            tasks[taskid]();
        }
        else{
            std::cout << "child process " << getpid() << " is quited!" << std::endl;
            break;
        }
    }
}

void CreateChannelAndProcess(int ProNum, std::vector<Channel> &channels){
    for(int i = 0; i < ProNum; i++){
        // 创建管道
        int pipefd[2];
        int rp = pipe(pipefd);
        if(rp < 0) { exit(-1); };

        int id = fork();
        if(id < 0) { exit(-1); }
        // 子进程执行逻辑
        else if(id == 0){
            // 子进程关闭写端
            close(pipefd[1]);
            // 子进程执行逻辑
            task(pipefd[0]);
            // 关闭并退出
            close(pipefd[0]);
            exit(0);
        }        

        // 父进程关闭读端
        close(pipefd[0]);

        // 记录该管道信息
        std::string name = "channel_";
        name += std::to_string(i);
        channels.push_back({pipefd[1], id, name});
    }
}

int NextChannel(int channelsize){
    static int id = 0;
    int channel = id;

    id++;
    id %= channelsize;

    return channel;
}

void SendTaskCommend(int taskid, int wfd){
    write(wfd, &taskid, sizeof(taskid));
}

void WorksForProcess(std::vector<Channel> &channels, int times){
    while(times--){
        // 选择任务
        int taskid = rand() % Max;

        // 选择进程
        int processid = NextChannel(channels.size());

        // 发送任务
        SendTaskCommend(taskid, channels[processid].GetFd());
        std::cout << "The parent process give " << channels[processid].GetName() << " task " << taskid << std::endl;
        std::cout << std::endl;

        sleep(1);
    }
}

void CollectPipeAndProcess(std::vector<Channel> &channels){
    for(auto &channel : channels){
        channel.ClosePipe();
    }

    for(auto &channel : channels){
        channel.wait();
    }
}

int main(int argc, char *argv[]){
    
    if(argc != 2){
        std::cerr << "Usage " << argv[0] << " processnum" << std::endl;
    }
    
    int ProNum = std::stoi(argv[1]);
    
    // 加载任务
    LoadTask();

    // 管理所有管道信息
    std::vector<Channel> channels;

    // 创建子进程和管道执行任务
    CreateChannelAndProcess(ProNum, channels); 

    // 为子进程发配任务
    WorksForProcess(channels, 5);

    // 回收管道和子进程
    CollectPipeAndProcess(channels);

    return 0;
}