#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

const char *trim(char *string);
void path_dos_to_unix(char *path);
FILE *fopen_case(const char *name, const char *flags);
const char *extension(const char *name);

#ifdef __cplusplus
}
#endif


#endif
