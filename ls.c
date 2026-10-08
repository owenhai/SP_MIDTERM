#include "ls.h"
#include "cmp.h"
#include "print.h"

void process_target(const char *target, Options opts) {
    struct stat st;
    if (lstat(target, &st) == -1) {
        perror(target);
        return;
    }

    if (!S_ISDIR(st.st_mode) || opts.dir_only) {
        FileInfo fi;
        fi.name = (char *)target;
        fi.path = (char *)target;
        fi.st = st;
        print_entries(&fi, 1, opts);
        return;
    }

    DIR *dir = opendir(target);
    if (!dir) {
        perror(target);
        return;
    }

    struct dirent *entry;
    FileInfo *files = NULL;
    size_t count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!opts.all) {
            if (opts.almost_all) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
            } else {
                if (entry->d_name[0] == '.')
                    continue;
            }
        }

        char full_path[1024];
        if (strcmp(target, ".") == 0) {
            snprintf(full_path, sizeof(full_path), "%s", entry->d_name);
        } else {
            snprintf(full_path, sizeof(full_path), "%s/%s", target, entry->d_name);
        }

        struct stat fst;
        if (lstat(full_path, &fst) == -1) {
            continue;
        }

        FileInfo *new_files = realloc(files, (count + 1) * sizeof(FileInfo));
        if (!new_files) {
            free(files);
            closedir(dir);
            return;
        }
        files = new_files;

        files[count].name = strdup(entry->d_name);
        files[count].path = strdup(full_path);
        files[count].st = fst;
        count++;
    }
    closedir(dir);

    set_cmp_options(opts);
    qsort(files, count, sizeof(FileInfo), cmp_entries);
    print_entries(files, count, opts);

    for (size_t i = 0; i < count; i++) {
        free(files[i].name);
        free(files[i].path);
    }
    free(files);
}

int main(int argc, char **argv) {
    Options opts = {0, 0, 0, 0, 0, 0};
    int ch;

    while ((ch = getopt(argc, argv, "Aadrt1l")) != -1) {
        switch (ch) {
            case 'a': opts.all = 1; break;
            case 'A': opts.almost_all = 1; break;
            case 'l': opts.long_fmt = 1; break;
            case 't': opts.sort_time = 1; break;
            case 'r': opts.reverse = 1; break;
            case 'd': opts.dir_only = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-Aadrt1l] [file ...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    argv += optind;
    argc -= optind;

    if (argc == 0) {
        process_target(".", opts);
    } else {
        for (int i = 0; i < argc; i++) {
            if (argc > 1) {
                printf("%s:\n", argv[i]);
            }
            process_target(argv[i], opts);
            if (argc > 1 && i < argc - 1) {
                printf("\n");
            }
        }
    }

    return 0;
}
