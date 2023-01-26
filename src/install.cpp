#ifndef INSTALL_CPP
#define INSTALL_CPP
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

        for (int i=2;i<argc;i++)
        {
            if (argv[i] == NULL)
            {
                continue;
            }

            string target = argv[i];
            if (target == "laptop" || target == "base" || 
                target == "ironleo")
            {
                if (IsInstalled(target))
                {
                    cout << "Target \"" << target << "\" already installed \n"; 
                }
                else
                {          
                    try {
                        if (sos_common.Queue(&toBeInstalled, &toYayInstall, &selectedPackages, target, &packageCount))
                        {
                            return 1;
                        }
                    } catch (string e) {
                        cout << e;
                        return 1;
                    }
                    if (target == "base")
                    {
                        isBasePackage = 1;
                    }
                }
            }
            else 
            {
                cout << "Unknown target \"" << target << "\" \n"; 
                return 1;
            }
        }

        if (isBasePackage == 1)
        {
            /*Clean_Up();
            if(Install_Yay())
            {
                return 1;
            }
            if (Install_Config())
            {
                return 1;
            }
            Clean_Up();*/
        }
        if (packageCount > 0)
        {
            if(sos_common.Install_Packages(toBeInstalled, toYayInstall, selectedPackages))
            {
                return 1;
            }

            if (!selectedPackages.empty())
            {
                system(("echo \"" + selectedPackages + "\" >> /etc/satalinos/utility/db/installed").c_str());
            }
            cout << "Finished installing packages";
        }
        return 0;
    }
private:
    string toBeInstalled;
    string toYayInstall;
    string selectedPackages;
    int packageCount = 0;
    int isBasePackage = 0;
    SOS_Common sos_common;

    void Clean_Up ()
    {
        if (fs::exists("/tmp/linux-configs")) {
            system("rm -r /tmp/linux-configs");
        }

        if (fs::exists("/tmp/yay")) {
            system("rm -r /tmp/yay");
        }
        
        if (fs::exists("/.cache")) {
            system("rm -r /.cache");
        }

        cout << "Removed sources";
    }

    int Install_Yay ()
    {
        cout << "Installing AUR helper \"Yay\"\n";
        if (system("cd /tmp && sudo -u nobody git clone https://aur.archlinux.org/yay.git \
        && cd yay && mkdir /.cache && chmod o+w /.cache && sudo -u nobody makepkg && pacman -U yay*.tar.zst && cd .. && rm -r yay"))
        {
            cout << "Failed to install yay";
            return 1;
        }
        else
        {
            return 0;
        }
    }

    int Install_Config ()
    {
        string git_url = "https://github.com/IronLeoo/linux-configs.git";
        fs::path users[1];

        cout << "Installing configs... \n";

        if (fs::exists("/tmp/linux-configs"))
        {
            system("rm -r /tmp/linux-configs");
        }

        //If git clone fails
        if (system(("cd /tmp && git clone " + git_url).c_str()))
        {
            cout << "Failed to clone repository! \n";
            return 1;
        }
        cout << "Retrieved repository \n";

        system("rm -r /tmp/linux-configs/.git");

        for (auto & user : fs::directory_iterator("/home"))
        {
            cout << "Copying files into home directory... \n";
            for (auto & file : fs::directory_iterator("/tmp/linux-configs"))
            {
                try
                {
                    fs::copy(file.path(), user.path(), fs::copy_options::overwrite_existing);
                }
                catch (string e)
                {
                    cout << e;
                    return 1;
                }
            }
            cout << "Successfully copied files to \"" << user.path().string() << "\"\n";
        }
        return 0;
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