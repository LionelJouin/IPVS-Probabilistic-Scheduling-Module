/*
 * IPVS:        Probabilistic Scheduling Module
 *
 * Authors:     Lionel Jouin <lionel.jouin@outlook.com>
 */

#define KMSG_COMPONENT "IPVS"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/random.h>

#include <net/ip_vs.h>

/*
 * Probabilistic Scheduling Scheduling
 */
static struct ip_vs_dest *
ip_vs_prob_schedule(struct ip_vs_service *svc, const struct sk_buff *skb,
					struct ip_vs_iphdr *iph)
{
	struct ip_vs_dest *current_dest, *dest = NULL;
	int weights_sum = 0;
	int random_value = 0;
	int current_sum = 0;

	IP_VS_DBG(6, "%s(): Scheduling...\n", __func__);

	list_for_each_entry_rcu(current_dest, &svc->destinations, n_list)
	{
		weights_sum += atomic_read(&current_dest->weight);
	}

	random_value = get_random_int() % weights_sum + 1;

	list_for_each_entry_rcu(current_dest, &svc->destinations, n_list)
	{
		current_sum += atomic_read(&current_dest->weight);
		if (random_value <= current_sum)
		{
			dest = current_dest;
			break;
		}
	}

	if (!dest)
		ip_vs_scheduler_err(svc, "no destination available");
	else
		IP_VS_DBG_BUF(6, "PROB: server %s:%u activeconns %d "
						 "inactconns %d\n",
					  IP_VS_DBG_ADDR(dest->af, &dest->addr),
					  ntohs(dest->port),
					  atomic_read(&dest->activeconns),
					  atomic_read(&dest->inactconns));

	return dest;
}

static struct ip_vs_scheduler ip_vs_prob_scheduler = {
	.name = "prob", /* name */
	.refcnt = ATOMIC_INIT(0),
	.module = THIS_MODULE,
	.n_list = LIST_HEAD_INIT(ip_vs_prob_scheduler.n_list),
	.schedule = ip_vs_prob_schedule,
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