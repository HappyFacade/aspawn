#ifndef  __aspawn_syscall_syscall_H__
# define __aspawn_syscall_syscall_H__

# include <sys/types.h>
# include <fcntl.h>

# define GET_ARG_N(_null, _0, _1, _2, _3, _4, _5, _6, _7, ...) _7
# define GET_NARGS_(...) GET_ARG_N(__VA_ARGS__)
/**
 * GET_NARGS can at most detect nargs up util 7.
 */
# define GET_NARGS(...) GET_NARGS_(99, ## __VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)

/**
 * All psys_* here returns negative error code on failure and does not modify errno.
 *
 * The negative error code is equaivlent to (-errno).
 */

/**
 * Rationale on why syscall takes long:
 *  - https://stackoverflow.com/questions/35628927/what-is-the-type-of-system-call-arguments-on-linux
 */
long pure_syscall(long syscall_number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6);

# define GET_syscall_args(_syscall_num, _1, _2, _3, _4, _5, _6, ...) _syscall_num, _1, _2, _3, _4, _5, _6
/**
 * pure_syscall2 takes at most 6 arguments.
 */
# define pure_syscall2(syscall_number, ...) \
    pure_syscall(GET_syscall_args(syscall_number, ## __VA_ARGS__, 0, 0, 0, 0, 0, 0))

int psys_openat_impl(int dirfd, const char *pathname, int flags, mode_t mode);
/**
 * @return in addition to errors specified in manpage for openat, 
 *         openat would return EINVAL when the presence of mode doesn't match the presence of 
 *         O_CREAT in flags.
 */
#define psys_openat(dirfd, pathname, flags, ... /* mode */) \
    ({                                           \
        int ret;                                 \
        if (GET_NARGS(__VA_ARGS__) == 0) {       \
            if ((flags & O_CREAT) != 0)          \
                ret = (-EINVAL);                 \
            else                                 \
                ret = psys_openat_impl((dirfd), (pathname), (flags), 0); \
        } else {                                 \
            if ((flags & O_CREAT) == 0)          \
                ret = (-EINVAL);                 \
            else                                 \
                ret = psys_openat_impl((dirfd), (pathname), (flags), __VA_ARGS__); \
        }                                        \
        ret;                                     \
     })

int psys_close(int fd);

int psys_dup3(int oldfd, int newfd, int flags);

int psys_chdir(const char *path);
int psys_fchdir(int fd);

ssize_t psys_write(int fd, const void *buf, size_t count);
ssize_t psys_read(int fd, void *buf, size_t count);

/**
 * @param ignore If ignore == 1, ignore the signal. Otherwise set it to default handler.
 */
int psys_sig_set_handler(int signum, int ignore);

#endif
