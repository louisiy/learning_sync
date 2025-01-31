#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
// #include <wait.h>

void error(char *msg){
    fprintf(stderr,"%s: %s\n",msg,strerror);
    exit(1);
}

int main(int argc, char* argv[]){
    char *phrase = argv[1];
    char *vars[] = {"RSS_FEED=http://www.cnn.com/rss/celebs.xml", NULL};
    FILE *f = fopen("stories.txt","w");
    if(!f){
        error("Can't open stories.txt");
    }
    pid_t pid = fork();
    if (pid == -1) {
        error("Can't fork process");
    }
    if (!pid) {
        if(dup2(fileno(f),1) == -1){
            error("Can't redirect Standard Output");
        }
        if(execlp("/usr/bin/python", "/usr/bin/python","./rssgossip.py",phrase,NULL,vars)){
            error("Can't run script");
        }
    }
    int pid_status;
    if (waitpid(pid, &pid_status, 0) == -1) {
        error("等待子进程时发生了错误");
    }
    return 0;
}