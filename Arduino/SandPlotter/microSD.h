/*
 * Connect the SD card to the following pins:
 *
 * SD Card | Huzzah32 Feather
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
// #ifndef SD_h
// #define SD_h
// #endif
#include "SD.h"
#include "SPI.h"
// include queue library header.
#include "QueueList.h"

struct gCodeStruct
{
    float x;
    float y;
}; // pointOne, pointTwo;

// globals
String gCodeFiles[10] = {"NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE", "NONE"};
byte gCodeFileNumber = 0;
byte gCodeFilesTotal = 0;
const int bSize = 25;
char Buffer[bSize];
bool fileEndFound = false;

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void listGCodeFiles(fs::FS &fs)
{
    gCodeFileNumber = 0;
    const char *dirname = "/";
    uint8_t levels = 0;
    //    Serial.printf("Listing directory: %s\n", dirname);
    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    File file = root.openNextFile();
    while (file)
    {
        if (!file.isDirectory())
        { // if This is a file and NOT a directory
            String fileName = file.name();
            if (fileName.indexOf("/._") < 0)
            { //ignore all Mac ._blah.ext files
                if (fileName.indexOf(".gcode") > 0)
                { // but still grab the rest of the gcode files
                    gCodeFiles[gCodeFileNumber] = fileName;
                    gCodeFileNumber++;
                    //                Serial.print("  FILE: ");
                    //                Serial.print(file.name());
                    //                Serial.print("  SIZE: ");
                    //                Serial.println(file.size());
                }
            }
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

File myFile;

String readLine()
{
    //    Serial.printf("Reading One line from file");
    int bytesRead = 0;
    while (myFile.available() && (bytesRead == 0))
    {
        //      for(int a = 0; a<3;a++){
        bytesRead = myFile.readBytesUntil('\n', Buffer, bSize);
        //        Serial.print("Bytes read:");
        //        Serial.println(bytesRead);
        //        Serial.println(Buffer);
        //     }
    }
    if (bytesRead > 0)
    {
        return Buffer;
    }
    else
    { //end of file. nothing was read
        return "";
    }
}

bool openFile(fs::FS &fs, const char *path)
{
    //  Opening File on SD Card
    myFile = fs.open(path);
    if (!myFile)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }
    //else
    return true;
}
void closeFile()
{
    myFile.close();
}

gCodeStruct gCodeToXandY(String gCodeString)
{

    if (gCodeString.startsWith("G01 "))
    {
        // Serial.println(gCodeString);
        gCodeString.remove(0, 4); // Remove six characters starting at index=2, "G01 "
        char *str = new char[gCodeString.length() + 1];
        strcpy(str, gCodeString.c_str());
        char *pch;
        pch = strtok(str, " XY");
        float inputs[2];
        int index = 0;
        while (pch != NULL)
        {
            //      Serial.println(pch);
            inputs[index] = strtof(pch, NULL);
            index++;
            if (index > 2)
            {
                Serial.println("THERE HAS BEEN AN ERROR");
                while (1)
                {
                    int x = 3;
                }
            }
            pch = strtok(NULL, " XY");
        }
        gCodeStruct returnPoint = {.x = inputs[0], .y = inputs[1]};
        return returnPoint;

        // //print the x and y values
        // for (int x = 0; x < 2; x++)
        // {
        //     Serial.println(inputs[x]);
        // }
    }
    fileEndFound = true;
    gCodeStruct returnPoint = {.x = -1, .y = -1};
    return returnPoint;
}


// create a queue of strings messages.
QueueList<gCodeStruct> queue;
// QueueList<String> queue;

double numsPushed = 0;

void otherCoreLoop()
{


    // set the printer of the queue.
    queue.setPrinter(Serial);

    if (!SD.begin(A5))
    {
        Serial.println("Card Mount Failed");
        return;
    }
    listGCodeFiles(SD);
    gCodeFilesTotal = gCodeFileNumber;
    //Runs on Task 0
    for (;;)
    {
        // Serial.println("\nBeginning Of Secondary Loop");
        
        for (int i = 0; i < gCodeFilesTotal; i++) // For each gCodeFile
        {
            numsPushed = 0;
            // Serial.println(gCodeFiles[i]); // Print the name of the File
            openFile(SD, gCodeFiles[i].c_str()); // converts string to const char* ?
            bool moreToFile = true;
            myFile.seek(0); // Move to begining of file incase we are re reading old Files.
            fileEndFound = false;
            while (moreToFile)
            {
                if (queue.count() < 30)
                {
                    // if(myFile.available()){ // This check doesnt seem to currently work...
                    gCodeStruct pushMe = gCodeToXandY(readLine());
                    if (fileEndFound)
                    {
                        moreToFile = false;
                        closeFile();
                    }
                    else{
                        queue.push(pushMe);
                        numsPushed++;
                    }
                    
                }
                delay(1);
            }
        }
        while(true){ //sit here forever when it is all done.
            int j=3;
            delay(5000);
        }
    }
}
