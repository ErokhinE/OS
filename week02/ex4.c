#include <stdio.h>
#include <ctype.h>
#include <string.h>
int count(char char1,char strNew[257]){
  int counter = 0;
  int tempForCountString = strlen(strNew)-2;
  while(tempForCountString>=0){
      if(char1 == strNew[tempForCountString]){
          counter++;
         
      }
      tempForCountString--;
  }
  return counter;
}
void countAll(char strNew[257]){
    for (int i=0;i<strlen(strNew)-1;i++){
        strNew[i] = tolower(strNew[i]);
        
        
    }
    
    
    for (int i=0;i<strlen(strNew)-1;i++){
        int match = count(strNew[i],strNew);
        printf("%c:",strNew[i]);
        printf("%d ",match);
    }
}
    

int main(){
    char string[257];
    fgets(string, sizeof(string),stdin);
    countAll(string);

    
    return 0;
}

