#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<dirent.h>




int main(){
	char *buffer;
	size_t bufsize =32;
	size_t characters;
	buffer = (char *)malloc(bufsize*sizeof(char));

	while(true)
	{
		printf("SergioC&: ");
		characters = getline(&buffer,&bufsize , stdin);
		//printf("%zu",characters);
		//printf("%s",buffer);
		if(buffer[0] == 'p'&& buffer[1]=='w' && buffer[2]=='d' ){
	
			char *dir;
			size_t dirsize = 32;
			dir = (char *)malloc(dirsize*sizeof(char));

			dir =   getcwd(dir,dirsize);


			printf("%s \n",dir);
		}
		if(buffer[0]=='m' && buffer[1]=='k' && buffer[2]=='d' && buffer[3]=='i' && buffer[4]=='r'){
			char *arg;
			size_t argsize = characters-7;
			arg = (char *)malloc(argsize*sizeof(char));
			for(int i=0;i<argsize;i++){
				arg[i]=buffer[6+i];
			}
		mkdir(arg,S_IRWXU);

		}
		if(buffer[0]=='c' && buffer[1]=='d'){
			char *arg;
			size_t argsize = characters-4;
			arg = (char *)malloc(argsize*sizeof(char));
			for(int i=0;i<argsize;i++){
				arg[i]=buffer[3+i];
			}
		chdir(arg);
		}

		if(buffer[0]=='r' && buffer[1]=='m'){
			char *arg;
			size_t argsize = characters-4;
			arg = (char *)malloc(argsize*sizeof(char));
			for(int i=0;i<argsize;i++){
				arg[i]=buffer[3+i];
			}
		unlink(arg);
		}
		if(buffer[0]=='n'&& buffer[1]=='a' && buffer[2]=='n' && buffer[3]=='o'){
			char *arg;
			size_t argsize = characters-6;
			arg = (char *)malloc(argsize*sizeof(char));
			for(int i =0; i<argsize;i++){
				arg[i] = buffer[5+i];
			}
			char *arg1[3]={"/bin/nano",arg,NULL};
			

			if(fork()==0)
			{
				
				execvp("/bin/nano",arg1);
//				exit(0);

			}else{
			
				wait(NULL);
			}

		}
		if(buffer[0]=='c' && buffer[1]=='a' && buffer[2]=='t'){
		
			char *arg;
			size_t argsize = characters-5;
			arg=(char *)malloc(argsize*sizeof(char));
			for(int i =0; i<argsize;i++){
			
				arg[i]=buffer[4+i];
			}
			char *arg1[3]={"/bin/cat",arg,NULL};

			if(fork()==0){
			
				execvp("/bin/cat",arg1);
			}else{
				wait(NULL);
			}
		}

		if(buffer[0]=='l' && buffer[1]=='s' ){
			DIR *dir;
			struct dirent *ti;
			dir = opendir(".");
			int i =1;
			while(ti=readdir(dir)){
				printf("%s \t \t",ti->d_name);
				if((i%3)==0){
					printf("\n");
				}
				i++;
			}	
			printf("\n");
		}

	}
	
}
