
#include <Arduino.h>
void writeIntIntoFRAM(int address, int number);
int readIntFromFRAM(int address);
void init_fram();
byte readByteFromFRAM(int address);

byte writeByteToFRAM(int address,int value);
void wifi_save_to_FRAM(String ssid, String pass);
String wifi_get_ssid_FRAM();
String wifi_get_pass_FRAM();

void wifi_set_ssid_to_FRAM(String ssid);
void wifi_set_pass_to_FRAM(String pass);