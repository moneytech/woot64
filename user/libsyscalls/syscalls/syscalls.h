#pragma once

#define SYS_read            0
#define SYS_write           1
#define SYS_open            2
#define SYS_close           3
#define SYS_exit            60

#define SYS_EXIT_THREAD     0x200
#define SYS_EXIT_PROCESS    0x201

typedef __SIZE_TYPE__ size_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

static __inline long __syscall0(long n)
{
	unsigned long ret;
    asm volatile("syscall": "=a"(ret) :"a"(n) :"rcx", "r11", "memory");
    return ret;
}

static __inline long __syscall1(long n, long a1)
{
	unsigned long ret;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall2(long n, long a1, long a2)
{
	unsigned long ret;
	asm volatile("syscall": "=a"(ret) :"a"(n), "D"(a1), "S"(a2): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall3(long n, long a1, long a2, long a3)
{
	unsigned long ret;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1), "S"(a2), "d"(a3): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall4(long n, long a1, long a2, long a3, long a4)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10):"rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall5(long n, long a1, long a2, long a3, long a4, long a5)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8): "rcx", "r11", "memory");
	return ret;
}

static __inline long __syscall6(long n, long a1, long a2, long a3, long a4, long a5, long a6)
{
	unsigned long ret;
	register long r10 __asm__("r10") = a4;
	register long r8 __asm__("r8") = a5;
	register long r9 __asm__("r9") = a6;
	asm volatile("syscall": "=a"(ret): "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r10), "r"(r8), "r"(r9): "rcx", "r11", "memory");
	return ret;
}

size_t sys_read(unsigned int fd, char *buf, size_t count);
size_t sys_write(unsigned int fd, const char *buf, size_t count);
int sys_open(const char *filename, int flags, int mode);
int sys_close(unsigned int fd);
long sys_exit(long error_code);

void sysExitThread(long result);
void sysExitProcess(long result);

#ifdef __cplusplus
}
#endif // __cplusplus
