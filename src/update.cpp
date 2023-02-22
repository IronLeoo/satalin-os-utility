#include <bits/types/FILE.h>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include "reinstall.cpp"
using namespace std;

namespace SOS_Utility {
class SOS_Update {

public:
    int Main (int argc, char * argv[])
    {
        if (argc < 3)
        {
            cout << "No target specified \n";
            return 1;
        }

        if (!strcmp(argv[2], "all"))
        {
            if (UpdatePackages()) return 1;

            if (UpdateSosu()) return 1;

            if (UpdateConfigs()) return 1;

            return 0;
        }

        if(!strcmp(argv[2], "packages"))
        {
            if (UpdatePackages()) return 1;
            
            return 0;
        }

        if(!strcmp(argv[2], "sosu"))
        {
            if (UpdateSosu()) return 1;
            
            return 0;
        }

        if(!strcmp(argv[2], "configs"))
        {

            if (UpdateConfigs()) 
            {
                cout << "Please update manually \n";
                return 1;
            }
            return 0;
        }

        cout << "Target not found: " << argv[2] << "\n";
        return 1;
    }

private:
    SOS_Reinstall sos_reinstall;

    int UpdatePackages ()
    {
        //Error if failed to update
        if (system("sudo -u $(logname) yay")) return 1;

        return 0;
    }

    int UpdateSosu ()
    {
        return 0;
    }

    int UpdateConfigs ()
    {
        string newVersion;
        string currentVersion;
        string logName;
        char buffer[128];
        FILE* pipe = popen("logname", "r");
        if (pipe == NULL) {
            cout << "Failed to get current version \n";
            return 1;
        }

        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            logName = string(buffer);
        }

        pclose(pipe);

        string newVersionPath = "/var/cache/satalinos/config_version";
        if(system(("curl https://raw.githubusercontent.com/IronLeoo/linux-configs/master/.CONFIG_VERSION > " + newVersionPath).c_str()))
        {
            cout << "Failed to get newest version \n";
            return 1;
        }

        string currentVersionPath = "/home/" + logName.replace(logName.length()-1, 1, "") + "/.CONFIG_VERSION";
        currentVersion = GetFileContent(currentVersionPath, "Current");

        if (!fs::exists("/var/cache/satalinos/"))
        {
            fs::create_directory("/var/cache/satalinos");
        }

        newVersion = GetFileContent(newVersionPath, "New");

        if (stoi(newVersion) <= stoi(currentVersion))
        {
            cout << "Already newest version \n";
            return 0;
        }

        if (sos_reinstall.ReinstallConfigs()) return 1;

        return 0;
    }

    string GetFileContent (string filePath, string versionType)
    {
        string currentVersion;
        string line;
        ifstream versionFile (filePath);
        while (getline (versionFile, line)) {
            currentVersion = line;
            cout << versionType << " Version: " << currentVersion << "\n";
        }
        versionFile.close();

        return currentVersion;
    }
};
}