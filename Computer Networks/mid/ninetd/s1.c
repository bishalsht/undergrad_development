#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
int main(){
    char buff[1024];
    for (; ; ) {
        int n=read(0,buff,1024);
        buff[n]='.';
        write(1,buff,n+1);
        if (buff[0]=='.') {
            break;
        }
    }
    
    int a=open("done",O_WRONLY);
    write(a,"1",1);
    close(a);
    return 0;
}