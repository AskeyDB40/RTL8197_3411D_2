/*
 * sys_service.h
 *
 *  This file is the header for all standard types used in the API code.
 *
 * Copyright (c) 2005, Legerity Inc.
 * All rights reserved
 *
 * This software is the property of Legerity , Inc. Please refer to the
 * Non Disclosure Agreement (NDA) that you have signed for more information
 * on legal obligations in using, modifying or distributing this file.
 */
#ifndef SYS_SERVICE_H
#define SYS_SERVICE_H
#include "vp_api_types.h"
#include <linux/smp.h>
#include <linux/spinlock.h>
/* Critical section types */
typedef enum {
	VP_MPI_CRITICAL_SEC, 	/* MPI access critical code section */
	VP_HBI_CRITICAL_SEC, 	/* HBI access critical code section */
	VP_CODE_CRITICAL_SEC, 	/* Critical code section */
	VP_NUM_CRITICAL_SEC_TYPES, /* The number of critical section types */
    VP_CRITICAL_SEC_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpCriticalSecType;

#define CRITICAL_DEPTH_MAX	10

#ifdef CONFIG_SMP
struct vpsys_spinlock {
	unsigned int depth;
	int locked[CRITICAL_DEPTH_MAX];
	int owner;
	char func[64];
	int flags;
	spinlock_t lock;
};

extern struct vpsys_spinlock VpSysSpinLock[VP_NUM_CRITICAL_SEC_TYPES];

#if 1
#define __VpSysEnterCritical(deviceId, criticalSecType) \
	do { \
		if(VpSysSpinLock[0].owner !=smp_processor_id()) \
			spin_lock_irqsave(&VpSysSpinLock[0].lock, VpSysSpinLock[0].flags); \
		else { \
			printk("[%s %d] multi-lock detection, caller=%p\n",__FUNCTION__,__LINE__,__builtin_return_address(0)); \
			printk("previous lock func is %s\n",VpSysSpinLock[0].func); \
		} \
		strcpy(VpSysSpinLock[0].func, __FUNCTION__); \
		VpSysSpinLock[0].owner=smp_processor_id(); \
	} while (0)

#define VpSysEnterCritical(deviceId, criticalSecType) \
	do { \
		if(VpSysSpinLock[0].owner != smp_processor_id()) { \
			__VpSysEnterCritical(deviceId, 0); \
			VpSysSpinLock[0].locked[VpSysSpinLock[0].depth] = 1; \
			if (VpSysSpinLock[0].depth) \
				printk("[%s %d] depth %d is wrong at lock\n", __FUNCTION__, __LINE__, VpSysSpinLock[0].depth); \
		} else \
			VpSysSpinLock[0].locked[VpSysSpinLock[0].depth] = 0; \
		VpSysSpinLock[0].depth++; \
		if (VpSysSpinLock[0].depth >= CRITICAL_DEPTH_MAX) \
			printk("[%s %d] depth is %d, over %d\n", __FUNCTION__, __LINE__, VpSysSpinLock[0].depth, CRITICAL_DEPTH_MAX); \
	} while(0)

#define VpSysExitCritical(deviceId, criticalSecType) \
	do { \
		VpSysSpinLock[0].depth--; \
		if (VpSysSpinLock[0].locked[VpSysSpinLock[0].depth]) { \
			VpSysSpinLock[0].func[0] = 0; \
			VpSysSpinLock[0].owner=-1; \
			if (VpSysSpinLock[0].depth) \
				printk("[%s %d] depth %d is wrong at unlock\n", __FUNCTION__, __LINE__, VpSysSpinLock[0].depth); \
			spin_unlock_irqrestore(&VpSysSpinLock[0].lock, VpSysSpinLock[0].flags); \
		} \
	} while (0)
#else
#define __VpSysEnterCritical(deviceId, criticalSecType) \
	do { \
		if(VpSysSpinLock[criticalSecType].owner !=smp_processor_id()) \
			spin_lock_irqsave(&VpSysSpinLock[criticalSecType].lock, VpSysSpinLock[criticalSecType].flags); \
		else { \
			printk("[%s %d] multi-lock detection, caller=%p\n",__FUNCTION__,__LINE__,__builtin_return_address(0)); \
			printk("previous lock func is %s\n",VpSysSpinLock[criticalSecType].func); \
		} \
		strcpy(VpSysSpinLock[criticalSecType].func, __FUNCTION__); \
		VpSysSpinLock[criticalSecType].owner=smp_processor_id(); \
	} while (0)

#define VpSysEnterCritical(deviceId, criticalSecType) \
	do { \
		if(VpSysSpinLock[criticalSecType].owner != smp_processor_id()) { \
			__VpSysEnterCritical(deviceId, criticalSecType); \
			VpSysSpinLock[criticalSecType].locked[VpSysSpinLock[criticalSecType].depth] = 1; \
			if (VpSysSpinLock[criticalSecType].depth) \
				printk("[%s %d] depth %d is wrong at lock\n", __FUNCTION__, __LINE__, VpSysSpinLock[criticalSecType].depth); \
		} else \
			VpSysSpinLock[criticalSecType].locked[VpSysSpinLock[criticalSecType].depth] = 0; \
		VpSysSpinLock[criticalSecType].depth++; \
		if (VpSysSpinLock[criticalSecType].depth >= CRITICAL_DEPTH_MAX) \
			printk("[%s %d] depth is %d, over %d\n", __FUNCTION__, __LINE__, VpSysSpinLock[criticalSecType].depth, CRITICAL_DEPTH_MAX); \
	} while(0)

#define VpSysExitCritical(deviceId, criticalSecType) \
	do { \
		VpSysSpinLock[criticalSecType].depth--; \
		if (VpSysSpinLock[criticalSecType].locked[VpSysSpinLock[criticalSecType].depth]) { \
			VpSysSpinLock[criticalSecType].func[0] = 0; \
			VpSysSpinLock[criticalSecType].owner=-1; \
			if (VpSysSpinLock[criticalSecType].depth) \
				printk("[%s %d] depth %d is wrong at unlock\n", __FUNCTION__, __LINE__, VpSysSpinLock[criticalSecType].depth); \
			spin_unlock_irqrestore(&VpSysSpinLock[criticalSecType].lock, VpSysSpinLock[criticalSecType].flags); \
		} \
	} while (0)
#endif
#else
EXTERN uint8
VpSysEnterCritical(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType);
EXTERN uint8
VpSysExitCritical(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType);
#endif

EXTERN void
VpSysWait(
    uint8 time);
EXTERN void
VpSysDisableInt(
    VpDeviceIdType deviceId);
EXTERN void
VpSysEnableInt(
    VpDeviceIdType deviceId);
EXTERN bool
VpSysTestInt(
    VpDeviceIdType deviceId);
EXTERN void
VpSysDtmfDetEnable(
    VpDeviceIdType deviceId,
    uint8 channelId);
EXTERN void
VpSysDtmfDetDisable(
    VpDeviceIdType deviceId,
    uint8 channelId);

EXTERN void *
VpSysTestHeapAcquire(
    uint8 *pHeapId);
EXTERN bool
VpSysTestHeapRelease(
    uint8 heapId);
EXTERN void
VpSysPcmCollectAndProcess(
    void *pLineCtx,
    VpDeviceIdType deviceId,
    uint8 channelId,
    uint8 startTimeslot,
    uint16 operationTime,
    uint16 settlingTime,
    uint16 operationMask);

#endif /* SYS_SERVICE_H */
