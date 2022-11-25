#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>

SPIClass *sd_spi = NULL;

//ESP32S2 5/7/8/9
#define PIN_SPI_SD_CS   5
#define PIN_SPI_SD_CLK  7
#define PIN_SPI_SD_MOSI 8
#define PIN_SPI_SD_MISO 9
 
void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.print(entry.size(), DEC);
      time_t lw = entry.getLastWrite();
      struct tm * tmstruct = localtime(&lw);
      Serial.printf("\tLAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    }
    entry.close();
  }
}

void setupSD() {

sd_spi = new SPIClass(FSPI);
Serial.println(F("SPIClass enabled"));
pinMode(PIN_SPI_SD_CS, OUTPUT);
sd_spi->begin(PIN_SPI_SD_CLK, PIN_SPI_SD_MISO, PIN_SPI_SD_MOSI, PIN_SPI_SD_CS);
Serial.println(F("SD SPI begin"));

if (!SD.begin(PIN_SPI_SD_CS, *sd_spi)){
    Serial.println("Card Mount Failed");
    return;
    }

  // print the type of card
  Serial.println();
  Serial.print("Card type: ");
  switch (SD.cardType()) {
    case CARD_NONE:
      Serial.println("NONE");
      break;
    case CARD_MMC:
      Serial.println("MMC");
      break;
    case CARD_SD:
      Serial.println("SD");
      break;
    case CARD_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
    }
 
    // print the type and size of the first FAT-type volume
    //uint32_t volumesize;
    //Serial.print("Volume type is:    FAT");
    //Serial.println(SDFS.usefatType(), DEC);
 
    Serial.print("Card size: (k)");
    Serial.println((float)SD.cardSize()/1000);
 
    Serial.print("Total bytes: ");
    Serial.println(SD.totalBytes());
 
    Serial.print("Used bytes: ");
    Serial.println(SD.usedBytes());
    Serial.println("==============");

    File dir =  SD.open("/");
    printDirectory(dir, 0);
 
}