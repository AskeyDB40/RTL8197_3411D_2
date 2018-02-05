#ifndef __RTL_CRYPTO_API_H
#define __RTL_CRYPTO_API_H


#ifdef CONFIG_RTL_ICTEST
#include "rtl_types.h"
#else
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl_glue.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
#include "../../../net/rtl819x/AsicDriver/rtl865xc_asicregs.h"
#else
#include <asm/rtl865x/rtl865xc_asicregs.h>
#endif
#endif

#include <net/netlink.h>
#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <linux/version.h>

#include "rtl_ipsec.h"
#include "./crypto_engine/sw_sim/aes.h"


#define RTL_CRYPTO_SUCCESS (0)
#define RTL_CRYPTO_FAILED  (-1)

#define RTL_CRYPTO_MAX_PAYLOAD 1600

#define RTL_MAX_CRYPTO_SIZE				(16*1024-16)	

#define RTL_CRYPTO_MAX_KEY_LEN	64
#define RTL_CRYPTO_MAX_DIGEST_LEN	64
#define RTL_CRYPTO_MAX_IV_LEN	64

#define RTL_CRYPTO_IV_LEN	16
#define RTL_CRYPTO_BLOCK_SIZE	16

#define MAX_KEY_LEN	256
#define MAX_DIGEST_LEN		64

#define DATA_LIMIT_LEN		192

#define CACHE_LINE			32

enum
{
	CRYPTO_TYPE_FLAG = 0,
	HASH_TYPE_FLAG = 1,
    DEC_KEY_FLAG = 2,
    ENC_KEY_FLAG = 3,
    HMAC_DEC_KEY_FLAG = 4,
    HMAC_ENC_KEY_FLAG= 5,
    IV_FLAG = 6,
    ONLY_ENC_DATA_FLAG = 7,
    ONLY_DEC_DATA_FLAG = 8,
    ONLY_HASH_DATA_FLAG = 9,
    ENC_THEN_HASH_DATA_FLAG = 10,
    HASH_THEN_DEC_DATA_FLAG = 11
};

typedef struct _rtl_crypto_api_info
{
	u8 *data;  //used for input data  
	u8 *result; //used for output result
	u8 *digest; // for digest result
	u8 *enc_key;
	u8 *dec_key;
	u8 crypt_key_len;
	u8 *enc_hmac_key;
	u8 *dec_hmac_key;
	u8 hmac_key_len;
	u8 *iv;	 // iv
	u8 iv_len;
	u8 digest_len; //digest length
	int data_len;
}rtl_crypto_api_info;


#endif

