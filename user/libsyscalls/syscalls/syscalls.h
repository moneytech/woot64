#pragma once

#define SYS_read            0
#define SYS_write           1
#define SYS_open            2
#define SYS_close           3
#define SYS_stat            4
#define SYS_fstat           5
#define SYS_lseek           8
#define SYS_mmap            9
#define SYS_munmap          11
#define SYS_brk             12
#define SYS_readv           19
#define SYS_writev          20
#define SYS_exit            60
#define SYS_arch_prctl      158
#define SYS_set_tid_address 218
#define SYS_exit_group      231

typedef __SIZE_TYPE__ size_t;
typedef __INTPTR_TYPE__ off_t;
typedef __UINTPTR_TYPE__ uintptr_t;

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

size_t sys_read(int fd, char *buf, size_t count);
size_t sys_write(int fd, const char *buf, size_t count);
int sys_open(const char *filename, int flags, int mode);
int sys_close(int fd);
int sys_stat(const char *filename, void *statbuf);
int sys_fstat(int fd, void *statbuf);
off_t sys_lseek(int fd, off_t offset, unsigned int origin);
void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
int sys_munmap(void *addr, size_t len);
void *sys_brk(void *brk);
long sys_readv(int fd, const void *vec, size_t vlen);
long sys_writev(int fd, const void *vec, size_t vlen);
long sys_exit(long error_code);
int sys_arch_prctl(int code, uintptr_t addr);
int sys_set_tid_address(int *tidptr);
long sys_exit_group(long error_code);

#ifdef __cplusplus
}
#endif // __cplusplus
