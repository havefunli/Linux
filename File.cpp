#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;


int main(){

    umask(0);
    
    close(1);// 关闭显示器文件

    // 写方式打开文件，并且文件不存在就创建一个，文件的权限是 rw-rw-r--
    int fd = open("log.txt", O_WRONLY | O_CREAT, 0666);    
    if(fd == -1) perror("open");
    printf("fd = %d.\n", fd);        
    

    return 0;
}


//int main(){
//
//    const char* msg = "Its a test!!!\n";
//    ssize_t ret = write(1, msg, strlen(msg));
//
//    return 0;
//}
//
















//int main(){
//    int fd1 = open("log1.txt", O_WRONLY | O_CREAT, 0666);    
//    printf("fd1: %d.\n", fd1);
//
//    int fd2 = open("log2.txt", O_WRONLY | O_CREAT, 0666);    
//    printf("fd2: %d.\n", fd2);
//    
//    int fd3 = open("log3.txt", O_WRONLY | O_CREAT, 0666);    
//    printf("fd3: %d.\n", fd3);
//    
//    int fd4 = open("log4.txt", O_WRONLY | O_CREAT, 0666);    
//    printf("fd4: %d.\n", fd4);
//    return 0;
//}














//int main(){
//    umask(0);
//    
//    // 写方式打开文件，并且文件不存在就创建一个，文件的权限是 rw-rw-r--
//    int fd = open("log.txt", O_WRONLY | O_CREAT, 0666);    
//    if(fd == -1) perror("open");
//    
//    // 写入指定信息
//    const char* msg = "Its a test!!!\n";
//    ssize_t ret = write(fd, msg, strlen(msg));
//    if(ret == -1) perror("write");
//    
//    close(fd); // 关闭文件
//
//    return 0;
//}

































#define FLAG1 1 // 0000 0001
#define FLAG2 2 // 0000 0010
#define FLAG3 4 // 0000 0100


//int Func(int num, int flags){
//    if(flags & FLAG1){
//        num = num + 1;
//    }
//    if(flags & FLAG2){
//        num = num - 1;
//    }
//    if(flags & FLAG3){
//        num = num * 2;
//    }
//
//    return num;
//}
//
//
//int main(){
//    int num = 10;
//    
//    num = Func(num, FLAG1 | FLAG3);
//    cout << "num = " << num << endl;
//
//    return 0;
//}

