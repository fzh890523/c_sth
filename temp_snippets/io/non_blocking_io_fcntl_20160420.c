#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int ac, char * av[])
{
  if(ac != 2){
    printf("ac %d is not 2\n", ac);
    exit(1);
  }

  printf("av[0]is %s, av[1] is %s\n", av[0], av[1]);

  int fd;
  if((fd = open(av[1], O_RDONLY)) == -1){
    printf("open file %s failed\n", av[1]);
  }

  int flags = fcntl(fd, F_GETFL);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);  // but for normal file, O_NONBLOCK makes no sense(is ignored)

  char buf[21];
  buf[20] = '\0';

  int read_num;

  // while(true) {
  while(1) {  // c没有true，噗...
    read_num = read(fd, buf, 20);
    if(read_num == -1) {
      if(errno == EAGAIN) {
        sleep(1);
        continue;
      }
      printf("read from fd met ERROR: %d\n", errno);
      exit(1);
    } else if(read_num == 0){
      printf("read from fd met EOF\n");
      exit(1);
    }
    buf[read_num] = '\0';
    printf("%s", buf);
  }
}
