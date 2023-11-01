#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<stdbool.h>
#include<string.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

char** RAM;
char** disk;
struct PTE* table;
int pages;
int frames;
int disk_accesses = 0;

struct PTE{
	bool valid;
	int frame;
	bool dirty;
	int referenced;	
	
};


int find_page_in_table(int page_to_check){
	for(int i = 0; i < pages; ++i){
		if(table[i].frame == page_to_check){
			return i;
		}
	}
	
	return -1;
}

int find_page_asked_by_mmu(){
	int index = -1;
	for(int i = 0; i < pages; ++i){
		if(table[i].referenced != 0){
			index = i;
		}
	}
	
	return index;

}

int free_frame(){
	for(int i = 0; i < frames; ++i){
		if(strlen(RAM[i])==0){
			return i;
		}
	}
	
	return -1;

}


void signal_handler(int sig){
	
	int found = find_page_asked_by_mmu();
	
	
	
	if(found!=-1){
		int mmu_pid = table[found].referenced;
		printf("A disk access request from MMU Process(pid=%d)\n", mmu_pid);
		printf("Page %d is referenced\n", found);
		int free_frame_found = free_frame();
		if(free_frame_found==-1){
			printf("We do not have free frames in RAM\n");
			free_frame_found = rand()%frames;
			printf("%d<---free\n", free_frame_found);
			int related_page_in_table = find_page_in_table(free_frame_found);
			printf("Chose a random victim page %d\n", related_page_in_table);
			printf("Replace/Evict it with page %d to be allocated to frame %d in RAM\n", found, free_frame_found);
			if(table[related_page_in_table].dirty){
				strcpy(disk[related_page_in_table], RAM[free_frame_found]);
				disk_accesses++;
				table[related_page_in_table].dirty = false;
			}
			table[related_page_in_table].valid = false;
			table[related_page_in_table].referenced = 0;
			table[related_page_in_table].frame=-1;
		} else{
			printf("We can allocate to free frame %d\n", free_frame_found);
		}
		printf("Copy data from the disk(page=%d) to RAM(frame=%d)\n", found, free_frame_found);
		disk_accesses++;
		strcpy(RAM[free_frame_found], disk[found]);
		table[found].valid = true;
		table[found].referenced = false;
		table[found].dirty = false;
		table[found].frame=free_frame_found;
		printf("RAM array\n");
		
		for(int i = 0; i < frames; ++i){
		printf("Frame %d ----> %s\n", i, RAM[i]);
	        }
	        
		printf("disk acceses is %d so far\n", disk_accesses);
		printf("Resume MMU Process\n---------------------\n");
		kill(mmu_pid, SIGCONT);
	} else{
		printf("Total disk accesses: %d\n", disk_accesses);
		munmap(table, pages*sizeof(struct PTE));
		for(int i = 0; i < frames; ++i){
			free(RAM[i]);
		}
		free(RAM);
		for(int i = 0; i < pages; ++i){
			free(disk[i]);
		}
		free(disk);
		printf("Pager is terminated\n");
		char c;
		scanf("%c",&c); //Waiting enter to close terminal
		raise(SIGTERM);
	}
}

int main(int argc, char* argv[]){
	signal(SIGUSR1, signal_handler);
	pages = atoi(argv[1]);
	frames = atoi(argv[2]);
	mkdir("/tmp/ex2", 0700);
	
	FILE* f = fopen("/tmp/ex2/page_table", "w+");
	if(f==NULL){
		perror("Cannot open or create file");
		exit(EXIT_FAILURE);
	}
	fclose(f);
	int frame_table = open("/tmp/ex2/page_table", O_RDWR);
	
	if(frame_table == -1){
		perror("Something wrong with table file");
		exit(EXIT_FAILURE);
	}
	FILE* random = fopen("/dev/random","r");
	if(random == NULL){
		perror("Cannot open /dev/random");
		exit(EXIT_FAILURE);
	}
	
	RAM = (char**)malloc(sizeof *RAM*frames);
	if(RAM == NULL){
		perror("Cannot allocate memory:");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < frames; ++i){
		RAM[i] = malloc(sizeof *RAM[i]*9);
		if(RAM[i] == NULL){
			perror("Cannot allocate memory:");
			exit(EXIT_FAILURE);
		}
	}
	disk = (char**)malloc(sizeof *disk*pages);
	if(disk == NULL){
		perror("Cannot allocate memory:");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < pages; ++i){
		disk[i] = malloc(sizeof *disk[i]*9);
		if(disk[i] == NULL){
			perror("Cannot allocate memory:");
			exit(EXIT_FAILURE);
		}
		int j = 0;
		while(j < 8){
			fscanf(random, "%c", &(disk[i][j]));
			if(isprint(disk[i][j])){
				j++;
			}
		}
		disk[i][8] = '\0';
	}
	fclose(random);
	
	ftruncate(frame_table, pages*sizeof(struct PTE));
	table = (struct PTE*)mmap(NULL, pages*sizeof(struct PTE), PROT_READ|PROT_WRITE, MAP_SHARED, frame_table, 0);
	if(table == NULL){
		perror("Cannot map the table");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < pages; ++i){
		table[i].valid = false;
		table[i].frame = -1;
		table[i].dirty = false;
		table[i].referenced = 0;
	}
	printf("Initialized table\n-----------------------------------\n");
	for(int i = 0; i < pages; ++i){
		printf("Page %d ----> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, table[i].valid, table[i].frame, 	
		table[i].dirty, table[i].referenced);
	}
	printf("Initialized RAM\n-----------------------------------\n");
	for(int i = 0; i < frames; ++i){
		printf("Frame %d ----> %s\n", i, RAM[i]);
	}
	printf("Initialized disk\n-----------------------------------\n");
	for(int i = 0; i < pages; ++i){
		printf("Page %d ----> %s\n", i, disk[i]);
	}
	printf("Initialized table\n-----------------------------------\n");
	while(true){
		
	}
	
	
	return 0;
}
