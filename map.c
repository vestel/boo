#include <stdio.h>
#include <string.h>

//char SAMPLE[33]="66d9978935150b34b9dc0741bc642be2\0";
/* This is md5 hash for my task. For a testing purpose
 * in single-threaded code, I've used a md5 hash for
 * word "TEST" to determine end of processing.
 */

int LENGTH=7;
int FILE_LINE_LENGTH=99;
char SAMPLE[33]="033bd94b1168d7e4f0d644c3c95e35bf\0";
char SYMBOLS[17]="0123456789abcdef\0";

void substr(char dest[], char src[], int offset, int len)
{
    int i;
    for(i = 0; i < len && src[offset + i] != '\0'; i++)
            dest[i] = src[i + offset];
    dest[i] = '\0';
}

void strip_line(char line[]){
    char tmp[LENGTH];
    char *result;
    int line_length=strlen(line);
    int i,j,k;
    result="";
    for(i=0;i<line_length;i++){
        for(j=0; j<LENGTH;){
            for(k=0; k<4; k++){
                j++;
                substr(tmp,line,i,j);
                if (strlen(tmp)<j) break;
                if (j>LENGTH) break;
                printf("\"%s\" ",tmp);
            }
            if (j<=LENGTH) printf("\n");
        }
    }
    return;
}

FILE *fr; //File Reference

int main(int argc, char *argv[]){
    fr = fopen("testfile.txt", "rt");
    char line[FILE_LINE_LENGTH];
    char* one;
    while( (fgets(line,FILE_LINE_LENGTH,fr)!=NULL) ){
        strip_line(line);
    }
    close(fr);
    return 0;
}
