#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<stdbool.h>
#include<string.h>
#include<sys/mman.h>
#include<fcntl.h>

struct PTE* table;
int pages;


struct PTE{
	bool valid;
	int frame;
	bool dirty;
	int referenced;	
	
};

int main(int argc, char* argv[]){
	pages = atoi(argv[1]);
	int pager_pid = atoi(argv[argc-1]);
	int frame_table = open("/tmp/ex2/page_table", O_RDWR);
	if(frame_table==-1){
		perror("Cannot open file:");
		exit(EXIT_FAILURE);
	}
	table = (struct PTE*)mmap(NULL, pages*sizeof(struct PTE), PROT_READ|PROT_WRITE, MAP_SHARED, frame_table, 0);
	if(table==NULL){
		perror("Cannot map the table:");
		exit(EXIT_FAILURE);
	}
	printf("Initialized table\n-----------------------------------\n");
	for(int i = 0; i < pages; ++i){
			printf("Page %d ----> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, table[i].valid, table[i].frame, 
			table[i].dirty, table[i].referenced);
	}
	printf("--------------------------------------------\n");
	for(int i = 2; i < argc-1; ++i){
		char operation = argv[i][0];
		int page_num = argv[i][1] - '0';
		if(operation == 'W'){
			printf("Write request for Page %d\n", page_num);
			table[page_num].dirty = true;
		} else{
			printf("Read request for Page %d\n", page_num);
		}
		if(!table[page_num].valid){
			printf("It is not a valid page ---> page fault\n");
			printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
			table[page_num].referenced = getpid();
			kill(pager_pid, SIGUSR1);
			raise(SIGSTOP);
			printf("MMU Process resumed\n");
		}else{
			printf("It is a valid page\n");
		}
		printf("Page table\n");
		
		for(int i = 0; i < pages; ++i){
			printf("Page %d ----> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, table[i].valid, table[i].frame, 
			table[i].dirty, table[i].referenced);
	}
		printf("--------------------------------------------\n");
	}
	kill(pager_pid, SIGUSR1);
	munmap(table, pages*sizeof(struct PTE));
	close(frame_table);
	printf("Terminated\n");
	char c;//Waiting for enter to close
	scanf("%c",&c);
	return 0;
}
