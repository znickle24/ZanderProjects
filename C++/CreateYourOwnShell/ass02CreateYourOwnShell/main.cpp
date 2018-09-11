#include <string>
#include <vector>
#include <iterator>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/types.h>
#include <dirent.h>
#include "shelpers.hpp"


//char *commandNames[] = {
//
////    while (dirp) {
////        errno = 0;
////        if ((auto dp = readdir(dirp)) != NULL) {
////            if (strcmp(dp->d_name, name) == 0) {
////                closedir(dirp);
////                return FOUND;
////            }
////        } else {
////            if (errno == 0) {
////                closedir(dirp);
////                return NOT_FOUND;
////            }
////            closedir(dirp);
////        }
////    }
//    //open dir or read dir
//    //filesys
//    "compress", "head", "tail", "touch",
//    NULL
//};
//char * tabGenerator(const char *text, int state)
//{
//    static int list_index, len;
//    char *name;
//
//    if (!state) {
//        list_index = 0;
//        len = strlen(text);
//    }
//
//    while ((name = commandNames[list_index++])) {
//        if (strncmp(name, text, len) == 0) {
//            return strdup(name);
//        }
//    }
//
//    return NULL;
//}
//char ** tabCompletion(const char *text, int start, int end)
//{
//    rl_attempted_completion_over = 1;
//    return rl_completion_matches(text, tabGenerator);
//}

int main(int argc, const char * argv[]) {
    std::string line;
    std::vector<pid_t> zombies;
//    rl_attempted_completion_function = tabCompletion;
    while(true){
        char* buffer = readline("> ");
        if (buffer) {
            std::string bufferString (buffer);
            line = bufferString;
            
            free(buffer);
        }
        int status;
        while (pid_t zombieBaby = waitpid(-1, &status, WNOHANG) > 0){
            zombies.erase(std::remove(zombies.begin(), zombies.end(), zombieBaby), zombies.end());
            std::cout << "A zombie has been killed and removed." << std::endl;
        }
        std::vector<Command> commandVector ;
        auto tokens = tokenize(line);
        commandVector = getCommands(tokens);
        if (commandVector[0].exec == "cd") {
            if (commandVector[0].argv[1] == NULL) {
                chdir(getenv("HOME"));
                continue;
            } else {
                //if there are multiple arguments here, it means that cd is trying to take us somewhere.
                chdir(commandVector[0].argv[1]);
                continue;
            }
        }
        std::vector<pid_t> waitForMe;
        
        //clean up all of the file descriptors for the first commands output (want to make sure that it is closed)
        for (int i = 0; i < commandVector.size(); i++) {
            pid_t nextCommand = fork();
            if (commandVector[i].background == true) {
                zombies.push_back(nextCommand);
            } else {
                waitForMe.push_back(nextCommand);
            }
            std::cout << "The child pid is: " << nextCommand << std::endl;
            if (nextCommand == 0) {
                Command command = commandVector[i];
                dup2(command.fdStdout, 1);
                dup2(command.fdStdin, 0);
                //close(commandVector[i-1].fdStdout);
                for (int j = 0; j < commandVector.size(); j++) {
                    if (j!=i) {
                        if (commandVector[j].fdStdin != 0) {
                            close(commandVector[j].fdStdin);
                        }
                        if (commandVector[j].fdStdout !=1) {
                            close(commandVector[j].fdStdout);
                        }
                    }
                }
                execvp(commandVector[i].exec.c_str(), (const_cast<char* const*>(commandVector[i].argv.data())));
            } else if (nextCommand < 0) {
                std::cout << "Failure to create fork through pipe." << std::endl;
            }
            
        }
        
        for (int j = 0; j < waitForMe.size(); j++) {
            if (commandVector[j].fdStdin != 0) {
                close(commandVector[j].fdStdin);
            }
            if (commandVector[j].fdStdout !=1) {
                close(commandVector[j].fdStdout);
            }
            std::cout << "Waiting for child: " << waitForMe[j] << std::endl;
            waitpid(waitForMe[j], NULL, 0);
            std::cout << "The child finishing is: " << waitForMe[j] << std::endl;
        }
        //no hang
//            execvp(commandVector[0].argv[0], (const_cast<char* const*>(commandVector[0].argv.data())));
            //execvp is a c string and the second one is a const c string
            //exec wipes away all memory
        std::cout << "getCommand$";
    }
    return 0;
}
