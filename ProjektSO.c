#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <pthread.h>
//sigprocmask dodac

#define buffsize 10

struct sigaction defaction;
struct sigaction sapmstart;
struct sigaction sap2start;
struct sigaction sap1usr1;
struct sigaction sap2usr1;
struct sigaction sap3usr1;
struct sigaction saignore13;
int flag, p1flag, p2flag, p3flag, p1fin, p2fin, p3fin;

#define SHM_KEY 0x1267
#define SHM_KEY2 0x1268
#define K1KEY 0x1111
int sig;

void zeruj(char *tab){
    memset(tab, 0, 4095*sizeof(char));
    /*for(int i = 0; i < 4096; i++){
      tab[i] = '\0';
    }*/
    
}


//char* buffer;
//char* ch;
char* bufptr;
//kolejka
int *sharedbuff;
sem_t* sems;
 char* pmowi;
void dtsemsandbuff(){
	shmdt(sems);
	shmdt(sharedbuff);
}

pid_t pids[4];
void hp2start(int signo, siginfo_t *si, void *data) {
  /*if(si->si_pid==getpid()){
	  sigaction(signo,&defaction,NULL);
	  raise(signo);
	  
	  
  }else*/
  
  kill(getppid(),si->si_signo);
  
}
void pmsigstpcnt(int signo, siginfo_t *si, void *data){
	
if(si->si_pid==pids[2]){
//zapis do dzielonej

sem_wait(&sems[0]);
sem_wait(&sems[0]);
sem_wait(&sems[0]);
*sharedbuff=signo;
sem_post(&sems[1]);
sem_post(&sems[2]);
sem_post(&sems[3]);


kill(pids[1],SIGUSR1);
if(signo==SIGTSTP){
	
}else if(signo==SIGCONT){
}
else if(signo==SIGTERM){
	flag=0;
	}


}
}
void husr1p1(int signo, siginfo_t *si, void *data){
	
	if(si->si_pid==getppid()){
		sem_wait(&sems[1]);
		sig=*sharedbuff;
		sem_post(&sems[0]);
		if(sig==SIGTSTP){
			kill(pids[2],SIGUSR1);
			p1flag=0;
			}else if(sig==SIGCONT){
				kill(pids[2],SIGUSR1);
			p1flag=1;
			}else if(sig==SIGTERM){
				p1flag=1;
				p1fin=1;
			}
		
		
		
		}
		
		
		
		
		
	
	
}
void husr1p2(int signo, siginfo_t *si, void *data){
	//int sig;
	if(si->si_pid==pids[1]){
		sem_wait(&sems[2]);
		sig=*sharedbuff;
		sem_post(&sems[0]);
		if(sig==SIGTSTP){
			kill(pids[3],SIGUSR1);
			p2flag=0;
			}else if(sig==SIGCONT){
				kill(pids[3],SIGUSR1);
			p2flag=1;
			}else if(sig==SIGTERM){
				p2flag=1;
				p2fin=1;
			}
		//odczyt sygnalu z pamieci dzielonej
		//if(sigodcz==SIGTERM){
		//dokoncz pobieranie i wysylanie
		//sigaction(sigodcz,&defaction,NULL);
		//raise(sigodcz);
		//}
		//else if(sigodcz==SIGCONT)sigaction(SIGTSTP,&sap2start,NULL);
		//else if(sigodcz==SIGTSTP)sigaction(SIGCONT,&sap2start,NULL);
		//fprintf(stderr,"p2 usr1");
		//wyslanie sygnalu do p3
		
		//sig=*sharedbuff;
		//if(sig==SIGTSTP)printf("usr1 w p2 stp\n");
		
		
		//wykonanie na sobie sygnalu z pamieci
		//sigaction(sigodcz,&defaction,NULL);
		//raise(sigodcz);
		
	}
	
}
void husr1p3(int signo, siginfo_t *si, void *data){
	//int sig;
	if(si->si_pid==pids[2]){
		sem_wait(&sems[3]);
		sig=*sharedbuff;
		sem_post(&sems[0]);
		if(sig==SIGTSTP){
			
			p3flag=0;
			}else if(sig==SIGCONT){
			p3flag=1;
			}else if(sig==SIGTERM){
				
				p3flag=1;
				p3fin=1;
				
			}
		//odczyt sygnalu z pamieci dzielonej
		//if(sigodcz==SIGTERM){
		//dokoncz pobieranie i wysylanie
		//sigaction(sigodcz,&defaction,NULL);
		//raise(sigodcz);
		//}
		//if(sigodcz==SIGCONT)sigaction(SIGTSTP,&saignore13,NULL);
		//else if(sigodcz==SIGTSTP)sigaction(SIGCONT,&saignore13,NULL);
		//wykonanie na sobie sygnalu z pamieci
		//sigaction(sigodcz,&defaction,NULL);
		//raise(sigodcz);
		
		
	}
	
}
void hignorestart13(int signo, siginfo_t *si, void *data){
	
}



int main(void) {
	pmowi="PM handler";
	sigset_t p1p3set,pmset,p2set;
	sigfillset(&p1p3set);
	sigfillset(&pmset);
	sigfillset(&p2set);
	sigdelset(&p1p3set,SIGUSR1);
	sigdelset(&pmset,SIGTERM);
	sigdelset(&pmset,SIGCONT);
	sigdelset(&pmset,SIGTSTP);
	sigdelset(&p2set,SIGTERM);
	sigdelset(&p2set,SIGCONT);
	sigdelset(&p2set,SIGTSTP);
	sigdelset(&p2set,SIGUSR1);
	
	char* nline="\n";
	
	int qid;
	//sems=malloc(4*sizeof(sem_t));
	flag=1;
	p1flag=1;
	p2flag=1;
	p3flag=1;
	p1fin=0;
	p2fin=0;
	p3fin=0;
	int shmid,shmbuffid;
	
	sem_t sem0,sem1,sem2,sem3;
	sem_init(&sem0,1,3);
   sem_init(&sem1,1,0);
   sem_init(&sem2,1,0);
   sem_init(&sem3,1,0);
   shmid = shmget(SHM_KEY, sizeof(sem_t)*4, 0644|IPC_CREAT);
   shmbuffid=shmget(SHM_KEY2, sizeof(int), 0644|IPC_CREAT);
   if (shmid == -1) {
      perror("Shared memory");
      
      return 1;
   }
   if (shmbuffid == -1) {
      perror("Shared memory");
      
      return 1;
   }
	sems = shmat(shmid, NULL, 0);
	
	sems[0]=sem0;
  sems[1]=sem1;
  sems[2]=sem2;
  sems[3]=sem3;
  sharedbuff=shmat(shmbuffid,NULL,0);
  //sharedbuff=malloc(sizeof(int));  
    //deklaracje kolejki
  int k1 = msgget(K1KEY, IPC_CREAT|0666);

  //struktura dla kolejki
    struct kbufstr {

        long typ; // typ komunikatu
        char tekst[4096]; // dane komunikatu (dowolna długość i interpretacja)
    } kol1;
	
	char *ch=malloc(sizeof(char));
	char *buffer=malloc(sizeof(char)*(buffsize));
	bufptr=buffer;
	
	
	
	int     fdMto1[2],fdMto2[2],fdMto3[2];
	saignore13.sa_flags = SA_SIGINFO;
	saignore13.sa_sigaction=hignorestart13;
	defaction.sa_handler=SIG_DFL;
	 if (pipe(fdMto1) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
           }
       if (pipe(fdMto2) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
           }
       if (pipe(fdMto3) == -1) {
               perror("pipe");
               exit(EXIT_FAILURE);
           }
  pids[0]=getpid();
  memset(&sapmstart, 0, sizeof(sapmstart));
  sapmstart.sa_flags = SA_SIGINFO;
  sapmstart.sa_sigaction = pmsigstpcnt;
  
  
  pids[1]=fork();
  
  
  if(pids[1]==0){
	  
	  int errintr=0;
	  int first=1;
	  int stop=0;
		int fd;
		int nbytes;
		int stpd=0;
		int menufirst=1;
		int stdinbytes;
	  char* choice=malloc(sizeof(char));
	  char* napis="Menu:\n1. Plik\n2. Stdin\n";
	 char* wykonane="wykonane\n";
	 char* nazwaPliku=malloc(sizeof(char)*200);
	 char* podajnazwe="Podaj nazwe pliku: ";
	 char* brakPliku="Taki plik nie istnieje.\n";
	 char* bladOtw="Blad przy otwarciu pliku\n";
	 sigprocmask(SIG_BLOCK,&p1p3set,NULL);
	  close(fdMto1[1]);
	  close(fdMto2[0]);
	  close(fdMto2[1]);
	  close(fdMto3[0]);
	  close(fdMto3[1]);
	  read(fdMto1[0],pids,sizeof(pid_t)*4);
	  close(fdMto1[0]);
	  memset(&sap1usr1, 0, sizeof(sap1usr1));
  sap1usr1.sa_flags = SA_SIGINFO;
  sap1usr1.sa_sigaction = husr1p1;
	  if (sigaction(SIGUSR1, &sap1usr1, NULL) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));}
    if (sigaction(SIGTERM, &saignore13, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
    if (sigaction(SIGCONT, &saignore13, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
    if (sigaction(SIGTSTP, &saignore13, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
  
 while(1){
	 if(p1fin){
		 
		 kill(pids[2],SIGUSR1);
		 dtsemsandbuff();
		  
		 raise(SIGKILL);
		 }
	 errno=0;
	 errintr=0;
	 
	 stop=0;
	 int i=0;
	 
	 
	 if(p1flag&&menufirst){write(1,napis,strlen(napis));
		 menufirst=0;}

	read(0,choice,sizeof(char));   
	if(errno==EINTR){
	errintr=1;
	}
		 
			
	 
	 fcntl(0,F_SETFL,O_NONBLOCK);
	 while(read(0,ch,sizeof(char))>0){}		 
	 fcntl(0,F_SETFL,NULL);
	 
	 if(p1fin){
		 
		 kill(pids[2],SIGUSR1);
		 dtsemsandbuff();
		  
		 raise(SIGKILL);
		 
		 
		 }
	 if(*choice=='1'&&!errintr&&p1flag){
		 write(1,podajnazwe,strlen(podajnazwe));
		 do{
		 errno=0;
		 
		 while(i<200&&read(0,ch,sizeof(char))>0&&*ch!='\n'){
			 nazwaPliku[i]=*ch;
			 i++;
		 }
		 nazwaPliku[i]='\0';
		write(1,nazwaPliku,sizeof(char)*i+1);
		 i=0;
		 if(p1fin==1){
			 kill(pids[2],SIGUSR1);
			 dtsemsandbuff();
			 
			 raise(SIGKILL);}
		 }while(errno==EINTR||!p1flag);
		 
		 fd=open(nazwaPliku,O_RDONLY);
		   


		 if(fd==-1){
			if(errno==ENOENT){
				write(1,brakPliku,strlen(brakPliku));
				menufirst=1;
			}else{
				write(1,bladOtw,strlen(bladOtw));
				menufirst=1;
			}
		 }else{
		 while(!stop){
		errno=0;
		 while(p1flag&&i<buffsize&&(nbytes=read(fd,ch,sizeof(char)))>0&&*ch!='\n'){
			 buffer[i]=*ch;
			 i++;
		 }
		
		 while(!p1flag){
			 //printf("To tu\n");
			 pause();
			 //printf("Za suspendem\n");
		 }
		
		 if(errno!=EINTR&&p1flag){
		 
		 //write(1,buffer,sizeof(char)*i);//zamiast tego wyslanie buffera do p2 kolejka
		  zeruj(kol1.tekst);
      strcpy(kol1.tekst, buffer);
      kol1.typ = 112;
      msgsnd(k1, &kol1, sizeof(char)*i, 0);
      
		 if(*ch=='\n'){
			 //write(1,ch,sizeof(char)); //wyslac *ch do p2 jako info o koncu wiersza
			 zeruj(kol1.tekst);
      kol1.tekst[0] = *ch;
      kol1.typ = 112;
      msgsnd(k1, &kol1, sizeof(char), 0);
      
			 }
			 i=0;
		
		}
		if(nbytes==0){
			stop=1;
			menufirst=1;
			
		}
		
		}}
		 close(fd);
		 i=0;
		 
	 
		 
	 }//koniec wyboru 1 (plik)
	 else if(*choice=='2'&&!errintr&&p1flag){
	 
		
	
	 while(!stop){
		 
		 /*if(!p1flag){
			 read(0,ch,sizeof(char));
			 fcntl(0,F_SETFL,O_NONBLOCK);
			while(read(0,ch,sizeof(char))>0){}		 
			fcntl(0,F_SETFL,NULL);
		 }*/
		 if(first&&p1flag){
			do{
				stpd=0;
				errno=0;
				read(0,ch,sizeof(char));
				
				if(errno==EINTR)stpd=1;
				if(stpd&&p1fin){
					kill(pids[2],SIGUSR1);
					dtsemsandbuff();
					 
					raise(SIGKILL);
					}
				while(!p1flag){
		pause();
		}}while(stpd);
		 if(*ch=='.'){
			 
			read(0,ch,sizeof(char));
			if(*ch=='\n'){
				fcntl(0,F_SETFL,O_NONBLOCK);
	 while(read(0,ch,sizeof(char))>0){}		 
	 fcntl(0,F_SETFL,NULL);
		menufirst=1;
			break;
			}else{
			buffer[0]='.';
			buffer[1]=*ch;
			i=2;
			first=0;
			}
	}else{
		
	buffer[0]=*ch;
	i=1;
	first=0;
	}}
		//read(0,choice,sizeof(char));
		errno=0;
		
		//sleep(1);//debug
		//printf("Menu:\n1. Plik\n2. Stdin\n");
		while(p1flag&&i<buffsize&&(stdinbytes=read(0,ch,sizeof(char)))>0&&*ch!='\n'){
		
		buffer[i]=*ch;
		
		//printf(".\n");
		i++;
		}
		if(p1fin&&stdinbytes<0){
			fcntl(0,F_SETFL,NULL);
			kill(pids[2],SIGUSR1);
			dtsemsandbuff();
			 
			raise(SIGKILL);
			}
		while(!p1flag){
		pause();
		}
		if(p1flag&&errno!=EINTR){
		//write(1,buffer,sizeof(char)*i);//zamiast tego wysylanie kolejka do p2
		//write(1,wykonane,strlen(wykonane));//debug
		zeruj(kol1.tekst);
    strcpy(kol1.tekst, buffer);
    //printf("test:\n");
    //write(1, kol1.tekst, sizeof(char)*i);
    kol1.typ = 112;
    msgsnd(k1, &kol1, sizeof(char)*i, 0);
		i=0;
		}
		if(*ch=='\n'){
			//write(1,ch,sizeof(char));
			zeruj(kol1.tekst);
      kol1.tekst[0] = *ch;
      kol1.typ = 112;
      msgsnd(k1, &kol1, sizeof(char), 0);
			//tutaj wyslanie \n do p2 jako sygnalu do konca odczytu
			first=1;
			if(p1fin){
				fcntl(0,F_SETFL,O_NONBLOCK);
				}
			}
		
	}}//koniec wyboru 2 (stdin)
	
		
	
  }//koniec while(1)
  exit(1);
}
  pids[2]=fork();
  if(pids[2]==0){
	  int inter=0;
	  
	  long int tmp = 0;
	  sigprocmask(SIG_BLOCK,&p2set,NULL);
	  close(fdMto2[1]);
	  close(fdMto1[0]);
	  close(fdMto1[1]);
	  close(fdMto3[0]);
	  close(fdMto3[1]);
	  read(fdMto2[0],pids,sizeof(pid_t)*4);
	  close(fdMto2[0]);
	  
  memset(&sap2start, 0, sizeof(sap2start));
  sap2start.sa_flags = SA_SIGINFO;
  sap2start.sa_sigaction = hp2start;
  memset(&sap2usr1, 0, sizeof(sap2usr1));
  sap2usr1.sa_flags = SA_SIGINFO;
  sap2usr1.sa_sigaction = husr1p2;
  if (sigaction(SIGTERM, &sap2start, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
  if (sigaction(SIGTSTP, &sap2start, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
  if (sigaction(SIGCONT, &sap2start, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
  if (sigaction(SIGUSR1, &sap2usr1, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
  
  
  printf("ID P2: %d\n",pids[2]);
  char* specialp2="koniec p2 przed sigkill:";
  char tekstostatni[4096];
  while(1){
	  
	 
	  
	  
	
		 if(p2fin){
			 while(1){
				 
				 zeruj(kol1.tekst);
				 msgrcv(k1, &kol1, 4096, 112, IPC_NOWAIT);
				 
				 if(errno==ENOMSG){
					 
					 
					 kill(pids[3],SIGUSR1);
					 dtsemsandbuff();
					  
					 raise(SIGKILL);
					 }
				 if(kol1.tekst[0] != '\n'){
					tmp = tmp + strlen(kol1.tekst) ;
				 }
				 else{
					kol1.typ = 123;
					zeruj(kol1.tekst);
					snprintf(kol1.tekst, sizeof(kol1.tekst), "%li", tmp);
					tmp = 0;
					
					msgsnd(k1, &kol1, 4096, 0);
					
    }
		 }}
		 zeruj(kol1.tekst);
		 
		 do{
			 inter=0;
			 errno=0;
    msgrcv(k1, &kol1, 4096, 112, 0);
    if(errno==EINTR)inter=1;
	if(inter&&p2fin){
		
		kill(pids[3],SIGUSR1);
		dtsemsandbuff();
		 
		raise(SIGKILL);
	}
    while(!p2flag){
	pause();
	}
	if(inter&&p2fin){
		
		kill(pids[3],SIGUSR1);
		dtsemsandbuff();
		 
		raise(SIGKILL);
	}
}while(inter);
    if(kol1.tekst[0] != '\n'){
    tmp = tmp + strlen(kol1.tekst) ;
    }
    else{
      kol1.typ = 123;
      zeruj(kol1.tekst);
      snprintf(kol1.tekst, sizeof(kol1.tekst), "%li", tmp);
      tmp = 0;
      
      msgsnd(k1, &kol1, 4096, 0);
     
    }
	  
	  
	  
  }
  
  }
  pids[3]=fork();
  if(pids[3]==0){
	  int inter=0;
	  sigprocmask(SIG_BLOCK,&p1p3set,NULL);
	  close(fdMto3[1]);
	  close(fdMto2[0]);
	  close(fdMto2[1]);
	  close(fdMto1[0]);
	  close(fdMto1[1]);
	  read(fdMto3[0],pids,sizeof(pid_t)*4);
	  close(fdMto3[0]);
	  if (sigaction(SIGTERM, &saignore13, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
    if (sigaction(SIGCONT, &saignore13, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
    if (sigaction(SIGTSTP, &saignore13, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
	  memset(&sap3usr1, 0, sizeof(sap3usr1));
  sap3usr1.sa_flags = SA_SIGINFO;
  sap3usr1.sa_sigaction = husr1p3;
	   if (sigaction(SIGUSR1, &sap3usr1, 0) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
	  while(1){
	  
	  if(p3fin){
		  
		  while(1){
			  msgrcv(k1, &kol1, 4096, 123, IPC_NOWAIT);
			  
			  if(errno==ENOMSG){
				  dtsemsandbuff();
				   
				  raise(SIGKILL);
			  }
			  write(1, kol1.tekst, 4096);
				 write(1, nline, strlen(nline));
			  }
		  }
		  do{inter=0;
			  errno=0;
			  
		  msgrcv(k1, &kol1, 4096, 123, 0);
		  
		  if(errno==EINTR)inter=1;
		  if(inter&&p3fin){
			  dtsemsandbuff();
			   
		  raise(SIGKILL);
		  }
		  while(!p3flag){
			  pause();
		  }
		  if(inter&&p3fin){
			  dtsemsandbuff();
			
		  raise(SIGKILL);
		  }
		  }while(inter);
		  
    write(1, kol1.tekst, 4096);
    write(1, nline, strlen(nline));
	  }
	  }
	  sigprocmask(SIG_BLOCK,&pmset,NULL);
	  close(fdMto1[0]);
	  close(fdMto2[0]);
	  close(fdMto3[0]);
	  write(fdMto1[1],pids,4*sizeof(pid_t));
	  write(fdMto2[1],pids,4*sizeof(pid_t));
	  write(fdMto3[1],pids,4*sizeof(pid_t));
	  close(fdMto3[1]);
	  close(fdMto2[1]);
	  close(fdMto3[1]);
	  if (sigaction(SIGTERM, &sapmstart, NULL) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
  
  if (sigaction(SIGTSTP, &sapmstart, NULL) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
  if (sigaction(SIGCONT, &sapmstart, NULL) == -1) {
    fprintf(stderr, "%s: %s\n", "sigaction", strerror(errno));
  }
 
  
  
  /*while(1){
	  sleep(10);
	  
  }*/
  int finished=0;
  
  while(flag||finished!=3){
	finished=0;
  waitpid(pids[1],NULL,0);
  if(errno==ECHILD){
	  
	  finished++;
	  }
  errno=0;
  waitpid(pids[2],NULL,0);
  if(errno==ECHILD){finished++;}
  errno=0;
  waitpid(pids[3],NULL,0); 
  if(errno==ECHILD){finished++;}
  errno=0;
  }
  msgctl(k1,IPC_RMID,NULL);
  
  
  
  
  return 0;
}
