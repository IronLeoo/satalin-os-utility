//Imports
#include "install.cpp"
#include "reinstall.cpp"
#include "update.cpp"
#include <unistd.h>
namespace fs = experimental::filesystem;
using namespace std;
using namespace SOS_Utility;

//Initialize the other classes
SOS_Install sosInstall;
SOS_Reinstall sosReinstall;
SOS_Update sosUpdate;
SOS_Common sosCommon;

//Check if user has root privileges
int IsRoot ()
{
    if (!getuid()) return 1;
    return 0;
}

//Make sure all working directories are there
void CheckDirs ()
{
    string etcUtility = "/etc/satalinos/utility/";
    string utilityDB = "/etc/satalinos/utility/db/";
    string varCache = "/var/cache/satalinos/";
    string settingsJSONPath = "/etc/satalinos/utility/settings.json";

    if (!fs::exists(etcUtility))
    {
        fs::create_directory(etcUtility);
    }
    if (!fs::exists(settingsJSONPath))
    {
        ofstream file;
        file.open(settingsJSONPath);
        file << "{\n    \"hidpi\" : false\n}";
        file.flush();
    }
    if (!fs::exists(utilityDB))
    {
        fs::create_directory(utilityDB);
    }
    if (!fs::exists(varCache))
    {
        fs::create_directory(varCache);
    }
}

//Main function
int main(int argc, char * argv[])
{
    //Check for root privileges
    if (!IsRoot())
    {
        cout << "You need to execute this program with elevated privileges! \n";
        return 1;
    }

    //Check if directory structure is complete
    CheckDirs();

    //Read settings.json and store values in variable
    sosCommon.ReadSettingsJSON();

    if (argc < 2)
    {
        cout << "No argument supplied \n";
        return 1;
    }

    if (!strcmp(argv[1], "install"))
    {
        if (!sosInstall.Main(argc, argv)) return 0;

        return 1;
    }
    
    if (!strcmp(argv[1], "reinstall"))
    {
        if (!sosReinstall.Main(argc, argv)) return 0;

        return 1;
    }

    if (!strcmp(argv[1], "update"))
    {
        if (!sosUpdate.Main(argc, argv)) return 0;

        return 1;
    }

    cout << "Unknown Module \n";
    return 1;
}