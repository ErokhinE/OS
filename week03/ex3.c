#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX_NAME 63
#define MAX_PATH 2048
#define MAX_DATA 1024
#define MAX_COUNT 255
struct File{
    int id;
    char name[MAX_NAME+1];
    int size;
    char data[MAX_DATA+1];
    struct Directory *directory;
};
struct Directory{
    char name[MAX_NAME+1];
    unsigned char nd;
    unsigned char nf;
    struct File files[MAX_COUNT];
    struct Directory* sub_dir[MAX_COUNT];
    char path[MAX_PATH+1];
};
void overwrite_to_file(struct File* file, const char* str){
    strcpy(file->data,str);
    file->size = strlen(str);
    
} 
void append_to_file(struct File* file, const char* str){
    strcat(file->data,str);
    file->size = strlen(file -> data);
}
void printp_file(struct File* file){
    if (strcmp(file -> directory -> path,"/")){
        printf("%s%s%s\n",file -> directory ->path,"/",file->name);
    }
    else {
        printf("%s%s\n","/",file->name);
    }
}
void add_file(struct File* file, struct Directory *dir){
    dir -> files[dir -> nf] = *file;
    dir -> nf++;
    file -> directory = dir;
}
void add_dir(struct Directory *dir1, struct Directory *dir2){
    if (dir1 && dir2) {
        dir2->sub_dir[dir2->nd++] = dir1;
        char temp_path[MAX_PATH + 1];
        if (strcmp(dir2->path, "/")) {
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
        else {
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}
void show_file(struct File *file) {
    printf("%s ", file->name);
}
void show_dir(struct Directory *dir)
{
    printf("\nDIRECTORY\n");
    printf(" path: %s\n", dir->path);
    printf(" files:\n");
    printf(" [ ");
    for (int i = 0; i < dir->nf; i++){
        show_file(&(dir->files[i]));
    }
    printf("]\n");
    printf(" directories:\n");
    printf(" { ");
    for (int i = 0; i < dir->nd; i++){
        show_dir(dir->sub_dir[i]);
    }
    printf("}\n");
}
int main(){
    struct Directory root;
    strcpy(root.name,"/");
    strcpy(root.path,"/"); 
    root.nf = 0;
    root.nd = 0;
    struct Directory home;
    strcpy(home.name,"home"); 
    home.nf = 0;
    home.nd = 0;
    struct Directory bin;
    strcpy(bin.name,"bin"); 
    bin.nf = 0;
    bin.nd = 0;
    add_dir(&home,&root);
    add_dir(&bin,&root);
    struct File bash;
    strcpy(bash.name,"bash"); 
    add_file(&bash,&bin);
    struct File ex3_1;
    strcpy(ex3_1.name,"ex3_1.c"); 
    ex3_1.id = 1;
    struct File ex3_2;
    strcpy(ex3_2.name,"ex3_2.c"); 
    ex3_2.id = 2;
    add_file(&ex3_1,&home);
    add_file(&ex3_2,&home);
    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);");
    overwrite_to_file(&ex3_2, "//This is a comment in C language");
    overwrite_to_file(&bash,"Bourne Again Shell!!");
    append_to_file(&ex3_1,"int main(){printf('Hello World!')}");
    printp_file(&ex3_1);
    printp_file(&ex3_2);
    printp_file(&bash);
    // show_dir(&root);
    
    

}


