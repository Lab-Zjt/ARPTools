#ifndef ARPTOOLS_MYCLI_HPP
#define ARPTOOLS_MYCLI_HPP

#include "myarpt.hpp"
#include "mydns.hpp"

#define clear() printf("\033[H\033[J")
void attack(IMMAP *mapp);
IMMAP *broadcast(IMMAP *mapp);
void menu();
void printMap(IMMAP *mapp);
void mainLoop();
void DNSQuery();

#endif
