#include <stdio.h>
#include <string.h>


int main()
{
    char str[257];
    char onStr;
    int counter = 0;
    int dotFind = -1;
    
    fgets(str,sizeof(str),stdin);
    while((onStr !='.')&&(counter !=strlen(str))){
        onStr = str[counter];
        counter++;
    }
    if (onStr == '.'){
        dotFind = counter;
    }
    else{
        dotFind = strlen(str);
    }
    for (int i=dotFind-2;i>-1;i--){
        printf("%c",str[i]);
    }
    

    return 0;
}
