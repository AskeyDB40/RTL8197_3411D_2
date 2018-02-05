/*
 * rtl819xD_rt5621.c - Realtek machine ASoC driver.
 *
 * Author: Johnny Hsu <johnnyhsu@realtek.com>
 * Copyright 2012 Realtek Semiconductor Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/tlv.h>
#include <sound/pcm_params.h>

#include "../codecs/rt5621.h"
#include "rtl819x-pcm.h"
#include "rtl8197d-i2s.h"

static int rtl819xd_rt5621_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	struct snd_soc_dai *cpu_dai = rtd->cpu_dai;
	unsigned int clk = 0;
	int bfs, rfs, ret = 0;

	/*
    printk("rate %d format %x\n", params_rate(params),
		params_format(params));
	*/
       
	pr_debug("%s rate %d format %x\n", __func__, params_rate(params),
		params_format(params));

	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_U8:
	case SNDRV_PCM_FORMAT_S8:
		bfs = 16;
		break;
	case SNDRV_PCM_FORMAT_U16_LE:
	case SNDRV_PCM_FORMAT_S16_LE:
	case SNDRV_PCM_FORMAT_S16_BE:
		bfs = 32;
		break;
	default:
		return -EINVAL;
	}

	switch (params_rate(params)) {
	case 16000:
	case 32000:
	case 48000:
#if defined(CONFIG_RTL_8881A)		
	case 44100:
#endif		
	case 8000:
		rfs = 256;
		break;
	default:
		return -EINVAL;
	}
	clk = params_rate(params) * rfs;
	/* set codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
		SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);	
	if (ret < 0)
		return ret;
	/* set cpu DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S |
		SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);	
	if (ret < 0){
		return ret;
	}
	ret = snd_soc_dai_set_sysclk(codec_dai, 0, clk, SND_SOC_CLOCK_OUT);
	if (ret < 0){
		return ret;
	}
	return 0;
}

static struct snd_soc_ops rtl819xd_rt5621_ops = {
	.hw_params = rtl819xd_rt5621_hw_params,
};

static const struct snd_soc_dapm_widget rtl819xd_dapm_widgets[] = {
	SND_SOC_DAPM_MIC("Int Mic", NULL),
};

static const struct snd_soc_dapm_route rtl819xd_audio_map[] = {
	{"Mic Bias1", NULL, "Int Mic"},
	{"Mic1", NULL, "Mic Bias1"},
};


static int rtl819xd_rt5621_rt5621_init(struct snd_soc_codec *codec)
{
	printk("%s, %d\n", __FUNCTION__, __LINE__);
if(codec == NULL){
	printk("%s, %d, codec is NULL\n", __FUNCTION__, __LINE__);
}
#if 0
	snd_soc_dapm_new_controls(codec, rtl819xd_dapm_widgets, 
		ARRAY_SIZE(rtl819xd_dapm_widgets));
		
		
		printk("%s, %d\n", __FUNCTION__, __LINE__);

	snd_soc_dapm_add_routes(codec, rtl819xd_audio_map, 
		ARRAY_SIZE(rtl819xd_audio_map));
#endif		
//printk("%s, %d\n", __FUNCTION__, __LINE__);
		
	//snd_soc_dapm_enable_pin(codec, "Int Mic");
	//printk("%s, %d\n", __FUNCTION__, __LINE__);

	//snd_soc_dapm_sync(codec);
	//printk("%s, %d\n", __FUNCTION__, __LINE__);
		
	return 0;
}

static struct snd_soc_dai_link rtl819xd_rt5621_dai[] = {
	{ 
		.name = "rtl819xd_rt5621_dai",
		.stream_name = "AIF1 Playback",
		.cpu_dai_name = "rtl819x-iis",
		.codec_dai_name = "rt5621-hifi",
		.platform_name = "rtl819x-iis",
		.codec_name = "rt5621",
		.ops = &rtl819xd_rt5621_ops,
		.init = rtl819xd_rt5621_rt5621_init,
	},
	



		
};
#if 0
static struct rt5621_setup_data rtl819xd_rt5621_setup = {
	.i2c_bus = 0,
	.i2c_address = 0x1c,
};

static struct snd_soc_card rtl819xd_rt5621 = {
	.name = "rtl819xd_rt5621",
	.platform = &rtl819x_soc_platform,
	.dai_link = &rtl819xd_rt5621_dai,
	.num_links = 1,
};
#endif

static struct snd_soc_card rtl819xd_rt5621 = {
	.name = "rtl819xd_rt5621",
	.owner = THIS_MODULE,
	.dai_link = rtl819xd_rt5621_dai,
	.num_links = ARRAY_SIZE(rtl819xd_rt5621_dai),

	.dapm_widgets = rtl819xd_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(rtl819xd_dapm_widgets),
	
	.dapm_routes = rtl819xd_audio_map,
	.num_dapm_routes = ARRAY_SIZE(rtl819xd_audio_map),
	
};


#if 0
static struct snd_soc_device rtl819xd_snd_devdata = {
	.card = &rtl819xd_rt5621,
	.codec_dev = &soc_codec_dev_rt5621,
	.codec_data = &rtl819xd_rt5621_setup,
};
#endif

static struct platform_device *rtl819xd_snd_device;


static int __init rtl819x_rt5621_modinit(void)
{
	int ret=0;
	printk("%s, %d\n", __FUNCTION__, __LINE__);
	rtl819xd_snd_device = platform_device_alloc("soc-audio", -1);
	if (!rtl819xd_snd_device)
		return -ENOMEM;
	printk("%s, %d\n", __FUNCTION__, __LINE__);

	platform_set_drvdata(rtl819xd_snd_device, &rtl819xd_rt5621);
	//rtl819xd_rt5621.dev = &rtl819xd_snd_device->dev;
	ret = platform_device_add(rtl819xd_snd_device);
	if (ret){
		printk("%s, %d, ret=%d\n", __FUNCTION__, __LINE__,ret);
		platform_device_put(rtl819xd_snd_device);
	}
	printk("%s, %d, ret=%d\n", __FUNCTION__, __LINE__,ret);
	return 0;
}
module_init(rtl819x_rt5621_modinit);

static void __exit rtl819x_rt5621_modexit(void)
{
	platform_device_unregister(rtl819xd_snd_device);
}
module_exit(rtl819x_rt5621_modexit);




MODULE_DESCRIPTION("ASoC RT5621 driver");
MODULE_AUTHOR("Johnny Hsu <johnnyhsu@realtek.com>");
MODULE_LICENSE("GPL");
