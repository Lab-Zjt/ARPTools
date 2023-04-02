#ifndef ARPTOOLS_MYARPT_HPP
#define ARPTOOLS_MYARPT_HPP

#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <linux/if_ether.h>
#include <unistd.h>
#include <cstdio>
#include <netpacket/packet.h>
#include <net/if.h>
#include <thread>
#include <atomic>

typedef unsigned char byte;
class bytestr {
  byte *ustr;
  int size;
  bool tempFlag;
public:
  byte *getStr(){
    return ustr;
  }
  int getSize(){
    return size;
  }
  bytestr(byte *str, int s){
    ustr = str;
    size = s;
    tempFlag = false;
  }
  bytestr & operator+(bytestr & b){
    auto nstr = new bytestr(nullptr, 0);
    nstr->tempFlag = true;
    nstr->ustr = new byte[this->size + b.size];
    nstr->size = this->size + b.size;
    memcpy(nstr->ustr, this->ustr, this->size);
    memcpy(nstr->ustr + this->size, b.ustr, b.size);
    return *nstr;
  }
  bytestr & ref(){
    return *this;
  }
  ~bytestr(){
    if (tempFlag) {
      delete[]ustr;
    }
  }
};
struct IMMAP {
  byte IPAddress[4] = {0x00};
  byte MACAddress[6] = {0x00};
};
struct countMutex {
  volatile bool mutex = false;
  int count = 0;
};
struct EtherPack {
  byte destMAC[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};//Broadcast MAC
  byte srcMAC[6] = {0xb0, 0x35, 0x9f, 0x6e, 0x49, 0x52};//Host MAC
  byte frameType[2] = {0x08, 0x06};
  struct {
    byte hardType[2] = {0x00, 0x01};
    byte proType[2] = {0x08, 0x00};
    byte hardLen = 0x06;
    byte proLen = 0x04;
    byte opreate[2] = {0x00, 0x01};
    byte hostMAC[6] = {0X00};
    byte hostIP[4] = {0xc0, 0xa8, 0x01, 0xc7};
    byte distMAC[6] = {0x00};
    byte distIP[4] = {0xc0, 0xa8, 0x01, 0x00};
  } arp;
  EtherPack();
};
IMMAP *ARPRequest();
void ARPRequest(IMMAP *mapp);
void IMMAPprint(IMMAP *mapp);
void ARPBroadcastMultiThread(EtherPack arpPack, IMMAP *mapp, byte beginIndex, byte endIndex, std::atomic<int> & count);
void ARPAttack(int victim, IMMAP *mapp);
int bytencmp(const byte *b1, const byte *b2, int n);
void printHex(byte *str, int size);
void dataTransmit(int target, IMMAP *mapp);
void selectDataTransmitTarget(IMMAP *mapp);
void dataTransmitPrepare(int target, IMMAP *mapp);
#endif
