# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int grep(char *regexp, FILE *f, char *name);
int match(char *regexp, char *text);
int matchhere(char *regexp, char *text);
int matchstar(int c, char *regexp, char *text);


int main(int ac, char * av[]) {
    int i, nmatch;
    FILE * f;

    if (ac < 2) {
        fprintf(stderr, "usage: grep regexp [file ...]\n");
        exit(1);
    }
    nmatch = 0;
    if (ac == 2) {
        if (grep(av[1], stdin, NULL)) {
            nmatch++;
        }
    } else {
        for (i = 2; i < ac; i++) {
            f = fopen(av[i], "r");
            if (f == NULL) {
                fprintf(stderr, "cannot open %s\n", av[i]);
                continue;
            }
            if (grep(av[1], f, ac > 3 ? av[i] : NULL) > 0) {
                nmatch++;
            }
            fclose(f);
        }
    }
    return nmatch == 0;
}


int grep(char *regexp, FILE *f, char *name) {
    int n, nmatch;
    char buf[BUFSIZ];
    nmatch = 0;
    while (fgets(buf, sizeof buf, f) != NULL) {
        n = strlen(buf);
        if (n > 0 && buf[n-1] == '\n') {
            buf[n-1] = '\0';
        }
        if (match(regexp, buf)) {
            nmatch++;
            if (name != NULL) {
                printf("%s: ", name);
            }
            printf("%s\n", buf);
        }
    }
    return nmatch;
}


/* match: search for regexp anywhere in text */
int match(char *regexp, char *text) {
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}


/* matchhere: search for regexp at beginning of text */
int matchhere(char *regexp, char *text) {
    if (regexp[0] == '\0')
        return 1;
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
        return matchhere(regexp+1, text+1);
    return 0;
}


/* matchstar: search for c*regexp at beginning of text */
int matchstar(int c, char *regexp, char *text) {
    do {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}
