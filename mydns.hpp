#ifndef ARPTOOLS_MYDNS_HPP
#define ARPTOOLS_MYDNS_HPP

#include "myarpt.hpp"

struct dnsHeader {
  byte transID[2];
  byte flags[2];
  byte questions[2];
  byte resource[2];
  byte authority[2];
  byte addtional[2];
};
struct dnsQuery {
  byte *name;
  byte type[2];
  byte Class[2];
};
struct dnsAnswerHeader {
  byte pointer[2];
  byte type[2];
  byte Class[2];
  byte TimeToLive[4];
  byte dataLen[2];
};
namespace DNSType {
  enum {
    IPv6 = 0x001c, IPv4 = 0x0001, CNAME = 0x0005
  };
}
void DNSRequest();
void DNSAnswerParser(byte *buf);
bytestr addrToDnsFormat();
void DNSParser(bytestr & res, const int & answerOffset);
#endif
