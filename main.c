#include "ls.h"

int main(int argc, char ** argv)
{
	int		i, j, k, num;
	char		path[PATH_MAX+1];
	char		param[32];	      
	int		flag_param = PARAM_NONE; 
	struct stat	buf;


	j   = 0,
	num = 0;

	if ((num + 1) == argc) 
	{	
		strcpy(path, "./");
		path[2] = '\0';
		display_dir(flag_param, path);
		return 0;
	}
	for(i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			for(k = 1; k < strlen(argv[i]); k++, j++)
			{
				param[j] = argv[i][k];
			}
			num++;
		}
	}
	for(i = 0; i < j; i++)
	{
		if(param[i] == 'a')
		{
			flag_param |= PARAM_A;
			continue;
		}
		else if(param[i] == 'l')
		{
			flag_param |= PARAM_L;
			continue;
		}
		else if(param[i] == 'R')
		{
			flag_param |= MAX_PATH;
			fsize(".");
			continue;
		}
		else
		{
			printf("my_ls:invalid option -%c\n", param[i]);
			exit(1);
		}
	}
	param[j] = '\0';

	i=1;
	
	if(argc == 1)
	{
		fsize(".");
	}
	   
	do 
	{
		
		if (argv[i][0] == '-') 
		{ 
			i++;
			continue;	
		} 
		else 
		{
			strcpy(path, argv[i]);

			
			if ( stat(path, &buf) == -1 )  
				my_err("stat", __LINE__);	

			if ( S_ISDIR(buf.st_mode) ) 
			{   
				
				if ( path[ strlen(argv[i])-1 ] != '/') 
				{
					path[ strlen(argv[i]) ] = '/';
					path[ strlen(argv[i])+1 ] = '\0';
				}
				else
					path[ strlen(argv[i]) ] = '\0';

				display_dir(flag_param,path);
				i++;
			}
			else 
			{  
				display(flag_param, path);
				i++;
			}
		}
	} while (i<argc);

	return 0;
}
