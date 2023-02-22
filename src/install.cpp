#ifndef INSTALL_CPP
#define INSTALL_CPP
#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string.h>
#include "common.cpp"
#include <experimental/filesystem>
#include <string>
using namespace std;
namespace fs = experimental::filesystem;

namespace SOS_Utility {
class SOS_Install {

public:
    int Main (int argc, char* argv[])
    {
        if (argc < 3)
        {
            cout << "No target specified \n";
            return 1;
        }

        //Convert argv** to string array
        std::string *argstr = new std::string[argc];
        for (int i = 0; i < argc; ++i)
        {
            argstr[i] = argv[i];
        }

        for (int i=2;i<argc;i++)
        {
            string target(argstr[i]);

            if (target != "laptop" && target != "base" && 
                target != "ironleo")
            {
                cout << "Unknown target \"" << target << "\" \n"; 
                return 1;
            }

            if (IsInstalled(target))
            {
                cout << "Target \"" << argv[i] << "\" already installed \n"; 
                continue;
            }

            if (target == "base")
            {
                sos_common.Clean_Up();

                if(Install_Yay()) return 1;
                
                if (!get_isReinstallPackages()) {
                    if (sos_common.Install_Config()) return 1;
                }
                
                sos_common.Clean_Up();
            } 
            try {
                if (sos_common.Queue(&toBeInstalled, &toYayInstall, &selectedPackages, target, &packageCount))
                {
                    return 1;
                }
            } catch (string e) {
                cout << e;
                return 1;
            }
        }

        if (packageCount > 0)
        {
            if(sos_common.Install_Packages(toBeInstalled, toYayInstall, selectedPackages))
            {
                return 1;
            }

            if (!selectedPackages.empty())
            {
                // Remove last newline char in array to prevent unnecessary 
                // spaces in "installed" file
                system(("echo \"" + selectedPackages.replace(selectedPackages.find_last_of("\n"), 1, "") + "\" >> /etc/satalinos/utility/db/installed").c_str());
            }
            cout << "Finished installing packages";
        }
        return 0;
    }

    void set_isReinstallPackages (bool value)
    {
        isReinstallPackages = value;
    }

    bool get_isReinstallPackages ()
    {
        return isReinstallPackages;
    }

private:
    string toBeInstalled;
    string toYayInstall;
    string selectedPackages;
    int packageCount = 0;
    int isBasePackage = 0;
    SOS_Common sos_common;
    bool isReinstallPackages;

    int Install_Yay ()
    {
        cout << "Installing AUR helper \"Yay\" \n";
        if (system("cd /tmp && sudo -u nobody git clone https://aur.archlinux.org/yay.git \
        && cd yay && mkdir /.cache && chmod o+w /.cache && sudo -u nobody makepkg && pacman -U yay*.tar.zst && cd .. && rm -r yay"))
        {
            cout << "Failed to install yay \n";
            return 1;
        }
        else
        {
            return 0;
        }
    }

    bool IsInstalled (string package)
    {
        string dbPath = "/etc/satalinos/utility/db/installed";
        string line;
        if (!fs::exists(dbPath))
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
#endif