//Imports
#include <algorithm>
#include <iostream>
#include <string.h>
#include "install.cpp"
#include "reinstall.cpp"
#include "update.cpp"
#include <string>
using namespace std;
using namespace SOS_Utility;

//Initialize the other classes
SOS_Install sosInstall;
SOS_Reinstall sosReinstall;
SOS_Update sosUpdate;

//Main function
int main(int argc, char * argv[])
{
    if (!strcmp(argv[1], "install"))
    {
        if(!sosInstall.Main(argc, argv))
        {
            return 0;
        }
        return 1;
    }
    if (!strcmp(argv[1], "reinstall"))
    {
        if(!sosReinstall.Main(argc, argv))
        {
            return 0;
        }
        return 1;
    }
    if (!strcmp(argv[1], "update"))
    {
        if (!sosUpdate.Main(argc, argv))
        {
            return 0;
        }
        return 1;
    }
    cout << "Unknown Module \n";
    return 1;
}
