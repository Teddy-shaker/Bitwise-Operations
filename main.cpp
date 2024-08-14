// Fedor Vasilev
// Description:
// This program reads a text document, “packs” the ASCII characters
// from that document into unsigned int variables, and outputs those variables
// to another text file as integers in HEXADECIMAL format.
// This program also reverses the process, converting the unsigned int numbers
// back into a copy of the original text document
#include <cstddef>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

bool verboseMode; // Global variable (controls printing of "debug" output)

string getInputName();
string getOutputName();
bool confirmYN(string);
void displayHelpText();


void pack(ifstream& file, ofstream& oFile) // function that convert (packs) chars of a string into unsigned ints 
{
    string nameOfThefile = getInputName();
    string nameOfTheOutFile = getOutputName();
    file.open(nameOfThefile);
    oFile.open(nameOfTheOutFile);
    if (!file) {
        cout << "Error opening input file." << endl;
        exit(0);
    }
    string outputText;
    while (getline(file, outputText)) // reads  a line from a text file 
    {
        cout << "Input text (length=" << outputText.length() << "):" << outputText << endl;
        outputText = outputText + '\n';
        unsigned int outputValue = 0;
        unsigned int textCharacter = 0;
        int offset = 0;
        for (int position = 0; position <= outputText.length(); position++) // inner loop to go through each element of a string
        {
            textCharacter = outputText[position]; // assigns char from a string to unsigned int 
            offset = position % 4; // stores the value that determines the ultimate location of the char to be stored in
            unsigned int shifted_character = 0; //stores the value for the char in the bit position

            if (position < outputText.length()) {
                switch (offset) // deals with bits according to a numbers of an element  
                {
                case 0:
                    shifted_character = textCharacter << 24; // shifts char to the location depending on the offset value
                    outputValue = outputValue | shifted_character;// does bitwise OR Operation to store the value in the right location 
                    break;                                        // of the unsigned int
                case 1:
                    shifted_character = textCharacter << 16;
                    outputValue = outputValue | shifted_character;
                    break;
                case 2:
                    shifted_character = textCharacter << 8;
                    outputValue = outputValue | shifted_character;
                    break;
                case 3:
                    shifted_character = textCharacter << 0;
                    outputValue = outputValue | shifted_character;
                    oFile << hex << uppercase << outputValue << endl; // passes the value to of the unsigned int that stores 4 chars to a file 
                    outputValue = 0;
                    break;
                }
            }
            else {
                //      switch (offset) // deals with bits according to a numbers of an element  
                //     {
                //        case 0:
                //        shifted_character = textCharacter << 24; // shifts char to the location depending on the offset value
                // 	   outputValue = outputValue | shifted_character;// does bitwise OR Operation to store the value in the right location 
                // 	    break;                                        // of the unsigned int
                //        case 1:
                // 	   shifted_character = textCharacter << 16; 
                // 	   outputValue = outputValue | shifted_character;
                // 	    break;
                //        case 2:
                // 	   shifted_character = textCharacter << 8;
                // 	   outputValue = outputValue | shifted_character;
                // 	    break;
                //        case 3:
                // 	   shifted_character = textCharacter << 0;
                // 	   outputValue = outputValue | shifted_character;                                                
                //         break;  
                // } 
                shifted_character = textCharacter << 24;
                outputValue = outputValue | shifted_character;
                oFile << hex << uppercase << outputValue << endl;

            }
        }
        oFile << endl; // separates lines that contain the entire string read
    }

    file.close();
    oFile.close();
}

void unpack(ifstream& inputFile, ofstream& outputFile) // function that convert (unpacks) unsigned ints into chars of a string 
{
    string nameOfThefile = getInputName();
    string nameOfTheOutFile = getOutputName();
    inputFile.open(nameOfThefile);
    outputFile.open(nameOfTheOutFile);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cout << "Error opening the file." << endl;
        return;
    }

    unsigned int inputNumber;
    while (inputFile >> hex >> inputNumber) {
        for (int shift = 24; shift >= 0; shift -= 8) {
            unsigned char unpackedBits = (inputNumber >> shift) & 0xFF; // Extract one byte at a time

            if (unpackedBits != 0) {
                outputFile << unpackedBits; // Output the unpacked byte
            }
        }
    }

    inputFile.close();
    outputFile.close();
}

int main() {


    ifstream file; // file to read the initial data from
    ofstream oFile; // file to output packed data to  
    ifstream inputFile; // file to read packed data from 
    ofstream outputFile; // file to output unpacked data to 

    string command;

    bool keepRunning = true;

    while (keepRunning == true)
    {
        // Prompt for command 
        cout << "Command: ";
        getline(cin, command);

        if (command == "p") {
            pack(file, oFile);
        }
        else if (command == "u") {
            unpack(inputFile, outputFile);
        }
        else if (command == "q") {
            keepRunning = !confirmYN("Are you sure you want to exit the program? ");
        }
        else if (command == "h") {
            displayHelpText();
        }
        else {
            cout << "Invalid command: " << command << endl;
        }

    }
    return 0;
}

string getInputName() { //function that gets a name of an input file
    string name = "";
    cout << "Enter the input filename: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Please enter a filename: ";
        getline(cin, name);
    }
    return name;
}

string getOutputName() { // function that gets a name of output file
    string name = "";
    cout << "Enter the output filename: ";
    getline(cin, name);
    while (name.empty()) {
        cout << "Please enter a filename: ";
        getline(cin, name);
    }
    return name;
}

bool confirmYN(string promptText)
{
    string inputString;
    bool confirmationValue = false;
    bool inputIsOK = false;

    do
    {
        // input from user 
        cout << promptText;
        getline(cin, inputString);

        if (inputString == "y" || inputString == "Y")
        {
            confirmationValue = true;
            inputIsOK = true;
        }
        else if (inputString == "n" || inputString == "N")
        {
            confirmationValue = false;
            inputIsOK = true;
        }
        else
        {
            cout << "Invalid input:  " << inputString << ".  Please input y or n." << endl;
        }
    } while (inputIsOK == false);
    return confirmationValue;

} // (end function 'confirmYN')

void displayHelpText() {
    // Help text.
    cout << "Supported commands: \n"
        << "     p    Build Packed Data File.\n"
        << "     u    Create unpacked (text) data from packed data.\n"
        << "     h    Print this help text.\n"
        << "     q    quit (end the program).\n"
        << endl;
}