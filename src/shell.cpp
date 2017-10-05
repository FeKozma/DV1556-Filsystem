#include <sstream>
#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 17;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "q","quit","exit","format","ls","create","cat","createImage","restoreImage",
    "rm","cp","append","mv","mkdir","cd","pwd","help"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
bool quit();
std::string help();

/* More functions ... */

int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	FileSystem fileSys(250);

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output

	int printInt;
	std::string printString;

    bool bRun = true;

    do {
        std::cout << user << ":" << currentDir << "$ ";
        getline(std::cin, userCommand);

        int nrOfCommands = parseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = findCommand(commandArr[0]);
            switch(cIndex) {

			case 0: // q
			case 1: // quit
			case 2: // exit
				bRun = quit();                
                break;
            case 3: // format
                break;
            case 4: // ls
                std::cout << "Listing directory" << std::endl;
                break;
            case 5: // create
				printInt = fileSys.createFileOn("Det var en gang en pirat som borjade beratta en saga som lod: ");
				std::cout << std::to_string(printInt) + "\n";
                break;
            case 6: // cat
                break;
            case 7: // createImage
                break;
            case 8: // restoreImage
                break;
            case 9: // rm
				printString = fileSys.viewFileOn();
				std::cout << printString + "\n";
                break;
            case 10: // cp
                break;
            case 11: // append
                break;
            case 12: // mv
                break;
            case 13: // mkdir
				std::cout << " create folder given or not named\n";
                break;
            case 14: // cd
                break;
            case 15: // pwd
                break;
            case 16: // help
                std::cout << help() << std::endl;
                break;
            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
    } while (bRun == true);

    return 0;
}

int parseCommandString(const std::string &userCommand, std::string strArr[]) {
    std::stringstream ssin(userCommand);
    int counter = 0;
    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int findCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

bool quit() {
	std::cout << "Exiting\n";
	return false;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit, q, exit                     Quit OSD Disk Tool\n";
    helpStr += "* format                            Formats disk\n";
    helpStr += "* ls     <path>                     Lists contents of <path>.\n";
    helpStr += "* create <path>                     Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <path>                     Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>          Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>          Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>                     Removes <file>\n";
    helpStr += "* cp     <source> <destination>     Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>     Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>      Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>                Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>                Changes current working directory to <directory>\n";
    helpStr += "* pwd                               Get current working directory\n";
    helpStr += "* help                              Prints this help screen\n";
    return helpStr;
}

/* Insert code for your shell functions and call them from the switch-case */
