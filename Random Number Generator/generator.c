#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main(){
    int ret, fd;
    char result[5];
    fd = open("/dev/numGenerator", O_RDWR);
    if (fd < 0)
    {
        perror("Failed to open device !!\n");
        return errno;
    }
    ret = read(fd, result, 1);
    if (ret < 0)
    {
        perror("Failed to read from device !!\n");
        return errno;
    }
    printf("\033[0;36m"); // Color for output
    printf("Your random number is '", result[0]);
    printf("\033[1;35m");
    printf("%u", result[0]);
    printf("\033[0;36m");
    printf("'\n*** Finished ***\n");
    printf("\033[0m");
    return 0;
}