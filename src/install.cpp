#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/stat.h>
#include <experimental/filesystem>
using namespace std;

namespace SOS_Utility {

class SOS_Install {
public:
    void Main (int argc, char * argv[])
    {
        if (argc < 3)
        {
            cout << "No packages specified";
            return;
        }

        for (int i=2;i<argc;i++)
        {
            if (strcmp(argv[i],"laptop") || strcmp(argv[i], "base") || strcmp(argv[i], "ironleo"))
            {
                if (IsInstalled(argv[i]))
                {
                    cout << "Package \"" << argv[i] << "\" already installed \n"; 
                }
                else
                {            
                    Install(argv[i]);
                }
            }
            else 
            {
                cout << "Unknown package \"" << argv[i] << "\" \n"; 
                break;
            }
        }
    }
private:
    void Install (string package)
    {
        cout << "Installing " << package << " package \n"; 
        cout << "Downloading package list... \n";
        string dlCmd = "curl https://ironleo.de/satalinos/data/db/" + package + " > /etc/satalinos/utility/db/" + package;
        if (!system(dlCmd.c_str()))
        {
            cout << "Done \n";
        }
        else
        {
            cout << "Error downloading file! \n";
        }
        system(("echo \"" + package + "\" >> /etc/satalinos/utility/db/installed").c_str());
    }

    bool IsInstalled (string package)
    {
        string dbPath = "/etc/satalinos/utility/db/installed";
        string line;
        if (!experimental::filesystem::exists(dbPath))
        {
            return false;
        }
        ifstream dbFile(dbPath);

        while (getline(dbFile, line)) {
            if (package == line)
            {
                return true;
            }
        }
        return false;
    }
};
}