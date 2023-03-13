#ifndef REINSTALL_CPP
#define REINSTALL_CPP
#include <cstdlib>
#include <experimental/bits/fs_fwd.h>
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
        cout << "Reinstalling packages \n";
        if (PreparePackageReinstall()) return 1;
        cout << "Reinstalling sosu \n";
        if (ReinstallSosu()) return 1;
        cout << "Reinstalling configs \n";
        if (ReinstallConfigs()) return 1;
        return 0;
    }
    if(!strcmp(argv[2], "packages"))
    {
        cout << "Reinstalling packages \n";
        if (PreparePackageReinstall()) return 1;
        return 0;
    }
    if(!strcmp(argv[2], "sosu"))
    {
        cout << "Reinstalling sosu \n";
        if (ReinstallSosu()) return 1;
        
        return 0;
    }
    if(!strcmp(argv[2], "configs"))
    {
        cout << "Reinstalling configs \n";
        if (ReinstallConfigs()) return 1;
        
        return 0;
    }
    cout << "Target not found: " << argv[2] << "\n";
    return 1;
}

int ReinstallConfigs ()
{
    // Backup old configs before attempting update
    string dbPath = "/etc/satalinos/utility/db/configfiles";
    string line;
    string dlCmd = "curl https://ironleo.de/satalinos/data/db/configfiles > " + dbPath;
    
    if (!system(dlCmd.c_str()))
    {
        cout << "Received list of files to backup \n";
    }
    else
    {
        cout << "Error downloading list of files to backup \n";
        return 1;
    }
    for (auto & user : fs::directory_iterator("/home"))
    {
        string sosuBakPath = user.path().string().append("/.sosu_bak/");
        if (fs::exists(sosuBakPath)) fs::remove_all(sosuBakPath);
        fs::create_directory(sosuBakPath);
        ifstream dbFile(dbPath);

        while (getline(dbFile, line)) {
            if (line.empty())
            {
                continue;
            }

            string filePath = user.path().string() + "/" + line;
            if (sos_common.CopyFile(filePath, sosuBakPath + line)) {
                cout << "Failed to Backup file \"" << line << "\" \n";
            }
                
        }
        cout << "Backed up old configs to " << user.path().string().append("/.sosu_bak") << "\n";
    }
    if (sos_common.Install_Config()) return 1;
    return 0;
}

int ReinstallSosu ()
{
    cout << "Downloading repository \n";
    if (system("git -C /usr/src/satalin-os-utility pull")) return 1;

    cout << "Building binary \n";
    if (system("/usr/src/satalin-os-utility/build.sh")) return 1;

    
    cout << "Installing \n";
    if (system("chmod a+x /usr/src/satalin-os-utility/patches/*")) return 1;
    if (fs::exists("/usr/sbin/sosu")) return 0;
    if (system("ln -s /usr/src/satalin-os-utility/sosu /usr/bin/sosu")) return 1;

    return 0;
}

private:
    
string toBeInstalled;
string toYayInstall;
string selectedPackages;
int my_argc;
SOS_Common sos_common;
SOS_Install sos_install;

int PreparePackageReinstall ()
{
    //Prevent from reinstalling configs
    sos_install.set_isReinstallPackages(true);

    //If ran successfully, delete backup file, if not revert back
    if (!ReinstallPackages())
    {
        fs::remove("/etc/satalinos/utility/db/installed.old");
        return 0;
    } else {
        try {
            fs::rename("/etc/satalinos/utility/db/installed.old", "/etc/satalinos/utility/db/installed");
        } catch (fs::filesystem_error& e) {
            cout << e.what() << '\n';
        }

        cout << "Failed to reinstall! \n";
        return 1;
    }
}

int ReinstallPackages ()
{
    //Make backup of file before applying changes
    try {
        fs::rename("/etc/satalinos/utility/db/installed", "/etc/satalinos/utility/db/installed.old");
        std::ofstream outfile ("/etc/satalinos/utility/db/installed");
        outfile.close();
    } catch (fs::filesystem_error& e) {
        cout << e.what() << '\n';
        return 1;
    }

    if (!sos_install.Main(my_argc, GetTargets())) return 0;
    
    return 1;
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

    my_argc = i;
    return targets;
}

};
}
#endif