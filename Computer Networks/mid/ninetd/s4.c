#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
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
    return 0;
}