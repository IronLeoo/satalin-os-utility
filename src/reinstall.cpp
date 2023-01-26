#include <array>
#include <fstream>
#include <iostream>
#include <string.h>
#include <experimental/filesystem>
#include <vector>
#include "common.cpp"
#include "install.cpp"
using namespace std;
namespace fs = experimental::filesystem;

namespace SOS_Utility {

class SOS_Reinstall {
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
            return 0;
        }

        if(!strcmp(argv[2], "packages"))
        {
            if (!ReinstallPackages())
            {
                fs::remove("/etc/satalinos/utility/db/installed.old");
                return 0;
            } else {
                try {
                    experimental::filesystem::rename("/etc/satalinos/utility/db/installed.old", "/etc/satalinos/utility/db/installed");
                } catch (experimental::filesystem::filesystem_error& e) {
                    //cout << e.what() << '\n';
                    cout << "aaa";
                }
                cout << "Failed to reinstall! \n";
                return 1;
            }
        }

        if(!strcmp(argv[2], "sosu"))
        {
            return 0;
        }

        if(!strcmp(argv[2], "configs"))
        {
            return 0;
        }

        cout << "Target not found: " << argv[2];
        return 1;
    }

private:
    string toBeInstalled;
    string toYayInstall;
    string selectedPackages;
    int my_argc;
    SOS_Common sos_common;
    SOS_Install sos_install;

    int ReinstallPackages ()
    {
        try {
            experimental::filesystem::rename("/etc/satalinos/utility/db/installed", "/etc/satalinos/utility/db/installed.old");
            std::ofstream outfile ("/etc/satalinos/utility/db/installed");
            outfile.close();
        } catch (experimental::filesystem::filesystem_error& e) {
            cout << e.what() << '\n';
            return 1;
        }

        if (!sos_install.Main(my_argc, GetTargets()))
        {
            return 0;
        } else {
            return 1;
        }
    }

    char** GetTargets ()
    {
        string dbPath = "/etc/satalinos/utility/db/installed.old";
        string line;
        string lines[8];
        char** targets = new char* [8];
        lines[0] = "sosu";
        lines[1] = "install";
        int i = 2;
        
        if (!fs::exists(dbPath))
        {
            return {};
        }
        ifstream dbFile(dbPath);

        while (getline(dbFile, line)) {
            if (!line.empty())
            {
                lines[i] = line;
            }
            i++;
        }
        for (unsigned i=0;i<9;i++)
        {
            if (!lines[i].empty())
            {
                targets[i] = const_cast<char*>(lines[i].c_str());
            }
        }
        cout << "\n";
        my_argc = i;
        return targets;
    }
};
}