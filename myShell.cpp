#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>
#include <iostream>

using namespace std;

#define NUM 256
#define SEP " "

char *UsrCommand[NUM];
char Cwd[NUM];

string GetCwd(){
    string cwd = getenv("PWD"); // 获取当前工作目录 
    size_t index = cwd.find_last_of('/');
    cwd = cwd.substr(index + 1, cwd.size());

    return cwd.size() == 0 ? "/" : cwd;
}

void MakeCommandLine(){
    string usrname = getenv("USER"); // 获取用户名
    string hostname = getenv("HOSTNAME"); // 获取主机名
    
    string cwd = GetCwd(); // 获取当前工作目录 
    // 打印格式刷新缓冲区
    printf("[%s@%s %s]>> ", usrname.c_str(), hostname.c_str(), cwd.c_str());
    fflush(stdout);
}

void GetUsrCommand(char CommandLine[]){
    // 获取用户的输入指令
    fgets(CommandLine, NUM, stdin);
    // 去除读入的 \0
    CommandLine[strlen(CommandLine) - 1] = '\0';
}

void SplitCommandLine(char CommandLine[]){
    int index = 0;
    // 利用 strtok 函数依次取出指令格式
    UsrCommand[index++] = strtok(CommandLine, SEP);
    while((UsrCommand[index++] = strtok(NULL, SEP)));
}

bool CheckBuilin(){
    // cd 为自建命令，不会让子进程执行
    if(!strcmp(UsrCommand[0], "cd")){
        return true;
    }
    
    return false;
}

// 返回当前用户的家目录
const char *GetHome(){
    const char *home = getenv("HOME");
    if(home == NULL) return "/";
    return home;
}


void ExecuteTheBuildin(){
    // 没有输入地址的情况
    const char *path = UsrCommand[1];
    if(path == NULL) path = GetHome(); 
    
    // 改变工作地址    
    chdir(path);
    
    // 刷新环境变量
    char NewEnv[NUM];
    getcwd(NewEnv, NUM);
    snprintf(Cwd, NUM, "PWD=%s", NewEnv);
    putenv(Cwd);


    // 下述方法只能获取相对路径，会出错
    // 将现在的地址更新到环境变量中
    //  char NewEnv[NUM];
    //  snprintf(NewEnv, NUM, "PWD=%s", path);
    //  putenv(NewEnv);
} 

void ExecuteTheCommand(){
    // 判断是否为自建命令
    bool flag = CheckBuilin();
    if(flag) {
        ExecuteTheBuildin();
        return;
    }
    
    pid_t pid = fork();
    
    if(pid < 0) cout << "Failed to execute!!!" << endl;
    // child
    else if(pid == 0){
        execvp(UsrCommand[0], UsrCommand);
        // errno 为全局变量，表示最后一次系统调用的错误代码
        exit(errno);
    }
    // parent
    else{
        // 获取退出信息
        int status = 0;
        pid_t rid = waitpid(pid, &status, 0);

        if(rid > 0){
            int ExitedInfo = WEXITSTATUS(status);
            // 代表执行出错
            if(ExitedInfo != 0){
                printf("%s : %s : %d\n", UsrCommand[0], strerror(ExitedInfo), ExitedInfo); // 错误信息
            }       
        }
        else{
            cout << "The child process is failed!" << endl;
        }
    }
}


int main(){
    
    while(1){
        // 命令行的显示
        MakeCommandLine();
        
        // 获取命令
        char CommandLine[NUM];
        GetUsrCommand(CommandLine);
        
        // 分割命令
        SplitCommandLine(CommandLine);
        
        // 执行命令
        ExecuteTheCommand();
    }

    return 0;
}
