#include <iostream>
#include <cctype>
#include <cstring>     // memset
#include <cstdlib>     // exit
#include <unistd.h>    // getopt()
#include <signal.h>    // Ctrl+C handling
#include <arpa/inet.h> // struct in_addr
#include <pcap.h>      // libpcap
/*
  Ethernet frames captured by libpcap are automatically stripped of
  their leading preamble and SFD fields, as well as their leading FCS and interframe gap fields.
  Therefore these removed fields are not available to the callback function,
  its packet parameter pointing to the first byte of the Ethernet frame's destination MAC field
 */
using namespace std;
pcap_t *pcap_session = NULL; // libpcap session handler


unsigned short char2word(const unsigned char *p) {
    unsigned short i = p[0];
    return i << 8 | p[1];
}

unsigned int char4word(const unsigned char *p) {
    unsigned int res = p[0];

    res = res << 8 | p[1];
    res = res << 8 | p[2];
    res = res << 8 | p[3];

    return res;
}


void bypass_sigint(int sig_no) {
  cout << endl << "*** Capture process interrupted by user..." << endl;

  // Close libpcap session
  if (pcap_session != NULL)
    pcap_close(pcap_session);

  exit(0);
}
void MACaddress(const unsigned char *packet){
  char outstr[3];
  char str[18];
  str[0]='\0';
  int i;
  for(i=0;i<5;i++){
    sprintf(outstr, "%.2x.",packet[i]);
    strncat(str,outstr,3);
  }
  sprintf(outstr, "%.2x",packet[i]);
  strncat(str,outstr,2);
  cout<<str<<"\n";
}
void printEthernetHeader(const unsigned char *packet){
  cout<<"_________________________Ethernet header___________________________\n";
  cout<<"Destination MAC address  : \t";MACaddress(packet);
  cout<<"Source MAC address       : \t";MACaddress(packet+6);
  char outstr[8];
  int type = char2word(packet+12);
  sprintf(outstr, "0x%.4x",type);
  cout<<"ether ";
  if (type <= 0x05DC)cout<<"Type\t[Length]\t"<<type<<"\n";
  switch (type){
    case 0x6000 : cout<<"Type\t[DEC]\t"<<type<<"\n";break;
    case 0x0609 : cout<<"Type\t[DEC]\t"<<type<<"\n";break;
    case 0x0600 : cout<<"Type\t[XNS]\t"<<type<<"\n";break;
    case 0x0800 : cout<<"Type\t[IPv4]\t"<<type<<"\n";break;
    case 0x0806 : cout<<"Type\t[ARP]\t"<<type<<"\n";break;
    case 0x8019 : cout<<"Type\t[Domain]\t"<<type<<"\n";break;
    case 0x8035 : cout<<"Type\t[RARP]\t"<<type<<"\n";break;
    case 0x8037 : cout<<"Type\t[IPX]\t"<<type<<"\n";break;
    case 0x809B : cout<<"Type\t[AppleTalk]\t"<<type<<"\n";break;
    case 0x86DD : cout<<"Type\t[IPv6]\t"<<type<<"\n";break;
    case 0x9000 : cout<<"Type\t[loopback]\t"<<type<<"\n";break;
    default     : cout<<"Type\t[other]\t"<<type<<"\n";break;
  }
}
void IPAdress(const unsigned char *packet){
  char outstr[15];
  int k=0,i=0;
  for(;i<3;i++){
    sprintf(&outstr[k],"%d",packet[i]);
    k=strlen(outstr);
    outstr[k]='.';
    k++;
  }
  sprintf(&outstr[k],"%d",packet[i]);
  cout<<outstr<<"\n";
}
void gprotocol(int type){
  if  ( type == 0)		/* Dummy protocol for TCP		*/
                               cout<< 		"IP";
  else if  ( type == 1)		/* Internet Control Message Protocol	*/
                               cout<< 		"ICMP";
  else if  ( type == 2)		/* Internet Group Management Protocol	*/
                               cout<< 		"IGMP";
  else if  ( type == 4)		/* IPIP tunnels (older KA9Q tunnels use 94) */
                               cout<< 		"IPIP";
  else if  ( type == 6)		/* Transmission Control Protocol	*/
                               cout<< 		"TCP";
  else if  ( type == 8)		/* Exterior Gateway Protocol		*/
                               cout<< 		"EGP";
  else if  ( type == 12)		/* PUP protocol				*/
                               cout<< 		"PUP";
  else if  ( type == 17)		/* User Datagram Protocol		*/
                               cout<< 		"UDP";
  else if  ( type == 22)		/* XNS IDP protocol			*/
                               cout<< 		"IDP";
  else if  (type == 29)		/* SO Transport Protocol Class 4	*/
                               cout<< 		"TP";
  else if  ( type == 33)		/* Datagram Congestion Control Protocol */
                               cout<< 		"DCCP";
  else if  (type == 41)		/* IPv6-in-IPv4 tunnelling		*/
                               cout<<     "IPV6";
  else if  ( type == 46)		/* RSVP Protocol			*/
                               cout<< 		"RSVP";
  else if  ( type == 47)		/* Cisco GRE tunnels (rfc 1701)1702)	*/
                               cout<< 		"GRE";
  else if  ( type == 50)		/* Encapsulation Security Payload protocol */
                               cout<< 		"ESP";
  else if  ( type == 51)		/* Authentication Header protocol	*/
                               cout<< 		"AH";
  else if  ( type == 92)		/* Multicast Transport Protocol		*/
                               cout<< 		"MTP";
  else if  ( type == 94)		/* IP option pseudo header for BEET	*/
                               cout<< 		"BEETPH";
  else if  ( type == 98)		/* Encapsulation Header			*/
                               cout<< 		"ENCAP";
  else if  ( type == 103)		/* Protocol Independent Multicast	*/
                               cout<<      "PIM";
  else if  ( type == 108)		/* Compression Header Protocol		*/
                               cout<<   	"COMP";
  else if  ( type == 132)		/* Stream Control Transport Protocol	*/
                               cout<<    	"SCTP";
  else if  ( type == 136)	/* UDP-Lite (RFC 3828)			*/
                               cout<< 		"UDPLITE";
  else if  ( type == 255)		/* Raw IP packets			*/
                               cout<< 		"RAW";
  else  cout<<type;
}
void printIPHeader(const unsigned char *packet ,int length){
  //Assumption IPV4 packet is there
 cout<<"_________________________IP Header_________________________________\n";
 u_short ipv = packet[0];
 ipv >>= 4;
 cout<<"IP version IPV :\t"<<ipv<<"\n";
 u_short ihl = packet[0];
 ihl  = ihl & 0x000F;/**16 bits [##,## ] ## => 1 Byte*/
 ihl = ihl * 5;
 cout<<"IP header length IHL :\t"<<ihl<<"\n";
 cout<<"Type of service TOS :\t" <<int(packet[1])<<"\n";
 u_short totlen = char2word(packet+2);
 cout<<"Total IP packet length :\t"<<totlen<<"\n";
 u_short id = char2word(packet+4);
 cout<<"Identification number :\t"<<id<<"\n";
 ipv = char2word(packet+6);
 cout<<"Reserved key :\t"<<(ipv>>15)<<"\n";
 u_short mf = ipv>>14;
 mf = mf&(~(1<<2));
 cout<<"M/F :\t"<<mf<<"\n";
 u_short df = ipv >> 13;
 df = df&(~(1<<2))&(~(1<<3));
 cout<<"D/F :\t"<<df<<"\n";
 u_short foffset  = ipv;
 foffset >>= 3;
 cout<<"Fragments offsets :\t"<<foffset<<"\n";
 cout<<"TTL :\t"<<(int)packet[8]<<"\n";
 cout<<"Protocol :\t";gprotocol(int(packet[9]));
 cout<<"\nHeader Check sum :\t"<<char2word(packet+10)<<"\n";
 cout<<"src IP address :\t";IPAdress(&packet[12]);
 cout<<"dest IP address :\t";IPAdress(&packet[16]);
 //print optinal field
 if(ihl>20){

 }
}
void printRaw(const unsigned char *packet,int length){


}
void printIBit(int8_t num ,int8_t i){
    int8_t num2 = 1;
    if(num&(num2<<(i-1))){
        cout<<'1';
    }
    else cout<<'0';
}
void printPacket(const unsigned char *packet,int length){
  printEthernetHeader(packet);/**0 to 14 Bytes optinal 4 bytes(0 to 16) */
  printIPHeader(packet+14,length-14);/**Ethernet payloads*/
  int8_t ihl = packet[0];
  ihl  = ihl & 0x0F;/**16 bits [##,## ] ## => 1 Byte*/
  ihl = ihl * 5;
  int j = 14+ihl;
  int8_t proto = int8_t(packet[23]);
  switch(proto){
    case 6:/**TCP*/{
            cout<<"__________________________TCP header______________________\n";
            cout<<"Src Port Number :\t"<<char2word(&packet[j])<<"\n";j+=2;
            cout<<"Dest port Number :\t"<<char2word(&packet[j])<<"\n";j=j+2;
            cout<<"Sequence number :\t"<<char4word(&packet[j])<<"\n";j=j+4;
            cout<<"Acknowledgment number :\t"<<char2word(&packet[j])<<"\n";j=j+4;
            int8_t offset = int8_t(packet[j]);
            offset >>=4;
            cout<<"DATA Offset :\t"<<offset<<"\n";
            int8_t resv = int8_t(packet[j]);
            resv<<=4;
            resv>>=5;
            cout<<"Reserved :\t"<<resv<<"\n";
            cout<<"NS :\t";printIBit(packet[j],1);
            j+=1;
        }
        break;
    case 17:/**UDP*/{
            cout<<"__________________________UDP header______________________\n";
            cout<<"Src Port Number :\t"<<char2word(&packet[j])<<"\n";j+=2;
            cout<<"Dest port Number :\t"<<char2word(&packet[j])<<"\n";j=j+2;
            cout<<"UDP Header length :\t"<<char2word(&packet[j])<<"\n";j=j+2;
            cout<<"Checksum :\t"<<char2word(&packet[j]);j=j+2;
        }
    break;
  }
}

void process_packet(unsigned char *user, const struct pcap_pkthdr * h, const unsigned char * packet) {
  cout << "\nGrabbed " << h->caplen << " bytes (" << static_cast<int>(100.0 * h->caplen / h->len)
       << "%) of datagram received on " << ctime((const time_t*)&h->ts.tv_sec);
  printPacket(packet,h->caplen);
}

int main(int argc, char *argv[]) {
  char *device = NULL;            // device to sniff
  char  argch;                    // to manage command line arguments
  char  errbuf[PCAP_ERRBUF_SIZE]; // to handle libpcap error messages
  int   siz     = 1518,           // max number of bytes captured for each datagram
        promisc = 0,              // deactive promiscuous mode
        cnt     = -1;             // capture indefinitely  *** ML ***

  // Install Ctrl+C handler
  struct sigaction sa, osa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &bypass_sigint;
  sigaction(SIGINT, &sa, &osa);

  // Process command line arguments
  while ((argch = getopt(argc, argv, "hpd:n:")) != EOF)  // *** ML ***
    switch (argch) {
      case 'd':           // device name
        device = optarg;
        break;

      case 'h':           // show help info
        cout << "Usage: sniff [-d XXX -h]" << endl;
        cout << " -d XXX : device to capture from, where XXX is device name (ex: eth0)." << endl;
        cout << " -h : show this information." << endl;
        cout << " -n : number of datagrams to capture." << endl;  // *** ML ***
        cout << " -p : activate promiscuous capture mode." << endl;

        // Exit if only argument is -h
        if (argc == 2) return 0;
        break;

      case 'n':           // number of datagrams to capture *** ML ***
        cnt = atoi(optarg);
        break;

      case 'p':           // active promiscuous mode
        promisc = 1;
        break;
    }

  // Identify device to use
  if (device == NULL && (device = pcap_lookupdev(errbuf)) == NULL) {
    cerr << "error - " << errbuf << endl;
    return -2;
  }
  else
    cout << "device = " << device << (promisc ? " (promiscuous)" : "") << endl;

  // Extract IP information for network connected to device
  bpf_u_int32 netp,  // ip address of network
              maskp; // network mask
  if ((pcap_lookupnet(device, &netp, &maskp, errbuf)) == -1) {
    cerr << "error - " << errbuf << endl;
    return -3;
  }

  // Translate ip address into textual form for display
  struct  in_addr addr;
  char   *net;
  addr.s_addr = netp;
  if ((net = inet_ntoa(addr)) == NULL)
    cerr << "error - inet_ntoa() failed" << endl;
  else
    cout << "network ip = " << net << endl;

  // Translate network mask into textual form for display
  char *mask;
  addr.s_addr = maskp;
  if ((mask = inet_ntoa(addr)) == NULL)
    cerr << "error - inet_ntoa() failed" << endl;
  else
    cout << "network mask = " << mask << endl;

  // Open a libpcap capture session
  pcap_session = pcap_open_live(device, siz, promisc, 1000, errbuf);
  if (pcap_session == NULL) {
    cerr << "error - pcap_open_live() failed (" << errbuf << ")" << endl;
    return -4;
  }

  // Start capturing... *** ML ***
  pcap_loop(pcap_session, cnt, process_packet, NULL);

  // Close libpcap session
  pcap_close(pcap_session);

  return 0;
}
