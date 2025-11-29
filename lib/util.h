#define CHIMERA_IMPLEMENTATION
#define CHIMERA_STRIP_PREFIX
#include <chimera.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>


typedef char* File;
typedef struct {
  File *items;
  size_t count;
  size_t cap;
} Files;

bool list_dir(char *dir, Files *files) {
  DIR *dp;
  struct dirent *ep;
  dp = opendir(dir);
  if (dp != NULL) {
    while ((ep = readdir(dp)) != NULL) {
      if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;
      da_push(files, strdup(ep->d_name));
    }
    closedir(dp);
    return true;
  } else {
    return false;
  }
}
