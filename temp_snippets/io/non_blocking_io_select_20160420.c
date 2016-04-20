#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

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

  char buf[21];
  buf[20] = '\0';

  int read_num;

  while(1){
    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd, &set);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;
    int rv;

    rv = select(fd + 1, &set, NULL, NULL, &timeout);  // 第一个参数为 int nfds，表示需要监视的fd的范围（减小底层的工作量）（[0, nfds)），所以需要为 MAX(fds) + 1

    if (rv == -1){
      perror("select()");
      exit(1);
    } else if (rv) {  // 对文件fd，一直ready...
      // printf("Data is available now.\n");
      /* FD_ISSET(0, &rfds) will be true. */
      if(FD_ISSET(fd, &set)) {  // 因为只监视了fd这一个，所以此时本fd肯定ok...
        read_num = read(fd, buf, 20);
        if(read_num == 0){  // EOF
          break;
        } else if(read_num == -1) {
          perror("read error");  // 应该不用再判断errno == EAGAIN了
          exit(1);
        } else {
          buf[read_num] = '\0';
          printf("%s", buf);
        }
      }
    } else {
      // do nothing
      // printf("No data within five seconds.\n");
    }
  }

  exit(EXIT_SUCCESS);
}
