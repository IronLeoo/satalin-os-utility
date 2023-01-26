#ifndef COMMON_CPP
#define COMMON_CPP
#include <string.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
using namespace std;

namespace SOS_Utility {

class SOS_Common {
public:
    int Install_Packages (string toBeInstalled, string toYayInstall, string selectedPackages)
    {
        if (!system(("pacman -Sy " + toBeInstalled).c_str()) &&
            !system(("sudo -u nobody yay -S " + toYayInstall).c_str()))
        {
            return 0;
        }
        else
        {
            cout << "Failed to install packages";
            return 1;
        }
    }

    int Queue (string* toBeInstalled, string* toYayInstall, string* selectedPackages, string package, int* packageCount = 0)
    {
        string dbPath = "/etc/satalinos/utility/db/" + package;
        string line;
        cout << "Downloading package list for \"" + package + "\"\n";
        string dlCmd = "curl https://ironleo.de/satalinos/data/db/" + package + " > " + dbPath;
        if (!system(dlCmd.c_str()))
        {
            cout << "Successfully downloaded package list \n";
        }
        else
        {
            cout << "Error downloading file! \n";
            return 1;
        }

        ifstream dbFile(dbPath);

        while (getline(dbFile, line)) {
            if (line.empty())
            {
                continue;
            }
            if (line.find("<") != string::npos) {
                toYayInstall->append(line + " ");
            } else {
                toBeInstalled->append(line + " ");
            }
        }
        selectedPackages->append(package + "\n");
        *packageCount += 1;
        return 0;
    }
};
}
#endif