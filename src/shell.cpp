#include <sstream>
#include "filesystem.h"
#include <signal.h>
#include <windows.h> // WinApi header


const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 19;
const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
	"q","quit","exit","format","ls","create","cat","createImage","restoreImage",
	"rm","cp","append","mv","mkdir","cd","pwd","help", "disk", "chmod"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(const std::string &command);
bool quit();

std::string help();
std::string getInput(const std::string question);
void print(const std::string, const int color);
void print(const std::string text);
void printLine(const std::string text, const int color = -1);
void emptyCommands(std::string *arr);
void SignalHandler(int signal);

// Add color to text
const int colorPrompt = 3;
const int colorGreen = 2;
const int colorRed = 4;

int main(void) {
	SetConsoleTitle(TEXT("FELIX OCH JONATHAN´S FILSYSTEM"));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	FileSystem fileSys(250, 512);

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1556"; // Change this if you want another user to be displayed
	std::string currentDir = "/"; // Current directory, used for output

	bool bRun = true;

	/*********************
	*** Start of tests ***
	*********************/
	/*
	// createFile
	if (!fileSys.createFile("file1\\nroot\\n", "file1")) printLine("err1", colorRed);
	if (!fileSys.createFile("file2\\nroot\\n", "file2")) printLine("err2", colorRed);
	if (fileSys.createFile("not right\\nroot folder\\n", "file2")) printLine("err3", colorRed);

	// mkdir
	if (!fileSys.createFolderi("/dir1")) printLine("err4", colorRed);
	if (!fileSys.createFolderi("/dir2")) printLine("err5", colorRed);
	if (!fileSys.createFolderi("dir3")) printLine("err6", colorRed);

	if (!fileSys.createFile("file1\ndir2\\n", "/dir2/file1")) printLine("err7", colorRed);

	if (fileSys.createFolderi("/dir2")) printLine("err8", colorRed);

	printLine("Current directory: " + fileSys.getCurrentPath());
	print(fileSys.listDir(""));
	*/
	/*********************
	**** End of tests ****
	*********************/


	do {
		print(user + ":" + currentDir + "$ ", colorPrompt);

		getline(std::cin, userCommand);

		int nrOfCommands = parseCommandString(userCommand, commandArr);
		if (nrOfCommands > 0) {

			int cIndex = findCommand(commandArr[0]);
			switch (cIndex) {
				case 0: // q
				case 1: // quit
				case 2: // exit
					bRun = quit();
					break;
				case 3: // format
					std::cout << "Blocks deleted: " << fileSys.formatSystem() << std::endl;
					break;
				case 4: // ls
					print(fileSys.listDir(commandArr[1]));
					break;
				case 5: // create
					if (fileSys.createFile(getInput("Enter content"), commandArr[1])) {
						printLine("File created.", colorGreen);
					}
					else {
						printLine("File not created.", colorRed);
					}
					break;
				case 6: // cat
					commandArr[2] = fileSys.viewFileOn(commandArr[1]);
					if (commandArr[2] == "") {
						printLine("Could not find the file or you do not have permission to read it.", colorRed);
					}
					else {
						printLine(fileSys.viewFileOn(commandArr[1]));
					}
					break;
				case 7: // createImage
					if (fileSys.createImage(commandArr[1])) {
						printLine("Image created.", colorGreen);
					}
					else {
						printLine("Image could not be created.", colorRed);
					}
					break;
				case 8: // restoreImage
					if (fileSys.loadImage(commandArr[1])) {
						printLine("Filesystem successfully restored.", colorGreen);
					}
					else {
						printLine("Could not find image.", colorRed);
					}
					break;
				case 9: // rm
					if (fileSys.removeFile(commandArr[1])) {
						printLine("File successfully removed.", colorGreen);
					}
					else {
						printLine("File not found or you don't have write permission.", colorRed);
					}
					break;
				case 10: // cp
					if (fileSys.copyFile(commandArr[1], commandArr[2])) {
						printLine("File copied.", colorGreen);
					}
					else {
						printLine("File not found or you don't have read permission.", colorRed);
					}
					break;
				case 11: // append
					if (fileSys.appendFile(commandArr[1], commandArr[2])) {
						printLine("Success.", colorGreen);
					}
					else {
						printLine("File not found or you might not have the permissions you need.", colorRed);
					}
					break;
				case 12: // mv
					if (fileSys.renameFileGivenPath(commandArr[1], commandArr[2])) {
						printLine("File moved.", colorGreen);
					}
					else {
						printLine("Error, could not move file.", colorRed);
					}
					break;
				case 13: // mkdir
					for (int i = 1; i < nrOfCommands; ++i) {
						if (fileSys.createFolderi(commandArr[i])) {
							printLine("Folder created: " + commandArr[i], colorGreen);
						}
						else {
							printLine("Folder could not be created: " + commandArr[i], colorRed);
						}
					}
					break;
				case 14: // cd
					fileSys.goToFolder(commandArr[1]);
					break;
				case 15: // pwd
					printLine(fileSys.getCurrentPath());
					break;
				case 16: // help
					print(help());
					break;
				case 17: // disk
					print(fileSys.getDiskAllocations());
					break;
				case 18: // chmod
					if (fileSys.changePermission(commandArr[1], commandArr[2])) {
						printLine("Permission of the file " + commandArr[2] + " has been changed.", colorGreen);
					}
					else {
						printLine("Could not change permission. The permission type has to be 2 (w), 4 (r) or 6 (rw).", colorRed);
					}
;					break;
				default:
					printLine("Unknown command: " + commandArr[0]);
			}

			// Empty command.
			emptyCommands(commandArr);

		}
	} while (bRun == true);

	return 0;
}


// Set text color and print text.
void print(const std::string toPrint, const int color) {
	SetConsoleTextAttribute(hConsole, color);
	std::cout << toPrint;
	SetConsoleTextAttribute(hConsole, 7);
}

// Used to print text to the screen.
void print(const std::string text) {
	if (text == "") {
		print("No data.\n", colorRed);
	}
	else {
		int prevPrint = 0;
		for (int i = 0; i < text.size() - 1; ++i) {
			if (text[i] == '\\') {
				if (text[i + 1] == 'n') {
					std::cout << text.substr(prevPrint, i - prevPrint) << std::endl;
					prevPrint = i + 2;
				}
			}
		}
		std::cout << text.substr(prevPrint, text.size() - prevPrint);
	}
}

void printLine(const std::string text, const int color) {
	if (color == -1) {
		print(text + "\n");
	} else {
		print(text + "\n", color);
	}
}

// Write a question/string, then return the input when the user press enter.
std::string getInput(const std::string question) {
	std::string retString;
	std::cout << (question + ": ");
	std::getline(std::cin, retString);
	return retString;
}

// Return the amount of commands the user entered (spaces probably).
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

// Find the current command that is used.
int findCommand(const std::string &command) {
	int index = -1;
	for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {
		if (command == availableCommands[i]) {
			index = i;
		}
	}
	return index;
}

// Will show a message that the program is existing.
bool quit() {
	std::cout << "Exiting\n";
	return false;
}

// Will return a string with all help information.
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
	helpStr += "* disk                              Displays the allocation state of all blocks.\n";
	helpStr += "* chmod  <accessrights> <filepath>  Change permission of a file.\n";
	helpStr += "                                      2 = w = Write\n";
	helpStr += "                                      4 = r = Read\n";
	helpStr += "                                      6 = rw = Read/Write\n";
	return helpStr;
}

// A signal handler for when you press for example Ctrl + C.
void SignalHandler(int signal) {
	if (signal == SIGINT) {
		std::cout << "\nCommand not found.\n";
		exit(signal);
	} else {
		// ...  
	}
}

// Is used when you have no command to show.
void emptyCommands(std::string *arr) {
	for (int i = 0; i < MAXCOMMANDS; ++i) {
		arr[i] = "";
	}
}
