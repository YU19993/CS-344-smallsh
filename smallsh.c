#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<ftw.h>
#include<stdio.h>
#include<fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include<sys/time.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>

#define MAX 2048
#define MAXA 512

int FF = 0;


void catchSIGINT(int signo)
{
	exit(1);
}

void catchSIGTSTP(int signo){
	if(FF == 0){
		printf("\nEntering foreground-only mode (& is now ignored)\n:");
		fflush(stdout);
		FF = 1;
	}
	else{
		printf("\nExiting foreground-only mode\n:");
		fflush(stdout);
		FF = 0;
	}
	

}


/*This function will find the "$$" by strstr()function and replace all of them by PID*/
void check44(char * check) {

		/*get the PID*/
	char pid[20];
	sprintf(pid,"%d",getpid());

		/*buffer to hold pieces information*/
	char buffer[MAX];
		/*hold where find the $$*/
        char * ch;
      
		/*if there is $$*/
	while((ch = strstr(check, "$$"))){
			/*copy first half part*/
		strncpy(buffer, check, ch-check);
		buffer[ch-check] = '\0';
			/*find last half part*/
		ch = ch+2;
			/*put pid where there is a $$*/	
		strcat(buffer,pid);
			/*put back last half*/
		strcat(buffer,ch);
			/*copy back to check*/
		strcpy(check,buffer);
		}
}


void freeS(char *a[]){
	int t;
	for(t=0;t<512;t++){
		if(a[t] != NULL){
		free(a[t]);}
	}
	a = NULL;
}

void ffreeS(char *a[],int c){
        int t;
        for(t=0;t<c;t++){
		if(a[t]!=NULL){
                free(a[t]);}
        }
        a = NULL;
}


//set status in file, which allow child talk back to parent process
void saw(int a, char * cc){
int fd;
char fil[1000];
sprintf(fil, "%s/status.txt", cc);
fd = open(fil, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
ssize_t  nw;
char aa[1];
sprintf(aa,"%d",a);
//printf("%s\n", aa);
nw = write(fd, aa, sizeof(char));
close(fd);
}

void sar(char * cc){
int fd;
char fil[1000];
sprintf(fil, "%s/status.txt", cc);
//printf("%s\n", fil);
fd = open(fil, O_RDONLY);
ssize_t  nr;
if(fd<0){printf("ERROR");}
char * a;
a = malloc(sizeof(char));
nr = read(fd, a, sizeof(char));
close(fd);
//printf("%s\n", a);
printf("EXIT STATUS %s\n", a);
fflush(stdout);
free(a);

}

int main(){
	int numCharsEntered = -5; // How many chars we entered
	size_t bufferSize = 0; // Holds how large the allocated buffer is
	size_t maxSize = MAX;
	char* lineEntered = NULL; // Points to a buffer allocated by getline() that holds our entered string + \n + \0
	int RIN = 0;
	int ROUT = 0;
	int INBG = 0;
	char RINS[1000];
	char ROUTS[1000];
	char *  sortIN[512];
	int t;
        char somechar[1000];
	int exx = 0;
	int fc = 0;
	pid_t bgc[100];
	for(t=0;t<100;t++){bgc[t]=0;}
	int cbgc = 0;
	char cc[1000];
	strcpy(cc,getenv("HOME"));
//	chdir(cc);
	saw(0, cc);


	struct sigaction SIGINT_action = {0}, SIGTSTP_action = {0}, ignore_action = {0};
        SIGINT_action.sa_handler = catchSIGINT;
        sigfillset(&SIGINT_action.sa_mask);
        SIGINT_action.sa_flags = 0;

        SIGTSTP_action.sa_handler = catchSIGTSTP;
        sigfillset(&SIGTSTP_action.sa_mask);
        SIGTSTP_action.sa_flags = 0;


        ignore_action.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ignore_action, NULL);
        sigaction(SIGTSTP, &SIGTSTP_action, NULL);
	saw(0, cc);

		
		/*program / loop start here until reseive exit*/
	while(exx == 0){

		fc = 0;
        	if(cbgc > 0){
                int io;
                pid_t spawn = -5;
                int childExit = -5;
		int y;
                                /*check for every child process*/
                for(io=0;io<cbgc;io++){
		//	printf("%d\n",io);
		//	printf("FLAGE\n");
                        if(bgc[io] != 0){
			//	printf("CHECK CHILD\n");
                                        /*check if it complete*/
                                spawn = waitpid(bgc[io], &childExit, WNOHANG);
			//	printf("SECOND CHECK\n");
                                if(spawn !=0){
					//printf("THIRD CHECK\n");
                                                /*kill it and free the space*/
                                        spawn = waitpid(bgc[io], &childExit, 0);
				//	printf("Check\n");
                                        y = WIFEXITED(childExit);
                                        printf("PROCESS%d TERMINATE\nEXIT STATUS %d\n",bgc[io],y);
                                        bgc[io] = 0;
                                }
                        }
                }
        }
//		printf("T1\n");


	        for(t=0; t<512; t++){
        	        sortIN[t] = malloc(1000 * sizeof(char));
			memset(sortIN[t], '\0', sizeof(sortIN[t]));
	        }


	        printf(":");
                fflush(stdout);


		while(1)
    		{
     		      // Get a line from the user
			numCharsEntered = getline(&lineEntered, &bufferSize, stdin);
			if (numCharsEntered == -1){
				clearerr(stdin);
			}
			else{
				break; // Exit the loop - we've got input
			}
		}


		lineEntered[strcspn(lineEntered, "\n")] = '\0';

		char input[MAX];
	
		strcpy(input,lineEntered);
			
		free(lineEntered);
		lineEntered = NULL;
		check44(input);

		char * token;
		int count = 0;
		size_t l;
		l=strlen(input)-1;
		if(input[l] == '\n'){input[l]='\0';}
//		printf("1\n");			
		token = strtok(input, " ");
		while(token != NULL){
			memset(sortIN[count],'\0',sizeof(sortIN[count]));
			strcpy(sortIN[count],token);
			count++;
			token = strtok(NULL," ");
		}
		int o;
	//	for(o=0;o<count;o++){printf("%d: %s\n",o,sortIN[o]);}
	  //     printf("MN%sNM\n",sortIN);

		int i;
		int c;
		RIN = 0;
		ROUT = 0;
		INBG = 0;
		memset(RINS,'\0',sizeof(RINS));
		memset(ROUTS,'\0',sizeof(ROUTS));
		char lb[3][10];
		strcpy(lb[0],"<");
		strcpy(lb[1],">");
		strcpy(lb[2],"&");
		int sv; sv = 0;
	
		for(i=0;i<count;i++){
			c = strcmp(sortIN[i],lb[0]);	
			if(c==0){
				if(sv==0){sv=i;}
				RIN = 1; 
				strcpy(RINS,sortIN[i+1]);
//				strcpy(sortIN[i],NULL);
				memset(sortIN[i],'\0',sizeof(sortIN[i]));
				}
			c = strcmp(sortIN[i],lb[1]);
			if(c==0){
				if(sv==0){sv = i;}
				ROUT=1; 
				strcpy(ROUTS,sortIN[i+1]);
//				strcpy(sortIN[i],NULL);	
				memset(sortIN[i],'\0',sizeof(sortIN[i]));
				}
			c = strcmp(sortIN[i],lb[2]);
			if(c==0){
				if(sv==0){sv=i;}
				INBG = 1; 
//				strcpy(sortIN[i],NULL);
                                memset(sortIN[i],'\0',sizeof(sortIN[i]));
				
				}
		}
//		printf("FIRST: %s\n", sortIN[0]);
//		printf("2\n");
	//printf("SV %d\n",sv);
          //     printf("MN%sNM\n",sortIN);
		/*Blank Line*/
		if(count == 0){
			freeS(sortIN);
			fc = 1;                        
		}
		char sset[4][10];
		strcpy(sset[0], "exit");
		strcpy(sset[1], "cd");
		strcpy(sset[2], "status");
		strcpy(sset[3], "#");
		char t[10];
		strncpy(t,sortIN[0],1);
//	        printf("6\n");

//				/*input is comments, do nothing*/
		c = strcmp(t, sset[3]);
		if(c==0){
			printf("");
			freeS(sortIN);
			fc=1;}
		size_t ln = strlen(sortIN[0])-1;
		if(sortIN[0][ln] == '\n'){sortIN[0][ln] = '\0';}
				/*exit: set variable to 1 and terminate next loop*/
		c = strcmp(sortIN[0],sset[0]);
		if(c==0){
//			printf("EXIT CALLED\n");
			exx = 1;
			freeS(sortIN);
			fc=1;}
//		printf("5\n");

		c = strcmp(sortIN[0],sset[1]);
                         	/*check for cd's argument, if it could not be opened, it is not exit*/
		if(c==0){
			if(count==1){chdir(cc);}
			else{
	//			printf("IN CD\n");	
				char w[100];
				memset(w,'\0',sizeof(w));
				sprintf(w,"%s",sortIN[1]);
				chdir(w);
				freeS(sortIN);
				fc=1;
			}	
		
		}
//		printf("4\n");
			/*receive status and return value in the variable status*/
		c = strcmp(sortIN[0],sset[2]);
		if(c==0){

//			printf("exit value %d\n",statt);
//			fflush(stdout);
			sar(cc);
			freeS(sortIN);
			fc=1;
		}

			/*run command as fork*/
		if(fc==0){
//				printf("3\n");
			 int st;
                         if(sv==0){st = count;}
                         else{st = sv;}
                         char * sort[st+1];

//			printf("IN EXEC\nRIN: %d\nROUT: %dBG: %d\n",RIN, ROUT, INBG);
			int save = dup(1);
			int sav = dup(0);
			pid_t  spawnpid = -5;
			int childExitMethod = -5;
			spawnpid = fork();
					/*check error*/
			if (spawnpid == -1){
					perror("Hull Breach!");
					saw(1, cc);
			}

			
					/*child process*/
			else if(spawnpid == 0){
//				printf("4\n");
						/*IN FOREGROUND, CHILD BE ABLE TO RECEIVE SIGINT*/
				if(INBG==0 || FF == 1){  
						sigaction(SIGINT, &SIGINT_action, NULL);
					//	sigaction(SIGTSTP, &SIGTSTP_action,NULL);
						}

	
				int fg;
				int ln;
	                        if(ROUT == 1){
	                         			/*check redir, if error, print it and set status 1*/
	                                ln = strlen(ROUTS)-1;
	                                if(ROUTS[ln] == '\n'){ROUTS[ln] = '\0';}
			                               fg = open(ROUTS,O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR);
	                                if(fg < 0){printf("ROUT ERROR\n");fflush(stdout);saw(1, cc);}
					else{dup2(fg, 1);}
	                                }
				if(FF==0){
				 if(ROUT == 0 && INBG == 1 ){
	                                fg = open("/dev/null",O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR);
	                                if(fg < 0){printf("ROUT ERROR\n");fflush(stdout);saw(1, cc);}
					else{dup2(fg, 1);}
		
				}
				}
		
        	                int fo;
                                /*redirect stdin*/
        	                if(RIN == 1){
        	                        ln = strlen(RINS)-1;
  	        	                if(RINS[ln] == '\n'){RINS[ln] = '\0';}
                        	        fo = open(RINS,O_RDONLY | O_CLOEXEC);
					if(fo < 0)
						{printf("COUFD NOT ONPEN FILE\n");
						fflush(stdout);
						saw(1, cc);	
						}
					else{dup2(fo, 0);}
        	                }
				if(RIN == 0 && INBG == 1 && FF == 0){
		                        fo = open("/dev/null",O_RDONLY | O_CREAT | O_CLOEXEC);
	                                if(fo < 0){printf("COUFD NOT ONPEN FILE\n");fflush(stdout);saw(1, cc);}
	                                else{dup2(fo, 0);}
	
				}
				for(sv=0;sv<st;sv++){
					sort[sv] = malloc(1000 * sizeof(char));
					strcpy(sort[sv],sortIN[sv]);
				}
				sort[st] = malloc(1000*sizeof(char));
				sort[st] = NULL;

				saw(0, cc);
				if (execvp(sort[0], sort) < 0)
				{
						//exec fail,set status to 1 by the function
					saw(1, cc);
					perror("Exec failure!");
						//kill the fail child by exit()
					exit(1);
			
			
				}
				else{saw(0,cc);}
					}
			
		
		
				/*parent process*/
			if(INBG == 0 || FF == 1){
			//	printf("Parent\n");
				waitpid(spawnpid,&childExitMethod,0);
			//	printf("WAIT\n");
			
	                	dup2(save,1);
		                close(save);
				dup2(sav,0);
				close(sav);
		/*                if(ROUT == 1){close(fg);}
				if(RIN == 1){close(fo);*/
			//	printf("SAVE\n");

	
					/*check child status*/

		
				if (WIFEXITED(childExitMethod)!=0)
				{	
					//if exec fail and capture exit 1, status = 1;
				//	printf("%d\n",$return);	
				//	saw(0);
					//printf("exec success, 0\n");
				}		
				else{ 
					saw(1, cc);
					printf("exec fail, 1\n");
				}
				if (WIFSIGNALED(childExitMethod) != 0){
					int termSignal = WTERMSIG(childExitMethod);
					printf("TERMINATED BY SIGNAL %d\n",termSignal);
				}
				freeS(sortIN);
			//	printf("HERE\n");
				//ffreeS(sort,st);
			//	printf("WHERE\n");
//		
			}
			
			else{
//				printf("IN BG\n");
//				cbgc=addbgc(bgc,cbgc,spawnpid);
			        printf("BACKGROUND PROCESS%d STARTING\n",spawnpid);
			        int ff = 0;
			        int ii;
			        ii = 0;
			        while(ff ==0 && i < cbgc){
			                if(bgc[i] ==0){
			                        bgc[i] = spawnpid;
                        	
						ff = 1;
               			 	}
               				 ii++;
		                }
			        if(ff==0)
					{bgc[cbgc] = spawnpid;
					cbgc++;}

//				printf("After\n");
/*				dup2(save,1);
                                close(save);
                                dup2(sav,0);
                                close(sav);
*/				freeS(sortIN);
		//		ffreeS(sort,st);
//				printf("IN GB\n");
		
			
				}
	


		}




	}




		//kill all background process exit called


                if(cbgc > 0){
                int io;
                pid_t spawn = -5;
                int childExit = -5;
                int y;
                                /*check for every child process*/
                for(io=0;io<cbgc;io++){
                	if(bgc[io] != 0){
                        	spawn = waitpid(bgc[io], &childExit, 0);
	                        y = WIFEXITED(childExit);
        	                printf("PROCESS%d TERMINATE\nEXIT STATUS %d\n",bgc[io],y);
                	               bgc[io] = 0;
                        }
                       }  }

return 0;
}
                                                                                       
