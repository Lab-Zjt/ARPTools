#include <arpa/inet.h>
#include "mydns.hpp"

void DNSRequest(){
  int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  sockaddr_in dest;
  dest.sin_addr.s_addr = inet_addr("202.114.0.242");
  dest.sin_port = htons(53);
  dest.sin_family = AF_INET;
  dnsHeader header;
  header.transID[0] = 0x12;
  header.transID[1] = 0x34;
  header.questions[0] = 0x00;
  header.questions[1] = 0x01;
  header.authority[0] = 0x00;
  header.authority[1] = 0x00;
  header.addtional[0] = 0x00;
  header.addtional[1] = 0x00;
  dnsQuery q;
  byte addr[] = {0x03, 0x77, 0x77, 0x77, 0x0c, 0x73, 0x6f, 0x72, 0x61, 0x6b, 0x61, 0x73, 0x75, 0x67, 0x61, 0x6e, 0x6f,
                 0x03, 0x63, 0x6f, 0x6d, 0x00};
  q.name = addr;
  q.type[0] = 0x00;
  q.type[1] = 0x01;
  q.Class[0] = 0x00;
  q.Class[1] = 0x01;
  auto msg = (bytestr((byte *) &header, sizeof(header)).ref()) + (bytestr(q.name, sizeof(addr)).ref()) +
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
  printf("Name:www.sorakasugano.com\n");
  auto offset = msg.getSize() + 12;
  printf("IP:%d.%d.%d.%d\n", buf[offset], buf[offset + 1], buf[offset + 2], buf[offset + 3]);
}

