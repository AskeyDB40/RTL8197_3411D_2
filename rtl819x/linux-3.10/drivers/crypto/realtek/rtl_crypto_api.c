#include "rtl_crypto_api.h"

static rtl_crypto_api_info info;

static struct sock *nl_sk = NULL;
static u32 rtl_crypt_pid;
int rtl_crypt_type = -1;
int rtl_hash_type = -1;

//for statistic
unsigned int crypto_api_recv_cnt = 0;
unsigned int crypto_api_send_cnt = 0;
unsigned int crypto_api_only_enc_cnt = 0;
unsigned int crypto_api_only_dec_cnt = 0;


//for show debug info
int crypto_api_dbg = 0;

#define ALIGNED(x,alignmask)	(((unsigned int)x + alignmask-1) & ~(alignmask-1))


#define UNCACHED_ADDRESS(x) CKSEG1ADDR(x)
#define CACHED_ADDRESS(x) CKSEG0ADDR(x)

void rtl_dump_info(unsigned char *buff, unsigned int len, unsigned char *str)
{
	unsigned char *p = NULL;
	int i = 0;

	if (buff){
		p = (unsigned char *)buff;
		panic_printk("\n%s: \n", str);
		for (i = 0; i < len; i++){
			if ((i != 0)&&((i % 8) == 0))
				panic_printk("\n");

			panic_printk("0x%02x ", p[i]);

		}
		panic_printk("\n");
	}
}


/* return value is 1, means supported crypto type
 * return value is 0,  means un-supported crypto type
 */
int rtl_crypto_type_check(int crypto_type)
{
	int result = 1;	

	switch (crypto_type)
	{
		case DECRYPT_CBC_DES:	
			break;
			
		case DECRYPT_CBC_3DES:	
			break;
			
		case DECRYPT_ECB_DES:	
			break;
			
		case DECRYPT_ECB_3DES:	
			break;
			
		case ENCRYPT_CBC_DES:	
			break;
			
		case ENCRYPT_CBC_3DES:	
			break;
			
		case ENCRYPT_ECB_DES:	
			break;
			
		case ENCRYPT_ECB_3DES:	
			break;
			
		case DECRYPT_CBC_AES:	
			break;
			
		case DECRYPT_ECB_AES:	
			break;
			
		case DECRYPT_CTR_AES:	
			break;
			
		case ENCRYPT_CBC_AES:	
			break;
			
		case ENCRYPT_ECB_AES:	
			break;
			
		case ENCRYPT_CTR_AES:	
			break;
			
		default:
			result = 0;
			printk("\n%s %d un-supported crypto type! \n\n",__FUNCTION__, __LINE__);
			break;						
	}

	return result;
	
}

/* return value is 1, means supported hash type
 * return value is 0,  means un-supported hash type
 */
int rtl_hash_type_check(int hash_type)
{
	int result = 1;	

	switch (hash_type)
	{
		case HASH_MD5:	
			break;
			
		case HASH_SHA1:	
			break;
			
		case HMAC_MD5:	
			break;
			
		case HMAC_SHA1:	
			break;
						
		default:
			result = 0;
			printk("\n%s %d un-supported hash type! \n\n",__FUNCTION__, __LINE__);
			break;						
	}

	return result;
	
}

int	rtl_aes_set_dec_key(u8 *org_key, u8 *dec_key, unsigned int key_len)
{
	AES_KEY aes_key;
	unsigned char temp_key[RTL_CRYPTO_MAX_KEY_LEN] = {0};
	
	memset((void *)&aes_key, 0, sizeof(aes_key));
	
	memcpy(temp_key, org_key, key_len);
	
	if (crypto_api_dbg){
		printk("\n%s %d  temp key key_len=0x%x \n\n", __func__, __LINE__, key_len);
		rtl_dump_info(temp_key, key_len, "temp key");
		printk("\n\n");
	}
	
	AES_set_encrypt_key(temp_key, key_len*8, &aes_key);

	switch (key_len)
	{
		case 128/8:
			memcpy(dec_key, &aes_key.rd_key[4*10], 16);
			break;
		case 192/8:
			memcpy(dec_key, &aes_key.rd_key[4*12], 16);
			memcpy((dec_key+16), &aes_key.rd_key[4*11+2], 8);
			break;
		case 256/8:
			memcpy(dec_key, &aes_key.rd_key[4*14], 16);
			memcpy(dec_key+16, &aes_key.rd_key[4*13], 16);
			break;
		default:
			printk("\n %s: unknown aes key_len=%d\n", __FUNCTION__, key_len);
			return RTL_CRYPTO_FAILED;
	}

	return RTL_CRYPTO_SUCCESS;
}

unsigned int rtl_crypto_api_get_skb_len(struct sk_buff *skb)
{
	return skb->len;
}


void rtl_crypto_api_netlink_send(int pid, struct sock *nl_sk, char *data, int data_len)
{	
	struct nlmsghdr *nlh;
	struct sk_buff *skb;
	int rc, len;

	crypto_api_send_cnt++;
	
	if(data_len > RTL_CRYPTO_MAX_PAYLOAD){
		printk("%s:%d##date len is too long!\n",__FUNCTION__,__LINE__);
		return;
	}	

	len = NLMSG_SPACE(RTL_CRYPTO_MAX_PAYLOAD);
	skb = alloc_skb(len, GFP_ATOMIC);
	if(!skb){
		printk(KERN_ERR "net_link: allocate failed.\n");
		return;
	}
	
	nlh = nlmsg_put(skb,0,0,0,len,0);
	if(nlh==NULL){
		printk("data_len=%d len=%d nlh is NULL!\n\n", data_len, len);
		return;
	}
	
	//NETLINK_CB(skb).pid = 0; /* from kernel */
	NETLINK_CB(skb).portid = 0; /* from kernel */
	memcpy(NLMSG_DATA(nlh), data, data_len);
	
	nlh->nlmsg_len=data_len+NLMSG_HDRLEN;

	rc = netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT);
	if (rc < 0) {
		printk(KERN_ERR "net_link: can not unicast skb (%d)\n", rc);
	}
	
	return;
}

/* from kernel start */
static  inline void crypto_inc_byte(u8 *a, unsigned int size)
{
	u8 *b = (a + size);
	u8 c;

	for (; size; size--) {
		c = *--b + 1;
		*b = c;
		if (c)
			break;
	}
}

static void crypto_inc(u8 *a, unsigned int size)
{
	__be32 *b = (__be32 *)(a + size);
	u32 c;

	for (; size >= 4; size -= 4) {
		c = be32_to_cpu(*--b) + 1;
		*b = cpu_to_be32(c);
		if (c)
			return;
	}

	crypto_inc_byte(a, size);
}

int32 rtl_hw_enc_dec_only(uint32 crypto_type, 
						unsigned char *src, void *dst,
						uint32 enc_Key_len, void* enc_key, 
						void *iv, uint32 data_len, u32 bsize)
{
	rtl_ipsecScatter_t scatter[1];
	int err = -1;
	uint32 type = crypto_type;
	if (type == DECRYPT_CTR_AES){
		type |= 4;
	}
	
	scatter[0].len = (data_len / bsize) * bsize;
	scatter[0].ptr = (void *) CKSEG1ADDR(src);
	
	dma_cache_wback((u32) src, data_len);
	dma_cache_wback((u32) enc_key, enc_Key_len);
	dma_cache_wback((u32) iv, bsize);
	dma_cache_wback((u32) dst, data_len);

	if (crypto_api_dbg){
		printk("\n%s %d  data_len=%d scatter[0].len=%d crypto_type=0x%x type=0x%x \n\n", __func__, __LINE__, data_len, scatter[0].len, crypto_type, type);
		rtl_dump_info(src, data_len, "input data");
		printk("\n\n");
		printk("\n%s %d  enc_Key_len=%u \n\n", __func__, __LINE__, enc_Key_len);
		rtl_dump_info(enc_key, enc_Key_len, "enc_key");
		printk("\n\n");
		printk("\n%s %d IV \n\n", __func__, __LINE__, enc_Key_len);
		rtl_dump_info(iv, bsize, "iv");
		printk("\n\n");
	}
	
	err = rtl_ipsecEngine(type,
		-1, 1, scatter,
		(void *) CKSEG1ADDR(dst),
		enc_Key_len, (void *) CKSEG1ADDR(enc_key),
		0, NULL,
		(void *) CKSEG1ADDR(iv), NULL, NULL,
		0, scatter[0].len);
	
	if (unlikely(err)){
		printk("%s:%d rtl_ipsecEngine failed\n", __FUNCTION__,__LINE__);
		return RTL_CRYPTO_FAILED;
	}

	dma_cache_wback_inv((u32) dst, data_len);
	dma_cache_wback_inv((u32) iv, bsize);
	
	if (crypto_api_dbg)
	{
		printk("\n%s %d  hw result catter[0].len=%d rtl_crypt_pid=%u\n\n", __func__, __LINE__, scatter[0].len, rtl_crypt_pid);
		rtl_dump_info(dst, scatter[0].len, "result");
		printk("\n\n");
	}
	
	return data_len - scatter[0].len;;
}

int rtl_hw_cipher_wrapper(uint32 crypto_type, 
						unsigned char *src, void *dst,
						uint32 keylen, void *key, 
						void *orig_iv, uint32 nbytes, u32 bsize)
{
	int nbytes0 = 0,ret = 0;
	unsigned char padding0[RTL_CRYPTO_BLOCK_SIZE+CACHE_LINE]={0},out_padding0[RTL_CRYPTO_BLOCK_SIZE+CACHE_LINE] = {0};
	unsigned char iv0[CACHE_LINE+RTL_CRYPTO_IV_LEN] = {0};
	unsigned int pad_val = 0,left = 0;
	unsigned int encryptd_len = 0, encrypt_len = 0;
	unsigned char *padding,*out_padding, *iv;
	unsigned char *psrc = src;
	unsigned char *pdst = dst;
	
	/* ctr mode over flag */
	int i, over_flag = 0;
	unsigned int one = 0, len = 0;			
	u8 over_iv[RTL_CRYPTO_IV_LEN] = {0};


	/* cbc padding */
	//if(crypto_type == DECRYPT_CBC_AES)
	{
		left = nbytes % bsize;
		pad_val = bsize - left;
		padding = (unsigned char*)ALIGNED(padding0,CACHE_LINE);
		out_padding = (unsigned char*)ALIGNED(out_padding0,CACHE_LINE);
		if(left != 0){
			memset(padding,pad_val,bsize);
			memcpy(padding,src+nbytes/bsize*bsize,left);
		}else{
			if(crypto_type == DECRYPT_CBC_AES){
				left = RTL_CRYPTO_BLOCK_SIZE;
				memset(padding,pad_val,bsize);
			}
		}
	}


	iv = (unsigned char*)ALIGNED(iv0,CACHE_LINE);
	if(crypto_type != DECRYPT_ECB_AES){
		memcpy(iv,orig_iv,RTL_CRYPTO_IV_LEN);
	}

	
	while(encryptd_len < nbytes){
		encrypt_len = (nbytes-encryptd_len)>=RTL_MAX_CRYPTO_SIZE ? RTL_MAX_CRYPTO_SIZE: (nbytes-encryptd_len);
		
		over_flag = 0;
		if((crypto_type == DECRYPT_CTR_AES)||(crypto_type == ENCRYPT_CTR_AES)){
			one = *((unsigned int *)(iv + bsize - 4));
			one = htonl(one);
			
			for (i = 0; i < (encrypt_len / bsize); i++)
			{					
				if (one == 0xffffffff)
				{
					over_flag = 1;
					break;
				}
				one++;
			}

			if (over_flag){
				//before ONE overflow 
				len = bsize*(i+1);
				if (crypto_api_dbg){
					printk("\n%s %d len=%u bsize=%u encrypt_len=%u encryptd_len=%u \n", __FUNCTION__, __LINE__,len, bsize, encrypt_len, encryptd_len);
				}
				nbytes0 = rtl_hw_enc_dec_only(crypto_type, psrc, pdst, keylen, key, iv, len, bsize);
				if(nbytes0 < 0){
					ret = -1;
					//printk("%s	%d \n", __FUNCTION__, __LINE__);
					goto OUT;
				}
				psrc += (len - nbytes0);
				pdst += (len - nbytes0);
				#if 1	
				//after ONE overflow,update IV
				memcpy(over_iv, iv, bsize - 4);
				crypto_inc(over_iv, bsize-4);
				memcpy(iv, over_iv, bsize);
				#endif
				nbytes0 = rtl_hw_enc_dec_only(crypto_type, psrc, pdst, keylen, key, iv, encrypt_len -len, bsize);
				if(nbytes0 < 0){
					ret = -1;
					//printk("%s	%d \n", __FUNCTION__, __LINE__);
					goto OUT;
				}
				/* increment counter in counterblock */
				for (i = 0; i < ((encrypt_len -len) / bsize); i++)
					crypto_inc(iv, bsize);
				psrc += encrypt_len-len-nbytes0;
				pdst += encrypt_len-len-nbytes0;
			}else{
				if (crypto_api_dbg){
					printk("\n%s	%d len=%u bsize=%u encrypt_len=%u encryptd_len=%u \n", __FUNCTION__, __LINE__,len, bsize, encrypt_len, encryptd_len);
				}
				nbytes0 = rtl_hw_enc_dec_only(crypto_type, psrc, pdst, keylen, key, iv, encrypt_len, bsize);
				if(nbytes0 < 0){
					ret = -1;
					//printk("%s	%d \n", __FUNCTION__, __LINE__);
					goto OUT;
				}
				for (i = 0; i < (nbytes / bsize); i++)
						crypto_inc(iv, bsize);
				psrc += encrypt_len - nbytes0;
				pdst += encrypt_len - nbytes0;
			}
		}else{
			nbytes0 = rtl_hw_enc_dec_only(crypto_type, psrc, pdst, keylen, key, iv, encrypt_len, bsize);
			if(nbytes0 < 0){
				ret = -1;
				//printk("%s	%d \n", __FUNCTION__, __LINE__);
				goto OUT;
			}
			psrc += encrypt_len - nbytes0;
			pdst += encrypt_len - nbytes0;
		}

		encryptd_len += encrypt_len;
	}

	/* padding */
	//if(crypto_type == DECRYPT_CBC_AES)
	{
		if(left > 0){
			if (crypto_api_dbg){
				printk("\n%s	%d len=%u bsize=%u encrypt_len=%u encryptd_len=%u lef=%u nbytes0=%d \n", __FUNCTION__, __LINE__,len, bsize, encrypt_len, encryptd_len, left, nbytes0);
			}
			nbytes0 = rtl_hw_enc_dec_only(crypto_type, padding, out_padding, keylen, key, iv, bsize, bsize);
			if(nbytes0 < 0){
				ret = -1;
				//printk("%s	%d \n", __FUNCTION__, __LINE__);
				goto OUT;
			}
			memcpy(pdst,out_padding,left);
			
			if((crypto_type == DECRYPT_CTR_AES)||(crypto_type == ENCRYPT_CTR_AES))
				crypto_inc(iv, bsize);
		}
	}
	ret = 0;
	rtl_crypto_api_netlink_send(rtl_crypt_pid, nl_sk, dst, nbytes);

OUT:

	return ret;
}


void rtl_crypto_api_netlink_receive(struct sk_buff *skb)
{
	int len = 0, offset = 0, err = -1;
	static u8 recv_buf[RTL_CRYPTO_MAX_PAYLOAD] = {0};
	struct nlmsghdr *nlh=NULL;

	crypto_api_recv_cnt++;

	if (!skb || !skb->data){
		printk("\n%s %d receive null message! \n", __func__, __LINE__);
		return ;
	}
	
	nlh = nlmsg_hdr(skb);
	if (!nlh){
		printk("\n%s %d nlh = null ! \n", __func__, __LINE__);
		return ;
	}

	len = nlh->nlmsg_len - NLMSG_HDRLEN;
	if (len <= 0){
		printk("\n%s %d message is too short! \n", __func__, __LINE__);
		return;
	}	
	if (len > RTL_CRYPTO_MAX_PAYLOAD){
		printk("\n%s %d message is too long! len=%d\n", __func__, __LINE__, len);
		return;
	}
	
	rtl_crypt_pid = nlh->nlmsg_pid; /*pid of sending process */

	memset(recv_buf, 0, RTL_CRYPTO_MAX_PAYLOAD);
	memcpy(recv_buf, NLMSG_DATA(nlh), len);

	if (crypto_api_dbg){
		printk("\n%s %d  len=%d nlh->nlmsg_len=%u rtl_crypt_pid=%u \n\n", __func__, __LINE__, len, nlh->nlmsg_len, rtl_crypt_pid);
		rtl_dump_info(recv_buf, len, "recv_buf");
		printk("\n\n");
	}
	
	while(offset<len)
	{
		switch(recv_buf[offset])
		{
			case CRYPTO_TYPE_FLAG: 
				//crypto type defined in rtl_ipsec.h
				memcpy(&rtl_crypt_type, recv_buf+offset+2, recv_buf[offset+1]);
				offset += 2 + recv_buf[offset+1];
				//supported crypto type check ?
				if (rtl_crypt_type >= 0){
					err = rtl_crypto_type_check(rtl_crypt_type);
					if (!err){
						printk("\n%s:%d rtl_crypto_type_check failed \n\n", __FUNCTION__,__LINE__);
					}
				}				
				if (crypto_api_dbg){
					printk("\n %s %d  rtl_crypto_type=%d \n\n",__func__, __LINE__, rtl_crypt_type);
				}
				
				break;
				
			case HASH_TYPE_FLAG: 
				//hash type defined in rtl_ipsec.h
				memcpy(&rtl_hash_type, recv_buf+offset+2, recv_buf[offset+1]);
				offset += 2 + recv_buf[offset+1];
				//supported hash type check??
				if (rtl_hash_type >= 0){
					err = rtl_hash_type_check(rtl_hash_type);
					if (!err){
						printk("\n%s:%d rtl_hash_type_check failed \n\n", __FUNCTION__,__LINE__);
					}
				}

				if (crypto_api_dbg){
					printk("\n %s %d  rtl_hash_type=%d \n\n",__func__, __LINE__, rtl_hash_type);
				}
				
				break;

			case ENC_KEY_FLAG:
				if (info.enc_key){
					memcpy(info.enc_key, recv_buf+offset+2, recv_buf[offset+1]);
					info.crypt_key_len = recv_buf[offset+1];
					offset += 2 + info.crypt_key_len;
					
					if (crypto_api_dbg){
						printk("\n%s %d  info.crypt_key_len=%u \n\n", __func__, __LINE__, info.crypt_key_len);
						rtl_dump_info(info.enc_key, info.crypt_key_len, "key");
						printk("\n\n");
					}
					
				}
				else{
					printk("\n%s %d info.enc_key is NULL!!\n\n", __func__, __LINE__);
					return;
				}
				
				break;

			case DEC_KEY_FLAG:
				if (info.dec_key){
					memcpy(info.dec_key, recv_buf+offset+2, recv_buf[offset+1]);
					info.crypt_key_len = recv_buf[offset+1];
					offset += 2 + info.crypt_key_len;
					
					if (crypto_api_dbg){
						printk("\n%s %d  info.crypt_key_len=%u \n\n", __func__, __LINE__, info.crypt_key_len);
						rtl_dump_info(info.dec_key, info.crypt_key_len, "dec key");
						printk("\n\n");
					}
				}				
				else{
					printk("\n%s %d info.dec_key is NULL!!\n\n", __func__, __LINE__);
					return;
				}
				
				break;

			case HMAC_DEC_KEY_FLAG:
				if (info.dec_hmac_key){
					memcpy(info.dec_hmac_key, recv_buf+offset+2, recv_buf[offset+1]);
					info.hmac_key_len = recv_buf[offset+1];
					offset += 2 + info.hmac_key_len;
					
					if (crypto_api_dbg){
						printk("\n%s %d  info.hmac_key_len=%u \n\n", __func__, __LINE__, info.hmac_key_len);
						rtl_dump_info(info.dec_hmac_key, info.hmac_key_len, "hmac dec key");
						printk("\n\n");
					}
					
				}
				else{
					printk("\n%s %d info.dec_hmac_key is NULL!!\n\n", __func__, __LINE__);
					return;
				}
				break;

			case HMAC_ENC_KEY_FLAG:
				if (info.enc_hmac_key){
					memcpy(info.enc_hmac_key, recv_buf+offset+2, recv_buf[offset+1]);
					info.hmac_key_len = recv_buf[offset+1];
					offset += 2 + info.hmac_key_len;
					
					if (crypto_api_dbg){
						printk("\n%s %d  info.hmac_key_len=%u \n\n", __func__, __LINE__, info.hmac_key_len);
						rtl_dump_info(info.enc_hmac_key, info.hmac_key_len, "hmac enc key");
						printk("\n\n");
					}
				}
				else{
					printk("\n%s %d info.enc_hmac_key is NULL!!\n\n", __func__, __LINE__);
					return;
				}
				
				break;
				
			case IV_FLAG:
				if (info.iv){
					memcpy(info.iv, recv_buf+offset+2, recv_buf[offset+1]);
					info.iv_len = recv_buf[offset+1];
					offset += 2 + info.iv_len;
					
					if (crypto_api_dbg){
						printk("\n%s %d  info.iv_len=%u \n\n", __func__, __LINE__, info.iv_len);
						rtl_dump_info(info.iv, info.iv_len, "iv");
						printk("\n\n");
					}
				}
				else{
					printk("\n%s %d info.iv is NULL!!\n\n", __func__, __LINE__);
					return;
				}
				
				break;
			case ONLY_ENC_DATA_FLAG:
				crypto_api_only_enc_cnt++;
				if (info.data && info.result){
					if (rtl_crypt_type >= 0){
						//encrypt/decrypt only, data length, 32 bits 
						memcpy((void *)&info.data_len, &recv_buf[offset+1], sizeof(int)); //??
						//data
						memcpy(info.data, recv_buf+offset+1+sizeof(int), info.data_len);
						offset += 1 + sizeof(int) + info.data_len;
						//hw enc/dec
						err = rtl_hw_cipher_wrapper(rtl_crypt_type, 
												info.data, info.result,
												info.crypt_key_len, info.enc_key, 
												info.iv, info.data_len, RTL_CRYPTO_BLOCK_SIZE);
						
						if (unlikely(err)){
							printk("%s:%d rtl_hw_enc_dec_only_and_send failed\n", __FUNCTION__,__LINE__);
						}						
					}
					else{
						printk("\n%s %d un-supported crypto type!! rtl_crypt_type=%d\n\n", __func__, __LINE__, rtl_crypt_type);
						return;
					}
				}
				else{
					printk("\n%s %d info.data or info.result is NULL!!\n\n", __func__, __LINE__);
					return;
				}
				break;
				
			case ONLY_DEC_DATA_FLAG:
				crypto_api_only_dec_cnt++;
				if (info.data && info.result){
					if (rtl_crypt_type >= 0){
						//encrypt/decrypt only, data length, 32 bits 
						memcpy((void *)&info.data_len, &recv_buf[offset+1], sizeof(int)); //??
						//data
						memcpy(info.data, recv_buf+offset+1+sizeof(int), info.data_len);
						offset += 1 + sizeof(int) + info.data_len;
						if ((rtl_crypt_type == DECRYPT_ECB_AES) || (rtl_crypt_type == DECRYPT_CBC_AES)){
							rtl_aes_set_dec_key(info.dec_key, info.dec_key, info.crypt_key_len);
						}
						//hw enc/dec
						err = rtl_hw_cipher_wrapper(rtl_crypt_type, 
												info.data, info.result,
												info.crypt_key_len, info.dec_key, 
												info.iv, info.data_len, RTL_CRYPTO_BLOCK_SIZE);
						if (unlikely(err)){
							printk("%s:%d rtl_hw_enc_dec_only_and_send failed\n", __FUNCTION__,__LINE__);
						}						
					}
					else{
						printk("\n%s %d un-supported crypto type!! rtl_crypt_type=%d\n\n", __func__, __LINE__, rtl_crypt_type);
						return;
					}
				}				
				else{
					printk("\n%s %d info.data or info.result is NULL!!\n\n", __func__, __LINE__);
					return;
				}
				break;
				
			case ONLY_HASH_DATA_FLAG:
				if (rtl_hash_type >= 0){
					//hash only 
					
				}
				break;
				
			case ENC_THEN_HASH_DATA_FLAG:
				
				break;
				
			case HASH_THEN_DEC_DATA_FLAG:
					
				break;
							
			default:
				if (net_ratelimit())
					printk("%s##Invalid Flag!\n",__FUNCTION__);
				break;						
		}
		
	}
	
}

int32 rtl_crypto_api_free( void ) 
{
	if (info.data)
	{
		kfree(info.data);
		info.data = NULL;
	}
	
	if (info.result)
	{
		kfree(info.result);
		info.result = NULL;
	}
	
	if (info.enc_key)
	{
		kfree(info.enc_key);
		info.enc_key = NULL;
	}

	if (info.dec_key)
	{
		kfree(info.dec_key);
		info.dec_key = NULL;
	}
	if (info.enc_hmac_key)
	{
		
		kfree(info.enc_hmac_key);
		info.enc_hmac_key = NULL;
	}
	if (info.dec_hmac_key)
	{
		kfree(info.dec_hmac_key);
		info.dec_hmac_key = NULL;
	}

	if (info.iv)
	{
		kfree(info.iv);
		info.iv = NULL;
	}

	if (info.digest)
	{
		kfree(info.digest);
		info.digest = NULL;
	}
	
  	if (nl_sk) 
	{
    	netlink_kernel_release(nl_sk);
		nl_sk = NULL;
  	}
	
	return RTL_CRYPTO_SUCCESS;
}

int32 rtl_crypto_api_alloc(void) 
{
	info.data = kmalloc(RTL_CRYPTO_MAX_PAYLOAD, GFP_ATOMIC);
	info.result = kmalloc(RTL_CRYPTO_MAX_PAYLOAD, GFP_ATOMIC);
	info.enc_key = kmalloc(RTL_CRYPTO_MAX_KEY_LEN, GFP_ATOMIC);
	info.dec_key = kmalloc(RTL_CRYPTO_MAX_KEY_LEN, GFP_ATOMIC);
	info.enc_hmac_key = kmalloc(RTL_CRYPTO_MAX_KEY_LEN, GFP_ATOMIC);
	info.dec_hmac_key = kmalloc(RTL_CRYPTO_MAX_KEY_LEN, GFP_ATOMIC);
	info.iv = kmalloc(RTL_CRYPTO_MAX_IV_LEN, GFP_ATOMIC);
	info.digest = kmalloc(RTL_CRYPTO_MAX_DIGEST_LEN, GFP_ATOMIC);
	
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	//for kernel-3.10
	struct netlink_kernel_cfg cfg = {
		.input	= rtl_crypto_api_netlink_receive,
	};
	
	nl_sk = netlink_kernel_create(&init_net, NETLINK_RTL_CRYPTO_API, &cfg);
	#else
	nl_sk = netlink_kernel_create(&init_net, NETLINK_RTL_CRYPTO_API, 0, rtl_crypto_api_netlink_receive, NULL, THIS_MODULE);
	#endif
	if (!nl_sk) 
	{
		printk("\nkernel create crypto api netlink socket fail!\n");
	}

	if (!info.data || !info.result || !info.enc_key || !info.dec_key || !info.enc_hmac_key 
		|| !info.dec_hmac_key || !info.iv || !info.digest || !nl_sk)
	{
		//free
		rtl_crypto_api_free();
		printk("\n%s %d malloc memory failed or create netlink socket failed \n", __FUNCTION__,__LINE__);
		return RTL_CRYPTO_FAILED;
	}
	return RTL_CRYPTO_SUCCESS;
}


int rtl_crypto_api_netlink_init(void) 
{

	rtl_crypto_api_free();
	rtl_crypto_api_alloc() ;

	crypto_api_dbg = 0;
	
	crypto_api_recv_cnt = 0;
	crypto_api_send_cnt = 0;
	
	crypto_api_only_enc_cnt = 0;
	crypto_api_only_dec_cnt = 0;

  	return 0;
}


