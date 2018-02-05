/* Shared library add-on to iptables to add MARK target support. */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <xtables.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_vlanpriorityt.h>

enum {
	F_VLANPRI = 1 << 0,
};

static void VLANPRI_help(void)
{
	printf(
"VLANPRIORITY target options:\n"
"  --set-vlanpri value                   Set nfmark value\n"
"  --and-vlanpri value                   Binary AND the nfmark with value\n"
"  --or-vlanpri  value                   Binary OR  the nfmark with value\n");
}

static const struct option VLANPRI_opts[] = {
	{ "set-vlanpri", 1, NULL, '1' },
	{ "and-vlanpri", 1, NULL, '2' },
	{ "or-vlanpri", 1, NULL, '3' },
	{ .name = NULL }
};

static const struct option vlanpri_tg_opts[] = {
	{.name = "set-xvlanpri", .has_arg = true, .val = 'X'},
	{.name = "set-vlanpri",  .has_arg = true, .val = '='},
	{.name = "and-vlanpri",  .has_arg = true, .val = '&'},
	{.name = "or-vlanpri",   .has_arg = true, .val = '|'},
	{.name = "xor-vlanpri",  .has_arg = true, .val = '^'},
	{ .name = NULL }
};

static void vlanpri_tg_help(void)
{
	printf(
"VLANPRI target options:\n"
"  --set-xvlanpri value[/mask]  Clear bits in mask and XOR value into nfmark\n"
"  --set-vlanpri value[/mask]   Clear bits in mask and OR value into nfmark\n"
"  --and-vlanpri bits           Binary AND the nfmark with bits\n"
"  --or-vlanpri bits            Binary OR the nfmark with bits\n"
"  --xor-mask bits           Binary XOR the nfmark with bits\n"
"\n");
}

/* Function which parses command options; returns true if it
   ate an option */
static int
VLANPRI_parse_v0(int c, char **argv, int invert, unsigned int *flags,
              const void *entry, struct xt_entry_target **target)
{
	struct xt_vlanpriority_target_info *vlanpriinfo
		= (struct xt_vlanpriority_target_info *)(*target)->data;
	unsigned int vlanpri = 0;

	switch (c) {
	case '1':
		if (!xtables_strtoui(optarg, NULL, &vlanpri, 0, UINT32_MAX))
			xtables_error(PARAMETER_PROBLEM, "Bad vlanpriority value \"%s\"", optarg);
		vlanpriinfo->vlanpri = vlanpri;
		if (*flags)
			xtables_error(PARAMETER_PROBLEM,
			           "VLANPRI target: Can't specify --set-vlanpri twice");
		*flags = 1;
		break;
	case '2':
		xtables_error(PARAMETER_PROBLEM,
			   "VLANPRI target: kernel too old for --and-vlanpri");
	case '3':
		xtables_error(PARAMETER_PROBLEM,
			   "VLANPRI target: kernel too old for --or-vlanpri");
	default:
		return 0;
	}

	return 1;
}

static void VLANPRI_check(unsigned int flags)
{
	if (!flags)
		xtables_error(PARAMETER_PROBLEM,
		           "VLANPRI target: Parameter --set/and/or-vlanpri"
			   " is required");
}

static int
VLANPRI_parse_v1(int c, char **argv, int invert, unsigned int *flags,
              const void *entry, struct xt_entry_target **target)
{
	struct xt_vlanpriority_target_info_v1 *vlanpriinfo
		= (struct xt_vlanpriority_target_info_v1 *)(*target)->data;
	unsigned int vlanpri = 0;

	switch (c) {
	case '1':
	        vlanpriinfo->mode = XT_VLANPRI_SET;
		break;
	case '2':
	        vlanpriinfo->mode = XT_VLANPRI_AND;
		break;
	case '3':
	        vlanpriinfo->mode = XT_VLANPRI_OR;
		break;
	default:
		return 0;
	}

	if (!xtables_strtoui(optarg, NULL, &vlanpri, 0, UINT32_MAX))
		xtables_error(PARAMETER_PROBLEM, "Bad VLANPRI value \"%s\"", optarg);
	vlanpriinfo->vlanpri = vlanpri;
	if (*flags)
		xtables_error(PARAMETER_PROBLEM,
			   "VLANPRI target: Can't specify --set-vlanpri twice");

	*flags = 1;
	return 1;
}

static int vlanpri_tg_parse(int c, char **argv, int invert, unsigned int *flags,
                         const void *entry, struct xt_entry_target **target)
{
	struct xt_vlanpriority_tginfo2 *info = (void *)(*target)->data;
	unsigned int value, mask = UINT32_MAX;
	char *end;

	switch (c) {
	case 'X': /* --set-xvlanpri */
	case '=': /* --set-vlanpri */
		xtables_param_act(XTF_ONE_ACTION, "VLANPRI", *flags & F_VLANPRI);
		xtables_param_act(XTF_NO_INVERT, "VLANPRI", "--set-xvlanpri/--set-vlanpri", invert);
		if (!xtables_strtoui(optarg, &end, &value, 0, UINT32_MAX))
			xtables_param_act(XTF_BAD_VALUE, "VLANPRI", "--set-xvlanpri/--set-vlanpri", optarg);
		if (*end == '/')
			if (!xtables_strtoui(end + 1, &end, &mask, 0, UINT32_MAX))
				xtables_param_act(XTF_BAD_VALUE, "VLANPRI", "--set-xvlanpri/--set-vlanpri", optarg);
		if (*end != '\0')
			xtables_param_act(XTF_BAD_VALUE, "VLANPRI", "--set-xvlanpri/--set-vlanpri", optarg);
		info->vlanpri = value;
		info->mask = mask;

		if (c == '=')
			info->mask = value | mask;
		break;

	case '&': /* --and-vlanpri */
		xtables_param_act(XTF_ONE_ACTION, "VLANPRI", *flags & F_VLANPRI);
		xtables_param_act(XTF_NO_INVERT, "VLANPRI", "--and-vlanpri", invert);
		if (!xtables_strtoui(optarg, NULL, &mask, 0, UINT32_MAX))
			xtables_param_act(XTF_BAD_VALUE, "VLANPRI", "--and-vlanpri", optarg);
		info->vlanpri = 0;
		info->mask = ~mask;
		break;

	case '|': /* --or-vlanpri */
		xtables_param_act(XTF_ONE_ACTION, "VLANPRI", *flags & F_VLANPRI);
		xtables_param_act(XTF_NO_INVERT, "VLANPRI", "--or-vlanpri", invert);
		if (!xtables_strtoui(optarg, NULL, &value, 0, UINT32_MAX))
			xtables_param_act(XTF_BAD_VALUE, "VLANPRI", "--or-vlanpri", optarg);
		info->vlanpri = value;
		info->mask = value;
		break;

	case '^': /* --xor-vlanpri */
		xtables_param_act(XTF_ONE_ACTION, "VLANPRI", *flags & F_VLANPRI);
		xtables_param_act(XTF_NO_INVERT, "VLANPRI", "--xor-vlanpri", invert);
		if (!xtables_strtoui(optarg, NULL, &value, 0, UINT32_MAX))
			xtables_param_act(XTF_BAD_VALUE, "VLANPRI", "--xor-vlanpri", optarg);
		info->vlanpri = value;
		info->mask = 0;
		break;

	default:
		return false;
	}

	*flags |= F_VLANPRI;
	return true;
}

static void vlanpri_tg_check(unsigned int flags)
{
	if (flags == 0)
		xtables_error(PARAMETER_PROBLEM, "VLANPRI: One of the --set-xvlanpri, "
		           "--{and,or,xor,set}-vlanpri options is required");
}

static void
print_vlanpri(unsigned int vlanpri)
{
	printf("%d ", vlanpri);
}

static void VLANPRI_print_v0(const void *ip,
                          const struct xt_entry_target *target, int numeric)
{
	const struct xt_vlanpriority_target_info *vlanpriinfo =
		(const struct xt_vlanpriority_target_info *)target->data;
	printf("VLANPRI set ");
	print_vlanpri(vlanpriinfo->vlanpri);
}

static void VLANPRI_save_v0(const void *ip, const struct xt_entry_target *target)
{
	const struct xt_vlanpriority_target_info *vlanpriinfo =
		(const struct xt_vlanpriority_target_info *)target->data;

	printf("--set-vlanpri ");
	print_vlanpri(vlanpriinfo->vlanpri);
}

static void VLANPRI_print_v1(const void *ip, const struct xt_entry_target *target,
                          int numeric)
{
	const struct xt_vlanpriority_target_info_v1 *vlanpriinfo =
		(const struct xt_vlanpriority_target_info_v1 *)target->data;

	switch (vlanpriinfo->mode) {
	case XT_VLANPRI_SET:
		printf("VLANPRI set ");
		break;
	case XT_VLANPRI_AND:
		printf("VLANPRI and ");
		break;
	case XT_VLANPRI_OR: 
		printf("VLANPRI or ");
		break;
	}
	print_vlanpri(vlanpriinfo->vlanpri);
}

static void vlanpri_tg_print(const void *ip, const struct xt_entry_target *target,
                          int numeric)
{
	const struct xt_vlanpriority_tginfo2 *info = (const void *)target->data;

	if (info->vlanpri == 0)
		printf("VLANPRI and 0x%x ", (unsigned int)(u_int32_t)~info->mask);
	else if (info->vlanpri == info->mask)
		printf("VLANPRI or 0x%x ", info->vlanpri);
	else if (info->mask == 0)
		printf("VLANPRI xor 0x%x ", info->vlanpri);
	else
		printf("VLANPRI xset 0x%x/0x%x ", info->vlanpri, info->mask);
}

static void VLANPRI_save_v1(const void *ip, const struct xt_entry_target *target)
{
	const struct xt_vlanpriority_target_info_v1 *vlanpriinfo =
		(const struct xt_vlanpriority_target_info_v1 *)target->data;

	switch (vlanpriinfo->mode) {
	case XT_VLANPRI_SET:
		printf("--set-vlanpri ");
		break;
	case XT_VLANPRI_AND:
		printf("--and-vlanpri ");
		break;
	case XT_VLANPRI_OR: 
		printf("--or-vlanpri ");
		break;
	}
	print_vlanpri(vlanpriinfo->vlanpri);
}

static void vlanpri_tg_save(const void *ip, const struct xt_entry_target *target)
{
	const struct xt_vlanpriority_tginfo2 *info = (const void *)target->data;

	printf("--set-xvlanpri 0x%x/0x%x ", info->vlanpri, info->mask);
}

static struct xtables_target vlanpri_target_v0 = {
	.family		= NFPROTO_IPV4,
	.name		= "VLANPRIORITY",
	.version	= XTABLES_VERSION,
	.revision	= 0,
	.size		= XT_ALIGN(sizeof(struct xt_vlanpriority_target_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_vlanpriority_target_info)),
	.help		= VLANPRI_help,
	.parse		= VLANPRI_parse_v0,
	.final_check	= VLANPRI_check,
	.print		= VLANPRI_print_v0,
	.save		= VLANPRI_save_v0,
	.extra_opts	= VLANPRI_opts,
};

static struct xtables_target vlanpri_target_v1 = {
	.family		= NFPROTO_IPV4,
	.name		= "VLANPRIORITY",
	.version	= XTABLES_VERSION,
	.revision	= 1,
	.size		= XT_ALIGN(sizeof(struct xt_vlanpriority_target_info_v1)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_vlanpriority_target_info_v1)),
	.help		= VLANPRI_help,
	.parse		= VLANPRI_parse_v1,
	.final_check	= VLANPRI_check,
	.print		= VLANPRI_print_v1,
	.save		= VLANPRI_save_v1,
	.extra_opts	= VLANPRI_opts,
};

static struct xtables_target vlanpri_target6_v0 = {
	.family		= NFPROTO_IPV6,
	.name		= "VLANPRIORITY",
	.version	= XTABLES_VERSION,
	.revision	= 0,
	.size		= XT_ALIGN(sizeof(struct xt_vlanpriority_target_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_vlanpriority_target_info)),
	.help		= VLANPRI_help,
	.parse		= VLANPRI_parse_v0,
	.final_check	= VLANPRI_check,
	.print		= VLANPRI_print_v0,
	.save		= VLANPRI_save_v0,
	.extra_opts	= VLANPRI_opts,
};

static struct xtables_target vlanpri_tg_reg_v2 = {
	.version       = XTABLES_VERSION,
	.name          = "VLANPRIORITY",
	.revision      = 2,
	.family        = NFPROTO_UNSPEC,
	.size          = XT_ALIGN(sizeof(struct xt_vlanpriority_tginfo2)),
	.userspacesize = XT_ALIGN(sizeof(struct xt_vlanpriority_tginfo2)),
	.help          = vlanpri_tg_help,
	.parse         = vlanpri_tg_parse,
	.final_check   = vlanpri_tg_check,
	.print         = vlanpri_tg_print,
	.save          = vlanpri_tg_save,
	.extra_opts    = vlanpri_tg_opts,
};

void _init(void)
{
	xtables_register_target(&vlanpri_target_v0);
	xtables_register_target(&vlanpri_target_v1);
	xtables_register_target(&vlanpri_target6_v0);
	xtables_register_target(&vlanpri_tg_reg_v2);
}
