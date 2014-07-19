#ifndef LS_H
#define LS_H

#define  PARAM_NONE     0
#define  PARAM_A        1
#define  PARAM_L        2
#define  MAX_PATH       1024
#define  MAXROWLEN      80

#include <stdio.h>

#include <fcntl.h>
#include <grp.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <linux/limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

extern int   g_leave_len;	
int   g_maxlen;		

void dirwalk(char *dir, void(*fcn)(char *));
void fsize(char *name);
void my_err(const char *err_string, int line);
void display_attribute(struct stat buf, char *name);
void display_single(char *name);
void display(int flag, char *pathname);
void display_dir(int flag_param, char *path);

#endif 
