#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
if(sig==SIGTERM)
printf("\nSIGTERM received!\n");
}
int main(){
signal(SIGTERM,handler);
printf("Process ID: %d\n",getpid());
printf("waiting for sigterm...\n");

while(1){
sleep(2);
}
return 0;
}
