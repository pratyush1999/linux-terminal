#include "globals.h"
void fg_execute(char **commands)
{
  pid_t pid;
  int* statusValueReturned=(int*)malloc(sizeof(int));
  pid = fork(); 
  if (pid == 0) 
  {
  	strcpy(fg_name,commands[0]);
  	setpgid(0,0);
   if(execvp(commands[0],commands)==-1)
    {
    	fprintf(stderr, "Error in executing command\n");
    }
   exit(1);
  } 
  else if(pid>0)
  { 
  	 fg_pid=pid;
     waitpid(pid,statusValueReturned,WUNTRACED);
  }
  else if (pid < 0)
  {
   	 fprintf(stderr,"Error in forking\n");
  } 
  free(statusValueReturned);
}
void findProcName(char* child,char** names,int* st,int* en,char** pids)
{
	names[*en]=child;
	*en+=1;
}
void bg_execute(char** commands,char** names,int* st,int* en,char** pids,char** msg)
{
  pid_t pid;
  int* statusValueReturned=(int*)malloc(sizeof(int));
  pid = fork(); 
  if (pid == 0) 
  {
  	 setpgid(0,0);
   if(execvp(commands[0],commands)==-1)
    {
       fprintf(stderr, "Error in executing command\n");
    }
   exit(1);
  } 
  else if (pid < 0)
  {
   	 fprintf(stderr,"Error in forking\n");
  } 
  else if(pid>0)
  { 
  	char* output=(char*)malloc(2000*sizeof(char));
  	sprintf(output, "%d", pid);  
  	msg[*en]=emp;
  	pids[*en]=output;
  	head=insert(output,commands[0]);
 	findProcName(commands[0],names,st,en,pids);
 	printf("%d\n",pid);
  }
  free(statusValueReturned);	
}
void reminder(char** tokens,char** names,int* st,int* en,char** pids,char** msg)
{
	tokens[0]="sleep";
	char* output=(char*)malloc(2000*sizeof(char));
	int i=4;
	output=tokens[2];
	tokens[2]=emp;
	bg_execute(tokens,names,st,en,pids,msg);
	msg[*en-1]=output;
}
void makeTokens_2(char* commands1,char* home,char** names,int* st,int* en,char** pids,char** msg)
{
	int index=0;
	char * token=strtok(commands1,delims2);
	char** tokens=(char**)malloc(2000*sizeof(char*));
	while(token!=emp)
	{
		tokens[index++]=token;
		token=strtok(emp,delims2);
	}
	tokens[index]=emp;
	loop_pipe(tokens,home,names,st,en,pids,msg);
	return ;
}
void exec(char** tokens,char* home,char** names,int* st,int* en,char** pids,char** msg)
{
	int fd,inpu,outpu;
	inpu=dup(0);
	outpu=dup(1);
	char* path=(char*)malloc(2000*sizeof(char));
	int index=0;
	for (int i = 0; tokens[i]!=emp; ++i)
	{
		if (tokens[i][0]=='<')
		{
			if (tokens[i+1]!=emp&&tokens[i+1][0]=='~')
			{
				strcpy(path,home);
				strcat(path,tokens[i+1]+1);
			}
			else if(tokens[i+1]!=emp)
			{
				strcpy(path,tokens[i+1]);
			}
			else if (tokens[i+1]==emp)
			{
				path=emp;
			}
			if (path==emp)
			{
				fprintf(stderr, "Error opening file\n");
				dup2(inpu,0);
				dup2(outpu,1);
				return;
			}
			fd=open(path, O_RDONLY);
			if (fd==-1)
			{
				fprintf(stderr, "Error opening file\n");
				dup2(inpu,0);
				dup2(outpu,1);
				return;
			}
			if(dup2(fd,0)==-1)
			{
				fprintf(stderr, "Error reading file\n");
				dup2(inpu,0);
				dup2(outpu,1);
				return;
			}
			close(fd);
			tokens[i]=emp;
		}
	   else if (tokens[i][0]=='>')
		{
			if (tokens[i+1]!=emp&&tokens[i+1][0]=='~')
			{
				strcpy(path,home);
				strcat(path,tokens[i+1]+1);
			}
			else if(tokens[i+1]!=emp)
			{
				strcpy(path,tokens[i+1]);
			}
			else if (tokens[i+1]==emp)
			{
				path=emp;
			}
			if (path==emp)
			{
				fprintf(stderr, "Error writing file\n");
				dup2(inpu,0);
				dup2(outpu,1);
				return;
			}
			if (tokens[i][1]=='>')
			{
				fd=open(path,O_WRONLY|O_CREAT|O_APPEND,0644);
				if (fd==-1)
				{
					fprintf(stderr, "Error opening file\n");
					dup2(inpu,0);
					dup2(outpu,1);
					return;
				}
			}
			else
			{
				fd=open(path,O_WRONLY|O_CREAT|O_TRUNC,0644);
				if (fd==-1)
				{
					fprintf(stderr, "Error opening file\n");
					dup2(inpu,0);
					dup2(outpu,1);
					return;
				}
			}
			close(1);
			if(dup2(fd,1)==-1)
			{
				fprintf(stderr, "Error writing file\n");
				dup2(inpu,0);
				dup2(outpu,1);
				return;
			}
			close(fd);
			tokens[i]=emp;
		}
		++index;
	}
	if (index>0)
	{
		if (strcmp(tokens[0],"pwd")==0)
		{
			pwd();
		}
		else if (strcmp(tokens[0],"quit")==0)
		{
			exit(0);
		}
		else if (strcmp(tokens[0],"echo")==0)
		{
			echo(tokens);
		}
		else if (strcmp(tokens[0],"cd")==0)
		{
			cd(tokens,home);
		}
		else if (strcmp(tokens[0],"pinfo")==0)
		{
			pinfo(tokens,home);
		}
		else if (strcmp(tokens[0],"ls")==0)
		{
			ls(tokens,home,index);
		}
		else if (strcmp(tokens[0],"remindme")==0)
		{
			reminder(tokens,names,st,en,pids,msg);
		}
		else if (strcmp(tokens[0],"clock")==0)
		{
			clock_mine(tokens);
		}
		else if (strcmp(tokens[0],"setenv")==0)
		{
			setENV(tokens);
		}
		else if (strcmp(tokens[0],"unsetenv")==0)
		{
			unsetENV(tokens);
		}
		else if (strcmp(tokens[0],"jobs")==0)
		{
			jobs_mine(tokens);
		}
		else if (strcmp(tokens[0],"kjob")==0)
		{
			kjob(tokens);
		}
		else if (strcmp(tokens[0],"overkill")==0)
		{
			overkill();
		}
		else if (strcmp(tokens[0],"bg")==0)
		{
			bg(tokens);
		}
		else if (strcmp(tokens[0],"fg")==0)
		{
			fg(tokens);
		}
		else if(tokens[0]!=emp)
		{
			if (tokens[1]!=emp&&strcmp(tokens[index-1],"&")==0)
			{
				tokens[index-1]=emp;
				bg_execute(tokens,names,st,en,pids,msg);
			}
			else
			{
				fg_execute(tokens);
			}
		}
	}
	dup2(inpu,0);
	dup2(outpu,1);
	free(path);
}
void  loop_pipe(char **cmd,char* home,char** names,int* st,int* en,char** pids,char** msg) 
{
  int   p[2];
  int pid;
  int   fildes_inpu = 0;
  int i=0;
  int start=0;
  while (cmd[i]!= emp)
    {
    	while(cmd[i]!= emp&&cmd[i][0]!='|')
    	{
    		++i;
    	}
    	if (cmd[i]==emp&&start==0)
    	{
    		exec(cmd,home,names,st,en,pids,msg);
    		return;
    	}
      pipe(p);
      pid = fork();
      if (pid == -1)
        {
        	fprintf(stderr, "Error in forking\n");
            exit(1);
        }
      else if (pid == 0)
        {
          if(dup2(fildes_inpu,0)==-1)
          	fprintf(stderr,"Error reading file\n");
          if (cmd[i+1] != emp)
            if(dup2(p[1],1)==-1)
            	fprintf(stderr, "Error writing file\n");
          close(p[0]);
          cmd[i]=emp;
         exec(cmd+start,home,names,st,en,pids,msg);
         exit(1);
        }
      else
        {
          wait(emp);
          close(p[1]);
          fildes_inpu = p[0]; 
          ++i;
        }
        start=i;
        if (cmd[i]== emp)
    	{
    		break;
    	}
    }
}
void makeTokens(char* commands,char** tokens,char* home,char** names,int* st,int* en,char** pids,char** msg)
{
	char* for_empty=emp;
	int index=0;
	int pos=0;
	char * token=strtok(commands,delims);
	while(token!=for_empty)
	{
		tokens[index]=token;
		token=strtok(for_empty,delims);
		++index;
	}
	tokens[index]=for_empty;
	for (int i = 0; tokens[i]!=for_empty; ++i)
	{
		makeTokens_2(tokens[i],home,names,st,en,pids,msg);
	}
	free(token);
	return ;
}
