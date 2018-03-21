# ARPTools

现在完成了基础功能，包括ARP广播，ARP攻击，数据转发，DNS查询。但是数据转发的速度很慢，而且我仍未找到解决办法。你需要使用root权限来运行本程序。

Now it accomplished the basic requirement, includes ARP broadcast, ARP attack, data transmission and DNS query. But the speed of data transmission is slow and I still can not solve this problem. You have to run this program by root.

## Command Line Interface

在你启动程序之后，你可以从目录界面的五个选项中选择一个。

After you start the program, you can choose 1 option from the menu which includes 5 options.

## ARP Broadcast

这个功能允许你广播ARP请求来寻找同一个局域网内的其他机器，现在你可以通过在与程序相同的目录下新建一个“im.config”文件来修改你的配置。格式如下：

```
ip=192.168.0.1
mac=aa:bb:cc:dd:ee::ff
gateway=192.168.0.254
```

It allows you to broadcast ARP request and find other machines in the LAN. Now you can create a file named "im.config" to modify your settings in the same directory with the program. Here is the format:

```
ip=192.168.0.1
mac=aa:bb:cc:dd:ee::ff
gateway=192.168.0.254
```

## ARP Attack

这个功能允许你选择一个受害者然后让他或者她的机器无法连接网络，你可以通过按回车键来结束攻击。

It allows you to select a victim and make him or her can not connect to the Internet, and you can enter ENTER key to stop the attack.

## Data Transmission

这个功能允许你选择一个受害者然后转发它的流量到你的机器上，然后你可以使用Wireshark或者其他的抓包软件来查看这些包。但你一旦使用这个功能，这次攻击将不会停止除非你退出程序。

It allows you to select a victim and transmit his or her data to your machine, and you can use wireshark or other packet capture tools to see what he or she doing. But once you select this option, the attack will not stop UNLESS you cancel the program.

## DNS Query

现在它可以用来查询一个域名的IPV4地址或者IPV6地址或CNAME（别名）。

Now it can work for querying a domain name's IPv4 address, IPv6 address and CNAME. Enter 5 at menu, and enter the domain name, you can get the answer later.

