#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>

#include <thread>
#include <chrono>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"

using namespace std;
namespace fs = std::filesystem;

unordered_map<string, vector<string>> branch_commits;

void copy_directory(const fs::path &source, const fs::path &destination)
{
    if (!fs::exists(destination))
        fs::create_directories(destination);

    for (const auto &entry : fs::directory_iterator(source))
    {
        const auto &path = entry.path();
        const auto &new_path = destination / path.filename();

        cout << GREEN << "\nFiles ===>  ";
        cout << path << RESET;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (fs::is_directory(path))
        {
            copy_directory(path, new_path);
        }
        else if (fs::is_regular_file(path))
            fs::copy_file(path, new_path, fs::copy_options::overwrite_existing);
    }
}

void display_branches()
{
    cout << CYAN << "\nAvailable branches:\n"
         << RESET;
    for (const auto &branch : branch_commits)
    {
        cout << branch.first << endl;
    }
}

void display_commits(const string &branch)
{
    cout << CYAN << "\nCommits in branch " << branch << ":\n"
         << RESET;
    for (const auto &commit : branch_commits[branch])
    {
        cout << commit << endl;
    }
}

void revert_commit(const string &branch, const string &commit)
{
    string source_dir = "dist/" + branch + "/" + commit;
    string dest_dir = "sourcecode";

    fs::path source_path(source_dir);
    fs::path dest_path(dest_dir);

    if (!fs::exists(source_path))
    {
        cout << RED << "\nCommit " << commit << " does not exist in branch " << branch << RESET << endl;
        return;
    }

    fs::remove_all(dest_path);              // Remove current source code
    copy_directory(source_path, dest_path); // Revert to commit

    cout << GREEN << "\n\nReverted to commit " << commit << " in branch " << branch << RESET << endl;
}

int main()
{
    string repo, branch, commit;
    int choice;
    while (true)
    {
        cout << RED << "\n\n====================================================\n\n"
             << RESET;
        cout << GREEN << "\t\tVersion Control System";
        cout << RED << "\n\n====================================================\n\n"
             << RESET;
        cout << CYAN << "\t\t1. Create Repository\n";
        cout << "\t\t2. Create Branch\n";
        cout << "\t\t3. Commit Changes\n";
        cout << "\t\t4. Revert Changes\n";
        cout << "\t\t5. Exit\n"
             << RESET;
        cout << RED << "\n\n___________________________________________________\n"
             << RESET;
        cout << CYAN << "Enter your choice: " << RESET;
        cin >> choice;

        if (choice == 1)
        {
            cout << CYAN << "\nEnter the repository name: " << RESET;
            cin >> repo;
            cout << GREEN << "Repository created: " << repo << RESET;
        }
        else if (choice == 2)
        {
            if (repo == "")
            {
                cout << RED << "\n Please create the repository first!\n"
                     << RESET;
                continue;
            }
            else
            {
                cout << CYAN << "\nEnter the branch name: " << RESET;
                cin >> branch;
                cout << GREEN << "Branch created: " << branch << RESET;
                //? Initialize the branch with an empty vector of commits
                branch_commits[branch] = vector<string>();
            }
        }
        else if (choice == 3)
        {
            if (repo == "" && branch == "")
            {
                cout << RED << "\n Please create the repository and branch first!\n"
                     << RESET;
                continue;
            }
            else
            {
                cout << CYAN << "\nEnter the commit name: " << RESET;
                cin >> commit;

                string dest_dir = "dist/" + branch + "/" + commit;
                fs::path source_path("sourcecode");
                fs::path dest_path(dest_dir);

                copy_directory(source_path, dest_path);

                cout << GREEN << "\n\nAll changes are made!";
                cout << "\n\nCheck the following directory:\n"
                     << RESET;
                cout << MAGENTA << dest_dir << RESET;

                //? Store the commit in the branch's commit list
                branch_commits[branch].push_back(commit);
            }
        }
        else if (choice == 4)
        {
            if (repo == "" || branch == "")
            {
                cout << RED << "\nPlease create the repository and branch first!\n"
                     << RESET;
                continue;
            }
            else
            {
                display_branches();
                cout << CYAN << "\nEnter the branch name: " << RESET;
                cin >> branch;

                display_commits(branch);
                cout << CYAN << "\nEnter the commit name to revert: " << RESET;
                cin >> commit;

                revert_commit(branch, commit);
            }
        }
        else if (choice == 5)
        {
            cout << CYAN << "\n\t Thank you for using the version control system!\n"
                 << RESET;
            cout << CYAN << "\n=============================================================\n"
                 << RESET;
            break;
        }
        else
        {
            cout << RED << "\nPlease enter a valid choice!\n"
                 << RESET;
            continue;
        }
    };

    return 0;
}
