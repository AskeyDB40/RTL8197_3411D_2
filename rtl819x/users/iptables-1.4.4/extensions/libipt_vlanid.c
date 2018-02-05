/* Shared library add-on to iptables to add VALN ID support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#if defined(__GLIBC__) && __GLIBC__ == 2
#include <net/ethernet.h>
#else
#include <linux/if_ether.h>
#endif
#include <xtables.h>
#include <linux/netfilter/xt_vlanid.h>

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"VLAN id options:\n"
" --vid-value [!] XX\n"
"Match VLAN id\n"
"\n");
}

static struct option opts[] = {
	{ "vid-value", 1, 0, '1' },
	{0}
};

static void
parse_vlanid(const char *vlanid, struct xt_vlanid_info *info)
{
	long number;
	char *end;

	if (vlanid == NULL)
		xtables_error(PARAMETER_PROBLEM, "vlanid pointer is null");
	
	number = strtol(vlanid, &end, 10);
//	printf("%s(%d): number=%d\n",__FUNCTION__,__LINE__, number);//Added for test

	if ((number<0) ||(number>4095))
		xtables_error(PARAMETER_PROBLEM, "Bad vlan vid `%s'", vlanid);

	info->vid = (unsigned char)number;
}

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const void *entry,
      struct xt_entry_match **match)
{
	struct xt_vlanid_info *vlanIdInfo = (struct xt_vlanid_info *)(*match)->data;

	switch (c) {
	case '1':
		xtables_check_inverse(optarg, &invert, &optind, 0);
		parse_vlanid(argv[optind-1], vlanIdInfo);
		if (invert)
			vlanIdInfo->invert = 1;
		*flags = 1;
		break;

	default:
		return 0;
	}

	return 1;
}

static void print_vlanid(unsigned char vlanid)
{
	printf("%u ", vlanid);
}

/* Final check; must have specified --prio. */
static void final_check(unsigned int flags)
{
	if (!flags)
		xtables_error(PARAMETER_PROBLEM,
			   "You must specify `--vid-value'");
}

/* Prints out the vlanPrioInfo. */
static void
print(const void *ip,
      const struct xt_entry_match *match,
      int numeric)
{
	printf("VLAN id ");

	if (((struct xt_vlanid_info *)match->data)->invert)
		printf("! ");
	
	print_vlanid(((struct xt_vlanid_info *)match->data)->vid);
}

/* Saves the union ipt_matchinfo in parsable form to stdout. */
static void save(const void *ip, const struct xt_entry_match *match)
{
	if (((struct xt_vlanid_info *)match->data)->invert)
		printf("! ");

	printf("--vid-value ");
	print_vlanid(((struct xt_vlanid_info *)match->data)->vid);
}

static struct xtables_match vlanid = { 
 	.name		= "vlanid",
	.version	= XTABLES_VERSION,
	.family		= NFPROTO_IPV4,
	.size		= XT_ALIGN(sizeof(struct xt_vlanid_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_vlanid_info)),
	.help		= &help,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts
};

void _init(void)
{
	xtables_register_match(&vlanid);
}

