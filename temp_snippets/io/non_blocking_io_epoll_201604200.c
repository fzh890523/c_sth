#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>

#define MAXEVENTS 64

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

  int ep_fd = epoll_create1(0);
  if(ep_fd == -1) {
    perror("epoll_create1");
    exit(1);
  }

  struct epoll_event ev;
  struct epoll_event *events;
  ev.data.fd = fd;
  ev.events = EPOLLIN | EPOLLET;
  int s = epoll_ctl(ep_fd, EPOLL_CTL_ADD, fd, &ev);
  if(s == -1) {
    // 可能会EEXIST，但此处不会
    perror("epoll_ctl");
    exit(1);
  }
  /*
  对于regular file，这里会抛 EPERM，表示target file does not support epoll_ctl，囧。
  I'm going to guess that doc is a regular file. Regular files are always ready for read(2) or write(2) operations, thus it doesn't make sense to epoll(7) or select(2) on regular files.

if the file has no poll() interface.
1373    // The target file descriptor must support poll
1374        error = -EPERM;
1375        if (!tfile->f_op || !tfile->f_op->poll)
1376                goto error_tgt_fput;
1377

昨天看到TLPI书上的这个结论时，有些奇怪，为啥epoll不能作用于regular file呢？之前用select和poll的时候也没有这条限制啊。今天来了经过google学习，终于大概搞懂了。其实select和poll也是“不支持”对regular file进行监控的，只不过它们被设计为可以接受regular file的fd，只是默认对任何event都全部返回True。epoll在设计的时候，考虑到既然对regular file‘s fd进行polling是没意义的，干脆我就不接受这种类型的fd，所以如果你传入一个真实文件的fd给epoll，它会直接报错返回-1，并将errno设置为EPERM错误码。
  */

  events = calloc(MAXEVENTS, sizeof ev);


  char buf[21];
  buf[20] = '\0';

  int read_num;

  while(1){
    int n, i;
    n = epoll_wait(ep_fd, events, MAXEVENTS, -1);  // timeout = -1 means block

    for(i=0; i<n; i++){
      if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))){
        fprintf(stderr, "epoll error\n");
        close(events[i].data.fd);
        continue;
      }else{
        int done = 0;
        while(1){
          read_num = read(events[i].data.fd, buf, 20);
          if(read_num == -1){
            // #yonka# 从网上看到的，好像还是需要判断是否为 EAGAIN。
            // 因为期间可能发生一些事情（比如被其他...读走）导致又变为not ready
            if(errno != EAGAIN){
              perror("read");
              done = 1;
            }
            break;
          } else if(read_num == 0){
            done = 1;
            break;
          } else {
            buf[read_num] = '\0';
            int res = printf("%s", buf);
            if(res == -1){
              perror("printf");
              exit(1);
            }
          }
        }
        if(done){
          close(events[i].data.fd);
        }
      }
    }

  }

  exit(EXIT_SUCCESS);
}
