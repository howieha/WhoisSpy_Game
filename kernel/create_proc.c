/* create_proc.c - create readable/writable proc file
 * Author: Shijie Qu & Yue Mei
 */


#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h> 
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>  
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/inet.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/if_arp.h>
#include <linux/icmp.h>
#include <linux/proc_fs.h>
#include <linux/netlink.h>
#include <linux/mroute.h>
#include <net/checksum.h>
#include <net/inet_ecn.h>
#include <net/xfrm.h>
#include <net/route.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/arp.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <net/inetpeer.h>
#include <net/protocol.h>
#include <net/flow.h>
#include <asm/types.h>

struct proc_dir_entry *spyLogFile;

#define PROCFS_MAX_SIZE 50
char proc_buf[PROCFS_MAX_SIZE] = "hello world\n";

static ssize_t spyLogFile_write(struct file *fp, const char *buf, size_t len, loff_t * off)
{	if(len > PROCFS_MAX_SIZE) { return -EFAULT; }
   	if(copy_from_user(proc_buf, buf, len)) { return -EFAULT; }

	return len;
}


static ssize_t spyLogFile_read(struct file *fp, char *buf, size_t len, loff_t * off)
{	static int finished=0; if(finished) {finished=0;return 0;} finished=1;
	
	sprintf(buf, "%s", proc_buf);
	return strlen(buf);
}

static struct file_operations spyLogFile_fops = { .owner=THIS_MODULE, .read=spyLogFile_read, .write=spyLogFile_write, };

static int proc_init(void)
{
 	spyLogFile = proc_create( "spyLogFile", 0666, NULL, &spyLogFile_fops); 
 	if(spyLogFile==NULL) {	printk(KERN_ALERT "Error: Could not initialize %s\n", "spyLogFile"); }
	
	return 0;
}

static void proc_exit(void) 
{  remove_proc_entry("spyLogFile", NULL);
}

module_init(proc_init);
module_exit(proc_exit);
