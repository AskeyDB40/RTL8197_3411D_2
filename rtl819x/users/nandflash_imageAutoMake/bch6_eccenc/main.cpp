// -csz 2048 -bso 5 -bdo 2000 example

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "bch6.h"

static void
show_message(const char *n="eccenc") {
    fprintf(stderr, 
        "This is bch6 encoder.\n"
        "usage: %s --chunk-size <n> --bbi-swap-offset <n> --bbi-dma-offset <n> <input-file1> <input-file1 offset> ....... <input-file n> <input-file n offset>\n"
        "\t--chunk-size <n>: chunk size in byte (eq -csz <n>)\n"
        "\t--bbi-swap-offset <n>: swap offset for bbi (eq -bso <n>)\n"
        "\t--bbi-dma-offset <n>: dma offset for bbi (eq -bdo <n>)\n"
        "\n"
        "\tNotice: --bbi-swap-offset and --bbi-dma-offset should be defined together.\n"
        "\tif neither --bbi-swap-offset nor --bbi-dma-offset is defined, no swap will be performanced\n"
        , n);
}
#define MINUS_ONE 0xffffffff
unsigned int chunk_size=MINUS_ONE;
unsigned int spare_size=MINUS_ONE;
unsigned int bbi_swap_offset=MINUS_ONE;
unsigned int bbi_dma_offset=MINUS_ONE;
unsigned int chunk_per_block=0;
unsigned int page_size=0;
unsigned int oob_size=0;
unsigned int ecc_size=0;
unsigned int unit_size=0;
const char *input_file_name=NULL;
bool enable_swap=true;
ecc_encoder_t *ecc_encoder=NULL;

enum C_TYPE
{
	RTL8198C_NAND = 0,
	RTL8881A_NAND,
	RTL819xD_NAND,
	RTL8197F_SPI_NAND,
	RTL8197F_PARALLEL_NAND
};
enum C_TYPE nandflash_controller_type = RTL8198C_NAND;

enum C_ENDIAN
{
	RTL_BIG_ENDIAN = 0,
	RTL_LITTLE_ENDIAN
};
enum C_ENDIAN	nandflash_endian = RTL_BIG_ENDIAN;
enum C_ENDIAN	compile_pc = RTL_BIG_ENDIAN;


/* store 97f bootcode name */
typedef struct spi_per_boot_info
{
	char name[256];
	unsigned int offset;
}SPI_PER_BOOT_INFO_T,*SPI_PER_BOOT_INFO_Tp;

typedef struct spi_boot_info
{
	int spi_boot_num;
	SPI_PER_BOOT_INFO_T per_spi_info[4];
}SPI_BOOT_INFO_T,*SPI_BOOT_INFO_Tp;

static SPI_BOOT_INFO_T boot_info;	//4 //4 bank boot

static int endian_convert(unsigned char* buf,int len)
{
	int val,i;
	unsigned char* buf2 = (unsigned char*)malloc(len);
	if(buf2 ==  NULL)
		return -1;

	for(i = 0;i < len;i+=4){
		val = *(int*)(buf+i);
		val = ((val & 0xff000000)>>24) | ((val & 0xff0000) >>8)	| ((val & 0xff00) <<8) | ((val & 0xff)<<24);
		memcpy(buf2+i,&val,4);
	}
	
	memcpy(buf,buf2,len);
	if(buf2){
		free(buf2);
		buf2 = NULL;
	}
	return 0;
}

static void rtl8197f_boot_write(char* outfile,char* inputfile,int offset)
{
	int r2,real_offset;
	unsigned int iteration=chunk_size/page_size;
	unsigned char chunk_data[chunk_size+iteration*(oob_size+ecc_size)];

	int hi=open(inputfile, O_RDONLY);
	if (hi<0) {
        fprintf(stderr, "unable to open input file (%s)\n", inputfile);
        return;
    }
	
	int ho=open(outfile, O_WRONLY|O_CREAT, 0666);
    if (ho<0) {
        fprintf(stderr, "unable to create output file (%s)\n", outfile);
        close(hi);
        return;
    }

	real_offset = (offset/chunk_size)*(chunk_size+spare_size);
	lseek(ho,real_offset,SEEK_SET);

	while(1){
		r2 = read(hi, chunk_data, chunk_size+iteration*(oob_size+ecc_size));
#if 0
		if(compile_pc != nandflash_endian){
			/* need do endian convert */
			if(endian_convert(chunk_data,chunk_size) < 0){
				printf("convert 97f spi nand bootcode fail\n");
				return;
			}
		}
#endif		
		write(ho, chunk_data, chunk_size+iteration*(oob_size+ecc_size));

#if 1
		if(spare_size > (iteration*(oob_size+ecc_size))){
			lseek(ho,(spare_size-(iteration*(oob_size+ecc_size))),SEEK_CUR);
		}
#endif
		if(r2 != (int)(chunk_size+iteration*(oob_size+ecc_size))) 
			break;
	}

	close(ho);
	close(hi);
	
	return;
}

static void rtl8197f_image_convert(char* infile,char* outfile)
{
	int r2;
	
	int hi=open(infile, O_RDONLY);
    if (hi<0) {
        fprintf(stderr, "unable to open input file (%s)\n", infile);
        return;
    }
    
    int ho=open(outfile, O_WRONLY|O_TRUNC|O_CREAT, 0666);
    if (ho<0) {
        fprintf(stderr, "unable to create output file (%s)\n", outfile);
        close(hi);
        return;
    }
	//unsigned int iteration=chunk_size/page_size;
    unsigned char chunk_data[chunk_size+spare_size];

	while(1){
		r2 = read(hi,chunk_data,chunk_size+spare_size);

		if(endian_convert(chunk_data,chunk_size+spare_size) < 0){
			printf("convert 97f spi nand bootcode fail\n");
			return;
		}

		write(ho,chunk_data,chunk_size+spare_size);

		if(r2 != (int)(chunk_size+spare_size)) 
			break;
	}

	close(hi);
	close(ho);

	return;
}

static void
append_ecc_one_chunk(int output_handle, const unsigned char *chunk_data, unsigned int len, const unsigned char *spare,unsigned char flag) {
    unsigned char local_chunk_data[chunk_size+oob_size+ecc_size];
    unsigned int iteration=chunk_size/page_size;
    unsigned char local_spare[iteration*oob_size];
    unsigned char ecc[iteration*ecc_size];
	unsigned char spare_buf[spare_size];
	
    unsigned char * tmp=local_chunk_data;
    int chunkallone=0;
    
    if (len>chunk_size) len=chunk_size;
    
    // copy source into local buffer
    memset(local_chunk_data, 0xff, sizeof(local_chunk_data));
    memset(local_spare, 0xff, sizeof(local_spare));
    memset(ecc, 0xff, sizeof(ecc));

#if 1
	chunkallone=memcmp(tmp,chunk_data,chunk_size);
//	printf("chunkallone is %d\n",chunkallone);
	if(chunkallone==0){
		//printf(" find all is 0xff\n");
        for (unsigned i=0;i<chunk_size;i+=page_size)
            write(output_handle, local_chunk_data, unit_size);

		if(spare_size > (iteration*(oob_size+ecc_size))){
			//printf("%s:%d\n",__func__,__LINE__);
			//printf("%s:%d:size=%d\n",__func__,__LINE__,spare_size-iteration*(oob_size+ecc_size));
			memset(spare_buf,0xff,spare_size);
			write(output_handle, spare_buf, spare_size-iteration*(oob_size+ecc_size));
		}

        return;
	}
#endif
	
    if (chunk_data!=NULL) memcpy(local_chunk_data, chunk_data, len);
    if (spare!=NULL) memcpy(local_spare, spare, sizeof(local_spare));
    
    
	
    // compute ECC and write out
    if ((chunk_data==NULL)&&(spare==NULL)) {
        // write out 0xff for whole chunk
        for (unsigned i=0;i<chunk_size;i+=page_size)
            write(output_handle, local_chunk_data, unit_size);
    } else {
        // bbi swap
        if (enable_swap && !flag) {
            local_spare[bbi_swap_offset]=local_chunk_data[bbi_dma_offset];
            local_chunk_data[bbi_dma_offset]=0xff;
        }
        // write out one chunk
        for (unsigned i=0,j=0,k=0; i<chunk_size; i+=page_size, j+=oob_size,k+=ecc_size) {
			ecc_encoder->encode_512B(ecc+k, local_chunk_data+i, local_spare+j);
		}
		if(nandflash_controller_type == RTL8197F_SPI_NAND
			|| nandflash_controller_type == RTL8197F_PARALLEL_NAND){
			write(output_handle, local_chunk_data, page_size*iteration);	

			if(nandflash_controller_type == RTL8197F_PARALLEL_NAND){
				for(unsigned i = 0;i < iteration;i++){
					write(output_handle, local_spare+oob_size*i, oob_size);
					write(output_handle, ecc+ecc_size*i, ecc_size);
				}
			}else{
				write(output_handle, local_spare, oob_size*iteration);
				write(output_handle,ecc,ecc_size*iteration);
			}	
		}else{
			for(unsigned i =0 ;i < iteration;i++){
				write(output_handle, local_chunk_data+page_size*i, page_size);
				write(output_handle, local_spare+oob_size*i, oob_size);
				write(output_handle, ecc+ecc_size*i, ecc_size);
			}
		}
    }

	if(spare_size > (iteration*(oob_size+ecc_size))){

		//printf("%s:%d:size=%d\n",__func__,__LINE__,spare_size-iteration*(oob_size+ecc_size));
		memset(spare_buf,0xff,spare_size);
		//write(output_handle, spare_buf, spare_size-iteration*(oob_size+ecc_size));
		write(output_handle, spare_buf,0);
	}

}

static int 
write_out() {
	#define BOOT_SIZE	0x100000
	int i;
	int total = 0;
    char ofile[1024],ofile2[1024];
    int hi=open(input_file_name, O_RDONLY);
    if (hi<0) {
        fprintf(stderr, "unable to open input file (%s)\n", input_file_name);
        return 1;
    }
    sprintf(ofile, "%s.ecc", input_file_name);
    int ho=open(ofile, O_WRONLY|O_TRUNC|O_CREAT, 0666);
    if (ho<0) {
        fprintf(stderr, "unable to create output file (%s)\n", ofile);
        close(hi);
        return 1;
    }
    
    unsigned char chunk_data[chunk_size];
    unsigned int ci=0;
    while (1) {
        int r=read(hi, chunk_data, chunk_size);
        if (r>0 && total >= BOOT_SIZE) append_ecc_one_chunk(ho, chunk_data, r, NULL,0);
		if (r>0 && total < BOOT_SIZE) append_ecc_one_chunk(ho, chunk_data, r, NULL,1);
        if (chunk_per_block>0) ci=(ci+1) % chunk_per_block;
        if (r!=(int)chunk_size) break;

		total += chunk_size;
    }
	
    if ((chunk_per_block>0)&&(ci>0)) {
        memset(chunk_data, 0xff, chunk_size);
        for (;ci<chunk_per_block; ++ci) 
            append_ecc_one_chunk(ho, chunk_data, chunk_size, NULL,0);
    }

    close(ho);
    close(hi);

	/* for 97f do here */
	sprintf(ofile2, "%s.ecc.convert", input_file_name);

	if(nandflash_controller_type == RTL8197F_SPI_NAND){
		for(i = 0;i < boot_info.spi_boot_num;i++){
			//printf("name=%s,offset=%x\n",boot_info.per_spi_info[i].name,boot_info.per_spi_info[i].offset);
			rtl8197f_boot_write(ofile,boot_info.per_spi_info[i].name,boot_info.per_spi_info[i].offset);	
		}
	}
	if(compile_pc != nandflash_endian){
		rtl8197f_image_convert(ofile,ofile2);
	}
    return 0;
}

int
main(int argc, char *argv[]) {
    bool error_out=false;

	#define MAX_IAMGE_SIZE  	10
	/* may modify */
	#define LAST_IMAGE_SIZE		0x528000
	
	char *name[MAX_IAMGE_SIZE];
	int  offset[MAX_IAMGE_SIZE];
	int i = 0,j,fd,fd2,total,max_size,r2,max_boot;
    
    if (argc==1) {
        show_message();
        return 0;
    }
    while (*(++argv)!=NULL) {
        if ((strcmp(*argv, "--chunk-size")==0)||(strcmp(*argv, "-csz")==0)) {
            chunk_size=strtoul(*(++argv), NULL, 0);
        } else if((strcmp(*argv, "--spare-size")==0)||(strcmp(*argv, "-ssz")==0)){
        	spare_size=strtoul(*(++argv), NULL, 0); 
		}else if ((strcmp(*argv, "--bbi-swap-offset")==0)||(strcmp(*argv, "-bso")==0)) {
            bbi_swap_offset=strtoul(*(++argv), NULL, 0);
        } else if ((strcmp(*argv, "--bbi-dma-offset")==0)||(strcmp(*argv, "-bdo")==0)) {
            bbi_dma_offset=strtoul(*(++argv), NULL, 0);
        } else if ((strcmp(*argv, "--chunk-per-block")==0)||(strcmp(*argv, "-cpb")==0)) {
            chunk_per_block=strtoul(*(++argv), NULL, 0);
        }else if ((strcmp(*argv, "--controller-type")==0)||(strcmp(*argv, "-ctype")==0)) {
			if(!strcmp(*(++argv),"RTL8198C_NAND")){
				nandflash_controller_type = RTL8198C_NAND;
			}else if(!strcmp(*(argv),"RTL8881A_NAND")){
				nandflash_controller_type = RTL8881A_NAND;
			}else if(!strcmp(*(argv),"RTL819xD_NAND")){
				nandflash_controller_type = RTL819xD_NAND;
			}else if(!strcmp(*(argv),"RTL8197F_SPI_NAND")){
				nandflash_controller_type = RTL8197F_SPI_NAND;
			}else if(!strcmp(*(argv),"RTL8197F_PARALLEL_NAND")){
				nandflash_controller_type = RTL8197F_PARALLEL_NAND;
			}else{
				printf("nand flash controller %s not support\n",*argv);
				return -1;
			}
        }else if ((strcmp(*argv, "--endian")==0)||(strcmp(*argv, "-edn")==0)) {
			if(!strcmp(*(++argv),"BIG_ENDIAN")){
				nandflash_endian = RTL_BIG_ENDIAN;
			}else if(!strcmp(*(argv),"LITTLE_ENDIAN")){
				nandflash_endian = RTL_LITTLE_ENDIAN;
			}else{
				printf("endian %s not support\n",*argv);
				return -1;
			}
        }
		else {
			name[i] = *argv++;
			offset[i] = atoi(*argv);
			i++;
            //input_file_name=*argv;
        }
    }
	
	input_file_name = "burn.bin";
	fd2=open(input_file_name, O_WRONLY|O_TRUNC|O_CREAT, 0666);
	unsigned char chunk_data2[chunk_size];
	max_boot = 0;
	for(j = 0;j < i;j++){
		total = 0;
		
		fd = open(name[j],O_RDONLY);

		/* spi nand  write 0xff to burn.bin here */
		if(nandflash_controller_type == RTL8197F_SPI_NAND){
			if(offset[j] == 0 || offset[j] == 0x40000 || 
				offset[j] == 0x80000 || offset[j] == 0xC0000){
				
				strcpy(boot_info.per_spi_info[max_boot].name,name[j]);
				boot_info.per_spi_info[max_boot++].offset = offset[j];

				if(j == (i-1)){
					max_size = 0x100000;
				}else if(j == 0){
					max_size = offset[j+1];
				}
				else{
					max_size = offset[j+1] - offset[j];
				}
				
				while(total < max_size){
					memset(chunk_data2, 0xff, chunk_size);
					write(fd2, chunk_data2, chunk_size);
					total += chunk_size;
				}
				close(fd);
				continue;
			}
		}

		while(1){
			r2 = read(fd, chunk_data2, chunk_size);

			write(fd2, chunk_data2, chunk_size);
			total += chunk_size;

			if(r2 != (int)chunk_size) 
				break;
		}
		
		if(j == (i-1)){
			max_size = LAST_IMAGE_SIZE;
		}else if(j == 0){
			max_size = offset[j+1];
		}
		else{
			max_size = offset[j+1] - offset[j];
		}
			
		
		while(total < max_size){
			memset(chunk_data2, 0xff, chunk_size);
			write(fd2, chunk_data2, chunk_size);
			total += chunk_size;
		}

		close(fd);
	}
	close(fd2);
	boot_info.spi_boot_num = max_boot;
	
    
    if (input_file_name==NULL) {
        fprintf(stderr, "input file name was defined\n");
        error_out=true;
    }
    switch (chunk_size) {
        case 512:
        case 2048:
        case 4096: 
            break;
        default:
            fprintf(stderr, "chunk size of %d bytes is not supported\n", chunk_size);
            error_out=true;
            break;
    }
    if (error_out) {
        show_message();
        return 1;
    }
    // select bch6 by default
    ecc_encoder=new ecc_bch6_encode_t();
    page_size=ecc_encoder->get_page_size();
    oob_size=ecc_encoder->get_oob_size();
    ecc_size=ecc_encoder->get_ecc_size();
    unit_size=ecc_encoder->get_unit_size();


	//printf("page_size=%d,oob_size=%d,ecc_size=%d,unit_size=%d\n",page_size,oob_size,ecc_size,unit_size);

    // limitation
    unsigned int max_swap_offset = oob_size * chunk_size / page_size;
    
    if ((bbi_swap_offset==MINUS_ONE)&&(bbi_dma_offset==MINUS_ONE)) {
        enable_swap=false;
    } else {
        if (bbi_swap_offset==MINUS_ONE) {
            fprintf(stderr, "--bbi-swap-offset and --bbi-dma-offset should be defined together.\n");
            error_out=true;
        } else if (bbi_swap_offset>=max_swap_offset) {
            fprintf(stderr, "bbi-swap-offset should be between 0 and %d\n", oob_size-1);
            error_out=true;
        } else if (bbi_dma_offset==MINUS_ONE) {
            fprintf(stderr, "--bbi-swap-offset and --bbi-dma-offset should be defined together.\n");
            error_out=true;
        } else if (bbi_dma_offset>=chunk_size) {
            fprintf(stderr, "bbi-dma-offset should be less than the chunk size\n");
            error_out=true;
        }
    }
    if (error_out) {
        show_message();
        return 1;
    }

    return write_out();
}
