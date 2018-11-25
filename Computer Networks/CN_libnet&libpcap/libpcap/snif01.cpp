#include <iostream>

#include <cstring>     // memset
#include <cstdlib>     // exit
#include <unistd.h>    // getopt()
#include <csignal>    // Ctrl+C handling

#include <pcap.h>      // libpcap

using namespace std;

// Ctrl+C interrupt handler
void bypass_sigint(int sig_no) {
  cout << endl << "*** Capture process interrupted by user..." << endl;

  exit(0); // we're done!
}

// First libpcap program : device selection
int main(int argc, char *argv[]) {
  char *device = NULL;             // device to sniff
  char argch;                      // to manage command line arguments
  char errbuf[PCAP_ERRBUF_SIZE];   // to handle libpcap error messages

  // Install Ctrl+C handler
  struct sigaction sa, osa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &bypass_sigint;
  sigaction(SIGINT, &sa, &osa);

  // Process command line arguments
  while ((argch = getopt(argc, argv, "hd:")) != EOF)
    switch (argch) {
      case 'd':           // device name
        device = optarg;
        break;

      case 'h':           // show help info
        cout << "Usage: sniff [-d XXX -h]" << endl;
        cout << " -d XXX : device to capture from, where XXX is device name (ex: eth0)." << endl;
        cout << " -h : show this information." << endl;

        // Exit if only argument is -h
        if (argc == 2) return 0;
        break;
    }

  // Identify device to use
  if (device == NULL && (device = pcap_lookupdev(errbuf)) == NULL) {
    cerr << "error - " << errbuf << endl;
    return -2;
  }
  else
    cout << "device = " << device << endl;

  return 0;
}
