#include <stdio.h>
#include <mpi.h>
#include <openssl/md5.h>

int LENGTH=7;
char SAMPLE[33]="033bd94b1168d7e4f0d644c3c95e35bf\0";
char SYMBOLS[17]="0123456789abcdef\0";
int flag = 0;

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

void print_hello(char *argv){
    int rank, size;
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    char chunk[LENGTH];
    split_line(chunk,argv,rank);
    printf("[%d.%d]: %s", rank, size, chunk);
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(chunk,strlen(chunk),result);
    if (cmpmd5(result)==0) {
        printf("\n\nMatch! The real substring is %s\n",chunk);
        return;
    }
}

void substr(char dest[], char src[], int offset, int len)
{
    int i;
    for(i = 0; i < len && src[offset + i] != '\0'; i++)
            dest[i] = src[i + offset];
    dest[i] = '\0';
}

void split_line(char dest[], char src[], int offset)
{
    int i;
    int j = 0;
    int pos = 0;
    for(i=0; i<strlen(src); i++){
        if (src[i] == '\t') {
            j++;
            i++;
        }
        if (j==offset){
            dest[pos++] = src[i];
        }
    }
    dest[pos]='\0';
}

FILE *fr; //File Reference
int FILE_LINE_LENGTH=90;
int PROC_COUNT=4;

int main(int argc, char *argv[])
{
    MPI_Init (&argc, &argv);
    fr = fopen("testfile.txt", "rt");
    char line[FILE_LINE_LENGTH];
    while( (fgets(line,FILE_LINE_LENGTH,fr)!=NULL) && (flag) ){
        MPI_Bcast( &line, LENGTH*10, MPI_CHAR, 0, MPI_COMM_WORLD );
        int i, j, k, count=1, pos=0;
        char tmp[LENGTH*4+1];
        for(i=0;i<strlen(line);i++){
            for(j=0;j<PROC_COUNT;j++){
                for(k=0;k<LENGTH;k++){
                    tmp[pos++]=str[i];
                }
            }
        }
        print_hello(line);
    }
    close(fr);
    MPI_Finalize();
    return 0;
}
