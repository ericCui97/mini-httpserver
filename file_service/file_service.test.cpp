#include "file_service.h"
#include <assert.h>
#include <cstdio>

int main()
{
    const char *res = get_file_type(".html");
    printf("%s\n",res);
    //assert(strcmp(res,"text/html")==0);
    return 0;
}