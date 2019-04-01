#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern int *__errno_location();

#ifdef __cplusplus
}
#endif // __cplusplus

#define errno       (*(__errno_location()))

#define ESUCCESS    0
#define EBUSY       1
#define EIO         2
#define ENOMEM      3
#define EINVAL      4
#define ENOSYS      5
#define ENOTDIR     6
#define ENOENT      7
#define ENOTEMPTY   8
#define EISDIR      9
#define EBADF       10
#define ERANGE      11
