#ifndef _XT_VLANPRI_H_target
#define _XT_VLANPRI_H_target

/* Version 0 */
struct xt_vlanpriority_target_info {
	unsigned int vlanpri;
};

/* Version 1 */
enum {
	XT_VLANPRI_SET=0,
	XT_VLANPRI_AND,
	XT_VLANPRI_OR,
};

struct xt_vlanpriority_target_info_v1 {
	u_int8_t vlanpri;
	u_int8_t mode;
};

struct xt_vlanpriority_tginfo2 {
	u_int32_t vlanpri, mask;
};

#endif /*_XT_VLANPRI_H_target */
