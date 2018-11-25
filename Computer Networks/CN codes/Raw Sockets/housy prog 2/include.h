#include <bits/stdc++.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/select.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>



#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <csignal>
#include <ctime>

#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

#define DEST "127.0.0.1"
#define SADRRESS htonl(INADDR_ANY)
#define DADDRESS htonl(INADDR_ANY)
#define FAMILY AF_INET
#define Bzero(var,size) memset(var,0,size)
#define HLEN 5
#define RAW SOCK_RAW
#define MAXHOSTSIZE 256
#define MAXBUFFERSIZE 1024
#define MAXQSIZE 10
#define MAXWAITTIME 3
#define MAXCONNECTION 100
#define NOOFSERVICES 1
#define NOOFRESOURCE 1
#define NOOFCHILD 1
#define FAMILY AF_INET
#define UFAMILY AF_LOCAL
#define TCP SOCK_STREAM
#define UDP SOCK_DGRAM
#define ADDRESS INADDR_ANY
#define PORTNUMBER 8080
#define Bzero(var,size) memset(var,0,size)
#define PARENTSOCKET 0
#define CHILDSOCKET 1
#define HAVE_MSGHDR_MSG_CONTROL
#define SA sockaddr
#define STDIN 0
#define STDOUT 1
#define STDERROR 2
#define DUPFD 3
#define NOOFCOUPON 5
#define MAXT 3
#define MAXC 3
#define RANGE 100

#ifdef DEBUG
	#define Socket(sockfd,addr,addrelen)	 					SocketWrapper(sockfd,addr,addrelen,__LINE__) 
	#define Bind(sockfd,addr,addrlen)		 					BindWrapper(sockfd,addr,addrlen,__LINE__)
	#define Recvfrom(sockfd,buf,len,flags,addr,socklen) 		RecvfromWrapper(sockfd,buf,len,flags,addr,socklen,__LINE__)
	#define Recv(sockfd,buf,len,flags) 							RecvWrapper(sockfd,buf,len,flags,__LINE__)
	#define Accept(sockfd,addr,addrlen) 						AcceptWrapper(sockfd,addr,addrlen,__LINE__)
	#define Close(fd) 											CloseWrapper(fd,__LINE__)
	#define Listen(sockfd,backlog) 								ListenWrapper(sockfd,backlog,__LINE__)
	#define Sendto(sockfd,buf,len,flags,dest_addr,addrlen) 		SendtoWrapper(sockfd,buf,len,flags,dest_addr,addrlen,__LINE__)
	#define Send(sockfd,buf,len,flags)							SendWrapper(sockfd,buf,len,flags,__LINE__)
	#define Gethostbyname(buf) 				 					GethostbynameWrapper(buf,__LINE__)
	#define Select(maxfd,rdfd,wrtfd,exceptionfd,tv) 			SelectWrapper(maxfd,rdfd,wrtfd,exceptionfd,tv,__LINE__)
    #define Socketpair(domain,type,protocol,sv)				    SocketpairWrapper(domain,type,protocol,sv,__LINE__)
	#define Dup2(oldd,newd)										Dup2Wrapper(oldd,newd,__LINE__)
	#define Write_fd(fd,ptr,nbytes,sendfd)                      Write_fd_Wrapper(fd,ptr,nbytes,sendfd,__LINE__)
	#define Read_fd(fd,ptr,nbytes,sendfd)                       Read_fd_Wrapper(fd,ptr,nbytes,sendfd,__LINE__)
	#define Dup(fd)												DupWrapper(fd,__LINE__)
	#define Connect(sockfd,addr,addrlen)						ConnectWrapper(sockfd,addr,addrlen,__LINE__)
	#define Setsockopt(sockfd,level,optname,optval,optlen)		SetsockoptWrapper(sockfd,level,optname,optval,optlen,__LINE__)

#else

	#define Socket(sockfd,addr,addrelen)		    			SocketWrapper(sockfd,addr,addrelen) 
	#define Bind(sockfd,addr,addrlen) 							BindWrapper(sockfd,addr,addrlen)
	#define Recvfrom(sockfd,buf,len,flags,addr,socklen)			RecvfromWrapper(sockfd,buf,len,flags,addr,socklen)
	#define Recv(sockfd,buf,len,flags) 							RecvWrapper(sockfd,buf,len,flags)
	#define Accept(sockfd,addr,addrlen) 						AcceptWrapper(sockfd,addr,addrlen)
	#define Close(fd) 											CloseWrapper(fd)
	#define Listen(sockfd,backlog) 								ListenWrapper(sockfd,backlog)
	#define Sendto(sockfd,buf,len,flags,dest_addr,addrlen)      SendtoWrapper(sockfd,buf,len,flags,dest_addr,addrlen)
	#define Send(sockfd,buf,len,flags) 							SendWrapper(sockfd,buf,len,flags)
	#define Gethostbyname(buf) 									GethostbynameWrapper(buf)
	#define Select(maxfd,rdfd,wrtfd,exceptionfd,tv) 			SelectWrapper(maxfd,rdfd,wrtfd,exceptionfd,tv)
	#define Socketpair(domain,type,protocol,sv)				    SocketpairWrapper(domain,type,protocol,sv)
	#define Dup2(oldd,newd)										Dup2Wrapper(oldd,newd)
	#define Write_fd(fd,ptr,nbytes,sendfd)                      Write_fd_Wrapper(fd,ptr,nbytes,sendfd)
	#define Read_fd(fd,ptr,nbytes,sendfd)                       Read_fd_Wrapper(fd,ptr,nbytes,sendfd)
	#define Connect(sockfd,addr,addrlen)						ConnectWrapper(sockfd,addr,addrlen)
	#define Setsockopt(sockfd,level,optname,optval,optlen)		SetsockoptWrapper(sockfd,level,optname,optval,optlen)


#endif


using namespace std;

/****************************************************************************************************************************
*																															*
*		Common Functions																									*
*																															*
******************************************************************************************************************************/
void err_quit(const char *s){
			fprintf(stderr, "%s failed with error : [%s]\n", s, strerror(errno));
			exit(1);
}
void err_quit(const char *s,int lineno){
			fprintf(stderr, "%s failed  with error : [%s] at line no %d\n", s, strerror(errno),lineno);
			exit(1);
}
void* shareMemory(size_t size ,unsigned int key = -1){
    int shmid;
    if(key==-1){
      shmid = shmget(IPC_PRIVATE,size,IPC_CREAT|0666);
    }
    else{
        shmid = shmget(key,size,IPC_CREAT|0666);
    }
    return shmat(shmid,(void *)0,  0);
}
ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd){
	struct msghdr	msg;
	struct iovec	iov[1];

#ifdef	HAVE_MSGHDR_MSG_CONTROL
	union {
	  struct cmsghdr	cm;
	  char				control[CMSG_SPACE(sizeof(int))];
	} control_un;
	struct cmsghdr	*cmptr;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);

	cmptr = CMSG_FIRSTHDR(&msg);
	cmptr->cmsg_len = CMSG_LEN(sizeof(int));
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;
	*((int *) CMSG_DATA(cmptr)) = sendfd;
#else
	msg.msg_accrights = (caddr_t) &sendfd;
	msg.msg_accrightslen = sizeof(int);
#endif

	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	return(sendmsg(fd, &msg, 0));
}
ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd){
	struct msghdr	msg;
	struct iovec	iov[1];
	ssize_t			n;

#ifdef	HAVE_MSGHDR_MSG_CONTROL
	union {
	  struct cmsghdr	cm;
	  char				control[CMSG_SPACE(sizeof(int))];
	} control_un;
	struct cmsghdr	*cmptr;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
#else
	int				newfd;

	msg.msg_accrights = (caddr_t) &newfd;
	msg.msg_accrightslen = sizeof(int);
#endif

	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	if ( (n = recvmsg(fd, &msg, 0)) <= 0)
		return(n);

#ifdef	HAVE_MSGHDR_MSG_CONTROL
	if ( (cmptr = CMSG_FIRSTHDR(&msg)) != NULL &&
	    cmptr->cmsg_len == CMSG_LEN(sizeof(int))) {
		if (cmptr->cmsg_level != SOL_SOCKET)
			err_quit("control level != SOL_SOCKET");
		if (cmptr->cmsg_type != SCM_RIGHTS)
			err_quit("control type != SCM_RIGHTS");
		*recvfd = *((int *) CMSG_DATA(cmptr));
	} else
		*recvfd = -1;		/* descriptor was not passed */
#else
	if (msg.msg_accrightslen == sizeof(int))
		*recvfd = newfd;
	else
		*recvfd = -1;		/* descriptor was not passed */
#endif

	return(n);
}
/****************************************************************************************************************************
*																															*
*		Wrapper function with line number 																					*
*																															*	
******************************************************************************************************************************/

int SetsockoptWrapper(int sockfd, int level, int optname,const void *optval, socklen_t optlen,int lineno){
		int k = setsockopt(sockfd,level, optname,optval,optlen);
		if( k < 0 ){
			err_quit("Setsockopt()",lineno);
		}
		return k;
}
int ConnectWrapper(int sockfd, const struct sockaddr *addr,socklen_t addrlen,int lineno){
	int k = connect(sockfd,addr,addrlen);
	if( k < 0 ){
		err_quit("connect()",lineno);
	}
	return k;
}

int CloseWrapper(int fd,int lineno){
	int k = close(fd);
	if( k < 0 ){
			err_quit("close()",lineno);
	}
	return 0;
}
int BindWrapper(int sockfd, const struct SA *addr,socklen_t addrlen,int lineno){
		int k = bind(sockfd,addr,addrlen);
		if( k < 0 ){
			close(sockfd);
			err_quit("bind()",lineno);
		}
		return 0;
}
int SocketWrapper(int domain, int type, int protocol,int lineno){
		int sfd = socket(domain,type,protocol);
		if(sfd<0){
			close(sfd);
			err_quit("socket()",lineno);
		}
		return sfd;
}
int AcceptWrapper(int sockfd, struct SA *addr, socklen_t *addrlen ,int lineno){
		int k  = accept(sockfd,addr,addrlen);
		if( k < 0 ){
			close(sockfd);
			err_quit( "accept()",lineno);
			
		}
		return k;
}
int ListenWrapper(int sockfd,int backlog,int lineno){
	int k = listen(sockfd,backlog);
	if(k<0){
			close(sockfd);
			err_quit( "listen()",lineno);
			
	}
	return k;
}
ssize_t SendWrapper(int sockfd, const void *buf, size_t len, int flags,int lineno){
	ssize_t k = send(sockfd,buf,len,flags);
	if( k < 0 ){
		err_quit( "send()",lineno);
		close(sockfd);
		
	}
	return k;
}
ssize_t SendtoWrapper(int sockfd, const void *buf, size_t len, int flags,const struct SA *dest_addr, socklen_t addrlen,int lineno){
	ssize_t k = sendto(sockfd,buf,len,flags,dest_addr,addrlen);
	if( k < 0 ){
		err_quit( "sendto()",lineno);
		close(sockfd);
		
	}
	return k;
}
ssize_t RecvfromWrapper(int sockfd,void *buf, size_t len, int flags,struct SA *addr, socklen_t *addrlen,int lineno){
		ssize_t k = recvfrom(sockfd,buf,len,flags,addr,addrlen);
		if( k < 0 ){
			close(sockfd);
			err_quit( "recvfrom()",lineno);
			
		}
		return k;
}
ssize_t RecvWrapper(int sockfd,void *buf, size_t len, int flags,int lineno){
		ssize_t k = recv(sockfd,buf,len,flags);
		if( k < 0 ){
			close(sockfd);
			err_quit( "recv()",lineno);
			
		}
		return k;
}
struct hostent *GethostbynameWrapper(const char *name,int lineno){
	hostent *t = gethostbyname(name);
	if(t==NULL){
		err_quit( "gethostbyname()",lineno);
		
	}
	return t;
}
int SelectWrapper(int maxfd, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout,int lineno){
	int k = select(maxfd,readfds,writefds,exceptfds,timeout);
	if(k<0){
		err_quit( "select()",lineno);
		
	}
	return k;
}
int SocketpairWrapper(int domain, int type, int protocol, int sv[2],int lineno){
	int k = socketpair(domain,type,protocol,sv);
	if(k<0){
		err_quit( "socketpair()",lineno);
		
	}
	return k;
}
int  Dup2Wrapper(int oldd,int newd,int lineno){
	int k = dup2(oldd,newd);
	if(k<0){
		err_quit( "dup2()",lineno);
			
	}
	return k;
}
ssize_t Write_fd_Wrapper(int fd, void *ptr, size_t nbytes, int sendfd,int lineno){
	
	ssize_t		k = write_fd(fd,ptr,nbytes,sendfd);
	if (k < 0){
		err_quit( "Write fd () ",lineno);
		
	}
	return k;
}
ssize_t Read_fd_Wrapper(int fd, void *ptr, size_t nbytes, int *recvfd,int lineno){
	ssize_t		n;
	if ( (n = read_fd(fd, ptr, nbytes, recvfd)) < 0){
		err_quit( "Read fd () ",lineno);
		
	}
	return(n);
}
int DupWrapper(int fd,int lineno){
	int k =  dup(fd);
	if(k<0){
		err_quit("dup()",lineno);
	}
	return k;
}
/**********************************************************************************************************************
*																													  *
*		Wrapper function without line number 																		  *
*																													  *
**********************************************************************************************************************/
int SetsockoptWrapper(int sockfd, int level, int optname,const void *optval, socklen_t optlen){
		int k = setsockopt(sockfd,level, optname,optval,optlen);
		if( k < 0 ){
			err_quit("Setsockopt()");
		}
		return k;
}
int ConnectWrapper(int sockfd, const struct sockaddr *addr,socklen_t addrlen){
 	int k = connect(sockfd,addr,addrlen);
 	if( k < 0 ){
 		err_quit("connect()");
 	}
 	return k;
 }
int CloseWrapper(int fd){
	int k = close(fd);
	if( k < 0 ){
			err_quit( "close() ");
			
	}
	return 0;
}
int BindWrapper(int sockfd, const struct SA *addr,socklen_t addrlen){
		int k = bind(sockfd,addr,addrlen);
		if( k < 0 ){
			close(sockfd);
			err_quit( "bind() ");
			
		}
		return 0;
}
int SocketWrapper(int domain, int type, int protocol){
		int sfd = socket(domain,type,protocol);
		if(sfd<0){
			close(sfd);
			err_quit( "socket() ");
			
		}
		return sfd;
}
int AcceptWrapper(int sockfd, struct SA *addr, socklen_t *addrlen){
		int k  = accept(sockfd,addr,addrlen);
		if( k < 0 ){
			close(sockfd);
			err_quit( "accept() ");
			
		}
		return k;
}
int ListenWrapper(int sockfd,int backlog){
	int k = listen(sockfd,backlog);
	if(k<0){
			close(sockfd);
			err_quit( "listen() ");
			
	}
	return k;
}
ssize_t SendWrapper(int sockfd, const void *buf, size_t len, int flags){
	ssize_t k = send(sockfd,buf,len,flags);
	if( k < 0 ){
		err_quit( "send() ");
		close(sockfd);
		
	}
	return k;
}
ssize_t SendtoWrapper(int sockfd, const void *buf, size_t len, int flags,const struct SA *dest_addr, socklen_t addrlen){
	ssize_t k = sendto(sockfd,buf,len,flags,dest_addr,addrlen);
	if( k < 0 ){
		err_quit( "sendto() ");
		close(sockfd);
		
	}
	return k;
}
ssize_t RecvfromWrapper(int sockfd,void *buf, size_t len, int flags,struct SA *addr, socklen_t *addrlen){
		ssize_t k = recvfrom(sockfd,buf,len,flags,addr,addrlen);
		if( k < 0 ){
			close(sockfd);
			err_quit( "recvfrom() ");
			
		}
		return k;
}
ssize_t RecvWrapper(int sockfd,void *buf, size_t len, int flags){
		ssize_t k = recv(sockfd,buf,len,flags);
		if(k==0){
			close(sockfd);
		}
		if( k < 0 ){
			if(errno==EAGAIN)return -1;
			else{
				close(sockfd);
				err_quit( "recv() ");
				
			}
		}
		return k;
}
struct hostent *GethostbynameWrapper(const char *name){
	hostent *t = gethostbyname(name);
	if(t==NULL){
		err_quit( "gethostbyname()");
		
	}
	return t;
}
int SelectWrapper(int maxfd, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout){
	int k = select(maxfd,readfds,writefds,exceptfds,timeout);
	if(k<0){
		err_quit( "select()");
		
	}
	return k;
}
int SocketpairWrapper(int domain, int type, int protocol, int sv[2]){
	int k = socketpair(domain,type,protocol,sv);
	if(k<0){
		err_quit( "socketpair() ");
		
	}
	return k;
}
int  Dup2Wrapper(int oldd,int newd){
	int k = dup2(oldd,newd);
	if(k<0){
		err_quit( "dup2() ");
			
	}
	return k;
}
ssize_t Write_fd_Wrapper(int fd, void *ptr, size_t nbytes, int sendfd){
	
	ssize_t		k = write_fd(fd,ptr,nbytes,sendfd);
	if (k < 0){
		err_quit( "Write fd ()");
		
	}
	return k;
}
ssize_t Read_fd_Wrapper(int fd, void *ptr, size_t nbytes, int *recvfd){
	ssize_t		n;
	if ( (n = read_fd(fd, ptr, nbytes, recvfd)) < 0){
		err_quit( "Read fd ()");
	}
	return(n);
}
int DupWrapper(int fd){
	int k =  dup(fd);
	if(k<0){
		err_quit("dup()");
	}
	return k;
}
/***********************************************************************************************
struct iphdr{
    unsigned int ihl;     // IP Header length 5 => 20Bytes
    unsigned int version; //Ip version 4=>IPV4 6=>IPV6
    u_int8_t tos;         //Type of service 
    u_int16_t tot_len;	  // Total IP packet size 
    u_int16_t id;		  //Identification number
    u_int16_t frag_off;  // Fragment offset 
    u_int8_t ttl;		 //Time to live 
    u_int8_t protocol;	//Higher level protocol ..
    u_int16_t check;	// Checksum
    u_int32_t saddr;	//source IP  address
    u_int32_t daddr;	//destination IP  address
    [...]The options start here. 
  }
  
  packet should contains these thins **IPheader+payloads(message)** when using sendto

****************************************************************************************************/
