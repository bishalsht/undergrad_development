#include <iostream>

#include <libnet.h>       // libnet

using namespace std;

// First libnet program
int main(int argc, char *argv[]) {
  libnet_t *libnet_ctx;                  // libnet session context
  char      errbuf[LIBNET_ERRBUF_SIZE];  // buffer to get error messages

  // Validate command line call
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <device>" << endl;
    return -1;
  }

  // Get libnet session context and validate 
  libnet_ctx = libnet_init(LIBNET_RAW4, argv[1], errbuf);
  if (libnet_ctx == NULL) {
    cerr << "error - libnet_init() failed (" << errbuf << ")" << endl;
    return -1; 
  }

  // Free libnet session context
  if (libnet_ctx) libnet_destroy(libnet_ctx);

  return 0;
}