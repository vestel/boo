#include <stdio.h>
#include <mpi.h>
#include <openssl/md5.h>

int LENGTH=7;
char SAMPLE[33]="033bd94b1168d7e4f0d644c3c95e35bf\0";
char SYMBOLS[17]="0123456789abcdef\0";
int flag = 1;
FILE *fr; //File Reference
int FILE_LINE_LENGTH=90;
int rank, size;

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
    char chunk[LENGTH];
    split_line(chunk,argv,rank);
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(chunk,strlen(chunk),result);
    if (cmpmd5(result)==0) {
        printf("\n[%d.%d] Match found! Answer is %s\n",rank,size,chunk);
        MPI_Abort(MPI_COMM_WORLD,0);
    }
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


int main(int argc, char *argv[])
{
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    fr = fopen("testfile.txt", "rt");
    char line[FILE_LINE_LENGTH];
    while( (fgets(line,FILE_LINE_LENGTH,fr)!=NULL) && (flag) ){
        MPI_Bcast( &line, FILE_LINE_LENGTH, MPI_CHAR, 0, MPI_COMM_WORLD );
        int source_pos=0, word_count=0, max_len=0, result_pos=0, iter;
        char tmp[LENGTH*size+1];
        for(source_pos=0;source_pos<strlen(line);source_pos++){
            for(max_len=1;max_len<LENGTH;max_len++){
                word_count++;
                if (word_count % size == 0)
                {
                    tmp[result_pos]='\0';
                    MPI_Bcast( &tmp, strlen(tmp), MPI_CHAR, 0, MPI_COMM_WORLD );
                    print_hello(tmp);
                    result_pos=0;
                }
                if (source_pos+max_len>strlen(line)) break;
                for(iter=0;iter<max_len; iter++){
                    tmp[result_pos++] = line[source_pos+iter];
                }
                tmp[result_pos++]='\t';
            }
        }
    }
    close(fr);
    MPI_Finalize();
    return 0;
}
