#ifndef _XT_VLANPRIORITY_H_target
#define _XT_VLANPRIORITY_H_target

#include <linux/types.h>

/* Version 0 */
struct xt_vlanpriority_target_info {
	__u8 vlanpri;
};

/* Version 1 */
enum {
	XT_VLANPRI_SET=0,
	XT_VLANPRI_AND,
	XT_VLANPRI_OR,
};

struct xt_vlanpriority_target_info_v1 {
	__u8 vlanpri;
	__u8 mode;
};

struct xt_vlanpriority_tginfo2 {
	__u32 vlanpri, mask;
};

#endif /*_XT_VLANPRIORITY_H_target */
