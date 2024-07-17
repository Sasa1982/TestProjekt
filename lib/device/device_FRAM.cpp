
#include "FRAM_MB85RC_I2C.h"


char ee_ssid[32];
char ee_pass[32];

//Fram
//Creating object for FRAM chip
boolean wp = false; //Allows memory write
uint16_t chipDensity = 16; //Just change that value to the density you want : 4, 16, 64, 128, 256, 512 or 1024
uint8_t chipaddress = 0x50;

//Creating object for FRAM chip
FRAM_MB85RC_I2C fram(chipaddress, wp, -1, chipDensity);
//Creating object for FRAM chip
//FRAM_MB85RC_I2C mymemory;

// byte resultw, resultr; 
// //dummy data used for the test
// uint8_t writevalue = 0xBE;
// uint16_t writevalue2 = 0xBEEF;
// uint8_t readvalue = 0xEF; // at the end, readvalue should be equal to writevalue
// uint16_t readvalue2 = 0xDEAD;

//Example for mamual mode

//random addresses to write from
uint16_t writeaddress =  0x750 ; // Beginning of the memory map
uint16_t writeaddress2 = (chipDensity * 128) - 80; // calculated regarding density to hit more or less the end of memory map

//define a struct of various data types
typedef struct MYDATA_t {
	bool data_0;
	float liter; 
	int k_faktor_1;
};
//define a struct joining MYDATA_t to an array of bytes to be stored
typedef union MYDATA4I2C_t {
 MYDATA_t datastruct;
 uint8_t I2CPacket[sizeof(MYDATA_t)];
};

MYDATA4I2C_t mydata; //data to be written in memory
MYDATA4I2C_t readdata; //data read from memory
void init_fram()
{
    fram.begin();
}


void writeIntIntoFRAM(int address, int number)
{ 
//   byte byte1 = number >> 8;
//   byte byte2 = number & 0xFF;
//   fram.writeByte(address, byte1);
//   fram.writeByte(address + 1, byte2);
    fram.writeWord(address, number);
}

int readIntFromFRAM(int address)
{
  uint16_t result=0;
  fram.readWord(address, &result);
//   byte byte1;
// byte byte2;
//   fram.readByte(address, &byte1);
//   fram.readByte(address, &byte1+1);
//   return (byte1 << 8) + byte2;
return int(result);
}

byte readByteFromFRAM(int address)
{
  byte result=false;

	fram.readByte(address, &result);
  return result;
}

byte writeByteToFRAM(int address,int value)
{
  byte result;

	result = fram.writeByte(address,value);
  return result;
}

void wifi_save_to_FRAM(String ssid, String pass)
{
  // if ((ssid != ee_ssid) || (pass != ee_pass))
  // {
  if (ssid.length() > 0)
  {
    for (int i = 0; i < 96; i++)
    {
      fram.writeByte(i, '\0');
    }

    for (int i = 0; i < ssid.length(); i++)
    {
      fram.writeByte(0 + i, ssid[i]);
    }

    for (int i = 0; i < pass.length(); i++)
    {
      fram.writeByte(32 + i, pass[i]);
    }
    // EEPROM.commit();
    //ee_ssid = ssid;
    //ee_pass = pass;
  }
  // }
}

void wifi_set_ssid_to_FRAM(String ssid)
{
  // if ((ssid != ee_ssid) || (pass != ee_pass))
  // {
  if (ssid.length() > 0)
  {
    for (int i = 0; i < 32; i++)
    {
      fram.writeByte(i, '\0');
    }

    for (int i = 0; i < ssid.length(); i++)
    {
      fram.writeByte(0 + i, ssid[i]);
    }

  }
  // }
}

void wifi_set_pass_to_FRAM(String pass)
{
   Serial.print("wifi_set_pass_to_FRAM: ");
  Serial.println(pass);
  for (int i = 0; i < 64; i++)
  {
    fram.writeByte(i, '\0');
  }

  for (int i = 0; i < pass.length(); i++)
  {
    fram.writeByte( i, pass[i]);
  }
}

String wifi_get_ssid_FRAM()
{


   for (int i = 0; i < 32; i++)
    {
      ee_ssid[i] = char(readByteFromFRAM(i));
    }
 // ee_ssid.trim();
  return ee_ssid;
}

String wifi_get_pass_FRAM()
{

  // ee_ssid = "";
  //  for (int i = 0; i < 32; i++)
  //  {
  //    ee_ssid += char(readByteFromFRAM(i));
  //     if(ee_ssid=='\0') break;//break when end of sting is reached before maxLength
  //  }
  for (int i = 0; i < 64; i++)
  {
    ee_pass[i] = char(readByteFromFRAM( i));
  }
  // ee_ssid.trim();
  Serial.print("wifi_get pass from FRAM: ");
  Serial.println(ee_pass);
  return ee_pass;
}
