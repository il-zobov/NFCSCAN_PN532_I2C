

//const int sclPin = D3;
//const int sdaPin = D4;
#include <vector>
#include <Wire.h>
#include "PN532_I2C.h"
#include "PN532.h"
#include "NfcAdapter.h"
//using namespace std;

std::vector<String> approvedTag = {"aaaa","bbbb"};
unsigned long myTime;
unsigned long currTime;
unsigned long zeroTime;

String tagId = "";
String secretTag = "04 21 23 0A 62 61 80";

bool isUnlocked = false;

PN532_I2C pn532i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532i2c);

void printTags( std::vector<String> tag)
{
  for (int i=0; i<tag.size(); i++)
  {
    Serial.println(tag[i]);
  }
}

bool tagPresent(std::vector<String> tagV,String tag)
{
  for (int i=0; i<tagV.size(); i++)
  {
    if(tagV[i].equals(tag))
    {
      return true;
    }
  }
  return false;
}

//list<>
 
void setup(void) 
{

  pinMode(D8, OUTPUT);
//pinMode(D9, OUTPUT);

 //printTags(approvedTag);
//  Serial.println(tagPresent(approvedTag,"aaaa"));
  
  /* for (int i=0;i<10;i++) v.push_back(i); 

   for (int i=0;i<20;i++) Serial.println(v[i]); */

  

 Wire.begin(D4, D3);
 nfc.begin();

zeroTime = millis();
  Serial.begin(115200);
 Serial.println("System initialized"); 
/*
   uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); 
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC); */
}


String tagToString(byte id[4])
{
  String tagId = "";
  for (byte i = 0; i < 4; i++)
  {
    if (i < 3) tagId += String(id[i]) + ".";
    else tagId += String(id[i]);
  }
  return tagId;
}
void loop() 
{
  currTime = millis();
  
  if((currTime - zeroTime)/1000 <30)
  {
    Serial.println("WORKING");
  }else
  {
    Serial.println("BLOCKED");
  }
  
 readNFC();

}


 
void readNFC() 
{
 if (nfc.tagPresent())
 {
   NfcTag tag = nfc.read();
  // tag.print();
   tagId = tag.getUidString();
   Serial.println(tagId);


   if (tagId.equals(secretTag)) {
    
    Serial.println("SECRET TAG");
   digitalWrite(D8, HIGH);
  // digitalWrite(D9, HIGH);
   delay(2000);
   digitalWrite(D8, LOW);
  // digitalWrite(D9, LOW);
    zeroTime = millis();
    if(isUnlocked)
    {
      isUnlocked = false;
    } else
    {
      isUnlocked = true;
    }
    
   }
   else if (tagPresent(approvedTag,tagId))
   {
    Serial.println("Tag present in list");
   }
   else 
   {
    Serial.println("Normal T");
    if(isUnlocked)
    {
      isUnlocked = false;
      approvedTag.push_back(tagId);
      printTags(approvedTag);
      
    }
    else
    {
      Serial.println("Blocked TAg");
    }
   }
   
 }
 delay(100);
} 
