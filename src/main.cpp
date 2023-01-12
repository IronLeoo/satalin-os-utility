//Imports
#include <algorithm>
#include <iostream>
#include <string.h>
#include "install.cpp"
#include "reinstall.cpp"
#include "update.cpp"
using namespace std;
using namespace SOS_Utility;

//Global vars
SOS_Install sosInstall;
SOS_Reinstall sosReinstall;
SOS_Update sosUpdate;

//Main function
int main(int argc, char * argv[])
{
    if (!strcmp(argv[1], "install"))
    {
        sosInstall.Main(argc, argv);
        return 0;
    }
    if (!strcmp(argv[1], "reinstall"))
    {
        sosReinstall.Main(argc, argv);
        return 0;
    }
    if (!strcmp(argv[1], "update"))
    {
        sosUpdate.Main(argc, argv);
        return 0;
    }
    cout << "Unknown Module";
    return 0;
}
