/*
 * IPVS:        Probabilistic Scheduling Module
 *
 * Authors:     Lionel Jouin <lionel.jouin@outlook.com>
 */

#define KMSG_COMPONENT "IPVS"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>

#include <net/ip_vs.h>


/*
 * Probabilistic Scheduling Scheduling
 */
static struct ip_vs_dest *
ip_vs_prob_schedule(struct ip_vs_service *svc, const struct sk_buff *skb,
		  struct ip_vs_iphdr *iph)
{
	return NULL;
}


static struct ip_vs_scheduler ip_vs_prob_scheduler = {
	.name =			"prob",			/* name */
	.refcnt =		ATOMIC_INIT(0),
	.module =		THIS_MODULE,
	.n_list =		LIST_HEAD_INIT(ip_vs_prob_scheduler.n_list),
	.schedule =		ip_vs_prob_schedule,
};

static int __init ip_vs_prob_init(void)
{
	return register_ip_vs_scheduler(&ip_vs_prob_scheduler);
}

static void __exit ip_vs_prob_cleanup(void)
{
	unregister_ip_vs_scheduler(&ip_vs_prob_scheduler);
	synchronize_rcu();
}

module_init(ip_vs_prob_init);
module_exit(ip_vs_prob_cleanup);
MODULE_LICENSE("GPL");