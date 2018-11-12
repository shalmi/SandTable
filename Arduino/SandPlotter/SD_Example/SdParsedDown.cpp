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
#include "SD.h"
#include "SPI.h"

// globals
String gCodeFiles[10] = {"NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE","NONE"};
byte gCodeFileNumber = 0;
byte gCodeFilesTotal = 0;
const int bSize = 25; 
char Buffer[bSize];


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void listGCodeFiles(fs::FS &fs){
  const char * dirname = "/";
  uint8_t levels = 0;
    Serial.printf("Listing directory: %s\n", dirname);
    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    File file = root.openNextFile();
    while(file){
        if( ! file.isDirectory()){ // if This is a file and NOT a directory
            String fileName = file.name();
            if(fileName.indexOf("/._") < 0){ //ignore all Mac ._blah.ext files
              if(fileName.indexOf(".gcode") > 0){ // but still grab the rest of the gcode files
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


void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

File myFile;

void readLine(fs::FS &fs, const char * path){
    Serial.printf("Reading One line from file: %s\n", path);

    myFile = fs.open(path);
    if(!myFile){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(myFile.available()){
//      for(int a = 0; a<3;a++){
        int bytesRead = myFile.readBytesUntil('\n',Buffer,bSize);
        Serial.print("Bytes read:");
        Serial.println(bytesRead);
        Serial.println(Buffer);
//      }
    }
//    Serial.println(Buffer);
    myFile.close();
}

void setup(){
    Serial.begin(115200);
    if(!SD.begin(A5)){
        Serial.println("Card Mount Failed");
        return;
    }

    listGCodeFiles(SD);
//    listDir(SD, "/", 2);
    readLine(SD, "/test.gcode");
//    readFile(SD, "/foo.txt");

    Serial.println("\nNames of gCode Files");
    gCodeFilesTotal = gCodeFileNumber;
    for (int i=0; i<gCodeFilesTotal; i++)  {   
        Serial.println(gCodeFiles[i] );
  }
    gCodeFileNumber=0;
}

void loop(){

}