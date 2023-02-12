#ifndef COMMON_CPP
#define COMMON_CPP
#include <string.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
using namespace std;
namespace fs = experimental::filesystem;

namespace SOS_Utility {

class SOS_Common {

public:
    int Install_Packages (string toBeInstalled, string toYayInstall, string selectedPackages)
    {
        if (!system(("pacman -Sy " + toBeInstalled).c_str()) &&
            !system(("sudo -u $(logname) yay -S --nodiffmenu --removemake " + toYayInstall).c_str()))
        {
            return 0;
        }
        else
        {
            cout << "Failed to install packages \n";
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
                toYayInstall->append(line.replace(0, 1, "") + " ");
            } else {
                toBeInstalled->append(line + " ");
            }
        }
        selectedPackages->append(package + "\n");
        *packageCount += 1;
        return 0;
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

        cout << "Removed sources \n";
    }
};
}
#endif