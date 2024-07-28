#include <iostream>
#include <filesystem>
#include <string>

#include <thread>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

void copy_directory(const fs::path &source, const fs::path &destination)
{
    if (!fs::exists(destination))
        fs::create_directories(destination);

    for (const auto &entry : fs::directory_iterator(source))
    {
        const auto &path = entry.path();
        const auto &new_path = destination / path.filename();

        cout << "\nAdding ";
        cout << path;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (fs::is_directory(path))
        {
            copy_directory(path, new_path);
        }

        else if (fs::is_regular_file(path))
            fs::copy_file(path, new_path, fs::copy_options::overwrite_existing);
    }
}

int main()
{
    string source_dir = "sourcecode";
    string repo, branch, commit;
    int choice;
    while (true)
    {
        cout << "\n\n**************************\n\n";
        cout << "1. Create Repository\n";
        cout << "2. Create Branch\n";
        cout << "3. Commit Changes\n";
        cout << "4. Exit\n";
        cout << "\n\n**************************\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter the repository name: ";
            cin >> repo;
        }
        else if (choice == 2)
        {
            if (repo == "")
            {
                cout << "\n Please create the repository first!\n";
                continue;
            }
            else
            {
                cout << "\nEnter the branch name: ";
                cin >> branch;
            }
        }
        else if (choice == 3)
        {
            if (repo == "" && branch == "")
            {
                cout << "\n Please create the repository and branch first!\n";
                continue;
            }
            else
            {
                cout << "\nEnter the commit name: ";
                cin >> commit;

                string dest_dir = "dist/" + repo + "/" + branch + "/" + commit;
                fs::path source_path(source_dir);
                fs::path dest_path(dest_dir);

                copy_directory(source_path, dest_path);

                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                cout << "\n\nAll changes are been made!";
                cout << "\n\nCheck the following directory\n!";
                cout << dest_dir;
                break;
            }
        }
        else if (choice == 4)
        {
            break;
        }
        else
        {
            cout << "\nPlease enter a valid choice!\n";
            continue;
        }
    };

    return 0;
}
