#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include "OLED.h"

// 重定向 printf 到串口
int _write(int fd, char *buf, int size) {return -1;}

// 空实现其他系统调用
int _close(int fd) { return -1; }
int _lseek(int fd, int ptr, int dir) { return -1; }
int _read(int fd, char *ptr, int len) { return -1; }
int _isatty(int fd) { return 1; }  // 告知标准库当前是终端设备
int _kill(int pid, int sig) { return -1; }
int _getpid(void) { return 1; }
void _exit(int status) { while (1); }  // 防止程序退出
int _fstat(int fd, struct stat *st) { return -1; }

extern uint8_t _end[];
extern uint8_t _estack[];
static uint8_t *heap_end = _end;

void *_sbrk(intptr_t increment) {
    uint8_t *prev_heap_end = heap_end;

    if (heap_end + increment > _estack) {
        errno = ENOMEM;
        return (void *)-1;
    }

    heap_end += increment;
    return (void *)prev_heap_end;
}
