#include <sys/utsname.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/dir.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#define m 1000
#define n 2000
#define emp NULL
#define delims ";\n"
#define delims2 " \t;\n"
typedef struct node{
	char pid[2003];
	int pos;
	struct node* next;
	char name[2003];
}node;
node* head;
int fg_pid;
char fg_name[2002];
void fg(char **tokens);
void baccha_handler();
node* findPrev(node* ser);
void handlesignal();
void StopKaro(int sig);
void sigintDekhle(int sig);
char * getHomeDirectory();
void bg(char** tokens);
node* insert(char pid[],char name[]);
void remov(node* temp);
node* search(int pos);
void setENV(char** tokens);
void unsetENV(char** tokens);
char* giveDirectory(char* home);
char* readInput();
void  loop_pipe(char **cmd,char* home,char** names,int* st,int* en,char** pids,char** msg) ;
void pwd();
void overkill();
void echo(char** tokens);
void cd(char** tokens,char* home);
void pinfo(char** args,char* home);
node* jobs_mine(char** tokens);
node* kjob(char** tokens);
char* printFileNames(struct dirent* file,DIR* dirra,int flag);
void print_permissions(mode_t st,char permission[]);
void print_ls(struct dirent* file,DIR* dir,int flag);
void findPath(char* home,char** tokens,int x,char** path);
void clock_mine(char** tokens);
void ls(char** tokens,char* home,int index);
char* readInput();
void fg_execute(char **commands);
void findProcName(char* child,char** names,int* st,int* en,char** pids);
void bg_execute(char** commands,char** names,int* st,int* en,char** pids,char** msg);
void reminder(char** tokens,char** names,int* st,int* en,char** pids,char** msg);
void makeTokens_2(char* commands1,char* home,char** names,int* st,int* en,char** pids,char** msg);
void makeTokens(char* commands,char** tokens,char* home,char** names,int* st,int* en,char** pids,char** msg);
