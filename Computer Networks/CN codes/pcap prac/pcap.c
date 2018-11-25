#include <pcap/pcap.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_u_int32 netp;
	bpf_u_int32 maskp;

	char *dev;
	pcap_t *handle;

	if ((dev = pcap_lookupdev(errbuf))==NULL)
		printf("pcap_lookupdev error: %s\n", errbuf);

	printf("Device Name: %s\n", dev);

	if ((handle = pcap_open_live(dev, 200, 0, 500, errbuf))==NULL)
		printf("pcap_open_live error: %s\n", errbuf);

	if (pcap_lookupnet(dev, &netp, &maskp, errbuf) == -1)
		printf("pcap_lookupnet error: %s\n", errbuf);

	char str1[INET_ADDRSTRLEN], str2[INET_ADDRSTRLEN];

	printf("netp = %s, maskp = %s\n", inet_ntop(AF_INET, &netp, str1, sizeof(str1)),inet_ntop(AF_INET, &maskp, str2, sizeof(str2)));

	return 0;
}