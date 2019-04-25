//Taken from https://stackoverflow.com/questions/15215865/netlink-sockets-in-c-using-the-3-x-linux-kernel?lq=1

#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
  
#define NETLINK_MY_UNIT	31		// uapi/linux/netlink.h

static struct sock *nl_sk = NULL;

static void hello_nl_recv_msg(struct sk_buff *skb) {
	struct nlmsghdr *nlh;
	int senderpid, res;
	struct sk_buff *skb_out;
	char msg[] = "Hello from kernel";
	int msg_size = strlen(msg);

	nlh = (struct nlmsghdr*)skb->data;
	senderpid = nlh->nlmsg_pid;
	pr_info("Entering %s(): %d -> \"%s\"\n",
		__FUNCTION__, senderpid, (char*)nlmsg_data(nlh));

	if (!(skb_out = nlmsg_new(msg_size, 0))) {
		pr_err("    failed to allocate new skb\n");
		return;
	} 
	if (!(nlh = nlmsg_put(	// Queue it up
		skb_out,
		0,		// port ID
		0,		// sequence
		NLMSG_DONE,	// type
		msg_size,	// len
		0  		// flags
	))) {
		pr_err("    nlmsg_put() failed\n");
		return;
	}
	NETLINK_CB(skb_out).dst_group = 0;	// not in mcast group
	strncpy(nlmsg_data(nlh), msg, msg_size);

	if ((res = nlmsg_unicast(nl_sk, skb_out, senderpid)) < 0)
    		pr_err("    nlmsg_unicast() failed: %d\n", res);
}

static int __init hello_init(void) {
	struct netlink_kernel_cfg cfg = {
		.input = hello_nl_recv_msg,
	};

	pr_info("Entering %s()\n",__FUNCTION__);

	// init_net is the original boottime namespace from 
	// net/core/net_namespace.c

	if (!(nl_sk = netlink_kernel_create(&init_net, NETLINK_MY_UNIT, &cfg)))
	{
		// bad parameters, OOM, OO resources/capacity
		pr_err("    netlink_kernel_create() failed\n");
		return -EINVAL;
	}
	return 0;
}

static void __exit hello_exit(void) {
	pr_info("Exiting netlink hello module\n");
	netlink_kernel_release(nl_sk);
}

module_init(hello_init);

module_exit(hello_exit);

MODULE_LICENSE("GPL");
