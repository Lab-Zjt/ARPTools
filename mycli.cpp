#include <thread>
#include "mycli.hpp"

void attack(IMMAP *mapp){
  clear();
  int victim;
  printf("Select a victim:");
  scanf("%d", &victim);
  getchar();
  ARPAttack(victim, mapp);
  printf("Attack Successfully.\n");
  getchar();
  menu();
}
IMMAP *broadcast(IMMAP *mapp){
  clear();
  printf("Broadcasting...\n");
  if (mapp != nullptr) {
    ARPRequest(mapp);
  } else {
    mapp = ARPRequest();
  }
  clear();
  IMMAPprint(mapp);
  printf("Broadcast Successfully.\n");
  getchar();
  menu();
  return mapp;
}
void menu(){
  clear();
  printf(
    "Welcome to use my ARP tools:\n\t1.ARP Broadcast\n\t2.ARP Attack\n\t3.Data Transmission\n\t4.Print IP-MAC Map\n"
      "Select one or enter q to quit:");
}
void printMap(IMMAP *mapp){
  clear();
  if (mapp == nullptr) {
    printf("IP-MAC Map Is Empty.\n");
  } else {
    IMMAPprint(mapp);
  }
  getchar();
  menu();
}
void mainLoop(){
  int op = 0;
  IMMAP *mapp = nullptr;
  while (true) {
    menu();
    op = getchar();
    getchar();
    if (op == '1') {
      mapp = broadcast(mapp);
    } else if (op == '2') {
      attack(mapp);
    } else if (op == 'q') {
      delete[]mapp;
      return;
    } else if (op == '3') {
      selectDataTransmitTarget(mapp);
    } else if (op == '4') {
      printMap(mapp);
    }
  }
}