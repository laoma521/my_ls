#include "ls.h"

void dirwalk(char *dir, void (*fcn)(char *))
{
 
    char name[MAX_PATH];
    struct dirent *dp;
    DIR *dfd;
 
    if ((dfd = opendir(dir)) == NULL)
    {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return ;
    }
 
    while ((dp = readdir(dfd)) != NULL)
    {
        if ((strcmp(dp->d_name,".") == 0) || strcmp(dp->d_name,"..") == 0)
            continue;
        if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
            fprintf(stderr,"dirwalk : name %s %s too long\n", dir, dp->d_name);
        else 
        {
            sprintf(name, "%s/%s",dir,dp->d_name);
            (*fcn)(name);
        }
    }
    closedir(dfd);
}
 
 
void fsize(char *name)
{
 
    struct stat stbuf;
 
    if(stat(name, &stbuf) == -1)
    {
     
        fprintf(stderr, "fsize: can't access %s\n", name);
        return;
    }
 
    if ((stbuf.st_mode& S_IFMT) == S_IFDIR)
    {
        dirwalk(name,fsize);
 
    }
 
    printf("%8ld %s\n",stbuf.st_size,name);
}


void my_err(const char *err_string, int line)
{
	fprintf(stderr, "line:%d  ", line);
	perror(err_string);
	exit(1);
}


void display_attribute(struct stat buf, char *name)
{
	char		buf_time[32];
	struct passwd	*psd;	
	struct group	*grp;	

	
	if (S_ISLNK(buf.st_mode)) 
	{
		printf("l");
	} 
	else if (S_ISREG(buf.st_mode)) 
	{
		printf("-");
	} 
	else if (S_ISDIR(buf.st_mode)) 
	{
		printf("d");
	} 
	else if (S_ISCHR(buf.st_mode)) 
	{
		printf("c");
	} 
	else if (S_ISBLK(buf.st_mode)) 
	{
		printf("b");
	} 
	else if (S_ISFIFO(buf.st_mode)) 
	{
		printf("f");
	} 
	else if (S_ISSOCK(buf.st_mode)) 
	{
		printf("s");
	}

	
	if (buf.st_mode & S_IRUSR)
	{
		printf("r");
	} 
	else 
	{
		printf("-");
	}
	if (buf.st_mode & S_IWUSR)
	{
		printf("w");
	} 
	else 
	{
		printf("-");
	}
	if (buf.st_mode & S_IXUSR)
	{
		printf("x");
	} 
	else 
	{
		printf("-");
	}

	
	if (buf.st_mode & S_IRGRP)
	{
		printf("r");
	} 
	else 
	{
		printf("-");
	}
	if (buf.st_mode & S_IWGRP)
	{
		printf("w");
	} 
	else 
	{
		printf("-");
	}
	if (buf.st_mode & S_IXGRP)
	{
		printf("x");
	} 
	else 
	{
		printf("-");
	}

	
	if (buf.st_mode & S_IROTH)
	{
		printf("r");
	} 
	else 
	{
		printf("-");
	}
	if (buf.st_mode & S_IWOTH)
	{
		printf("w");
	} 
	else 
	{
		printf("-");
	}
	if (buf.st_mode & S_IXOTH)
	{
		printf("x");
	} 
	else 
	{
		printf("-");
	}

	printf("    ");

	
	psd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);
	printf("%4d ", buf.st_nlink);   
	printf("%-8s", psd->pw_name);
	printf("%-8s", grp->gr_name);

	printf("%6d", buf.st_size);      
	strcpy(buf_time, ctime(&buf.st_mtime));
	buf_time[strlen(buf_time) - 1] = '\0';	
	printf("  %s", buf_time);				
}

g_leave_len = 80;

void display_single(char *name)
{
	int i, len;

	
	if (g_leave_len < g_maxlen) 
	{	
		printf("\n");
		g_leave_len = MAXROWLEN;
	}

	len = strlen(name);
	len = g_maxlen - len;
	printf("%-s", name);
	
	for (i=0; i<len; i++) 
	{
		printf(" ");
	}
	printf("  ");
	g_leave_len -= (g_maxlen + 2);
}

void display(int flag, char * pathname)
{
	int		i, j;
	struct stat	buf;
	char		name[NAME_MAX + 1];

	
	for (i=0, j=0; i<strlen(pathname); i++) 
	{
		if (pathname[i] == '/') 
		{
			j = 0;
			continue;
		}
		name[j++] = pathname[i];
	}
	name[j] = '\0';
	
	
	if ( lstat(pathname, &buf) == -1 ) 
	{
		my_err("stat", __LINE__);
	}
	
	switch (flag) 
	{
		case PARAM_NONE:
			if(name[0] != '.')
			{
				display_single(name);
			}
			break;
		case PARAM_A:	  
			display_single(name);
			break;

		case PARAM_L:	  
			if (name[0] != '.') 
			{
				display_attribute(buf, name);
				printf("  %-s\n", name);
			}
			break;

		case PARAM_A + PARAM_L:		
			display_attribute(buf, name);
			printf("  %-s\n", name);
			break;

		default:
			break;
	}
}

void display_dir(int flag_param, char *path)
{
        DIR            *dir;
        struct dirent  *ptr;
        int            count = 0;
        char           filenames[256][PATH_MAX+1],temp[PATH_MAX+1];

	
        dir = opendir(path);
        if (dir == NULL) 
		{
                my_err("opendir", __LINE__);
        }
        while ((ptr = readdir(dir))!=NULL) 
		{
                if (g_maxlen < strlen(ptr->d_name))
                         g_maxlen =  strlen(ptr->d_name);
                count++;
        }
        closedir(dir);

        if(count>256)
                my_err("too many files under this dir",__LINE__);
		

        int i, j, len = strlen(path);
	
        dir = opendir(path);
        for(i = 0; i < count; i++)
		{
                ptr = readdir(dir);
                if( ptr == NULL)
				{
                        my_err("readdir",__LINE__);
                }
                strncpy(filenames[i],path,len);
                filenames[i][len] = '\0';
                strcat(filenames[i],ptr->d_name);
                filenames[i][len+strlen(ptr->d_name)] = '\0';
        }

        for(i = 0; i < count-1; i++)
                for(j = 0; j < count-1-i; j++) 
				{
                        if( strcmp(filenames[j],filenames[j+1]) > 0 ) 
						{
                                strcpy(temp,filenames[j+1]);
                                temp[strlen(filenames[j+1])] = '\0';
                                strcpy(filenames[j+1],filenames[j]);
                                filenames[j+1][strlen(filenames[j])] = '\0';
                                strcpy(filenames[j], temp);
                                filenames[j][strlen(temp)] = '\0';
                        }
                }
        for(i = 0; i < count; i++)
                display(flag_param, filenames[i]);

        closedir(dir);

        if( (flag_param & PARAM_L) == 0)
                printf("\n");}

