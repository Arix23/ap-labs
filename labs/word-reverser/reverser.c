#include <stdio.h>

int main(){
    int i;
    char c, word[100];

    i = 0;

    while((c=getchar())!= EOF){
        word[i] = c;
        i++;
        if(c=='\n' || c=='\t'){
            for(int j=i-2;j>-1;j--){
                printf("%c",word[j]);
            }
            printf("\n");
            i=0;
        }
    }
    return 0;
}
