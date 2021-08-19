#include<cstdio>
#include<stdlib.h>


int main(){
    FILE* pFile;
    long lSize;
    char* buffer;

    size_t result;
    pFile=fopen("test.txt","rb");
    fseek(pFile,0,SEEK_END);//
    lSize=ftell(pFile);


}