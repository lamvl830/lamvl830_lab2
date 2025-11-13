// PID: 730762728
// I pledge to the COMP211 honor code.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// method to handle memory leaks/out of memory
static void oom(void) {
    fprintf(stderr, "out of memory\n");
    exit(EXIT_FAILURE);
}

// method to read full line into dynamically allocated string, returns ptr to
// line, return NULL when EOF is reached
static char* readline(void) {
    size_t cap = 0;    // current allocated capacity
    size_t len = 0;    // bytes used
    char* buf = NULL;  // ptr to heap buffer

    for (;;) {
        int c = getchar();  // read byte from stdin
        if (c == EOF) {
            // if nothing read, signal EOF, return NULL
            if (len == 0) {
                free(buf);
                return NULL;
            }
            break;
        }
        // grow buffer when full, statr at 128 bytes, double each time
        if (len + 1 >= cap) {
            size_t new_cap = cap ? cap * 2 : 128;
            char* tmp = (char*)realloc(buf, new_cap);
            if (!tmp) {
                free(buf);
                oom();
            }
            buf = tmp;
            cap = new_cap;
        }

        buf[len++] = (char)c;  // apend new char

        if (c == '\n') {
            break;  // stop once newline is read
        }
    }
    // ensure space for null term
    if (len + 1 >= cap) {
        size_t new_cap = cap ? cap + 1 : 1;
        char* tmp = (char*)realloc(buf, new_cap);
        if (!tmp) {
            free(buf);
            oom();
        }
        buf = tmp;
        cap = new_cap;
    }

    // terminate c-string
    buf[len] = '\0';
    return buf;
}

// qsort comparator for array of char pointer
static int cmp_strptr(const void* a, const void* b) {
    const char* sa = *(const char* const*)a;  // elmt is char ptr
    const char* sb = *(const char* const*)b;
    return strcmp(sa, sb);
}

int main(void) {
    size_t cap = 0;
    size_t n = 0;
    char** lines = NULL;

    // read lines until EOF
    for (;;) {
        char* line = readline();
        if (!line) break;

        if (n >= cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            char** tmp = (char**)realloc(lines, new_cap * sizeof(*lines));
            if (!tmp) {
                for (size_t i = 0; i < n; i++) free(lines[i]);
                free(lines);
                oom();
            }
            lines = tmp;
            cap = new_cap;
        }
        lines[n++] = line;
    }

    // sort ptrs and print
    qsort(lines, n, sizeof(*lines), cmp_strptr);

    for (size_t i = 0; i < n; i++) {
        printf("%s", lines[i]);
    }

    // free memory
    for (size_t i = 0; i < n; i++) free(lines[i]);
    free(lines);

    return EXIT_SUCCESS;
}