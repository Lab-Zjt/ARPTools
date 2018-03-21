#include <fstream>
#include "myconf.hpp"
#include "myarpt.hpp"

byte hostMAC[6];
byte hostIP[4];
byte gateway[4];

void initialize(){
  printf("Loading Config...\n");
  std::fstream f("./im.config", std::ios_base::in);
  std::string s[3];
  for (int i = 0 ; i < 3 ; i ++) {
    std::getline(f, s[i]);
    auto str = s[i].c_str();
    if (strncmp(str, "ip=", 3) == 0) {
      int temp[4];
      sscanf(str + 3, "%d.%d.%d.%d", temp, temp + 1, temp + 2, temp + 3);
      for (int j = 0 ; j < 4 ; j ++) {
        hostIP[j] = byte(temp[j]);
      }
    } else if (strncmp(str, "mac=", 4) == 0) {
      unsigned int mac[6];
      sscanf(str + 4, "%x:%x:%x:%x:%x:%x", mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
      for (int j = 0 ; j < 6 ; j ++) {
        hostMAC[j] = byte(mac[j]);
      }
    } else if (strncmp(str, "gateway=", 8) == 0) {
      int gw[4];
      sscanf(str + 8, "%d.%d.%d.%d", gw, gw + 1, gw + 2, gw + 3);
      for (int j = 0 ; j < 4 ; j ++) {
        gateway[j] = byte(gw[j]);
      }
    }
  }
  f.close();
  printf("Config Has Been Loaded Successfully. Enter Any Key To Start The Program.\n");
  getchar();
}