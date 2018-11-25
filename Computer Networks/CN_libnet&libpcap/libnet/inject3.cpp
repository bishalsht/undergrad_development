#include <iostream>
#include <cstdlib>        // sprintf, exit
#include <unistd.h>       // getopt()

#include <libnet.h>       // libnet

using namespace std;

libnet_t *libnet_ctx = NULL;     // libnet session context

// Function releasing all resources before ending program execution
void shutdown(int error_code) {
  // Free libnet session context
  if (libnet_ctx) 
    libnet_destroy(libnet_ctx);

  exit(error_code); // we're done!
}

// libnet program sending a ping (ICMP Echo Request)
int main(int argc, char *argv[]) {
  char                     *device = NULL;               // device with which to inject
  char                      argch;                       // to manage command line arguments
  char                      errbuf[LIBNET_ERRBUF_SIZE];  // buffer to get error messages
  u_int32_t                 ip_addr;                     // to manipulate IPv4 addresses
  struct libnet_ether_addr *mac_addr;                    // to manipulate MAC addresses
  char                     *target = NULL;               // IP of target host in string form
  u_int32_t                target_addr;                  // IP of target host

  // Process command line arguments
  while ((argch = getopt(argc, argv, "d:t:")) != EOF)
    switch (argch) {
      case 'd':           // device name
        device = optarg;
        break;

      case 'h':           // show help info
        cout << "Usage: inject [-d XXX -h]" << endl;
        cout << " -d XXX : device to capture from, where XXX is device name (ex: eth0)." << endl;
        cout << " -h : show this information." << endl;
        cout << " -t #.#.#.#  : IP address of ping target." << endl ;

        // Exit if only argument is -h
        if (argc == 2) return 0;
        break;
        
      case 't':           // target IP
        target = optarg;
        break;
    }
    
  // Make sure we have a target to ping
  if (target == NULL) {
    cerr << "error - no target to ping (use option -t)" << endl;
    shutdown(-1); 
  }

  // Get libnet session context and validate 
  libnet_ctx = libnet_init(LIBNET_RAW4, device, errbuf);
  if (libnet_ctx == NULL) {
    cerr << "error - libnet_init() failed (" << errbuf << ")" << endl;
    shutdown(-2); 
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

  // Convert target IP into integer form (with DNS resolution if needed)
  if ((target_addr = libnet_name2addr4(libnet_ctx, target, LIBNET_RESOLVE)) == -1) {
    cerr << "error - can't resolve " << target << endl;
    shutdown(-3);
  }
	
  // Tags for handling datagram building
  libnet_ptag_t icmp_ptag = LIBNET_PTAG_INITIALIZER;
  libnet_ptag_t ip_ptag   = LIBNET_PTAG_INITIALIZER;

  // Construct an ICMP Echo Request datagram
  icmp_ptag = libnet_build_icmpv4_echo(ICMP_ECHO, 0, 0, 0, 0, NULL, 0, libnet_ctx, 0);
  if (icmp_ptag == -1) {
    cerr << "error - can't build ICMP header (" << libnet_geterror(libnet_ctx) << ")" << endl;
    shutdown(-4); 
  }

  // Construct an IP packet to encapsulate the ICMP datagram
  ip_ptag = libnet_autobuild_ipv4(LIBNET_IPV4_H + LIBNET_ICMPV4_ECHO_H, IPPROTO_ICMP, 
                                  target_addr, libnet_ctx);
  if (ip_ptag == -1) {
    cerr << "error - can't build IP header (" << libnet_geterror(libnet_ctx) << ")" << endl;
    shutdown(-5); 
  }

  // Inject the resulting datagram
  int bytes = libnet_write(libnet_ctx);
  if (bytes == -1)
    cerr << "error - failed to inject (" << libnet_geterror(libnet_ctx) << ")" << endl;
  else {
    cout << "Sending " << bytes << " bytes of data to target " 
         << libnet_addr2name4(target_addr, LIBNET_DONT_RESOLVE) << endl;
    
    libnet_clear_packet(libnet_ctx);   // clear datagram associated to context (optional)
  }

  // Shutdown the application
  shutdown(0);
}