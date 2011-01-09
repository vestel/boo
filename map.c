#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>


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

void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
        printf("%02x",md[i]);
    }
}

void convert_md5_string(unsigned char* md, char result[]){
    int i;
    for(i=0; i<MD5_DIGEST_LENGTH; i++) {
        *result++ = SYMBOLS[(md[i]>>4) & 0xf];
        *result++ = SYMBOLS[md[i] & 0xf];
    }
    *result++ ='\0';
}

int cmpmd5(unsigned char* md){
    char md5str[MD5_DIGEST_LENGTH*2+1];
    convert_md5_string(md, md5str);
    return strcmp(md5str,SAMPLE);
}

int strip_line(char line[]){
    char tmp[LENGTH];
    unsigned char result[MD5_DIGEST_LENGTH];
    int line_length=strlen(line);
    int i,j,k;
    for(i=0;i<line_length;i++){
        for(j=1; j<LENGTH; j++){
            for(k=0; k<4; k++){
                substr(tmp,line,i,j);
                if (strlen(tmp)<j) break;
            }
            printf("\n");
        }
    }
    return 1;
}

FILE *fr; //File Reference

int main(int argc, char *argv[]){
    fr = fopen("testfile.txt", "rt");
    char line[FILE_LINE_LENGTH];
    char* one;
    char two[MD5_DIGEST_LENGTH*2+1];
    unsigned char result[MD5_DIGEST_LENGTH];
    int flag = 1;
    while( (fgets(line,FILE_LINE_LENGTH,fr)!=NULL) && (flag) ){
        flag = strip_line(line);
    }
    close(fr);
    return 0;
}
