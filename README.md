# ARPTools

Now it accomplished the basic requirement, includes ARP broadcast, ARP attack, and data transmission. But the speed of data transmission is slow and I still can not solve this problem. You have to run this program by root.

## Command Line Interface

After you start the program, you can choose 1 option from the menu which includes 5 options.

## ARP Broadcast

It allows you to broadcast ARP request and find other machines in the LAN. But you have to modify the HOSTMAC, HOSTIP, GATEWAY in myarpt.cpp:25,26,27,31,39,40 to yourself (You can find them by using ifconfig command) and recompile this program because I am lazy and hardcore them in this program.

## ARP Attack

It allows you to select a victim and make him or her can not connect to the Internet, and you can enter ENTER key to stop the attack.

## Data Transmission

It allows you to select a victim and transmit his or her data to your machine, and you can use wireshark or other packet capture tools to see what he or she doing. But once you select this option, the attack will not stop UNLESS you cancel the program.

## DNS Query

I just do the fxxk DNS Query and it can only get my blog's IP because I hardcode it in my code. This function only be used to learn what is DNS Query, and it will update a few days later.

