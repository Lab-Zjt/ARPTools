#include <arpa/inet.h>
#include <vector>
#include "mydns.hpp"

void DNSRequest(){
  int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  sockaddr_in dest;
  dest.sin_addr.s_addr = inet_addr("8.8.8.8");
  dest.sin_port = htons(53);
  dest.sin_family = AF_INET;
  dnsHeader header;
  header.flags[0] = 0x01;
  header.flags[1] = 0x00;
  header.transID[0] = 0x12;
  header.transID[1] = 0x34;
  header.questions[0] = 0x00;
  header.questions[1] = 0x01;
  header.authority[0] = 0x00;
  header.authority[1] = 0x00;
  header.addtional[0] = 0x00;
  header.addtional[1] = 0x00;
  dnsQuery q;
  auto addr = addrToDnsFormat();
  q.type[0] = 0x00;
  q.type[1] = 0x01;
  q.Class[0] = 0x00;
  q.Class[1] = 0x01;
  auto msg = (bytestr((byte *) &header, sizeof(header)).ref()) + addr +
             (bytestr(q.type, 2).ref()) + (bytestr(q.Class, 2).ref());
  auto res = sendto(fd, msg.getStr(), msg.getSize(), 0, (sockaddr *) &dest, sizeof(dest));
  if (res < 0) {
    perror("Fuck");
    return;
  }
  byte buf[4096];
  socklen_t l = sizeof(dest);
  res = recvfrom(fd, buf, 4096, 0, (sockaddr *) &dest, &l);
  if (res < 0) {
    perror("Fuck Again");
    return;
  }
  DNSParser(bytestr(buf, res).ref(), msg.getSize());
  //IPv6 Request
  bzero(buf, res);
  q.type[1] = 0x1c;
  auto ipv6Msg = (bytestr((byte *) &header, sizeof(header)).ref()) + addr +
                 (bytestr(q.type, 2).ref()) + (bytestr(q.Class, 2).ref());
  res = sendto(fd, ipv6Msg.getStr(), ipv6Msg.getSize(), 0, (sockaddr *) &dest, sizeof(dest));
  if (res < 0) {
    perror("Fuck");
    return;
  }
  res = recvfrom(fd, buf, 4096, 0, (sockaddr *) &dest, &l);
  if (res < 0) {
    perror("Fuck Again");
    return;
  }
  DNSParser(bytestr(buf, res).ref(), msg.getSize());
  close(fd);
}
bytestr addrToDnsFormat(){
  byte space[1] = {0x00};
  printf("Enter a domain:");
  auto addr = new char[1024];
  scanf("%s", addr);
  getchar();
  char *saveptr = nullptr;
  auto ptr = strtok_r(addr, ".", &saveptr);
  auto ptrlist = new char *[100];
  ptrlist[0] = ptr;
  int i = 1;
  for (; i < 100 ; i ++) {
    ptr = strtok_r(nullptr, ".", &saveptr);
    if (ptr != nullptr) {
      ptrlist[i] = ptr;
    } else {
      break;
    }
  }
  auto res = new byte[1024];
  int offset = 0;
  for (int j = 0 ; j < i ; j ++) {
    auto len = strlen(ptrlist[j]);
    res[offset ++] = byte(len);
    memcpy(res + offset, ptrlist[j], len);
    offset += len;
  }
  auto str = bytestr(res, offset).ref() + bytestr(space, 1).ref();
  delete[]addr;
  delete[]ptrlist;
  delete res;
  return str;
}
void DNSParser(bytestr & res, const int & answerOffset){
  auto buf = res.getStr();
  auto size = res.getSize();
  dnsHeader header;
  memcpy(&header, buf, sizeof(header));
  auto answer = header.resource[0] * 256 + header.resource[1];
  if (answer == 0) {
    return;
  }
  int offset = answerOffset;
  for (; offset < size ;) {
    dnsAnswerHeader answerHeader;
    memcpy(&answerHeader, buf + offset, sizeof(answerHeader));
    offset += sizeof(answerHeader);
    int len = answerHeader.dataLen[0] * 256 + answerHeader.dataLen[1];
    auto addr = new byte[len];
    memcpy(addr, buf + offset, len);
    if (answerHeader.type[1] == 0x05) {
      int tempOffset = 0;
      byte tempLen;
      bool comFlag = false;
      printf("CNAME:");
      while ((tempLen = buf[offset + tempOffset]) != 0x00) {
        if (addr[tempOffset + tempLen + 1] == 0xc0) {
          comFlag = true;
        }
        addr[tempOffset + tempLen + 1] = '.';
        tempOffset += tempLen + 1;
      }
      addr[tempOffset + tempLen] = '\0';
      if (comFlag){
        printf("%s.com\n", addr + 1);
      }else{
  
        printf("%s\n", addr + 1);
      }
    } else if (answerHeader.type[1] == 0x01) {
      printf("IP:%d.%d.%d.%d\n", buf[offset], buf[offset + 1], buf[offset + 2], buf[offset + 3]);
    } else if (answerHeader.type[1] == 0x1c) {
      printf("IP:");
      for (int i = 0 ; i < 14 ; i += 2) {
        printf("%02x%02x:", buf[offset + i], buf[offset + i + 1]);
      }
      printf("%02x%02x\n", buf[offset + 14], buf[offset + 15]);
    }
    delete addr;
    offset += len;
  }
}

