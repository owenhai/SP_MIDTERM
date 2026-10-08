#include "cmp.h"

static Options g_opts;

void set_cmp_options(Options opts) {
    g_opts = opts;
}

int cmp_entries(const void *a, const void *b) {
    const FileInfo *fa = (const FileInfo *)a;
    const FileInfo *fb = (const FileInfo *)b;
    int res = 0;

    if (g_opts.sort_time) {
        if (fb->st.st_mtime > fa->st.st_mtime) {
            res = 1;
        } else if (fb->st.st_mtime < fa->st.st_mtime) {
            res = -1;
        } else {
            res = strcmp(fa->name, fb->name);
        }
    } else {
        res = strcmp(fa->name, fb->name);
    }

    if (g_opts.reverse) {
        res = -res;
    }
    return res;
}
