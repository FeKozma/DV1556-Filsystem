#include <sstream>
#include "filesystem.h"
#include <signal.h>
#include <windows.h> // WinApi header


const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 18;
const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
	"q","quit","exit","format","ls","create","cat","createImage","restoreImage",
	"rm","cp","append","mv","mkdir","cd","pwd","help", "disk"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int parseCommandString(const std::string &userCommand, std::string strArr[]);
int findCommand(std::string &command);
bool quit();

std::string help();
std::string getInput(std::string Question);
void print(std::string, int);
void emptyCommands(std::string *arr);
void SignalHandler(int signal);
void print(std::string text);

// add color to text
const int colorPromt = 3;
const int colorGreen = 2;
const int colorRed = 4;

int main(void) {
	SetConsoleTitle(TEXT("FELIX OCH JONATHAN´S FILSYSTEM"));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	FileSystem fileSys(250, 512);

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";  // Change this if you want another user to be displayed
	std::string currentDir = "/";  // Current directory, used for output

	bool bRun = true;


	/* Tests */
	//createFile
	if (!fileSys.createFile("file1\nroot folder\n", "file1", "/"))
	{
		print("err1\n", colorRed);
	}
	if (!fileSys.createFile("file2\nroot folder\n", "file2", "/"))
	{
		print("err2\n", colorRed);
	}
	if (fileSys.createFile("not right\nroot folder\n", "file2", "/"))
	{
		print("err3\n", colorRed);
	}

	//mkdir
	std::cout << fileSys.createFolderi("/dir1") << std::endl;
	std::cout << fileSys.createFolderi("/dir2") << std::endl;
	std::cout << fileSys.createFolderi("dir3") << std::endl;

	if (!fileSys.createFile("file1\ndir2\n", "file1", "/dir2"))
	{
		print("err4\n", colorRed);
	}
	std::cout << fileSys.createFolderi("/dir2") << std::endl;
	//fileSys.removeFile("dir2/file1");


	/* End of test */


	do {

		print(user + ":" + currentDir + "$ ", colorPromt);
		
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
					std::cout << fileSys.listDir(commandArr[1]);
					break;
				case 5: // create
					if (fileSys.createFile(getInput("Enter content"), getInput("Enter title"), commandArr[1])) {
						print("File created.\n", colorGreen);
					}
					else {
						print("File not created.\n", colorRed);
					}
					break;
				case 6: // cat
					print(fileSys.viewFileOn(commandArr[1]));
					break;
				case 7: // createImage
					if (fileSys.createImage(commandArr[1])) {
						print("Image created.\n", colorGreen);
					}
					break;
				case 8: // restoreImage
					if (fileSys.loadImage(commandArr[1])) {
						print("Filesystem successfully restored.\n", colorGreen);
					}
					else {
						print("Could not find image.\n", colorRed);
					}
					break;
				case 9: // rm
					if (fileSys.removeFile(commandArr[1])) {
						print("File removed.\n", colorGreen);
					}
					else {
						print("File not found.\n", colorRed);
					}
					break;
				case 10: // cp
					if (fileSys.copyFile(commandArr[1], commandArr[2])) {
						print("File copied.\n", colorGreen);
					}
					else {
						print("Error: file already exists.\n", colorRed);
					}
					break;
				case 11: // append
					if (fileSys.appendFile(commandArr[1], commandArr[2])) {
						print("Success.\n", colorGreen);
					}
					else {
						print("Error.\n", colorRed);
					}
					break;
				case 12: // mv
					if (fileSys.renameFileGivenPath(commandArr[1], commandArr[2])) {
						print("File renamed.\n", colorGreen);
					}
					else {
						print("File not found.\n", colorRed);
					}
					break;
				case 13: // mkdir
					for (int i = 1; i < nrOfCommands; ++i) {
						fileSys.createFolderi(commandArr[i]);
						commandArr[i] = "";
					}
					break;
				case 14: // cd
					fileSys.goToFolder(commandArr[1]);
					break;
				case 15: // pwd
					std::cout << fileSys.getCurrentPath() << std::endl;
					break;
				case 16: // help
					std::cout << help() << std::endl;
					break;
				case 17: // disk
					std::cout << fileSys.getDiskAllocations() << std::endl;
					break;
				default:
					std::cout << "Unknown command: " << commandArr[0] << std::endl;
			}

			// Empty command.
			emptyCommands(commandArr);
			
		}
	} while (bRun == true);

	return 0;
}


//set text color and print text
void print(std::string toPrint, int color) {
	SetConsoleTextAttribute(hConsole, color);
	std::cout << toPrint;
	SetConsoleTextAttribute(hConsole, 7);
}

void print(std::string text)
{
	if (text == "")
	{
		print("No data.\n", colorRed);
	}
	else
	{
		int prevPrint = 0;
		for (int i = 0; i < text.size() - 1; ++i)
		{
			if (text[i] == '\\')
			{
				if (text[i + 1] == 'n')
				{
					std::cout << text.substr(prevPrint, i - prevPrint) << std::endl;
					prevPrint = i + 2;
				}
			}
		}
		std::cout << text.substr(prevPrint, text.size() - prevPrint) << std::endl;
	}
}

// Write a question/string, then return the input when the user press enter.
std::string getInput(std::string question) {
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
int findCommand(std::string &command) {
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
