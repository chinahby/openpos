/*
    Audio/video-routing-related ivtv functions.
    Copyright (C) 2003-2004  Kevin Thayer <nufan_wfk at yahoo.com>
    Copyright (C) 2005-2007  Hans Verkuil <hverkuil@xs4all.nl>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ivtv-driver.h"
#include "ivtv-i2c.h"
#include "ivtv-cards.h"
#include "ivtv-gpio.h"
#include "ivtv-routing.h"

#include <media/msp3400.h>
#include <media/m52790.h>
#include <media/upd64031a.h>
#include <media/upd64083.h>

/* Selects the audio input and output according to the current
   settings. */
void ivtv_audio_set_io(struct ivtv *itv)
{
	const struct ivtv_card_audio_input *in;
	u32 input, output = 0;

	/* Determine which input to use */
	if (test_bit(IVTV_F_I_RADIO_USER, &itv->i_flags))
		in = &itv->card->radio_input;
	else
		in = &itv->card->audio_inputs[itv->audio_input];

	/* handle muxer chips */
	input = in->muxer_input;
	if (itv->card->hw_muxer & IVTV_HW_M52790)
		output = M52790_OUT_STEREO;
	v4l2_subdev_call(itv->sd_muxer, audio, s_routing,
			input, output, 0);

	input = in->audio_input;
	output = 0;
	if (itv->card->hw_audio & IVTV_HW_MSP34XX)
		output = MSP_OUTPUT(MSP_SC_IN_DSP_SCART1);
	ivtv_call_hw(itv, itv->card->hw_audio, audio, s_routing,
			input, output, 0);
}

/* Selects the video input and output according to the current
   settings. */
void ivtv_video_set_io(struct ivtv *itv)
{
	int inp = itv->active_input;
	u32 input;
	u32 type;

	v4l2_subdev_call(itv->sd_video, video, s_routing,
		itv->card->video_inputs[inp].video_input, 0, 0);

	type = itv->card->video_inputs[inp].video_type;

	if (type == IVTV_CARD_INPUT_VID_TUNER) {
		input = 0;  /* Tuner */
	} else if (type < IVTV_CARD_INPUT_COMPOSITE1) {
		input = 2;  /* S-Video */
	} else {
		input = 1;  /* Composite */
	}

	if (itv->card->hw_video & IVTV_HW_GPIO)
		ivtv_call_hw(itv, IVTV_HW_GPIO, video, s_routing,
				input, 0, 0);

	if (itv->card->hw_video & IVTV_HW_UPD64031A) {
		if (type == IVTV_CARD_INPUT_VID_TUNER ||
		    type >= IVTV_CARD_INPUT_COMPOSITE1) {
			/* Composite: GR on, connect to 3DYCS */
			input = UPD64031A_GR_ON | UPD64031A_3DYCS_COMPOSITE;
		} else {
			/* S-Video: GR bypassed, turn it off */
			input = UPD64031A_GR_OFF | UPD64031A_3DYCS_DISABLE;
		}
		input |= itv->card->gr_config;

		ivtv_call_hw(itv, IVTV_HW_UPD64031A, video, s_routing,
				input, 0, 0);
	}

	if (itv->card->hw_video & IVTV_HW_UPD6408X) {
		input = UPD64083_YCS_MODE;
		if (type > IVTV_CARD_INPUT_VID_TUNER &&
		    type < IVTV_CARD_INPUT_COMPOSITE1) {
			/* S-Video uses YCNR mode and internal Y-ADC, the
			   upd64031a is not used. */
			input |= UPD64083_YCNR_MODE;
		}
		else if (itv->card->hw_video & IVTV_HW_UPD64031A) {
			/* Use upd64031a output for tuner and
			   composite(CX23416GYC only) inputs */
			if (type == IVTV_CARD_INPUT_VID_TUNER ||
			    itv->card->type == IVTV_CARD_CX23416GYC) {
				input |= UPD64083_EXT_Y_ADC;
			}
		}
		ivtv_call_hw(itv, IVTV_HW_UPD6408X, video, s_routing,
				input, 0, 0);
	}
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                        /*
 * Copyright 2004-2008 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @file mt9v111.c
 *
 * @brief mt9v111 camera driver functions
 *
 * @ingroup Camera
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/clk.h>
#include "mxc_v4l2_capture.h"
#include "mt9v111.h"

#ifdef MT9V111_DEBUG
static u16 testpattern;
#endif

static sensor_interface *interface_param;
static mt9v111_conf mt9v111_device;
static int reset_frame_rate = 30;

#define MT9V111_FRAME_RATE_NUM    20

static mt9v111_image_format format[2] = {
	{
	 .index = 0,
	 .width = 640,
	 .height = 480,
	 },
	{
	 .index = 1,
	 .width = 352,
	 .height = 288,
	 },
};

static int mt9v111_attach(struct i2c_adapter *adapter);
static int mt9v111_detach(struct i2c_client *client);

static struct i2c_driver mt9v111_i2c_driver = {
	.driver = {
		   .owner = THIS_MODULE,
		   .name = "MT9V111 Client",
		   },
	.attach_adapter = mt9v111_attach,
	.detach_client = mt9v111_detach,
};

static struct i2c_client mt9v111_i2c_client = {
	.name = "mt9v111 I2C dev",
	.addr = MT9V111_I2C_ADDRESS,
	.driver = &mt9v111_i2c_driver,
};

/*
 * Function definitions
 */

#ifdef MT9V111_DEBUG
static inline int mt9v111_read_reg(u8 reg)
{
	int val = i2c_smbus_read_word_data(&mt9v111_i2c_client, reg);
	if (val != -1)
		val = cpu_to_be16(val);
	return val;
}
#endif

static inline int mt9v111_write_reg(u8 reg, u16 val)
{
	pr_debug("write reg %x val %x.\n", reg, val);
	return i2c_smbus_write_word_data(&mt9v111_i2c_client, reg,
					 cpu_to_be16(val));
}

/*!
 * Initialize mt9v111_sensor_lib
 * Libarary for Sensor configuration through I2C
 *
 * @param       coreReg       Core Registers
 * @param       ifpReg        IFP Register
 *
 * @return status
 */
static u8 mt9v111_sensor_lib(mt9v111_coreReg * coreReg, mt9v111_IFPReg * ifpReg)
{
	u8 reg;
	u16 data;
	u8 error = 0;

	/*
	 * setup to IFP registers
	 */
	reg = MT9V111I_ADDR_SPACE_SEL;
	data = ifpReg->addrSpaceSel;
	mt9v111_write_reg(reg, data);

	/* Operation Mode Control */
	reg = MT9V111I_MODE_CONTROL;
	data = ifpReg->modeControl;
	mt9v111_write_reg(reg, data);

	/* Output format */
	reg = MT9V111I_FORMAT_CONTROL;
	data = ifpReg->formatControl;	/* Set bit 12 */
	mt9v111_write_reg(reg, data);

	/* AE limit 4 */
	reg = MT9V111I_SHUTTER_WIDTH_LIMIT_AE;
	data = ifpReg->gainLimitAE;
	mt9v111_write_reg(reg, data);

	reg = MT9V111I_OUTPUT_FORMAT_CTRL2;
	data = ifpReg->outputFormatCtrl2;
	mt9v111_write_reg(reg, data);

	reg = MT9V111I_AE_SPEED;
	data = ifpReg->AESpeed;
	mt9v111_write_reg(reg, data);

	/* output image size */
	reg = MT9V111i_H_PAN;
	data = 0x8000 | ifpReg->HPan;
	mt9v111_write_reg(reg, data);

	reg = MT9V111i_H_ZOOM;
	data = 0x8000 | ifpReg->HZoom;
	mt9v111_write_reg(reg, data);

	reg = MT9V111i_H_SIZE;
	data = 0x8000 | ifpReg->HSize;
	mt9v111_write_reg(reg, data);

	reg = MT9V111i_V_PAN;
	data = 0x8000 | ifpReg->VPan;
	mt9v111_write_reg(reg, data);

	reg = MT9V111i_V_ZOOM;
	data = 0x8000 | ifpReg->VZoom;
	mt9v111_write_reg(reg, data);

	reg = MT9V111i_V_SIZE;
	data = 0x8000 | ifpReg->VSize;
	mt9v111_write_reg(reg, data);

	reg = MT9V111i_H_PAN;
	data = ~0x8000 & ifpReg->HPan;
	mt9v111_write_reg(reg, data);
#if 0
	reg = MT9V111I_UPPER_SHUTTER_DELAY_LIM;
	data = ifpReg->upperShutterDelayLi;
	mt9v111_write_reg(reg, data);

	reg = MT9V111I_SHUTTER_60;
	data = ifpReg->shutter_width_60;
	mt9v111_write_reg(reg, data);

	reg = MT9V111I_SEARCH_FLICK_60;
	data = ifpReg->search_flicker_60;
	mt9v111_write_reg(reg, data);
#endif

	/*
	 * setup to sensor core registers
	 */
	reg = MT9V111I_ADDR_SPACE_SEL;
	data = coreReg->addressSelect;
	mt9v111_write_reg(reg, data);

	/* enable changes and put the Sync bit on */
	reg = MT9V111S_OUTPUT_CTRL;
	data = MT9V111S_OUTCTRL_SYNC | MT9V111S_OUTCTRL_CHIP_ENABLE | 0x3000;
	mt9v111_write_reg(reg, data);

	/* min PIXCLK - Default */
	reg = MT9V111S_PIXEL_CLOCK_SPEED;
	data = coreReg->pixelClockSpeed;
	mt9v111_write_reg(reg, data);

	/* Setup image flipping / Dark rows / row/column skip */
	reg = MT9V111S_READ_MODE;
	data = coreReg->readMode;
	mt9v111_write_reg(reg, data);

	/*zoom 0 */
	reg = MT9V111S_DIGITAL_ZOOM;
	data = coreReg->digitalZoom;
	mt9v111_write_reg(reg, data);

	/* min H-blank */
	reg = MT9V111S_HOR_BLANKING;
	data = coreReg->horizontalBlanking;
	mt9v111_write_reg(reg, data);

	/* min V-blank */
	reg = MT9V111S_VER_BLANKING;
	data = coreReg->verticalBlanking;
	mt9v111_write_reg(reg, data);

	reg = MT9V111S_SHUTTER_WIDTH;
	data = coreReg->shutterWidth;
	mt9v111_write_reg(reg, data);

	reg = MT9V111S_SHUTTER_DELAY;
	data = ifpReg->upperShutterDelayLi;
	mt9v111_write_reg(reg, data);

	/* changes become effective */
	reg = MT9V111S_OUTPUT_CTRL;
	data = MT9V111S_OUTCTRL_CHIP_ENABLE | 0x3000;
	mt9v111_write_reg(reg, data);

	return error;
}

/*!
 * mt9v111 sensor interface Initialization
 * @param param            sensor_interface *
 * @param width            u32
 * @param height           u32
 * @return  None
 */
static void mt9v111_interface(sensor_interface *param, u32 width, u32 height)
{
	param->Vsync_pol = 0x0;
	param->clk_mode = 0x0;	/*gated */
	param->pixclk_pol = 0x0;
	param->data_width = 0x1;
	param->data_pol = 0x0;
	param->ext_vsync = 0x0;
	param->Vsync_pol = 0x0;
	param->Hsync_pol = 0x0;
	param->width = width - 1;
	param->height = height - 1;
	param->active_width = width;
	param->active_height = height;
	param->pixel_fmt = IPU_PIX_FMT_UYVY;
	param->mclk = 27000000;
}

/*!
 * MT9V111 frame rate calculate
 *
 * @param frame_rate       int *
 * @param mclk             int
 * @return  None
 */
static void mt9v111_rate_cal(int *frame_rate, int mclk)
{
	int num_clock_per_row;
	int max_rate = 0;

	mt9v111_device.coreReg->horizontalBlanking = MT9V111_HORZBLANK_MIN;

	num_clock_per_row = (format[0].width + 114 + MT9V111_HORZBLANK_MIN) * 2;
	max_rate = mclk / (num_clock_per_row *
			   (format[0].height + MT9V111_VERTBLANK_DEFAULT));

	if ((*frame_rate > max_rate) || (*frame_rate == 0)) {
		*frame_rate = max_rate;
	}

	mt9v111_device.coreReg->verticalBlanking
	    = mclk / (*frame_rate * num_clock_per_row) - format[0].height;

	reset_frame_rate = *frame_rate;
}

/*!
 * MT9V111 sensor configuration
 *
 * @param frame_rate       int *
 * @param high_quality     int
 * @return  sensor_interface *
 */
sensor_interface *mt9v111_config(int *frame_rate, int high_quality)
{
	u32 out_width, out_height;

	if (interface_param == NULL)
		return NULL;

	mt9v111_device.coreReg->addressSelect = MT9V111I_SEL_SCA;
	mt9v111_device.ifpReg->addrSpaceSel = MT9V111I_SEL_IFP;

	mt9v111_device.coreReg->windowHeight = MT9V111_WINHEIGHT;
	mt9v111_device.coreReg->windowWidth = MT9V111_WINWIDTH;
	mt9v111_device.coreReg->zoomColStart = 0;
	mt9v111_device.coreReg->zomRowStart = 0;
	mt9v111_device.coreReg->digitalZoom = 0x0;

	mt9v111_device.coreReg->verticalBlanking = MT9V111_VERTBLANK_DEFAULT;
	mt9v111_device.coreReg->horizontalBlanking = MT9V111_HORZBLANK_MIN;
	mt9v111_device.coreReg->pixelClockSpeed = 0;
	mt9v111_device.coreReg->readMode = 0xd0a1;

	mt9v111_device.ifpReg->outputFormatCtrl2 = 0;
	mt9v111_device.ifpReg->gainLimitAE = 0x300;
	mt9v111_device.ifpReg->AESpeed = 0x80;

	/* here is the default value */
	mt9v111_device.ifpReg->formatControl = 0xc800;
	mt9v111_device.ifpReg->modeControl = 0x708e;
	mt9v111_device.ifpReg->awbSpeed = 0x4514;
	mt9v111_device.coreReg->shutterWidth = 0xf8;

	out_width = 640;
	out_height = 480;

	/*output size */
	mt9v111_device.ifpReg->HPan = 0;
	mt9v111_device.ifpReg->HZoom = 640;
	mt9v111_device.ifpReg->HSize = out_width;
	mt9v111_device.ifpReg->VPan = 0;
	mt9v111_device.ifpReg->VZoom = 480;
	mt9v111_device.ifpReg->VSize = out_height;

	mt9v111_interface(interface_param, out_width, out_height);
	set_mclk_rate(&interface_param->mclk);
	mt9v111_rate_cal(frame_rate, interface_param->mclk);
	mt9v111_sensor_lib(mt9v111_device.coreReg, mt9v111_device.ifpReg);

	return interface_param;
}

/*!
 * mt9v111 sensor set color configuration
 *
 * @param bright       int
 * @param saturation   int
 * @param red          int
 * @param green        int
 * @param blue         int
 * @return  None
 */
static void
mt9v111_set_color(int bright, int saturation, int red, int green, int blue)
{
	u8 reg;
	u16 data;

	switch (saturation) {
	case 100:
		mt9v111_device.ifpReg->awbSpeed = 0x4514;
		break;
	case 150:
		mt9v111_device.ifpReg->awbSpeed = 0x6D14;
		break;
	case 75:
		mt9v111_device.ifpReg->awbSpeed = 0x4D14;
		break;
	case 50:
		mt9v111_device.ifpReg->awbSpeed = 0x5514;
		break;
	case 37:
		mt9v111_device.ifpReg->awbSpeed = 0x5D14;
		break;
	case 25:
		mt9v111_device.ifpReg->awbSpeed = 0x6514;
		break;
	default:
		mt9v111_device.ifpReg->awbSpeed = 0x4514;
		break;
	}

	reg = MT9V111I_ADDR_SPACE_SEL;
	data = mt9v111_device.ifpReg->addrSpaceSel;
	mt9v111_write_reg(reg, data);

	/* Operation Mode Control */
	reg = MT9V111I_AWB_SPEED;
	data = mt9v111_device.ifpReg->awbSpeed;
	mt9v111_write_reg(reg, data);
}

/*!
 * mt9v111 sensor get color configuration
 *
 * @param bright       int *
 * @param saturation   int *
 * @param red          int *
 * @param green        int *
 * @param blue         int *
 * @return  None
 */
static void
mt9v111_get_color(int *bright, int *saturation, int *red, int *green, int *blue)
{
	*saturation = (mt9v111_device.ifpReg->awbSpeed & 0x3800) >> 11;
	switch (*saturation) {
	case 0:
		*saturation = 100;
		break;
	case 1:
		*saturation = 75;
		break;
	case 2:
		*saturation = 50;
		break;
	case 3:
		*saturation = 37;
		break;
	case 4:
		*saturation = 25;
		break;
	case 5:
		*saturation = 150;
		break;
	case 6:
		*saturation = 0;
		break;
	default:
		*saturation = 0;
		break;
	}
}

/*!
 * mt9v111 sensor set AE measurement window mode configuration
 *
 * @param ae_mode      int
 * @return  None
 */
static void mt9v111_set_ae_mode(int ae_mode)
{
	u8 reg;
	u16 data;

	mt9v111_device.ifpReg->modeControl &= 0xfff3;
	mt9v111_device.ifpReg->modeControl |= (ae_mode & 0x03) << 2;

	reg = MT9V111I_ADDR_SPACE_SEL;
	data = mt9v111_device.ifpReg->addrSpaceSel;
	mt9v111_write_reg(reg, data);

	reg = MT9V111I_MODE_CONTROL;
	data = mt9v111_device.ifpReg->modeControl;
	mt9v111_write_reg(reg, data);
}

/*!
 * mt9v111 sensor get AE measurement window mode configuration
 *
 * @param ae_mode      int *
 * @return  None
 */
static void mt9v111_get_ae_mode(int *ae_mode)
{
	if (ae_mode != NULL) {
		*ae_mode = (mt9v111_device.ifpReg->modeControl & 0xc) >> 2;
	}
}

/*!
 * mt9v111 Reset function
 *
 * @return  None
 */
static sensor_interface *mt9v111_reset(void)
{
	return mt9v111_config(&reset_frame_rate, 0);
}

struct camera_sensor camera_sensor_if = {
	.set_color = mt9v111_set_color,
	.get_color = mt9v111_get_color,
	.set_ae_mode = mt9v111_set_ae_mode,
	.get_ae_mode = mt9v111_get_ae_mode,
	.config = mt9v111_config,
	.reset = mt9v111_reset,
};

#ifdef MT9V111_DEBUG
/*!
 * Set sensor to test mode, which will generate test pattern.
 *
 * @return none
 */
static void mt9v111_test_pattern(bool flag)
{
	u16 data;

	/* switch to sensor registers */
	mt9v111_write_reg(MT9V111I_ADDR_SPACE_SEL, MT9V111I_SEL_SCA);

	if (flag == true) {
		testpattern = MT9V111S_OUTCTRL_TEST_MODE;

		data = mt9v111_read_reg(MT9V111S_ROW_NOISE_CTRL) & 0xBF;
		mt9v111_write_reg(MT9V111S_ROW_NOISE_CTRL, data);

		mt9v111_write_reg(MT9V111S_TEST_DATA, 0);

		/* changes take effect */
		data = MT9V111S_OUTCTRL_CHIP_ENABLE | testpattern | 0x3000;
		mt9v111_write_reg(MT9V111S_OUTPUT_CTRL, data);
	} else {
		testpattern = 0;

		data = mt9v111_read_reg(MT9V111S_ROW_NOISE_CTRL) | 0x40;
		mt9v111_write_reg(MT9V111S_ROW_NOISE_CTRL, data);

		/* changes take effect */
		data = MT9V111S_OUTCTRL_CHIP_ENABLE | testpattern | 0x3000;
		mt9v111_write_reg(MT9V111S_OUTPUT_CTRL, data);
	}
}
#endif

/*!
 * mt9v111 I2C detect_client function
 *
 * @param adapter            struct i2c_adapter *
 * @param address            int
 * @param kind               int
 *
 * @return  Error code indicating success or failure
 */
static int mt9v111_detect_client(struct i2c_adapter *adapter, int address,
				 int kind)
{
	mt9v111_i2c_client.adapter = adapter;
	if (i2c_attach_client(&mt9v111_i2c_client)) {
		mt9v111_i2c_client.adapter = NULL;
		printk(KERN_ERR "mt9v111_attach: i2c_attach_client failed\n");
		return -1;
	}

	interface_param = (sensor_interface *)
	    kmalloc(sizeof(sensor_interface), GFP_KERNEL);
	if (!interface_param) {
		printk(KERN_ERR "mt9v111_attach: kmalloc failed \n");
		return -1;
	}

	printk(KERN_INFO "MT9V111 Detected\n");

	return 0;
}

static unsigned short normal_i2c[] = { MT9V111_I2C_ADDRESS, I2C_CLIENT_END };

/* Magic definition of all other variables and things */
I2C_CLIENT_INSMOD;

/*!
 * mt9v111 I2C attach function
 *
 * @param adapter            struct i2c_adapter *
 * @return  Error code indicating success or failure
 */
static int mt9v111_attach(struct i2c_adapter *adap)
{
	uint32_t mclk = 27000000;
	struct clk *clk;
	int err;

	clk = clk_get(NULL, "csi_clk");
	clk_enable(clk);
	set_mclk_rate(&mclk);

	err = i2c_probe(adap, &addr_data, &mt9v111_detect_client);

	clk_disable(clk);
	clk_put(clk);

	return err;
}

/*!
 * mt9v111 I2C detach function
 *
 * @param client            struct i2c_client *
 * @return  Error code indicating success or failure
 */
static int mt9v111_detach(struct i2c_client *client)
{
	int err;

	if (!mt9v111_i2c_client.adapter)
		return -1;

	err = i2c_detach_client(&mt9v111_i2c_client);
	mt9v111_i2c_client.adapter = NULL;

	if (interface_param)
		kfree(interface_param);
	interface_param = NULL;

	return err;
}

extern void gpio_sensor_active(void);

/*!
 * MT9V111 init function
 *
 * @return  Error code indicating success or failure
 */
static __init int mt9v111_init(void)
{
	u8 err;

	gpio_sensor_active();

	mt9v111_device.coreReg = (mt9v111_coreReg *)
	    kmalloc(sizeof(mt9v111_coreReg), GFP_KERNEL);
	if (!mt9v111_device.coreReg)
		return -1;

	memset(mt9v111_device.coreReg, 0, sizeof(mt9v111_coreReg));

	mt9v111_device.ifpReg = (mt9v111_IFPReg *)
	    kmalloc(sizeof(mt9v111_IFPReg), GFP_KERNEL);
	if (!mt9v111_device.ifpReg) {
		kfree(mt9v111_device.coreReg);
		mt9v111_device.coreReg = NULL;
		return -1;
	}

	memset(mt9v111_device.ifpReg, 0, sizeof(mt9v111_IFPReg));

	err = i2c_add_driver(&mt9v111_i2c_driver);

	return err;
}

extern void gpio_sensor_inactive(void);
/*!
 * MT9V111 cleanup function
 *
 * @return  Error code indicating success or failure
 */
static void __exit mt9v111_clean(void)
{
	if (mt9v111_device.coreReg) {
		kfree(mt9v111_device.coreReg);
		mt9v111_device.coreReg = NULL;
	}

	if (mt9v111_device.ifpReg) {
		kfree(mt9v111_device.ifpReg);
		mt9v111_device.ifpReg = NULL;
	}

	i2c_del_driver(&mt9v111_i2c_driver);

	gpio_sensor_inactive();
}

module_init(mt9v111_init);
module_exit(mt9v111_clean);

/* Exported symbols for modules. */
EXPORT_SYMBOL(camera_sensor_if);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("Mt9v111 Camera Driver");
MODULE_LICENSE("GPL");
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ruct snd_pcm_hw_constraint_ratdens snd_es1371_hw_constraints_dac_clock = {
	.nrats = 1,
	.rats = &es1371_dac_clock,
};
static struct snd_ratnum es1371_adc_clock = {
	.num = 48000 << 15,
	.den_min = 32768, 
	.den_max = 393216,
	.den_step = 1,
};
static struct snd_pcm_hw_constraint_ratnums snd_es1371_hw_constraints_adc_clock = {
	.nrats = 1,
	.rats = &es1371_adc_clock,
};
#endif
static const unsigned int snd_ensoniq_sample_shift[] =
	{0, 1, 1, 2};

/*
 *  common I/O routines
 */

#ifdef CHIP1371

static unsigned int snd_es1371_wait_src_ready(struct ensoniq * ensoniq)
{
	unsigned int t, r = 0;

	for (t = 0; t < POLL_COUNT; t++) {
		r = inl(ES_REG(ensoniq, 1371_SMPRATE));
		if ((r & ES_1371_SRC_RAM_BUSY) == 0)
			return r;
		cond_resched();
	}
	snd_printk(KERN_ERR "wait src ready timeout 0x%lx [0x%x]\n",
		   ES_REG(ensoniq, 1371_SMPRATE), r);
	return 0;
}

static unsigned int snd_es1371_src_read(struct ensoniq * ensoniq, unsigned short reg)
{
	unsigned int temp, i, orig, r;

	/* wait for ready */
	temp = orig = snd_es1371_wait_src_ready(ensoniq);

	/* expose the SRC state bits */
	r = temp & (ES_1371_SRC_DISABLE | ES_1371_DIS_P1 |
		    ES_1371_DIS_P2 | ES_1371_DIS_R1);
	r |= ES_1371_SRC_RAM_ADDRO(reg) | 0x10000;
	outl(r, ES_REG(ensoniq, 1371_SMPRATE));

	/* now, wait for busy and the correct time to read */
	temp = snd_es1371_wait_src_ready(ensoniq);
	
	if ((temp & 0x00870000) != 0x00010000) {
		/* wait for the right state */
		for (i = 0; i < POLL_COUNT; i++) {
			temp = inl(ES_REG(ensoniq, 1371_SMPRATE));
			if ((temp & 0x00870000) == 0x00010000)
				break;
		}
	}

	/* hide the state bits */	
	r = orig & (ES_1371_SRC_DISABLE | ES_1371_DIS_P1 |
		   ES_1371_DIS_P2 | ES_1371_DIS_R1);
	r |= ES_1371_SRC_RAM_ADDRO(reg);
	outl(r, ES_REG(ensoniq, 1371_SMPRATE));
	
	return temp;
}

static void snd_es1371_src_write(struct ensoniq * ensoniq,
				 unsigned short reg, unsigned short data)
{
	unsigned int r;

	r = snd_es1371_wait_src_ready(ensoniq) &
	    (ES_1371_SRC_DISABLE | ES_1371_DIS_P1 |
	     ES_1371_DIS_P2 | ES_1371_DIS_R1);
	r |= ES_1371_SRC_RAM_ADDRO(reg) | ES_1371_SRC_RAM_DATAO(data);
	outl(r | ES_1371_SRC_RAM_WE, ES_REG(ensoniq, 1371_SMPRATE));
}

#endif /* CHIP1371 */

#ifdef CHIP1370

static void snd_es1370_codec_write(struct snd_ak4531 *ak4531,
				   unsigned short reg, unsigned short val)
{
	struct ensoniq *ensoniq = ak4531->private_data;
	unsigned long end_time = jiffies + HZ / 10;

#if 0
	printk(KERN_DEBUG
	       "CODEC WRITE: reg = 0x%x, val = 0x%x (0x%x), creg = 0x%x\n",
	       reg, val, ES_1370_CODEC_WRITE(reg, val), ES_REG(ensoniq, 1370_CODEC));
#endif
	do {
		if (!(inl(ES_REG(ensoniq, STATUS)) & ES_1370_CSTAT)) {
			outw(ES_1370_CODEC_WRITE(reg, val), ES_REG(ensoniq, 1370_CODEC));
			return;
		}
		schedule_timeout_uninterruptible(1);
	} while (time_after(end_time, jiffies));
	snd_printk(KERN_ERR "codec write timeout, status = 0x%x\n",
		   inl(ES_REG(ensoniq, STATUS)));
}

#endif /* CHIP1370 */

#ifdef CHIP1371

static void snd_es1371_codec_write(struct snd_ac97 *ac97,
				   unsigned short reg, unsigned short val)
{
	struct ensoniq *ensoniq = ac97->private_data;
	unsigned int t, x;

	mutex_lock(&ensoniq->src_mutex);
	for (t = 0; t < POLL_COUNT; t++) {
		if (!(inl(ES_REG(ensoniq, 1371_CODEC)) & ES_1371_CODEC_WIP)) {
			/* save the current state for latter */
			x = snd_es1371_wait_src_ready(ensoniq);
			outl((x & (ES_1371_SRC_DISABLE | ES_1371_DIS_P1 |
			           ES_1371_DIS_P2 | ES_1371_DIS_R1)) | 0x00010000,
			     ES_REG(ensoniq, 1371_SMPRATE));
			/* wait for not busy (state 0) first to avoid
			   transition states */
			for (t = 0; t < POLL_COUNT; t++) {
				if ((inl(ES_REG(ensoniq, 1371_SMPRATE)) & 0x00870000) ==
				    0x00000000)
					break;
			}
			/* wait for a SAFE time to write addr/data and then do it, dammit */
			for (t = 0; t < POLL_COUNT; t++) {
				if ((inl(ES_REG(ensoniq, 1371_SMPRATE)) & 0x00870000) ==
				    0x00010000)
					break;
			}
			outl(ES_1371_CODEC_WRITE(reg, val), ES_REG(ensoniq, 1371_CODEC));
			/* restore SRC reg */
			snd_es1371_wait_src_ready(ensoniq);
			outl(x, ES_REG(ensoniq, 1371_SMPRATE));
			mutex_unlock(&ensoniq->src_mutex);
			return;
		}
	}
	mutex_unlock(&ensoniq->src_mutex);
	snd_printk(KERN_ERR "codec write timeout at 0x%lx [0x%x]\n",
		   ES_REG(ensoniq, 1371_CODEC), inl(ES_REG(ensoniq, 1371_CODEC)));
}

static unsigned short snd_es1371_codec_read(struct snd_ac97 *ac97,
					    unsigned short reg)
{
	struct ensoniq *ensoniq = ac97->private_data;
	unsigned int t, x, fail = 0;

      __again:
	mutex_lock(&ensoniq->src_mutex);
	for (t = 0; t < POLL_COUNT; t++) {
		if (!(inl(ES_REG(ensoniq, 1371_CODEC)) & ES_1371_CODEC_WIP)) {
			/* save the current state for latter */
			x = snd_es1371_wait_src_ready(ensoniq);
			outl((x & (ES_1371_SRC_DISABLE | ES_1371_DIS_P1 |
			           ES_1371_DIS_P2 | ES_1371_DIS_R1)) | 0x00010000,
			     ES_REG(ensoniq, 1371_SMPRATE));
			/* wait for not busy (state 0) first to avoid
			   transition states */
			for (t = 0; t < POLL_COUNT; t++) {
				if ((inl(ES_REG(ensoniq, 1371_SMPRATE)) & 0x00870000) ==
				    0x00000000)
					break;
			}
			/* wait for a SAFE time to write addr/data and then do it, dammit */
			for (t = 0; t < POLL_COUNT; t++) {
				if ((inl(ES_REG(ensoniq, 1371_SMPRATE)) & 0x00870000) ==
				    0x00010000)
					break;
			}
			outl(ES_1371_CODEC_READS(reg), ES_REG(ensoniq, 1371_CODEC));
			/* restore SRC reg */
			snd_es1371_wait_src_ready(ensoniq);
			outl(x, ES_REG(ensoniq, 1371_SMPRATE));
			/* wait for WIP again */
			for (t = 0; t < POLL_COUNT; t++) {
				if (!(inl(ES_REG(ensoniq, 1371_CODEC)) & ES_1371_CODEC_WIP))
					break;		
			}
			/* now wait for the stinkin' data (RDY) */
			for (t = 0; t < POLL_COUNT; t++) {
				if ((x = inl(ES_REG(ensoniq, 1371_CODEC))) & ES_1371_CODEC_RDY) {
					mutex_unlock(&ensoniq->src_mutex);
					return ES_1371_CODEC_READ(x);
				}
			}
			mutex_unlock(&ensoniq->src_mutex);
			if (++fail > 10) {
				snd_printk(KERN_ERR "codec read timeout (final) "
					   "at 0x%lx, reg = 0x%x [0x%x]\n",
					   ES_REG(ensoniq, 1371_CODEC), reg,
					   inl(ES_REG(ensoniq, 1371_CODEC)));
				return 0;
			}
			goto __again;
		}
	}
	mutex_unlock(&ensoniq->src_mutex);
	snd_printk(KERN_ERR "es1371: codec read timeout at 0x%lx [0x%x]\n",
		   ES_REG(ensoniq, 1371_CODEC), inl(ES_REG(ensoniq, 1371_CODEC)));
	return 0;
}

static void snd_es1371_codec_wait(struct snd_ac97 *ac97)
{
	msleep(750);
	snd_es1371_codec_read(ac97, AC97_RESET);
	snd_es1371_codec_read(ac97, AC97_VENDOR_ID1);
	snd_es1371_codec_read(ac97, AC97_VENDOR_ID2);
	msleep(50);
}

static void snd_es1371_adc_rate(struct ensoniq * ensoniq, unsigned int rate)
{
	unsigned int n, truncm, freq, result;

	mutex_lock(&ensoniq->src_mutex);
	n = rate / 3000;
	if ((1 << n) & ((1 << 15) | (1 << 13) | (1 << 11) | (1 << 9)))
		n--;
	truncm = (21 * n - 1) | 1;
	freq = ((48000UL << 15) / rate) * n;
	result = (48000UL << 15) / (freq / n);
	if (rate >= 24000) {
		if (truncm > 239)
			truncm = 239;
		snd_es1371_src_write(ensoniq, ES_SMPREG_ADC + ES_SMPREG_TRUNC_N,
				(((239 - truncm) >> 1) << 9) | (n << 4));
	} else {
		if (truncm > 119)
			truncm = 119;
		snd_es1371_src_write(ensoniq, ES_SMPREG_ADC + ES_SMPREG_TRUNC_N,
				0x8000 | (((119 - truncm) >> 1) << 9) | (n << 4));
	}
	snd_es1371_src_write(ensoniq, ES_SMPREG_ADC + ES_SMPREG_INT_REGS,
			     (snd_es1371_src_read(ensoniq, ES_SMPREG_ADC +
						  ES_SMPREG_INT_REGS) & 0x00ff) |
			     ((freq >> 5) & 0xfc00));
	snd_es1371_src_write(ensoniq, ES_SMPREG_ADC + ES_SMPREG_VFREQ_FRAC, freq & 0x7fff);
	snd_es1371_src_write(ensoniq, ES_SMPREG_VOL_ADC, n << 8);
	snd_es1371_src_write(ensoniq, ES_SMPREG_VOL_ADC + 1, n << 8);
	mutex_unlock(&ensoniq->src_mutex);
}

static void snd_es1371_dac1_rate(struct ensoniq * ensoniq, unsigned int rate)
{
	unsigned int freq, r;

	mutex_lock(&ensoniq->src_mutex);
	freq = ((rate << 15) + 1500) / 3000;
	r = (snd_es1371_wait_src_ready(ensoniq) & (ES_1371_SRC_DISABLE |
						   ES_1371_DIS_P2 | ES_1371_DIS_R1)) |
		ES_1371_DIS_P1;
	outl(r, ES_REG(ensoniq, 1371_SMPRATE));
	snd_es1371_src_write(ensoniq, ES_SMPREG_DAC1 + ES_SMPREG_INT_REGS,
			     (snd_es1371_src_read(ensoniq, ES_SMPREG_D/*
 * osd_initiator - Main body of the osd initiator library.
 *
 * Note: The file does not contain the advanced security functionality which
 * is only needed by the security_manager's initiators.
 *
 * Copyright (C) 2008 Panasas Inc.  All rights reserved.
 *
 * Authors:
 *   Boaz Harrosh <bharrosh@panasas.com>
 *   Benny Halevy <bhalevy@panasas.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the Panasas company nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <scsi/osd_initiator.h>
#include <scsi/osd_sec.h>
#include <scsi/osd_attributes.h>
#include <scsi/osd_sense.h>

#include <scsi/scsi_device.h>

#include "osd_debug.h"

#ifndef __unused
#    define __unused			__attribute__((unused))
#endif

enum { OSD_REQ_RETRIES = 1 };

MODULE_AUTHOR("Boaz Harrosh <bharrosh@panasas.com>");
MODULE_DESCRIPTION("open-osd initiator library libosd.ko");
MODULE_LICENSE("GPL");

static inline void build_test(void)
{
	/* structures were not packed */
	BUILD_BUG_ON(sizeof(struct osd_capability) != OSD_CAP_LEN);
	BUILD_BUG_ON(sizeof(struct osdv2_cdb) != OSD_TOTAL_CDB_LEN);
	BUILD_BUG_ON(sizeof(struct osdv1_cdb) != OSDv1_TOTAL_CDB_LEN);
}

static const char *_osd_ver_desc(struct osd_request *or)
{
	return osd_req_is_ver1(or) ? "OSD1" : "OSD2";
}

#define ATTR_DEF_RI(id, len) ATTR_DEF(OSD_APAGE_ROOT_INFORMATION, id, len)

static int _osd_print_system_info(struct osd_dev *od, void *caps)
{
	struct osd_request *or;
	struct osd_attr get_attrs[] = {
		ATTR_DEF_RI(OSD_ATTR_RI_VENDOR_IDENTIFICATION, 8),
		ATTR_DEF_RI(OSD_ATTR_RI_PRODUCT_IDENTIFICATION, 16),
		ATTR_DEF_RI(OSD_ATTR_RI_PRODUCT_MODEL, 32),
		ATTR_DEF_RI(OSD_ATTR_RI_PRODUCT_REVISION_LEVEL, 4),
		ATTR_DEF_RI(OSD_ATTR_RI_PRODUCT_SERIAL_NUMBER, 64 /*variable*/),
		ATTR_DEF_RI(OSD_ATTR_RI_OSD_NAME, 64 /*variable*/),
		ATTR_DEF_RI(OSD_ATTR_RI_TOTAL_CAPACITY, 8),
		ATTR_DEF_RI(OSD_ATTR_RI_USED_CAPACITY, 8),
		ATTR_DEF_RI(OSD_ATTR_RI_NUMBER_OF_PARTITIONS, 8),
		ATTR_DEF_RI(OSD_ATTR_RI_CLOCK, 6),
		/* IBM-OSD-SIM Has a bug with this one put it last */
		ATTR_DEF_RI(OSD_ATTR_RI_OSD_SYSTEM_ID, 20),
	};
	void *iter = NULL, *pFirst;
	int nelem = ARRAY_SIZE(get_attrs), a = 0;
	int ret;

	or = osd_start_request(od, GFP_KERNEL);
	if (!or)
		return -ENOMEM;

	/* get attrs */
	osd_req_get_attributes(or, &osd_root_object);
	osd_req_add_get_attr_list(or, get_attrs, ARRAY_SIZE(get_attrs));

	ret = osd_finalize_request(or, 0, caps, NULL);
	if (ret)
		goto out;

	ret = osd_execute_request(or);
	if (ret) {
		OSD_ERR("Failed to detect %s => %d\n", _osd_ver_desc(or), ret);
		goto out;
	}

	osd_req_decode_get_attr_list(or, get_attrs, &nelem, &iter);

	OSD_INFO("Detected %s device\n",
		_osd_ver_desc(or));

	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("VENDOR_IDENTIFICATION  [%s]\n",
		(char *)pFirst);

	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("PRODUCT_IDENTIFICATION [%s]\n",
		(char *)pFirst);

	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("PRODUCT_MODEL          [%s]\n",
		(char *)pFirst);

	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("PRODUCT_REVISION_LEVEL [%u]\n",
		pFirst ? get_unaligned_be32(pFirst) : ~0U);

	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("PRODUCT_SERIAL_NUMBER  [%s]\n",
		(char *)pFirst);

	pFirst = get_attrs[a].val_ptr;
	OSD_INFO("OSD_NAME               [%s]\n", (char *)pFirst);
	a++;

	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("TOTAL_CAPACITY         [0x%llx]\n",
		pFirst ? _LLU(get_unaligned_be64(pFirst)) : ~0ULL);

	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("USED_CAPACITY          [0x%llx]\n",
		pFirst ? _LLU(get_unaligned_be64(pFirst)) : ~0ULL);

	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("NUMBER_OF_PARTITIONS   [%llu]\n",
		pFirst ? _LLU(get_unaligned_be64(pFirst)) : ~0ULL);

	if (a >= nelem)
		goto out;

	/* FIXME: Where are the time utilities */
	pFirst = get_attrs[a++].val_ptr;
	OSD_INFO("CLOCK                  [0x%02x%02x%02x%02x%02x%02x]\n",
		((char *)pFirst)[0], ((char *)pFirst)[1],
		((char *)pFirst)[2], ((char *)pFirst)[3],
		((char *)pFirst)[4], ((char *)pFirst)[5]);

	if (a < nelem) { /* IBM-OSD-SIM bug, Might not have it */
		unsigned len = get_attrs[a].len;
		char sid_dump[32*4 + 2]; /* 2nibbles+space+ASCII */

		hex_dump_to_buffer(get_attrs[a].val_ptr, len, 32, 1,
				   sid_dump, sizeof(sid_dump), true);
		OSD_INFO("OSD_SYSTEM_ID(%d)\n"
			 "        [%s]\n", len, sid_dump);
		a++;
	}
out:
	osd_end_request(or);
	return ret;
}

int osd_auto_detect_ver(struct osd_dev *od, void *caps)
{
	int ret;

	/* Auto-detect the osd version */
	ret = _osd_print_system_info(od, caps);
	if (ret) {
		osd_dev_set_ver(od, OSD_VER1);
		OSD_DEBUG("converting to OSD1\n");
		ret = _osd_print_system_info(od, caps);
	}

	return ret;
}
EXPORT_SYMBOL(osd_auto_detect_ver);

static unsigned _osd_req_cdb_len(struct osd_request *or)
{
	return osd_req_is_ver1(or) ? OSDv1_TOTAL_CDB_LEN : OSD_TOTAL_CDB_LEN;
}

static unsigned _osd_req_alist_elem_size(struct osd_request *or, unsigned len)
{
	return osd_req_is_ver1(or) ?
		osdv1_attr_list_elem_size(len) :
		osdv2_attr_list_elem_size(len);
}

static void _osd_req_alist_elem_encode(struct osd_request *or,
	void *attr_last, const struct osd_attr *oa)
{
	if (osd_req_is_ver1(or)) {
		struct osdv1_attributes_list_element *attr = attr_last;

		attr->attr_page = cpu_to_be32(oa->attr_page);
		attr->attr_id = cpu_to_be32(oa->attr_id);
		attr->attr_bytes = cpu_to_be16(oa->len);
		memcpy(attr->attr_val, oa->val_ptr, oa->len);
	} else {
		struct osdv2_attributes_list_element *attr = attr_last;

		attr->attr_page = cpu_to_be32(oa->attr_page);
		attr->attr_id = cpu_to_be32(oa->attr_id);
		attr->attr_bytes = cpu_to_be16(oa->len);
		memcpy(attr->attr_val, oa->val_ptr, oa->len);
	}
}

static int _osd_req_alist_elem_decode(struct osd_request *or,
	void *cur_p, struct osd_attr *oa, unsigned max_bytes)
{
	unsigned inc;
	if (osd_req_is_ver1(or)) {
		struct osdv1_attributes_list_element *attr = cur_p;

		if (max_bytes < sizeof(*attr))
			return -1;

		oa->len = be16_to_cpu(attr->attr_bytes);
		inc = _osd_req_alist_elem_size(or, oa->len);
		if (inc > max_bytes)
			return -1;

		oa->attr_page = be32_to_cpu(attr->attr_page);
		oa->attr_id = be32_to_cpu(attr->attr_id);

		/* OSD1: On empty attributes we return a pointer to 2 bytes
		 * of zeros. This keeps similar behaviour with OSD2.
		 * (See below)
		 */
		oa->val_ptr = likely(oa->len) ? attr->attr_val :
						(u8 *)&attr->attr_bytes;
	} else {
		struct osdv2_attributes_list_element *attr = cur_p;

		if (max_bytes < sizeof(*attr))
			return -1;

		oa->len = be16_to_cpu(attr->attr_bytes);
		inc = _osd_req_alist_elem_size(or, oa->len);
		if (inc > max_bytes)
			return -1;

		oa->attr_page = be32_to_cpu(attr->attr_page);
		oa->attr_id = be32_to_cpu(attr->attr_id);

		/* OSD2: For convenience, on empty attributes, we return 8 bytes
		 * of zeros here. This keeps the same behaviour with OSD2r04,
		 * and is nice with null terminating ASCII fields.
		 * oa->val_ptr == NULL marks the end-of-list, or error.
		 */
		oa->val_ptr = likely(oa->len) ? attr->attr_val : attr->reserved;
	}
	return inc;
}

static unsigned _osd_req_alist_size(struct osd_request *or, void *list_head)
{
	return osd_req_is_ver1(or) ?
		osdv1_list_size(list_head) :
		osdv2_list_size(list_head);
}

static unsigned _osd_req_sizeof_alist_header(struct osd_request *or)
{
	return osd_req_is_ver1(or) ?
		sizeof(struct osdv1_attributes_list_header) :
		sizeof(struct osdv2_attributes_list_header);
}

static void _osd_req_set_alist_type(struct osd_request *or,
	void *list, int list_type)
{
	if (osd_req_is_ver1(or)) {
		struct osdv1_attributes_list_header *attr_list = list;

		memset(attr_list, 0, sizeof(*attr_list));
		attr_list->type = list_type;
	} else {
		struct osdv2_attributes_list_header *attr_list = list;

		memset(attr_list, 0, sizeof(*attr_list));
		attr_list->type = list_type;
	}
}

static bool _osd_req_is_alist_type(struct osd_request *or,
	void *list, int list_type)
{
	if (!list)
		return false;

	if (osd_req_is_ver1(or)) {
		struct osdv1_attributes_list_header *attr_list = list;

		return attr_list->type == list_type;
	} else {
		struct osdv2_attributes_list_header *attr_list = list;

		return attr_list->type == list_type;
	}
}

/* This is for List-objects not Attributes-Lists */
static void _osd_req_encode_olist(struct osd_request *or,
	struct osd_obj_id_list *list)
{
	struct osd_cdb_head *cdbh = osd_cdb_head(&or->cdb);

	if (osd_req_is_ver1(or)) {
		cdbh->v1.list_identifier = list->list_identifier;
		cdbh->v1.start_address = list->continuation_id;
	} else {
		cdbh->v2.list_identifier = list->list_identifier;
		cdbh->v2.start_address = list->continuation_id;
	}
}

static osd_cdb_offset osd_req_encode_offset(struct osd_request *or,
	u64 offset, unsigned *padding)
{
	return __osd_encode_offset(offset, padding,
			osd_req_is_ver1(or) ?
				OSDv1_OFFSET_MIN_SHIFT : OSD_OFFSET_MIN_SHIFT,
			OSD_OFFSET_MAX_SHIFT);
}

static struct osd_security_parameters *
_osd_req_sec_params(struct osd_request *or)
{
	struct osd_cdb *ocdb = &or->cdb;

	if (osd_req_is_ver1(or))
		return (struct osd_security_parameters *)&ocdb->v1.sec_params;
	else
		return (struct osd_security_parameters *)&ocdb->v2.sec_params;
}

void osd_dev_init(struct osd_dev *osdd, struct scsi_device *scsi_device)
{
	memset(osdd, 0, sizeof(*osdd));
	osdd->scsi_device = scsi_device;
	osdd->def_timeout = BLK_DEFAULT_SG_TIMEOUT;
#ifdef OSD_VER1_SUPPORT
	osdd->version = OSD_VER2;
#endif
	/* TODO: Allocate pools for osd_request attributes ... */
}
EXPORT_SYMBOL(osd_dev_init);

void osd_dev_fini(struct osd_dev *osdd)
{
	/* TODO: De-allocate pools */

	osdd->scsi_device = NULL;
}
EXPORT_SYMBOL(osd_dev_fini);

static struct osd_request *_osd_request_alloc(gfp_t gfp)
{
	struct osd_request *or;

	/* TODO: Use mempool with one saved request */
	or = kzalloc(sizeof(*or), gfp);
	return or;
}

static void _osd_request_free(struct osd_request *or)
{
	kfree(or);
}

struct osd_request *osd_start_request(struct osd_dev *dev, gfp_t gfp)
{
	struct osd_request *or;

	or = _osd_request_alloc(gfp);
	if (!or)
		return NULL;

	or->osd_dev = dev;
	or->alloc_flags = gfp;
	or->timeout = dev->def_timeout;
	or->retries = OSD_REQ_RETRIES;

	return or;
}
EXPORT_SYMBOL(osd_start_request);

static void _osd_free_seg(struct osd_request *or __unused,
	struct _osd_req_data_segment *seg)
{
	if (!seg->buff || !seg->alloc_size)
		return;

	kfree(seg->buff);
	seg->buff = NULL;
	seg->alloc_size = 0;
}

static void _put_request(struct request *rq , bool is_async)
{
	if (is_async) {
		WARN_ON(rq->bio);
		__blk_put_request(rq->q, rq);
	} else {
		/*
		 * If osd_finalize_request() was called but the request was not
		 * executed through the block layer, then we must release BIOs.
		 * TODO: Keep error code in or->async_error. Need to audit all
		 *       code paths.
		 */
		if (unlikely(rq->bio))
			blk_end_request(rq, -ENOMEM, blk_rq_bytes(rq));
		else
			blk_put_request(rq);
	}
}

void osd_end_request(struct osd_request *or)
{
	struct request *rq = or->request;
	/* IMPORTANT: make sure this agrees with osd_execute_request_async */
	bool is_async = (or->request->end_io_data == or);

	_osd_free_seg(or, &or->set_attr);
	_osd_free_seg(or, &or->enc_get_attr);
	_osd_free_seg(or, &or->get_attr);

	if (rq) {
		if (rq->next_rq) {
			_put_request(rq->next_rq, is_async);
			rq->next_rq = NULL;
		}

		_put_request(rq, is_async);
	}
	_osd_request_free(or);
}
EXPORT_SYMBOL(osd_end_request);

int osd_execute_request(struct osd_request *or)
{
	return blk_execute_rq(or->request->q, NULL, or->request, 0);
}
EXPORT_SYMBOL(osd_execute_request);

static void osd_request_async_done(struct request *req, int error)
{
	struct osd_request *or = req->end_io_data;

	or->async_error = error;

	if (error)
		OSD_DEBUG("osd_request_async_done error recieved %d\n", error);

	if (or->async_done)
		or->async_done(or, or->async_private);
	else
		osd_end_request(or);
}

int osd_execute_request_async(struct osd_request *or,
	osd_req_done_fn *done, void *private)
{
	or->request->end_io_data = or;
	or->async_private = private;
	or->async_done = done;

	blk_execute_rq_nowait(or->request->q, NULL, or->request, 0,
			      osd_request_async_done);
	return 0;
}
EXPORT_SYMBOL(osd_execute_request_async);

u8 sg_out_pad_buffer[1 << OSDv1_OFFSET_MIN_SHIFT];
u8 sg_in_pad_buffer[1 << OSDv1_OFFSET_MIN_SHIFT];

static int _osd_realloc_seg(struct osd_request *or,
	struct _osd_req_data_segment *seg, unsigned max_bytes)
{
	void *buff;

	if (seg->alloc_size >= max_bytes)
		return 0;

	buff = krealloc(seg->buff, max_bytes, or->alloc_flags);
	if (!buff) {
		OSD_ERR("Failed to Realloc %d-bytes was-%d\n", max_bytes,
			seg->alloc_size);
		return -ENOMEM;
	}

	memset(buff + seg->alloc_size, 0, max_bytes - seg->alloc_size);
	seg->buff = buff;
	seg->alloc_size = max_bytes;
	return 0;
}

static int _alloc_set_attr_list(struct osd_request *or,
	const struct osd_attr *oa, unsigned nelem, unsigned add_bytes)
{
	unsigned total_bytes = add_bytes;

	for (; nelem; --nelem, ++oa)
		total_bytes += _osd_req_alist_elem_size(or, oa->len);

	OSD_DEBUG("total_bytes=%d\n", total_bytes);
	return _osd_realloc_seg(or, &or->set_attr, total_bytes);
}

static int _alloc_get_attr_desc(struct osd_request *or, unsigned max_bytes)
{
	OSD_DEBUG("total_bytes=%d\n", max_bytes);
	return _osd_realloc_seg(or, &or->enc_get_attr, max_bytes);
}

static int _alloc_get_attr_list(struct osd_request *or)
{
	OSD_DEBUG("total_bytes=%d\n", or->get_attr.total_bytes);
	return _osd_realloc_seg(or, &or->get_attr, or->get_attr.total_bytes);
}

/*
 * Common to all OSD commands
 */

static void _osdv1_req_encode_common(struct osd_request *or,
	__be16 act, const struct osd_obj_id *obj, u64 offset, u64 len)
{
	struct osdv1_cdb *ocdb = &or->cdb.v1;

	/*
	 * For speed, the commands
	 *	OSD_ACT_PERFORM_SCSI_COMMAND	, V1 0x8F7E, V2 0x8F7C
	 *	OSD_ACT_SCSI_TASK_MANAGEMENT	, V1 0x8F7F, V2 0x8F7D
	 * are not supported here. Should pass zero and set after the call
	 */
	act &= cpu_to_be16(~0x0080); /* V1 action code */

	OSD_DEBUG("OSDv1 execute opcode 0x%x\n", be16_to_cpu(act));

	ocdb->h.varlen_cdb.opcode = VARIABLE_LENGTH_CMD;
	ocdb->h.varlen_cdb.additional_cdb_length = OSD_ADDITIONAL_CDB_LENGTH;
	ocdb->h.varlen_cdb.service_action = act;

	ocdb->h.partition = cpu_to_be64(obj->partition);
	ocdb->h.object = cpu_to_be64(obj->id);
	ocdb->h.v1.length = cpu_to_be64(len);
	ocdb->h.v1.start_address = cpu_to_be64(offset);
}

static void _osdv2_req_encode_common(struct osd_request *or,
	 __be16 act, const struct osd_obj_id *obj, u64 offset, u64 len)
{
	struct osdv2_cdb *ocdb = &or->cdb.v2;

	OSD_DEBUG("OSDv2 execute opcode 0x%x\n", be16_to_cpu(act));

	ocdb->h.varlen_cdb.opcode = VARIABLE_LENGTH_CMD;
	ocdb->h.varlen_cdb.additional_cdb_length = OSD_ADDITIONAL_CDB_LENGTH;
	ocdb->h.varlen_cdb.service_action = act;

	ocdb->h.partition = cpu_to_be64(obj->partition);
	ocdb->h.object = cpu_to_be64(obj->id);
	ocdb->h.v2.length = cpu_to_be64(len);
	ocdb->h.v2.start_address = cpu_to_be64(offset);
}

static void _osd_req_encode_common(struct osd_request *or,
	__be16 act, const struct osd_obj_id *obj, u64 offset, u64 len)
{
	if (osd_req_is_ver1(or))
		_osdv1_req_encode_common(or, act, obj, offset, len);
	else
		_osdv2_req_encode_common(or, act, obj, offset, len);
}

/*
 * Device commands
 */
/*TODO: void osd_req_set_master_seed_xchg(struct osd_request *, ...); */
/*TODO: void osd_req_set_master_key(struct osd_request *, ...); */

void osd_req_format(struct osd_request *or, u64 tot_capacity)
{
	_osd_req_encode_common(or, OSD_ACT_FORMAT_OSD, &osd_root_object, 0,
				tot_capacity);
}
EXPORT_SYMBOL(osd_req_format);

int osd_req_list_dev_partitions(struct osd_request *or,
	osd_id initial_id, struct osd_obj_id_list *list, unsigned nelem)
{
	return osd_req_list_partition_objects(or, 0, initial_id, list, nelem);
}
EXPORT_SYMBOL(osd_req_list_dev_partitions);

static void _osd_req_encode_flush(struct osd_request *or,
	enum osd_options_flush_scope_values op)
{
	struct osd_cdb_head *ocdb = osd_cdb_head(&or->cdb);

	ocdb->command_specific_options = op;
}

void osd_req_flush_obsd(struct osd_request *or,
	enum osd_options_flush_scope_values op)
{
	_osd_req_encode_common(or, OSD_ACT_FLUSH_OSD, &osd_root_object, 0, 0);
	_osd_req_encode_flush(or, op);
}
EXPORT_SYMBOL(osd_req_flush_obsd);

/*TODO: void osd_req_perform_scsi_command(struct osd_request *,
	const u8 *cdb, ...); */
/*TODO: void osd_req_task_management(struct osd_request *, ...); */

/*
 * Partition commands
 */
static void _osd_req_encode_partition(struct osd_request *or,
	__be16 act, osd_id partition)
{
	struct osd_obj_id par = {
		.partition = partition,
		.id = 0,
	};

	_osd_req_encode_common(or, act, &par, 0, 0);
}

void osd_req_create_partition(struct osd_request *or, osd_id partition)
{
	_osd_req_encode_partition(or, OSD_ACT_CREATE_PARTITION, partition);
}
EXPORT_SYMBOL(osd_req_create_partition);

void osd_req_remove_partition(struct osd_request *or, osd_id partition)
{
	_osd_req_encode_partition(or, OSD_ACT_REMOVE_PARTITION, partition);
}
EXPORT_SYMBOL(osd_req_remove_partition);

/*TODO: void osd_req_set_partition_key(struct osd_request *,
	osd_id partition, u8 new_key_id[OSD_CRYPTO_KEYID_SIZE],
	u8 seed[OSD_CRYPTO_SEED_SIZE]); */

static int _osd_req_list_objects(struct osd_request *or,
	__be16 action, const struct osd_obj_id *obj, osd_id initial_id,
	struct osd_obj_id_list *list, unsigned nelem)
{
	struct request_queue *q = osd_request_queue(or->osd_dev);
	u64 len = nelem * sizeof(osd_id) + sizeof(*list);
	struct bio *bio;

	_osd_req_encode_common(or, action, obj, (u64)initial_id, len);

	if (list->list_identifier)
		_osd_req_encode_olist(or, list);

	WARN_ON(or->in.bio);
	bio = bio_map_kern(q, list, len, or->alloc_flags);
	if (IS_ERR(bio)) {
		OSD_ERR("!!! Failed to allocate list_objects BIO\n");
		return PTR_ERR(bio);
	}

	bio->bi_rw &= ~(1 << BIO_RW);
	or->in.bio = bio;
	or->in.total_bytes = bio->bi_size;
	return 0;
}

int osd_req_list_partition_collections(struct osd_request *or,
	osd_id partition, osd_id initial_id, struct osd_obj_id_list *list,
	unsigned nelem)
{
	struct osd_obj_id par = {
		.partition = partition,
		.id = 0,
	};

	return osd_req_list_collection_objects(or, &par, initial_id, list,
					       nelem);
}
EXPORT_SYMBOL(osd_req_list_partition_collections);

int osd_req_list_partition_objects(struct osd_request *or,
	osd_id partition, osd_id initial_id, struct osd_obj_id_list *list,
	unsigned nelem)
{
	struct osd_obj_id par = {
		.partition = partition,
		.id = 0,
	};

	return _osd_req_list_objects(or, OSD_ACT_LIST, &par, initial_id, list,
				     nelem);
}
EXPORT_SYMBOL(osd_req_list_partition_objects);

void osd_req_flush_partition(struct osd_request *or,
	osd_id partition, enum osd_options_flush_scope_values op)
{
	_osd_req_encode_partition(or, OSD_ACT_FLUSH_PARTITION, partition);
	_osd_req_encode_flush(or, op);
}
EXPORT_SYMBOL(osd_req_flush_partition);

/*
 * Collection commands
 */
/*TODO: void osd_req_create_collection(struct osd_request *,
	const struct osd_obj_id *); */
/*TODO: void osd_req_remove_collection(struct osd_request *,
	const struct osd_obj_id *); */

int osd_req_list_collection_objects(struct osd_request *or,
	const struct osd_obj_id *obj, osd_id initial_id,
	struct osd_obj_id_list *list, unsigned nelem)
{
	return _osd_req_list_objects(or, OSD_ACT_LIST_COLLECTION, obj,
				     initial_id, list, nelem);
}
EXPORT_SYMBOL(osd_req_list_collection_objects);

/*TODO: void query(struct osd_request *, ...); V2 */

void osd_req_flush_collection(struct osd_request *or,
	const struct osd_obj_id *obj, enum osd_options_flush_scope_values op)
{
	_osd_req_encode_common(or, OSD_ACT_FLUSH_PARTITION, obj, 0, 0);
	_osd_req_encode_flush(or, op);
}
EXPORT_SYMBOL(osd_req_flush_collection);

/*TODO: void get_member_attrs(struct osd_request *, ...); V2 */
/*TODO: void set_member_attrs(struct osd_request *, ...); V2 */

/*
 * Object commands
 */
void osd_req_create_object(struct osd_request *or, struct osd_obj_id *obj)
{
	_osd_req_encode_common(or, OSD_ACT_CREATE, obj, 0, 0);
}
EXPORT_SYMBOL(osd_req_create_object);

void osd_req_remove_object(struct osd_request *or, struct osd_obj_id *obj)
{
	_osd_req_encode_common(or, OSD_ACT_REMOVE, obj, 0, 0);
}
EXPORT_SYMBOL(osd_req_remove_object);


/*TODO: void osd_req_create_multi(struct osd_request *or,
	struct osd_obj_id *first, struct osd_obj_id_list *list, unsigned nelem);
*/

void osd_req_write(struct osd_request *or,
	const struct osd_obj_id *obj, u64 offset,
	struct bio *bio, u64 len)
{
	_osd_req_encode_common(or, OSD_ACT_WRITE, obj, offset, len);
	WARN_ON(or->out.bio || or->out.total_bytes);
	WARN_ON(0 ==  bio_rw_flagged(bio, BIO_RW));
	or->out.bio = bio;
	or->out.total_bytes = len;
}
EXPORT_SYMBOL(osd_req_write);

int osd_req_write_kern(struct osd_request *or,
	const struct osd_obj_id *obj, u64 offset, void* buff, u64 len)
{
	struct request_queue *req_q = osd_request_queue(or->osd_dev);
	struct bio *bio = bio_map_kern(req_q, buff, len, GFP_KERNEL);

	if (IS_ERR(bio))
		return PTR_ERR(bio);

	bio->bi_rw |= (1 << BIO_RW); /* FIXME: bio_set_dir() */
	osd_req_write(or, obj, offset, bio, len);
	return 0;
}
EXPORT_SYMBOL(osd_req_write_kern);

/*TODO: void osd_req_append(struct osd_request *,
	const struct osd_obj_id *, struct bio *data_out); */
/*TODO: void osd_req_create_write(struct osd_request *,
	const struct osd_obj_id *, struct bio *data_out, u64 offset); */
/*TODO: void osd_req_clear(struct osd_request *,
	const struct osd_obj_id *, u64 offset, u64 len); */
/*TODO: void osd_req_punch(struct osd_request *,
	const struct osd_obj_id *, u64 offset, u64 len); V2 */

void osd_req_flush_object(struct osd_request *or,
	const struct osd_obj_id *obj, enum osd_options_flush_scope_values op,
	/*V2*/ u64 offset, /*V2*/ u64 len)
{
	if (unlikely(osd_req_is_ver1(or) && (offset || len))) {
		OSD_DEBUG("OSD Ver1 flush on specific range ignored\n");
		offset = 0;
		len = 0;
	}

	_osd_req_encode_common(or, OSD_ACT_FLUSH, obj, offset, len);
	_osd_req_encode_flush(or, op);
}
EXPORT_SYMBOL(osd_req_flush_object);

void osd_req_read(struct osd_request *or,
	const struct osd_obj_id *obj, u64 offset,
	struct bio *bio, u64 len)
{
	_osd_req_encode_common(or, OSD_ACT_READ, obj, offset, len);
	WARN_ON(or->in.bio || or->in.total_bytes);
	WARN_ON(1 == bio_rw_flagged(bio, BIO_RW));
	or->in.bio = bio;
	or->in.total_bytes = len;
}
EXPORT_SYMBOL(osd_req_read);

int osd_req_read_kern(struct osd_request *or,
	const struct osd_obj_id *obj, u64 offset, void* buff, u64 len)
{
	struct request_queue *req_q = osd_request_queue(or->osd_dev);
	struct bio *bio = bio_map_kern(req_q, buff, len, GFP_KERNEL);

	if (IS_ERR(bio))
		return PTR_ERR(bio);

	osd_req_read(or, obj, offset, bio, len);
	return 0;
}
EXPORT_SYMBOL(osd_req_read_kern);

void osd_req_get_attributes(struct osd_request *or,
	const struct osd_obj_id *obj)
{
	_osd_req_encode_common(or, OSD_ACT_GET_ATTRIBUTES, obj, 0, 0);
}
EXPORT_SYMBOL(osd_req_get_attributes);

void osd_req_set_attributes(struct osd_request *or,
	const struct osd_obj_id *obj)
{
	_osd_req_encode_common(or, OSD_ACT_SET_ATTRIBUTES, obj, 0, 0);
}
EXPORT_SYMBOL(osd_req_set_attributes);

/*
 * Attributes List-mode
 */

int osd_req_add_set_attr_list(struct osd_request *or,
	const struct osd_attr *oa, unsigned nelem)
{
	unsigned total_bytes = or->set_attr.total_bytes;
	void *attr_last;
	int ret;

	if (or->attributes_mode &&
	    or->attributes_mode != OSD_CDB_GET_SET_ATTR_LISTS) {
		WARN_ON(1);
		return -EINVAL;
	}
	or->attributes_mode = OSD_CDB_GET_SET_ATTR_LISTS;

	if (!total_bytes) { /* first-time: allocate and put list header */
		total_bytes = _osd_req_sizeof_alist_header(or);
		ret = _alloc_set_attr_list(or, oa, nelem, total_bytes);
		if (ret)
			return ret;
		_osd_req_set_alist_type(or, or->set_attr.buff,
					OSD_ATTR_LIST_SET_RETRIEVE);
	}
	attr_last = or->set_attr.buff + total_bytes;

	for (; nelem; --nelem) {
		unsigned elem_size = _osd_req_alist_elem_size(or, oa->len);

		total_bytes += elem_size;
		if (unlikely(or->set_attr.alloc_size < total_bytes)) {
			or->set_attr.total_bytes = total_bytes - elem_size;
			ret = _alloc_set_attr_list(or, oa, nelem, total_bytes);
			if (ret)
				return ret;
			attr_last =
				or->set_attr.buff + or->set_attr.total_bytes;
		}

		_osd_req_alist_elem_encode(or, attr_last, oa);

		attr_last += elem_size;
		++oa;
	}

	or->set_attr.total_bytes = total_bytes;
	return 0;
}
EXPORT_SYMBOL(osd_req_add_set_attr_list);

static int _req_append_segment(struct osd_request *or,
	unsigned padding, struct _osd_req_data_segment *seg,
	struct _osd_req_data_segment *last_seg, struct _osd_io_info *io)
{
	void *pad_buff;
	int ret;

	if (padding) {
		/* check if we can just add it to last buffer */
		if (last_seg &&
		    (padding <= last_seg->alloc_size - last_seg->total_bytes))
			pad_buff = last_seg->buff + last_seg->total_bytes;
		else
			pad_buff = io->pad_buff;

		ret = blk_rq_map_kern(io->req->q, io->req, pad_buff, padding,
				       or->alloc_flags);
		if (ret)
			return ret;
		io->total_bytes += padding;
	}

	ret = blk_rq_map_kern(io->req->q, io->req, seg->buff, seg->total_bytes,
			       or->alloc_flags);
	if (ret)
		return ret;

	io->total_bytes += seg->total_bytes;
	OSD_DEBUG("padding=%d buff=%p total_bytes=%d\n", padding, seg->buff,
		  seg->total_bytes);
	return 0;
}

static int _osd_req_finalize_set_attr_list(struct osd_request *or)
{
	struct osd_cdb_head *cdbh = osd_cdb_head(&or->cdb);
	unsigned padding;
	int ret;

	if (!or->set_attr.total_bytes) {
		cdbh->attrs_list.set_attr_offset = OSD_OFFSET_UNUSED;
		return 0;
	}

	cdbh->attrs_list.set_attr_bytes = cpu_to_be32(or->set_attr.total_bytes);
	cdbh->attrs_list.set_attr_offset =
		osd_req_encode_offset(or, or->out.total_bytes, &padding);

	ret = _req_append_segment(or, padding, &or->set_attr,
				  or->out.last_seg, &or->out);
	if (ret)
		return ret;

	or->out.last_seg = &or->set_attr;
	return 0;
}

int osd_req_add_get_attr_list(struct osd_request *or,
	const struct osd_attr *oa, unsigned nelem)
{
	unsigned total_bytes = or->enc_get_attr.total_bytes;
	void *attr_last;
	int ret;

	if (or->attributes_mode &&
	    or->attributes_mode != OSD_CDB_GET_SET_ATTR_LISTS) {
		WARN_ON(1);
		return -EINVAL;
	}
	or->attributes_mode = OSD_CDB_GET_SET_ATTR_LISTS;

	/* first time calc data-in list header size */
	if (!or->get_attr.total_bytes)
		or->get_attr.total_bytes = _osd_req_sizeof_alist_header(or);

	/* calc data-out info */
	if (!total_bytes) { /* first-time: allocate and put list header */
		unsigned max_bytes;

		total_bytes = _osd_req_sizeof_alist_header(or);
		max_bytes = total_bytes +
			nelem * sizeof(struct osd_attributes_list_attrid);
		ret = _alloc_get_attr_desc(or, max_bytes);
		if (ret)
			return ret;

		_osd_req_set_alist_type(or, or->enc_get_attr.buff,
					OSD_ATTR_LIST_GET);
	}
	attr_last = or->enc_get_attr.buff + total_bytes;

	for (; nelem; --nelem) {
		struct osd_attributes_list_attrid *attrid;
		const unsigned cur_size = sizeof(*attrid);

		total_bytes += cur_size;
		if (unlikely(or->enc_get_attr.alloc_size < total_bytes)) {
			or->enc_get_attr.total_bytes = total_bytes - cur_size;
			ret = _alloc_get_attr_desc(or,
					total_bytes + nelem * sizeof(*attrid));
			if (ret)
				return ret;
			attr_last = or->enc_get_attr.buff +
				or->enc_get_attr.total_bytes;
		}

		attrid = attr_last;
		attrid->attr_page = cpu_to_be32(oa->attr_page);
		attrid->attr_id = cpu_to_be32(oa->attr_id);

		attr_last += cur_size;

		/* calc data-in size */
		or->get_attr.total_bytes +=
			_osd_req_alist_elem_size(or, oa->len);
		++oa;
	}

	or->enc_get_attr.total_bytes = total_bytes;

	OSD_DEBUG(
	       "get_attr.total_bytes=%u(%u) enc_get_attr.total_bytes=%u(%Zu)\n",
	       or->get_attr.total_bytes,
	       or->get_attr.total_bytes - _osd_req_sizeof_alist_header(or),
	       or->enc_get_attr.total_bytes,
	       (or->enc_get_attr.total_bytes - _osd_req_sizeof_alist_header(or))
			/ sizeof(struct osd_attributes_list_attrid));

	return 0;
}
EXPORT_SYMBOL(osd_req_add_get_attr_list);

static int _osd_req_finalize_get_attr_list(struct osd_request *or)
{
	struct osd_cdb_head *cdbh = osd_cdb_head(&or->cdb);
	unsigned out_padding;
	unsigned in_padding;
	int ret;

	if (!or->enc_get_attr.total_bytes) {
		cdbh->attrs_list.get_attr_desc_offset = OSD_OFFSET_UNUSED;
		cdbh->attrs_list.get_attr_offset = OSD_OFFSET_UNUSED;
		return 0;
	}

	ret = _alloc_get_attr_list(or);
	if (ret)
		return ret;

	/* The out-going buffer info update */
	OSD_DEBUG("out-going\n");
	cdbh->attrs_list.get_attr_desc_bytes =
		cpu_to_be32(or->enc_get_attr.total_bytes);

	cdbh->attrs_list.get_attr_desc_offset =
		osd_req_encode_offset(or, or->out.total_bytes, &out_padding);

	ret = _req_append_segment(or, out_padding, &or->enc_get_attr,
				  or->out.last_seg, &or->out);
	if (ret)
		return ret;
	or->out.last_seg = &or->enc_get_attr;

	/* The incoming buffer info update */
	OSD_DEBUG("in-coming\n");
	cdbh->attrs_list.get_attr_alloc_length =
		cpu_to_be32(or->get_attr.total_bytes);

	cdbh->attrs_list.get_attr_offset =
		osd_req_encode_offset(or, or->in.total_bytes, &in_padding);

	ret = _req_append_segment(or, in_padding, &or->get_attr, NULL,
				  &or->in);
	if (ret)
		return ret;
	or->in.last_seg = &or->get_attr;

	return 0;
}

int osd_req_decode_get_attr_list(struct osd_request *or,
	struct osd_attr *oa, int *nelem, void **iterator)
{
	unsigned cur_bytes, returned_bytes;
	int n;
	const unsigned sizeof_attr_list = _osd_req_sizeof_alist_header(or);
	void *cur_p;

	if (!_osd_req_is_alist_type(or, or->get_attr.buff,
				    OSD_ATTR_LIST_SET_RETRIEVE)) {
		oa->attr_page = 0;
		oa->attr_id = 0;
		oa->val_ptr = NULL;
		oa->len = 0;
		*iterator = NULL;
		return 0;
	}

	if (*iterator) {
		BUG_ON((*iterator < or->get_attr.buff) ||
		     (or->get_attr.buff + or->get_attr.alloc_size < *iterator));
		cur_p = *iterator;
		cur_bytes = (*iterator - or->get_attr.buff) - sizeof_attr_list;
		returned_bytes = or->get_attr.total_bytes;
	} else { /* first time decode the list header */
		cur_bytes = sizeof_attr_list;
		returned_bytes = _osd_req_alist_size(or, or->get_attr.buff) +
					sizeof_attr_list;

		cur_p = or->get_attr.buff + sizeof_attr_list;

		if (returned_bytes > or->get_attr.alloc_size) {
			OSD_DEBUG("target report: space was not big enough! "
				  "Allocate=%u Needed=%u\n",
				  or->get_attr.alloc_size,
				  returned_bytes + sizeof_attr_list);

			returned_bytes =
				or->get_attr.alloc_size - sizeof_attr_list;
		}
		or->get_attr.total_bytes = returned_bytes;
	}

	for (n = 0; (n < *nelem) && (cur_bytes < returned_bytes); ++n) {
		int inc = _osd_req_alist_elem_decode(or, cur_p, oa,
						 returned_bytes - cur_bytes);

		if (inc < 0) {
			OSD_ERR("BAD FOOD from target. list not valid!"
				"c=%d r=%d n=%d\n",
				cur_bytes, returned_bytes, n);
			oa->val_ptr = NULL;
			break;
		}

		cur_bytes += inc;
		cur_p += inc;
		++oa;
	}

	*iterator = (returned_bytes - cur_bytes) ? cur_p : NULL;
	*nelem = n;
	return returned_bytes - cur_bytes;
}
EXPORT_SYMBOL(osd_req_decode_get_attr_list);

/*
 * Attributes Page-mode
 */

int osd_req_add_get_attr_page(struct osd_request *or,
	u32 page_id, void *attar_page, unsigned max_page_len,
	const struct osd_attr *set_one_attr)
{
	struct osd_cdb_head *cdbh = osd_cdb_head(&or->cdb);

	if (or->attributes_mode &&
	    or->attributes_mode != OSD_CDB_GET_ATTR_PAGE_SET_ONE) {
		WARN_ON(1);
		return -EINVAL;
	}
	or->attributes_mode = OSD_CDB_GET_ATTR_PAGE_SET_ONE;

	or->get_attr.buff = attar_page;
	or->get_attr.total_bytes = max_page_len;

	or->set_attr.buff = set_one_attr->val_ptr;
	or->set_attr.total_bytes = set_one_attr->len;

	cdbh->attrs_page.get_attr_page = cpu_to_be32(page_id);
	cdbh->attrs_page.get_attr_alloc_length = cpu_to_be32(max_page_len);
	/* ocdb->attrs_page.get_attr_offset; */

	cdbh->attrs_page.set_attr_page = cpu_to_be32(set_one_attr->attr_page);
	cdbh->attrs_page.set_attr_id = cpu_to_be32(set_one_attr->attr_id);
	cdbh->attrs_page.set_attr_length = cpu_to_be32(set_one_attr->len);
	/* ocdb->attrs_page.set_attr_offset; */
	return 0;
}
EXPORT_SYMBOL(osd_req_add_get_attr_page);

static int _osd_req_finalize_attr_page(struct osd_request *or)
{
	struct osd_cdb_head *cdbh = osd_cdb_head(&or->cdb);
	unsigned in_padding, out_padding;
	int ret;

	/* returned page */
	cdbh->attrs_page.get_attr_offset =
		osd_req_encode_offset(or, or->in.total_bytes, &in_padding);

	ret = _req_append_segment(or, in_padding, &or->get_attr, NULL,
				  &or->in);
	if (ret)
		return ret;

	/* set one value */
	cdbh->attrs_page.set_attr_offset =
		osd_req_encode_offset(or, or->out.total_bytes, &out_padding);

	ret = _req_append_segment(or, out_padding, &or->enc_get_attr, NULL,
				  &or->out);
	return ret;
}

static inline void osd_sec_parms_set_out_offset(bool is_v1,
	struct osd_security_parameters *sec_parms, osd_cdb_offset offset)
{
	if (is_v1)
		sec_parms->v1.data_out_integrity_check_offset = offset;
	else
		sec_parms->v2.data_out_integrity_check_offset = offset;
}

static inline void osd_sec_parms_set_in_offset(bool is_v1,
	struct osd_security_parameters *sec_parms, osd_cdb_offset offset)
{
	if (is_v1)
		sec_parms->v1.data_in_integrity_check_offset = offset;
	else
		sec_parms->v2.data_in_integrity_check_offset = offset;
}

static int _osd_req_finalize_data_integrity(struct osd_request *or,
	bool has_in, bool has_out, u64 out_data_bytes, const u8 *cap_key)
{
	struct osd_security_parameters *sec_parms = _osd_req_sec_params(or);
	int ret;

	if (!osd_is_sec_alldata(sec_parms))
		return 0;

	if (has_out) {
		struct _osd_req_data_segment seg = {
			.buff = &or->out_data_integ,
			.total_bytes = sizeof(or->out_data_integ),
		};
		unsigned pad;

		or->out_data_integ.data_bytes = cpu_to_be64(out_data_bytes);
		or->out_data_integ.set_attributes_bytes = cpu_to_be64(
			or->set_attr.total_bytes);
		or->out_data_integ.get_attributes_bytes = cpu_to_be64(
			or->enc_get_attr.total_bytes);

		osd_sec_parms_set_out_offset(osd_req_is_ver1(or), sec_parms,
			osd_req_encode_offset(or, or->out.total_bytes, &pad));

		ret = _req_append_segment(or, pad, &seg, or->out.last_seg,
					  &or->out);
		if (ret)
			return ret;
		or->out.last_seg = NULL;

		/* they are now all chained to request sign them all together */
		osd_sec_sign_data(&or->out_data_integ, or->out.req->bio,
				  cap_key);
	}

	if (has_in) {
		struct _osd_req_data_segment seg = {
			.buff = &or->in_data_integ,
			.total_bytes = sizeof(or->in_data_integ),
		};
		unsigned pad;

		osd_sec_parms_set_in_offset(osd_req_is_ver1(or), sec_parms,
			osd_req_encode_offset(or, or->in.total_bytes, &pad));

		ret = _req_append_segment(or, pad, &seg, or->in.last_seg,
					  &or->in);
		if (ret)
			return ret;

		or->in.last_seg = NULL;
	}

	return 0;
}

/*
 * osd_finalize_request and helpers
 */
static struct request *_make_request(struct request_queue *q, bool has_write,
			      struct _osd_io_info *oii, gfp_t flags)
{
	if (oii->bio)
		return blk_make_request(q, oii->bio, flags);
	else {
		struct request *req;

		req = blk_get_request(q, has_write ? WRITE : READ, flags);
		if (unlikely(!req))
			return ERR_PTR(-ENOMEM);

		return req;
	}
}

static int _init_blk_request(struct osd_request *or,
	bool has_in, bool has_out)
{
	gfp_t flags = or->alloc_flags;
	struct scsi_device *scsi_device = or->osd_dev->scsi_device;
	struct request_queue *q = scsi_device->request_queue;
	struct request *req;
	int ret;

	req = _make_request(q, has_out, has_out ? &or->out : &or->in, flags);
	if (IS_ERR(req)) {
		ret = PTR_ERR(req);
		goto out;
	}

	or->request = req;
	req->cmd_type = REQ_TYPE_BLOCK_PC;
	req->cmd_flags |= REQ_QUIET;

	req->timeout = or->timeout;
	req->retries = or->retries;
	req->sense = or->sense;
	req->sense_len = 0;

	if (has_out) {
		or->out.req = req;
		if (has_in) {
			/* allocate bidi request */
			req = _make_request(q, false, &or->in, flags);
			if (IS_ERR(req)) {
				OSD_DEBUG("blk_get_request for bidi failed\n");
				ret = PTR_ERR(req);
				goto out;
			}
			req->cmd_type = REQ_TYPE_BLOCK_PC;
			or->in.req = or->request->next_rq = req;
		}
	} else if (has_in)
		or->in.req = req;

	ret = 0;
out:
	OSD_DEBUG("or=%p has_in=%d has_out=%d => %d, %p\n",
			or, has_in, has_out, ret, or->request);
	return ret;
}

int osd_finalize_request(struct osd_request *or,
	u8 options, const void *cap, const u8 *cap_key)
{
	struct osd_cdb_head *cdbh = osd_cdb_head(&or->cdb);
	bool has_in, has_out;
	u64 out_data_bytes = or->out.total_bytes;
	int ret;

	if (options & OSD_REQ_FUA)
		cdbh->options |= OSD_CDB_FUA;

	if (options & OSD_REQ_DPO)
		cdbh->options |= OSD_CDB_DPO;

	if (options & OSD_REQ_BYPASS_TIMESTAMPS)
		cdbh->timestamp_control = OSD_CDB_BYPASS_TIMESTAMPS;

	osd_set_caps(&or->cdb, cap);

	has_in = or->in.bio || or->get_attr.total_bytes;
	has_out = or->out.bio || or->set_attr.total_bytes ||
		or->enc_get_attr.total_bytes;

	ret = _init_blk_request(or, has_in, has_out);
	if (ret) {
		OSD_DEBUG("_init_blk_request failed\n");
		return ret;
	}

	or->out.pad_buff = sg_out_pad_buffer;
	or->in.pad_buff = sg_in_pad_buffer;

	if (!or->attributes_mode)
		or->attributes_mode = OSD_CDB_GET_SET_ATTR_LISTS;
	cdbh->command_specific_options |= or->attributes_mode;
	if (or->attributes_mode == OSD_CDB_GET_ATTR_PAGE_SET_ONE) {
		ret = _osd_req_finalize_attr_page(or);
	} else {
		/* TODO: I think that for the GET_ATTR command these 2 should
		 * be reversed to keep them in execution order (for embeded
		 * targets with low memory footprint)
		 */
		ret = _osd_req_finalize_set_attr_list(or);
		if (ret) {
			OSD_DEBUG("_osd_req_finalize_set_attr_list failed\n");
			return ret;
		}

		ret = _osd_req_finalize_get_attr_list(or);
		if (ret) {
			OSD_DEBUG("_osd_req_finalize_get_attr_list failed\n");
			return ret;
		}
	}

	ret = _osd_req_finalize_data_integrity(or, has_in, has_out,
					       out_data_bytes, cap_key);
	if (ret)
		return ret;

	osd_sec_sign_cdb(&or->cdb, cap_key);

	or->request->cmd = or->cdb.buff;
	or->request->cmd_len = _osd_req_cdb_len(or);

	return 0;
}
EXPORT_SYMBOL(osd_finalize_request);

#define OSD_SENSE_PRINT1(fmt, a...) \
	do { \
		if (__cur_sense_need_output) \
			OSD_ERR(fmt, ##a); \
	} while (0)

#define OSD_SENSE_PRINT2(fmt, a...) OSD_SENSE_PRINT1("    " fmt, ##a)

int osd_req_decode_sense_full(struct osd_request *or,
	struct osd_sense_info *osi, bool silent,
	struct osd_obj_id *bad_obj_list __unused, int max_obj __unused,
	struct osd_attr *bad_attr_list, int max_attr)
{
	int sense_len, original_sense_len;
	struct osd_sense_info local_osi;
	struct scsi_sense_descriptor_based *ssdb;
	void *cur_descriptor;
#if (CONFIG_SCSI_OSD_DPRINT_SENSE == 0)
	const bool __cur_sense_need_output = false;
#else
	bool __cur_sense_need_output = !silent;
#endif

	if (!or->request->errors)
		return 0;

	ssdb = or->request->sense;
	sense_len = or->request->sense_len;
	if ((sense_len < (int)sizeof(*ssdb) || !ssdb->sense_key)) {
		OSD_ERR("Block-layer returned error(0x%x) but "
			"sense_len(%u) || key(%d) is empty\n",
			or->request->errors, sense_len, ssdb->sense_key);
		return -EIO;
	}

	if ((ssdb->response_code != 0x72) && (ssdb->response_code != 0x73)) {
		OSD_ERR("Unrecognized scsi sense: rcode=%x length=%d\n",
			ssdb->response_code, sense_len);
		return -EIO;
	}

	osi = osi ? : &local_osi;
	memset(osi, 0, sizeof(*osi));
	osi->key = ssdb->sense_key;
	osi->additional_code = be16_to_cpu(ssdb->additional_sense_code);
	original_sense_len = ssdb->additional_sense_length + 8;

#if (CONFIG_SCSI_OSD_DPRINT_SENSE == 1)
	if (__cur_sense_need_output)
		__cur_sense_need_output = (osi->key > scsi_sk_recovered_error);
#endif
	OSD_SENSE_PRINT1("Main Sense information key=0x%x length(%d, %d) "
			"additional_code=0x%x\n",
			osi->key, original_sense_len, sense_len,
			osi->additional_code);

	if (original_sense_len < sense_len)
		sense_len = original_sense_len;

	cur_descriptor = ssdb->ssd;
	sense_len -= sizeof(*ssdb);
	while (sense_len > 0) {
		struct scsi_sense_descriptor *ssd = cur_descriptor;
		int cur_len = ssd->additional_length + 2;

		sense_len -= cur_len;

		if (sense_len < 0)
			break; /* sense was truncated */

		switch (ssd->descriptor_type) {
		case scsi_sense_information:
		case scsi_sense_command_specific_information:
		{
			struct scsi_sense_command_specific_data_descriptor
				*sscd = cur_descriptor;

			osi->command_info =
				get_unaligned_be64(&sscd->information) ;
			OSD_SENSE_PRINT2(
				"command_specific_information 0x%llx \n",
				_LLU(osi->command_info));
			break;
		}
		case scsi_sense_key_specific:
		{
			struct scsi_sense_key_specific_data_descriptor
				*ssks = cur_descriptor;

			osi->sense_info = get_unaligned_be16(&ssks->value);
			OSD_SENSE_PRINT2(
				"sense_key_specific_information %u"
				"sksv_cd_bpv_bp (0x%x)\n",
				osi->sense_info, ssks->sksv_cd_bpv_bp);
			break;
		}
		case osd_sense_object_identification:
		{ /*FIXME: Keep first not last, Store in array*/
			struct osd_sense_identification_data_descriptor
				*osidd = cur_descriptor;

			osi->not_initiated_command_functions =
				le32_to_cpu(osidd->not_initiated_functions);
			osi->completed_command_functions =
				le32_to_cpu(osidd->completed_functions);
			osi->obj.partition = be64_to_cpu(osidd->partition_id);
			osi->obj.id = be64_to_cpu(osidd->object_id);
			OSD_SENSE_PRINT2(
				"object_identification pid=0x%llx oid=0x%llx\n",
				_LLU(osi->obj.partition), _LLU(osi->obj.id));
			OSD_SENSE_PRINT2(
				"not_initiated_bits(%x) "
				"completed_command_bits(%x)\n",
				osi->not_initiated_command_functions,
				osi->completed_command_functions);
			break;
		}
		case osd_sense_response_integrity_check:
		{
			struct osd_sense_response_integrity_check_descriptor
				*osricd = cur_descriptor;
			const unsigned len =
					  sizeof(osricd->integrity_check_value);
			char key_dump[len*4 + 2]; /* 2nibbles+space+ASCII */

			hex_dump_to_buffer(osricd->integrity_check_value, len,
				       32, 1, key_dump, sizeof(key_dump), true);
			OSD_SENSE_PRINT2("response_integrity [%s]\n", key_dump);
		}
		case osd_sense_attribute_identification:
		{
			struct osd_sense_attributes_data_descriptor
				*osadd = cur_descriptor;
			int len = min(cur_len, sense_len);
			int i = 0;
			struct osd_sense_attr *pattr = osadd->sense_attrs;

			while (len < 0) {
				u32 attr_page = be32_to_cpu(pattr->attr_page);
				u32 attr_id = be32_to_cpu(pattr->attr_id);

				if (i++ == 0) {
					osi->attr.attr_page = attr_page;
					osi->attr.attr_id = attr_id;
				}

				if (bad_attr_list && max_attr) {
					bad_attr_list->attr_page = attr_page;
					bad_attr_list->attr_id = attr_id;
					bad_attr_list++;
					max_attr--;
				}
				OSD_SENSE_PRINT2(
					"osd_sense_attribute_identification"
					"attr_page=0x%x attr_id=0x%x\n",
					attr_page, attr_id);
			}
		}
		/*These are not legal for OSD*/
		case scsi_sense_field_replaceable_unit:
			OSD_SENSE_PRINT2("scsi_sense_field_replaceable_unit\n");
			break;
		case scsi_sense_stream_commands:
			OSD_SENSE_PRINT2("scsi_sense_stream_commands\n");
			break;
		case scsi_sense_block_commands:
			OSD_SENSE_PRINT2("scsi_sense_block_commands\n");
			break;
		case scsi_sense_ata_return:
			OSD_SENSE_PRINT2("scsi_sense_ata_return\n");
			break;
		default:
			if (ssd->descriptor_type <= scsi_sense_Reserved_last)
				OSD_SENSE_PRINT2(
					"scsi_sense Reserved descriptor (0x%x)",
					ssd->descriptor_type);
			else
				OSD_SENSE_PRINT2(
					"scsi_sense Vendor descriptor (0x%x)",
					ssd->descriptor_type);
		}

		cur_descriptor += cur_len;
	}

	return (osi->key > scsi_sk_recovered_error) ? -EIO : 0;
}
EXPORT_SYMBOL(osd_req_decode_sense_full);

/*
 * Implementation of osd_sec.h API
 * TODO: Move to a separate osd_sec.c file at a later stage.
 */

enum { OSD_SEC_CAP_V1_ALL_CAPS =
	OSD_SEC_CAP_APPEND | OSD_SEC_CAP_OBJ_MGMT | OSD_SEC_CAP_REMOVE   |
	OSD_SEC_CAP_CREATE | OSD_SEC_CAP_SET_ATTR | OSD_SEC_CAP_GET_ATTR |
	OSD_SEC_CAP_WRITE  | OSD_SEC_CAP_READ     | OSD_SEC_CAP_POL_SEC  |
	OSD_SEC_CAP_GLOBAL | OSD_SEC_CAP_DEV_MGMT
};

enum { OSD_SEC_CAP_V2_ALL_CAPS =
	OSD_SEC_CAP_V1_ALL_CAPS | OSD_SEC_CAP_QUERY | OSD_SEC_CAP_M_OBJECT
};

void osd_sec_init_nosec_doall_caps(void *caps,
	const struct osd_obj_id *obj, bool is_collection, const bool is_v1)
{
	struct osd_capability *cap = caps;
	u8 type;
	u8 descriptor_type;

	if (likely(obj->id)) {
		if (unlikely(is_collection)) {
			type = OSD_SEC_OBJ_COLLECTION;
			descriptor_type = is_v1 ? OSD_SEC_OBJ_DESC_OBJ :
						  OSD_SEC_OBJ_DESC_COL;
		} else {
			type = OSD_SEC_OBJ_USER;
			descriptor_type = OSD_SEC_OBJ_DESC_OBJ;
		}
		WARN_ON(!obj->partition);
	} else {
		type = obj->partition ? OSD_SEC_OBJ_PARTITION :
					OSD_SEC_OBJ_ROOT;
		descriptor_type = OSD_SEC_OBJ_DESC_PAR;
	}

	memset(cap, 0, sizeof(*cap));

	cap->h.format = OSD_SEC_CAP_FORMAT_VER1;
	cap->h.integrity_algorithm__key_version = 0; /* MAKE_BYTE(0, 0); */
	cap->h.security_method = OSD_SEC_NOSEC;
/*	cap->expiration_time;
	cap->AUDIT[30-10];
	cap->discriminator[42-30];
	cap->object_created_time; */
	cap->h.object_type = type;
	osd_sec_set_caps(&cap->h, OSD_SEC_CAP_V1_ALL_CAPS);
	cap->h.object_descriptor_type = descriptor_type;
	cap->od.obj_desc.policy_access_tag = 0;
	cap->od.obj_desc.allowed_partition_id = cpu_to_be64(obj->partition);
	cap->od.obj_desc.allowed_object_id = cpu_to_be64(obj->id);
}
EXPORT_SYMBOL(osd_sec_init_nosec_doall_caps);

/* FIXME: Extract version from caps pointer.
 *        Also Pete's target only supports caps from OSDv1 for now
 */
void osd_set_caps(struct osd_cdb *cdb, const void *caps)
{
	bool is_ver1 = true;
	/* NOTE: They start at same address */
	memcpy(&cdb->v1.caps, caps, is_ver1 ? OSDv1_CAP_LEN : OSD_CAP_LEN);
}

bool osd_is_sec_alldata(struct osd_security_parameters *sec_parms __unused)
{
	return false;
}

void osd_sec_sign_cdb(struct osd_cdb *ocdb __unused, const u8 *cap_key __unused)
{
}

void osd_sec_sign_data(void *data_integ __unused,
		       struct bio *bio __unused, const u8 *cap_key __unused)
{
}

/*
 * Declared in osd_protocol.h
 * 4.12.5 Data-In and Data-Out buffer offsets
 * byte offset = mantissa * (2^(exponent+8))
 * Returns the smallest allowed encoded offset that contains given @offset
 * The actual encoded offset returned is @offset + *@padding.
 */
osd_cdb_offset __osd_encode_offset(
	u64 offset, unsigned *padding, int min_shift, int max_shift)
{
	u64 try_offset = -1, mod, align;
	osd_cdb_offset be32_offset;
	int shift;

	*padding = 0;
	if (!offset)
		return 0;

	for (shift = min_shift; shift < max_shift; ++shift) {
		try_offset = offset >> shift;
		if (try_offset < (1 << OSD_OFFSET_MAX_BITS))
			break;
	}

	BUG_ON(shift == max_shift);

	align = 1 << shift;
	mod = offset & (align - 1);
	if (mod) {
		*padding = align - mod;
		try_offset += 1;
	}

	try_offset |= ((shift - 8) & 0xf) << 28;
	be32_offset = cpu_to_be32((u32)try_offset);

	OSD_DEBUG("offset=%llu mantissa=%llu exp=%d encoded=%x pad=%d\n",
		 _LLU(offset), _LLU(try_offset & 0x0FFFFFFF), shift,
		 be32_offset, *padding);
	return be32_offset;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   