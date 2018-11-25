#include <iostream>
#include <cstdlib>        // sprintf
#include <unistd.h>       // getopt()

#include <libnet.h>       // libnet

using namespace std;

// First libnet program with IPv4 and Ethernet address handling
int main(int argc, char *argv[]) {
  char                     *device = NULL;               // device with which to inject
  char                      argch;                       // to manage command line arguments
  libnet_t                 *libnet_ctx;                  // libnet session context
  char                      errbuf[LIBNET_ERRBUF_SIZE];  // buffer to get error messages
  u_int32_t                 ip_addr;                     // to manipulate IPv4 addresses
  struct libnet_ether_addr *mac_addr;                    // to manipulate MAC addresses

  // Process command line arguments
  while ((argch = getopt(argc, argv, "d:")) != EOF)
    switch (argch) {
      case 'd':           // device name
        device = optarg;
        break;

      case 'h':           // show help info
        cout << "Usage: inject [-d XXX -h]" << endl;
        cout << " -d XXX : device to capture from, where XXX is device name (ex: eth0)." << endl;
        cout << " -h : show this information." << endl;

        // Exit if only argument is -h
        if (argc == 2) return 0;
        break;
    }

  // Get libnet session context and validate 
  libnet_ctx = libnet_init(LIBNET_RAW4, device, errbuf);
  if (libnet_ctx == NULL) {
    cerr << "error - libnet_init() failed (" << errbuf << ")" << endl;
    return -1; 
  }
   
  // Get IPv4 address given to device
  ip_addr = libnet_get_ipaddr4(libnet_ctx);
  if (ip_addr != -1)
    cout << "IP address = " << libnet_addr2name4(ip_addr, LIBNET_DONT_RESOLVE) << endl;
  else
    cerr << "error - libnet_get_ipaddr4 failed (" << libnet_geterror(libnet_ctx) << ")" << endl;

  // Get MAC address given to device
  mac_addr = libnet_get_hwaddr(libnet_ctx);
  if (mac_addr != NULL) {
    char s[18];
    sprintf(s, "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr->ether_addr_octet[0],
                                                mac_addr->ether_addr_octet[1],
                                                mac_addr->ether_addr_octet[2],
                                                mac_addr->ether_addr_octet[3],
                                                mac_addr->ether_addr_octet[4],
                                                mac_addr->ether_addr_octet[5]);
    cout << "MAC address = " << s << endl;
  }
  else
    cerr << "error - libnet_get_hwaddr failed (" << libnet_geterror(libnet_ctx) << ")" << endl;

  // Free libnet session context
  if (libnet_ctx) libnet_destroy(libnet_ctx);

  return 0;
}