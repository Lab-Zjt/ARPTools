#include <arpa/inet.h>
#include "mydns.hpp"

void DNSRequest(){
  int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  sockaddr_in dest;
  dest.sin_addr.s_addr = inet_addr("202.114.0.242");
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
  socklen_t l;
  res = recvfrom(fd, buf, 4096, 0, (sockaddr *) &dest, &l);
  if (res < 0) {
    perror("Fuck Again");
    return;
  } else {
    printf("Parse A Record Successfully.\n");
  }
  auto offset = msg.getSize() + 12;
  printf("IP:%d.%d.%d.%d\n", buf[offset], buf[offset + 1], buf[offset + 2], buf[offset + 3]);
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

