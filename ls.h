#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200809L

#ifndef LS_H
#define LS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>

typedef struct {
    int all;        /* -a */
    int almost_all; /* -A */
    int long_fmt;   /* -l */
    int sort_time;  /* -t */
    int reverse;    /* -r */
    int dir_only;   /* -d */
} Options;

typedef struct {
    char *name;
    char *path;
    struct stat st;
} FileInfo;

void process_target(const char *target, Options opts);

#endif
