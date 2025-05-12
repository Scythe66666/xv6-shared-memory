#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"


int main(int argc, char* argv[])
{
    if(argc > 1)
    {
        /*paste();*/
    }
    char buf[1024];

    while(read(0, buf, 1024))
        printf(1, "received %s \n", buf);

}
