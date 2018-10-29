#include "globals.h"
node* insert(char pid[],char name[])
{
	node* new=(node *)malloc(sizeof(node));
	strcpy(new->pid,pid);
	strcpy(new->name,name);
	new->next=emp;
	new->pos=1;
	if (head==emp)
	{
		head=new;
		return new;
	}
	node* temp=head;
	while(temp->next!=emp)
	{
		temp=temp->next;
	}
	new->pos=temp->pos+1;
	temp->next=new;
	return head;	
}
node* search(int pos)
{
	node* temp=head;
	while(temp!=emp&&temp->pos!=pos)
	{
		temp=temp->next;
	}
	return temp;
}
node* searchByPid(int pid)
{
	node* temp=head;
	char* piddd=(char*)malloc(2000*sizeof(char));
	sprintf(piddd,"%d",pid);
	while(temp!=emp&&strcmp(piddd,temp->pid)!=0)
	{
		temp=temp->next;
	}
	return temp;
}
node* findPrev(node* ser)
{
	node* temp=head;
	if (ser==head)
	{
		return emp;
	}
	while(temp!=emp&&temp->next!=ser)
	{
		temp=temp->next;
	}
	return temp;
}
// node* remov(char pid[])
// {
// 	if (head==emp)
// 	{
// 		return head;
// 	}
// 	node* temp=head;
// 	if (strcmp(temp->pid,pid)==0)
// 	{
// 		flag[head->pos]=0;
// 		head=head->next;
// 		//free(temp);
// 		return head;
// 	}
// 	node* prev=head;
// 	while (temp!=emp&&strcmp(temp->pid,pid)==0)
// 		{	
// 			prev=temp;
// 			temp=temp->next;
// 		}
// 	if (temp==emp)
// 	{
// 		return head;
// 	}
// 	prev->next=temp->next;
// 	//free(temp);
// 	flag[temp->pos]=0;
// 	return head;
// }
void remov(node* temp)
{
	if (temp==emp)
	{
		return ;
	}
	node* prev=head;
	while(prev!=emp&&prev->next!=temp)
		{
			prev=prev->next;
		}
	if (temp==head)
				 	{
				 		head=head->next;
				 	}else{
				 		prev->next=temp->next;
				 	}
		free(temp);
}
void sigintDekhle(int sig)
{
	if (fg_pid!=-1)
	{
		kill(fg_pid,SIGINT);
		fg_pid=-1;
	}
}
void baccha_handler()
{
	 int sthithi;
	 int pid;
	 pid=waitpid(-1,&sthithi,WNOHANG|WUNTRACED);
	 if (pid>0&&WIFEXITED(sthithi)==1)
	 {
	 	node* req=searchByPid(pid);
	 	fprintf(stderr,"%s with pid %d exited normally\n",req->name, pid);
	 	remov(req);
	 }
	 else if (pid>0&&WIFSIGNALED(sthithi)==1)
	 {
	 	node* req=searchByPid(pid);
	 	fprintf(stderr,"%s with pid %d interrupted\n",req->name,pid );
	 	remov(req);
	 }
	 else if (pid>0&&WIFSTOPPED(sthithi)==1)
	 {
	   node* req=searchByPid(pid);
	 	fprintf(stderr,"%s with pid %d stopped\n",req->name, pid);
	 }
}
void StopKaro(int sig)
{
	if (fg_pid!=-1)
	{
		kill(fg_pid,SIGTSTP);
		char* piddd=(char*)malloc(2000*sizeof(char));
		sprintf(piddd,"%d",fg_pid);
		insert(piddd,fg_name);
		fg_pid=-1;
	}
}
void handlesignal()
{
	signal(SIGCHLD,baccha_handler);
	signal(SIGINT,sigintDekhle);
	signal(SIGTSTP,StopKaro);
	signal(SIGTTOU,SIG_IGN);
}
int main()
{	
	fg_pid=-1;
	char*home=(char*)malloc((size_t)40000);
	char*temp=(char*)malloc((size_t)40000);
	head=emp;
	home=getHomeDirectory();
	char*userName=(char*)malloc((size_t)40000);
	userName = getenv("USER");
	char*hostname=(char*)malloc((size_t)40000);
	gethostname(hostname,(size_t)40000);
	char * currdir=giveDirectory(home);
	printf("%s@%s:%s>",userName,hostname,currdir);
	char** tokens=(char**)malloc(2003*sizeof(char*));
	char** names=(char**)malloc(2003*sizeof(char*));
	char** pids=(char**)malloc(2003*sizeof(char*));
	char** msg=(char**)malloc(2003*sizeof(char*));
	char*childPid=(char*)malloc(2003*sizeof(char));
	int st=0;int en=0;
	int status;
	pid_t child;
	handlesignal();
	while(1)
	{
		userName = getenv("USER");
		gethostname(hostname,(size_t)40000);
		temp=readInput();
		makeTokens(temp,tokens,home,names,&st,&en,pids,msg);
		currdir=giveDirectory(home);
		// child=waitpid(-1,&status,WNOHANG);
		// while(child>0)
		// {
		// 	int i;
		// 	sprintf(childPid, "%d", child);  
		// 	for (i = st; i < en; ++i)
		// 	{
		// 		if (strcmp(pids[i],childPid)==0)
		// 		{
		// 			break;
		// 		}
		// 	}
		// 	if (msg[i]==emp)
		// 	{
		// 		node* temp=head;
		// 		while(temp!=emp&&strcmp(temp->pid,childPid)!=0)
		// 			{
		// 				temp=temp->next;
		// 			}
		// 		if(temp!=emp)
		// 		 {
		// 		 	fprintf(stderr,"%s with pid %d terminated with exit status %d\n", temp->name,child,status);
		// 		 	remov(temp);
		// 		 }
		// 	}
		// 	else
		// 	{
		// 		fprintf(stderr,"%s\n", msg[i]);
		// 	}
		// 	st+=1;
		// 	child=waitpid(-1,&status,WNOHANG);
		// }
		printf("%s@%s:%s>",userName,hostname,currdir);		
		handlesignal();
	}
	return 0;
}
