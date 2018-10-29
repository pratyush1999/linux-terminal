#include "globals.h"
void pwd()
{
	size_t size=40000;
	char * tempdir=(char*)malloc(size);
	tempdir=getcwd(tempdir,size);
	printf("%s\n",tempdir);
	free(tempdir);
}
void fg(char **tokens)
{
	if (tokens[1]==emp) {
		fprintf(stderr, "Less arguments given\n");
		return ;
	}
	int x[4];
	waitpid(-1, emp, WNOHANG);
	node *yeHai = search(atoi(tokens[1]));
	if (yeHai == emp) {
		fprintf(stderr, "No Such Process exists\n");
		return ;
	}
	fg_pid=atoi(yeHai->pid);
	strcpy(fg_name,yeHai->name);
	x[0] = tcgetpgrp(0);
	x[1] = tcgetpgrp(1);
	x[2]= tcgetpgrp(2);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	 tcsetpgrp(0, atoi(yeHai->pid));
	 tcsetpgrp(1, atoi(yeHai->pid));
	 tcsetpgrp(2, atoi(yeHai->pid));
	 if(kill(atoi(yeHai->pid), SIGCONT)==-1)
	 	{
	 		fprintf(stderr, "Error in executing kill\n");
	 		return ;
	 	}
	 int waaaait;
	waitpid(atoi(yeHai->pid), &waaaait, WUNTRACED);
	 tcsetpgrp(0, x[0]);
	 tcsetpgrp(1, x[1]);
	 tcsetpgrp(2, x[2]);
	 signal(SIGTTIN, SIG_DFL);
	 signal(SIGTTOU, SIG_DFL);
	 remov(yeHai);
}
node* jobs_mine(char** tokens)
{
	char* rasta = (char*)malloc(2003*sizeof(char));
	char output[4][2000];
	node* hd=head;
	node* hd1=head;
	while(hd!=emp)
	{
		sprintf(rasta,"/proc/%s/wchan", hd->pid);
		FILE* file=fopen(rasta,"r");
		if (file==emp)
		{
			 hd1=hd->next;
			 remov(hd);
			 hd=hd1;
			 continue;
		}
		else
		{
			fscanf(file,"%s",output[2]);
		}
		if (strcmp("do_signal_stop",output[2])==0)
		{
			strcpy(output[2],"Stopped");
		}
		else
		{
			strcpy(output[2],"Running");
		}
		fclose(file);
		printf("[%d] %s %s [%s]\n",hd->pos,output[2],hd->name,hd->pid);
		if (hd->next==emp)
		{
			break;
		}
		hd=hd->next;
	}
	return head;
}
node* kjob(char** tokens)
{
	if (tokens[1]==emp||tokens[2]==emp)
	{
		fprintf(stderr, "Less arguments given\n");
		return head;
	}
	node* req=search(atoi(tokens[1]));
	if (req==emp)
	{
		fprintf(stderr, "No such process exists\n");
		return head;
	}
	if (kill(atoi(req->pid),atoi(tokens[2]))==-1)
	{
		fprintf(stderr, "Error killing the process\n");
		return head;
	}
	//remov(req);
	return head;
}
void overkill()
{
	node* temp=head;
	while(temp!=emp)
	{		
		printf("Killing Process with pid:%s\n", temp->pid);
		if(kill(atoi(temp->pid),SIGKILL)==-1)
			{
				fprintf(stderr, "Error killing process\n");
				return ;
			}
		//remov(temp);	
		temp=temp->next;
	}
	//head=emp;
}
void bg(char** tokens)
{
	if (tokens[1]==emp)
	{
		fprintf(stderr, "Less arguments given\n");
		return ;
	}
	node* req=search(atoi(tokens[1]));
	if (req==emp)
	{
		fprintf(stderr, "No such process exists\n");
		return ;
	}
	if(kill(atoi(req->pid),SIGCONT)==-1)
			{
				fprintf(stderr, "Error in signalling the process\n");
				return ;
			}

}
void setENV(char** tokens)
{
	if (tokens[1]==emp)
	{
		fprintf(stderr, "less arguments given\n");
		return; 
	}
	else if (tokens[2]!=emp&&tokens[3]!=emp)
	{
		fprintf(stderr, "more than required arguments given\n");
		return ;
	}
	else if (tokens[2]==emp)
	{
		if(setenv(tokens[1],"",1)==-1)
			fprintf(stderr, "Error in setting of env variable\n");
		return ;
	}
	else if (tokens[1]!=emp&&tokens[2]!=emp)
	{
		if(setenv(tokens[1],tokens[2],1)==-1)
			fprintf(stderr, "Error in setting of env variable\n");
		return;
	}
}
void unsetENV(char** tokens)
{
	if (tokens[1]==emp)
	{
		fprintf(stderr, "less arguments given\n");
		return ;
	}
	if (unsetenv(tokens[1])==-1)
	{
		fprintf(stderr, "Error in unsetting of env variable\n");
		return;
	}
}
void echo(char** tokens)
{
	int i=1;
	while(tokens[i]!=emp)
	{
		printf("%s ",tokens[i]);
		++i;
	}
	printf("\n");
	return ;
}
void cd(char** tokens,char* home)
{
	int t;
	if (tokens[1]==emp)
	{
		t=chdir(home);
		if (t!=0)
		{
			fprintf(stderr, "Error Changing Directory\n");
		}
	}
	else if (strcmp(tokens[1],"~")==0)
	{
		t=chdir(home);
		if (t!=0)
		{
			fprintf(stderr, "Error Changing Directory\n");
		}
	}
	else 
	{
		t=chdir(tokens[1]);
		if (t!=0)
		{
			fprintf(stderr, "Error Changing Directory\n");
		}
	}
	
}
void pinfo(char** args,char* home)
{
	char* pid=(char*)malloc(n*sizeof(char));
	if (args[1]==emp)
	{
		sprintf(pid, "%d", getpid());
	}
	else
	{
		pid=args[1];	
	}
	char* buffer=(char*)malloc(n*sizeof(char));
	char* pathForExe=(char*)malloc(n*sizeof(char));
	char* exeAt=(char*)malloc(n*sizeof(char));
	char output[m+2][n];
	sprintf(buffer,"/proc/%s/stat", pid);
	FILE* file=fopen(buffer,"r");
	if (file==emp)
	{
		printf("%s\n",buffer);
		fprintf(stderr,"Error Opening File\n");
		return;
	}
	sprintf(pathForExe, "/proc/%s/exe", pid);  
	readlink(pathForExe,exeAt,n);
	for (int i = 0; i < m; ++i)
	{
		fscanf(file,"%s",output[i]);
	}
	fclose(file);
	printf("PID:%s\n",pid);
	printf("STATUS:%s\n",output[2]);
	printf("MEMORY:%s\n",output[22]);
	int i=0;
	char* relExePath=(char*)malloc(n);
	while(i <= strlen(exeAt)&&i <= strlen(home)&&exeAt[i]==home[i]&&home[i]!='\0'&&exeAt[i]!='\0')
		++i;
	if (i==strlen(home))
	{
		relExePath[0]='~';
		for (int j = i; j <strlen(exeAt) ; ++j)
		{
			relExePath[j-i+1]=exeAt[j];
		}
	}
	else
	{
		relExePath=exeAt;
	}
	printf("ExecutablePath-%s\n", relExePath);
}
char* printFileNames(struct dirent* file,DIR* dirra,int flag)
{
	while(file!=emp)
		{
			if (file->d_name[0]!='.')
			{
				printf("%s ",file->d_name);
			}
			else if (file->d_name[0]=='.'&&flag==1)
			{
				printf("%s ",file->d_name);
			}
			file=readdir(dirra);
		}
		printf("\n");
}
void print_permissions(mode_t st,char permission[])
{
	if (st&S_ISREG(st))
	{
		permission[0]='-';
	}
	else if (st&S_ISDIR(st))
	{
		permission[0]='d';
	}
	else if (st&S_ISBLK(st))
	{
		permission[0]='b';
	}
	else if (st&S_ISFIFO(st))
	{
		permission[0]='|';
	}
	else if (st&S_ISSOCK(st))
	{
		permission[0]='s';
	}
	else if (st&S_ISCHR(st))
	{
		permission[0]='c';
	}
	else if(st&S_ISLNK(st))
	{
		permission[0]='l';
	}
	permission[1] = (st & S_IRUSR) ? 'r':'-';
    permission[2] = (st & S_IWUSR) ? 'w':'-';
    permission[3] = (st & S_IXUSR) ? 'x':'-';
    permission[4] = (st & S_IRGRP) ? 'r':'-';
    permission[5] = (st & S_IWGRP) ? 'w':'-';
    permission[6] = (st & S_IXGRP) ? 'x':'-';
    permission[7] = (st & S_IROTH) ? 'r':'-';
    permission[8] = (st & S_IWOTH) ? 'w':'-';
    permission[9] = (st & S_IXOTH) ? 'x':'-';
    permission[10] = '\0';
    return;
}
void print_ls(struct dirent* file,DIR* dir,int flag)
{
	struct stat sbuf;
    char buffer[n];
    struct passwd passwent, * passwentp;
    struct group grop, * gropt;
    char dates[2*n];
    struct tm time;
    char permission[100];
	while(file!=emp) 
   	 {
       stat(file->d_name, &sbuf);
       if (file->d_name[0]!='.'||flag==1)
		{
			print_permissions(sbuf.st_mode,permission);
       printf("%s ",permission);
        printf(" %d", (int)sbuf.st_nlink);
        if (!getpwuid_r(sbuf.st_uid, &passwent, buffer, sizeof(buffer), &passwentp))
            printf(" %s", passwent.pw_name);
        else
            printf(" %d", sbuf.st_uid);

        if (!getgrgid_r (sbuf.st_gid, &grop, buffer, sizeof(buffer), &gropt))
            printf(" %s", grop.gr_name);
        else
            printf(" %d", sbuf.st_gid);
        printf(" %5d", (int)sbuf.st_size);  
        localtime_r(&sbuf.st_mtime, &time);
        strftime(dates, sizeof(dates), "%F %T", &time);
        printf(" %s", dates);
        printf(" %s\n",file->d_name);
    	}
    	file=readdir(dir);
    }
}
void findPath(char* home,char** tokens,int x,char** path)
{
	if (tokens[x]==emp)
		{
			getcwd(*path,n);
		}
		else
		{
			if (tokens[x][0]=='~')
			{
				strcpy(*path,home);
				strcat(*path,tokens[x]+1);
			}
			else
			{
				*path=tokens[x];
			//	printf("%s\n",path);
			}
		}
}

void clock_mine(char** tokens)
{
	char* currTime=(char*)malloc(n*sizeof(char));
	char* currDate=(char*)malloc(n*sizeof(char));
	char* time=(char*)malloc(n*sizeof(char));
	char* date=(char*)malloc(n*sizeof(char));
	char* interval=(char*)malloc(n*sizeof(char));
	char* duration=(char*)malloc(n*sizeof(char));
	interval=tokens[2];
	duration=tokens[4];
	for(int i=0;i<atoi(duration);++i)
	{
		currTime="/sys/class/rtc/rtc0/time";
		currDate="/sys/class/rtc/rtc0/date";
		FILE* file=fopen(currTime,"r");
		if (file==emp)
		{
		fprintf(stderr,"Error Opening File\n");
		return;
		}
		fscanf(file,"%s",time);
		fclose(file);
		file=fopen(currDate,"r");
		if (file==emp)
		{
			fprintf(stderr,"Error Opening File\n");
			return;
		}
		fscanf(file,"%s",date);
		fclose(file);
		printf("%s,%s\n",date,time);
		sleep(atoi(interval));
	}
}
void ls(char** tokens,char* home,int index)
{
	char* path=(char*)malloc(n*sizeof(char));
	DIR * dir; 
	struct dirent * file;
	int x=1;
	if (tokens[1]==emp)
	{
		getcwd(path,n);	
		dir = opendir(path);
   	 	if (dir==emp)
		{
			fprintf(stderr, "Error opening Directory\n");
			return ;
		}
		file=readdir(dir);
		printFileNames(file,dir,0);
		closedir(dir);
		return ;
	}
	if (strcmp(tokens[1],"-a")==0)
	{
		while(tokens[x]!=emp)
		{
		findPath(home,tokens,++x,&path);
		if (tokens[x]==emp&&x>2)
		{
			break;
		}
		dir = opendir(path);
   		if (dir==emp)
		{
			fprintf(stderr, "Error opening Directory\n");
			break;
		}
		file=readdir(dir);
		if (index>3)
		{
			printf("%s:\n", tokens[x]);
		}
		printFileNames(file,dir,1);
		closedir(dir);
		}
		return ;
	}
	if (strcmp(tokens[1],"-l")==0)
	{
		while(tokens[x]!=emp)
		{
		findPath(home,tokens,++x,&path);
		if (tokens[x]==emp&&x>2)
		{
			break;
		}
		dir = opendir(path);
   		if (dir==emp)
		{
			fprintf(stderr, "Error opening Directory\n");
			break;
		}
		file=readdir(dir);
		if (index>3)
		{
			printf("%s:\n", tokens[x]);
		}
		print_ls(file,dir,0);
	   	closedir(dir);
	   	}
   	 	return;
	}
	if (strcmp(tokens[1],"-la")==0||strcmp(tokens[1],"-al")==0)
	{
		while(tokens[x]!=emp)
		{
		findPath(home,tokens,++x,&path);
		if (tokens[x]==emp&&x>2)
		{
			break;
		}
		dir = opendir(path);
   		if (dir==emp)
		{
			fprintf(stderr, "Error opening Directory\n");
			break;
		}		
		file=readdir(dir);
		if (index>3)
		{
			printf("%s:\n", tokens[x]);
		}
		print_ls(file,dir,1);
	   closedir(dir);
		}
   	 	return;
	} 
	while(tokens[x]!=emp)	
	{
		findPath(home,tokens,x,&path);
		dir = opendir(path);
   		if (dir==emp)
		{
			fprintf(stderr, "Error opening Directory\n");
			break;
		}
		file=readdir(dir);
		if (index>2)
		{
			printf("%s:\n", tokens[x]);
		}
		printFileNames(file,dir,0);
		closedir(dir);
		x++;
	}
		return ;
	
}
