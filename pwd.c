# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>

ino_t get_inode(char *);
void print_path_to(ino_t);
void inum_to_name(ino_t, char *, int);

int main() {
    print_path_to(get_inode("."));
    putchar('\n');
    return 0;
}

void print_path_to(ino_t this_inode) {
    /*
     * prints path name recursively
     */
    ino_t my_inode;
    char its_name[BUFSIZ];
    if (get_inode("..") != this_inode) {
        chdir("..");
        inum_to_name(this_inode, its_name, BUFSIZ);
        my_inode = get_inode(".");
        print_path_to(my_inode);
        printf("/%s", its_name);
    }
}

void inum_to_name(ino_t inode_to_find, char * namebuf, int buflen) {
    /*
     * searches a inode in curent dir and get the filename
     */
    DIR *dir_ptr;
    struct dirent *direntp;
    dir_ptr = opendir(".");
    if (dir_ptr == NULL) {
        perror(".");
        exit(1);
    }
    while ((direntp = readdir(dir_ptr)) != NULL) {
        if (direntp->d_ino == inode_to_find) {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_ptr);
            return;
        }
    }
    fprintf(stderr, "error looking for inum %s\n", inode_to_find);
    exit(1);
}

ino_t get_inode(char * fname) {
    /*
     * returns inode number of the file
     */
    struct stat info;
    if (stat(fname, &info) == -1) {
        fprintf(stderr, "Cannot stat ");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}
