#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/netlink.h>
#include <sys/socket.h>

#define NETLINK_MY_DEMO 31	// See kernel module

struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

int main(int argc, char *argv[])
{
	int MAX_PAYLOAD = sysconf(_SC_PAGESIZE);

	if (!(nlh = calloc(1, NLMSG_SPACE(MAX_PAYLOAD)))) {
		fprintf(stderr, "calloc() failed");
		exit(1);
	}
	if ((sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_MY_DEMO)) < 0) {
		perror("socket() failed");
		exit(1);
	}

	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();
	if (bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr)) < 0) {
		perror("bind() failed");
		exit(1);
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;		// For Linux Kernel
	dest_addr.nl_groups = 0;	// unicast

	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_flags = 0;

	strcpy(NLMSG_DATA(nlh), "Hello world!");

	iov.iov_base = nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = &dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	printf("Sending message to kernel\n");
	if (sendmsg(sock_fd, &msg, 0) < 0) {
		perror("sendmsg() failed");
		exit(1);
	}

	printf("Waiting for message from kernel\n");
	recvmsg(sock_fd, &msg, 0);
	if (recvmsg(sock_fd, &msg, 0) < 0) {
		perror("recvmsg() failed");
		exit(1);
	}
	printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
	close(sock_fd);
}
