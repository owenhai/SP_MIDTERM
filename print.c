#include "print.h"

static void format_mode(mode_t m, char *str) {
    if (S_ISDIR(m)) str[0] = 'd';
    else if (S_ISLNK(m)) str[0] = 'l';
    else if (S_ISCHR(m)) str[0] = 'c';
    else if (S_ISBLK(m)) str[0] = 'b';
    else if (S_ISFIFO(m)) str[0] = 'p';
    else if (S_ISSOCK(m)) str[0] = 's';
    else str[0] = '-';

    str[1] = (m & S_IRUSR) ? 'r' : '-';
    str[2] = (m & S_IWUSR) ? 'w' : '-';
    str[3] = (m & S_IXUSR) ? 'x' : '-';
    str[4] = (m & S_IRGRP) ? 'r' : '-';
    str[5] = (m & S_IWGRP) ? 'w' : '-';
    str[6] = (m & S_IXGRP) ? 'x' : '-';
    str[7] = (m & S_IROTH) ? 'r' : '-';
    str[8] = (m & S_IWOTH) ? 'w' : '-';
    str[9] = (m & S_IXOTH) ? 'x' : '-';
    str[10] = '\0';
}

void print_entries(FileInfo *files, size_t count, Options opts) {
    for (size_t i = 0; i < count; i++) {
        if (!opts.long_fmt) {
            printf("%s\n", files[i].name);
        } else {
            char perm[11];
            format_mode(files[i].st.st_mode, perm);
            struct passwd *pw = getpwuid(files[i].st.st_uid);
            struct group *gr = getgrgid(files[i].st.st_gid);

            char date[32];
            struct tm *tm_info = localtime(&files[i].st.st_mtime);
            strftime(date, sizeof(date), "%b %e %H:%M", tm_info);

            printf("%s %2ld ", perm, (long)files[i].st.st_nlink);
            if (pw) printf("%-8s ", pw->pw_name);
            else printf("%-8d ", files[i].st.st_uid);

            if (gr) printf("%-8s ", gr->gr_name);
            else printf("%-8d ", files[i].st.st_gid);

            printf("%8ld %s %s\n", (long)files[i].st.st_size, date, files[i].name);
        }
    }
}
