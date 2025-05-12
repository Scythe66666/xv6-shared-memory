#include "types.h"
#include "user.h"

int main(int argc, char* argv[])
{
    for(int i = 0; i < 10; i++)
    {
        if(fork() != 0)
        {
            wait();
            break;
        }
    }
    
    for(int i = 0; i < 100000000; i++);
    exit();        
}
