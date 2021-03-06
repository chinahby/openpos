ailed\n");
		goto evidregd;
	}
	icd->vdev = vdev;

	return 0;

evidregd:
	video_device_release(vdev);
evidallocd:
	return err;
}
EXPORT_SYMBOL(soc_camera_video_start);

/* Called from client .remove() methods with .video_lock held */
void soc_camera_video_stop(struct soc_camera_device *icd)
{
	struct video_device *vdev = icd->vdev;

	dev_dbg(&icd->dev, "%s\n", __func__);

	if (!icd->dev.parent || !vdev)
		return;

	video_unregister_device(vdev);
	icd->vdev = NULL;
}
EXPORT_SYMBOL(soc_camera_video_stop);

static int __devinit soc_camera_pdrv_probe(struct platform_device *pdev)
{
	struct soc_camera_link *icl = pdev->dev.platform_data;
	struct i2c_adapter *adap;
	struct i2c_client *client;

	if (!icl)
		return -EINVAL;

	adap = i2c_get_adapter(icl->i2c_adapter_id);
	if (!adap) {
		dev_warn(&pdev->dev, "Cannot get adapter #%d. No driver?\n",
			 icl->i2c_adapter_id);
		/* -ENODEV and -ENXIO do not produce an error on probe()... */
		return -ENOENT;
	}

	icl->board_info->platform_data = icl;
	client = i2c_new_device(adap, icl->board_info);
	if (!client) {
		i2c_put_adapter(adap);
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, client);

	return 0;
}

static int __devexit soc_camera_pdrv_remove(struct platform_device *pdev)
{
	struct i2c_client *client = platform_get_drvdata(pdev);

	if (!client)
		return -ENODEV;

	i2c_unregister_device(client);
	i2c_put_adapter(client->adapter);

	return 0;
}

static struct platform_driver __refdata soc_camera_pdrv = {
	.probe	= soc_camera_pdrv_probe,
	.remove	= __devexit_p(soc_camera_pdrv_remove),
	.driver	= {
		.name = "soc-camera-pdrv",
		.owner = THIS_MODULE,
	},
};

static int __init soc_camera_init(void)
{
	int ret = bus_register(&soc_camera_bus_type);
	if (ret)
		return ret;
	ret = driver_register(&ic_drv);
	if (ret)
		goto edrvr;

	ret = platform_driver_register(&soc_camera_pdrv);
	if (ret)
		goto epdr;

	return 0;

epdr:
	driver_unregister(&ic_drv);
edrvr:
	bus_unregister(&soc_camera_bus_type);
	return ret;
}

static void __exit soc_camera_exit(void)
{
	platform_driver_unregister(&soc_camera_pdrv);
	driver_unregister(&ic_drv);
	bus_unregister(&soc_camera_bus_type);
}

module_init(soc_camera_init);
module_exit(soc_camera_exit);

MODULE_DESCRIPTION("Image capture bus driver");
MODULE_AUTHOR("Guennadi Liakhovetski <kernel@pengutronix.de>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:soc-camera-pdrv");
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                /***************************************************************************\
|*                                                                           *|
|*       Copyright 1993-1999 NVIDIA, Corporation.  All rights reserved.      *|
|*                                                                           *|
|*     NOTICE TO USER:   The source code  is copyrighted under  U.S. and     *|
|*     international laws.  Users and possessors of this source code are     *|
|*     hereby granted a nonexclusive,  royalty-free copyright license to     *|
|*     use this code in individual and commercial software.                  *|
|*                                                                           *|
|*     Any use of this source code must include,  in the user documenta-     *|
|*     tion and  internal comments to the code,  notices to the end user     *|
|*     as follows:                                                           *|
|*                                                                           *|
|*       Copyright 1993-1999 NVIDIA, Corporation.  All rights reserved.      *|
|*                                                                           *|
|*     NVIDIA, CORPORATION MAKES NO REPRESENTATION ABOUT THE SUITABILITY     *|
|*     OF  THIS SOURCE  CODE  FOR ANY PURPOSE.  IT IS  PROVIDED  "AS IS"     *|
|*     WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.  NVIDIA, CORPOR-     *|
|*     ATION DISCLAIMS ALL WARRANTIES  WITH REGARD  TO THIS SOURCE CODE,     *|
|*     INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGE-     *|
|*     MENT,  AND FITNESS  FOR A PARTICULAR PURPOSE.   IN NO EVENT SHALL     *|
|*     NVIDIA, CORPORATION  BE LIABLE FOR ANY SPECIAL,  INDIRECT,  INCI-     *|
|*     DENTAL, OR CONSEQUENTIAL DAMAGES,  OR ANY DAMAGES  WHATSOEVER RE-     *|
|*     SULTING FROM LOSS OF USE,  DATA OR PROFITS,  WHETHER IN AN ACTION     *|
|*     OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  ARISING OUT OF     *|
|*     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.     *|
|*                                                                           *|
|*     U.S. Government  End  Users.   This source code  is a "commercial     *|
|*     item,"  as that  term is  defined at  48 C.F.R. 2.101 (OCT 1995),     *|
|*     consisting  of "commercial  computer  software"  and  "commercial     *|
|*     computer  software  documentation,"  as such  terms  are  used in     *|
|*     48 C.F.R. 12.212 (SEPT 1995)  and is provided to the U.S. Govern-     *|
|*     ment only as  a commercial end item.   Consistent with  48 C.F.R.     *|
|*     12.212 and  48 C.F.R. 227.7202-1 through  227.7202-4 (JUNE 1995),     *|
|*     all U.S. Government End Users  acquire the source code  with only     *|
|*     those rights set forth herein.                                        *|
|*                                                                           *|
\***************************************************************************/

/*
 * GPL licensing note -- nVidia is allowing a liberal interpretation of
 * the documentation restriction above, to merely say that this nVidia's
 * copyright and disclaimer should be included with all code derived
 * from this source.  -- Jeff Garzik <jgarzik@pobox.com>, 01/Nov/99 
 */

/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/nv/riva_hw.h,v 1.21 2002/10/14 18:22:46 mvojkovi Exp $ */
#ifndef __RIVA_HW_H__
#define __RIVA_HW_H__
#define RIVA_SW_VERSION 0x00010003

#ifndef Bool
typedef int Bool;
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

/*
 * Typedefs to force certain sized values.
 */
typedef unsigned char  U008;
typedef unsigned short U016;
typedef unsigned int   U032;

/*
 * HW access macros.
 */
#include <asm/io.h>

#define NV_WR08(p,i,d)  (__raw_writeb((d), (void __iomem *)(p) + (i)))
#define NV_RD08(p,i)    (__raw_readb((void __iomem *)(p) + (i)))
#define NV_WR16(p,i,d)  (__raw_writew((d), (void __iomem *)(p) + (i)))
#define NV_RD16(p,i)    (__raw_readw(SS      0x000E
#define I2O_DSC_INVALID_MESSAGE_FLAGS          0x000F
#define I2O_DSC_INVALID_OFFSET                 0x0010
#define I2O_DSC_INVALID_PARAMETER              0x0011
#define I2O_DSC_INVALID_REQUEST                0x0012
#define I2O_DSC_INVALID_TARGET_ADDRESS         0x0013
#define I2O_DSC_MESSAGE_TOO_LARGE              0x0014
#define I2O_DSC_MESSAGE_TOO_SMALL              0x0015
#define I2O_DSC_MISSING_PARAMETER              0x0016
#define I2O_DSC_TIMEOUT                        0x0017
#define I2O_DSC_UNKNOWN_ERROR                  0x0018
#define I2O_DSC_UNKNOWN_FUNCTION               0x0019
#define I2O_DSC_UNSUPPORTED_VERSION            0x001A
#define I2O_DSC_DEVICE_BUSY                    0x001B
#define I2O_DSC_DEVICE_NOT_AVAILABLE           0x001C

/* DetailedStatusCode defines for Block Storage Operation: Table 6-7 Detailed
   Status Codes.*/

#define I2O_BSA_DSC_SUCCESS               0x0000
#define I2O_BSA_DSC_MEDIA_ERROR           0x0001
#define I2O_BSA_DSC_ACCESS_ERROR          0x0002
#define I2O_BSA_DSC_DEVICE_FAILURE        0x0003
#define I2O_BSA_DSC_DEVICE_NOT_READY      0x0004
#define I2O_BSA_DSC_MEDIA_NOT_PRESENT     0x0005
#define I2O_BSA_DSC_MEDIA_LOCKED          0x0006
#define I2O_BSA_DSC_MEDIA_FAILURE         0x0007
#define I2O_BSA_DSC_PROTOCOL_FAILURE      0x0008
#define I2O_BSA_DSC_BUS_FAILURE           0x0009
#define I2O_BSA_DSC_ACCESS_VIOLATION      0x000A
#define I2O_BSA_DSC_WRITE_PROTECTED       0x000B
#define I2O_BSA_DSC_DEVICE_RESET          0x000C
#define I2O_BSA_DSC_VOLUME_CHANGED        0x000D
#define I2O_BSA_DSC_TIMEOUT               0x000E

/* FailureStatusCodes, Table 3-3 Message Failure Codes */

#define I2O_FSC_TRANSPORT_SERVICE_SUSPENDED             0x81
#define I2O_FSC_TRANSPORT_SERVICE_TERMINATED            0x82
#define I2O_FSC_TRANSPORT_CONGESTION                    0x83
#define I2O_FSC_TRANSPORT_FAILURE                       0x84
#define I2O_FSC_TRANSPORT_STATE_ERROR                   0x85
#define I2O_FSC_TRANSPORT_TIME_OUT                      0x86
#define I2O_FSC_TRANSPORT_ROUTING_FAILURE               0x87
#define I2O_FSC_TRANSPORT_INVALID_VERSION               0x88
#define I2O_FSC_TRANSPORT_INVALID_OFFSET                0x89
#define I2O_FSC_TRANSPORT_INVALID_MSG_FLAGS             0x8A
#define I2O_FSC_TRANSPORT_FRAME_TOO_SMALL               0x8B
#define I2O_FSC_TRANSPORT_FRAME_TOO_LARGE               0x8C
#define I2O_FSC_TRANSPORT_INVALID_TARGET_ID             0x8D
#define I2O_FSC_TRANSPORT_INVALID_INITIATOR_ID          0x8E
#define I2O_FSC_TRANSPORT_INVALID_INITIATOR_CONTEXT     0x8F
#define I2O_FSC_TRANSPORT_UNKNOWN_FAILURE               0xFF

/* Device Claim Types */
#define	I2O_CLAIM_PRIMARY					0x01000000
#define	I2O_CLAIM_MANAGEMENT					0x02000000
#define	I2O_CLAIM_AUTHORIZED					0x03000000
#define	I2O_CLAIM_SECONDARY					0x04000000

/* Message header defines for VersionOffset */
#define I2OVER15	0x0001
#define I2OVER20	0x0002

/* Default is 1.5 */
#define I2OVERSION	I2OVER15

#define SGL_OFFSET_0    I2OVERSION
#define SGL_OFFSET_4    (0x0040 | I2OVERSION)
#define SGL_OFFSET_5    (0x0050 | I2OVERSION)
#define SGL_OFFSET_6    (0x0060 | I2OVERSION)
#define SGL_OFFSET_7    (0x0070 | I2OVERSION)
#define SGL_OFFSET_8    (0x0080 | I2OVERSION)
#define SGL_OFFSET_9    (0x0090 | I2OVERSION)
#define SGL_OFFSET_10   (0x00A0 | I2OVERSION)
#define SGL_OFFSET_11   (0x00B0 | I2OVERSION)
#define SGL_OFFSET_12   (0x00C0 | I2OVERSION)
#define SGL_OFFSET(x)   (((x)<<4) | I2OVERSION)

/* Transaction Reply Lists (TRL) Control Word structure */
#define TRL_SINGLE_FIXED_LENGTH		0x00
#define TRL_SINGLE_VARIABLE_LENGTH	0x40
#define TRL_MULTIPLE_FIXED_LENGTH	0x80

 /* msg header defines for MsgFlags */
#define MSG_STATIC	0x0100
#define MSG_64BIT_CNTXT	0x0200
#define MSG_MULTI_TRANS	0x1000
#define MSG_FAIL	0x2000
#define MSG_FINAL	0x4000
#define MSG_REPLY	0x8000

 /* minimum size msg */
#define THREE_WORD_MSG_SIZE	0x00030000
#define FOUR_WORD_MSG_SIZE	0x00040000
#define FIVE_WORD_MSG_SIZE	0x00050000
#define SIX_WORD_MSG_SIZE	0x00060000
#define SEVEN_WORD_MSG_SIZE	0x00070000
#define EIGHT_WORD_MSG_SIZE	0x00080000
#define NINE_WORD_MSG_SIZE	0x00090000
#define TEN_WORD_MSG_SIZE	0x000A0000
#define ELEVEN_WORD_MSG_SIZE	0x000B0000
#define I2O_MESSAGE_SIZE(x)	((x)<<16)

/* special TID assignments */
#define ADAPTER_TID		0
#define HOST_TID		1

/* outbound queue defines */
#define I2O_MAX_OUTBOUND_MSG_FRAMES	128
#define I2O_OUTBOUND_MSG_FRAME_SIZE	128	/* in 32-bit words */

/* inbound queue definitions */
#define I2O_MSG_INPOOL_MIN		32
#define I2O_INBOUND_MSG_FRAME_SIZE	128	/* in 32-bit words */

#define I2O_POST_WAIT_OK	0
#define I2O_POST_WAIT_TIMEOUT	-ETIMEDOUT

#define I2O_CONTEXT_LIST_MIN_LENGTH	15
#define I2O_CONTEXT_LIST_USED		0x01
#define I2O_CONTEXT_LIST_DELETED	0x02

/* timeouts */
#define I2O_TIMEOUT_INIT_OUTBOUND_QUEUE	15
#define I2O_TIMEOUT_MESSAGE_GET		5
#define I2O_TIMEOUT_RESET		30
#define I2O_TIMEOUT_STATUS_GET		5
#define I2O_TIMEOUT_LCT_GET		360
#define I2O_TIMEOUT_SCSI_SCB_ABORT	240

/* retries */
#define I2O_HRT_GET_TRIES		3
#define I2O_LCT_GET_TRIES		3

/* defines for max_sectors and max_phys_segments */
#define I2O_MAX_SECTORS			1024
#define I2O_MAX_SECTORS_LIMITED		128
#define I2O_MAX_PHYS_SEGMENTS		MAX_PHYS_SEGMENTS

/*
 *	Message structures
 */
struct i2o_message {
	union {
		struct {
			u8 version_offset;
			u8 flags;
			u16 size;
			u32 target_tid:12;
			u32 init_tid:12;
			u32 function:8;
			u32 icntxt;	/* initiator context */
			u32 tcntxt;	/* transaction context */
		} s;
		u32 head[4];
	} u;
	/* List follows */
	u32 body[0];
};

/* MFA and I2O message used by mempool */
struct i2o_msg_mfa {
	u32 mfa;		/* MFA returned by the controller */
	struct i2o_message msg;	/* I2O message */
};

/*
 *	Each I2O device entity has one of these. There is one per device.
 */
struct i2o_device {
	i2o_lct_entry lct_data;	/* Device LCT information */

	struct i2o_controller *iop;	/* Controlling IOP */
	struct list_head list;	/* node in IOP devices list */

	struct device device;

	struct mutex lock;	/* device lock */
};

/*
 *	Event structure provided to the event handling function
 */
struct i2o_event {
	struct work_struct work;
	struct i2o_device *i2o_dev;	/* I2O device pointer from which the
					   event reply was initiated */
	u16 size;		/* Size of data in 32-bit words */
	u32 tcntxt;		/* Transaction context used at
				   registration */
	u32 event_indicator;	/* Event indicator from reply */
	u32 data[0];		/* Event data from reply */
};

/*
 *	I2O classes which could be handled by the OSM
 */
struct i2o_class_id {
	u16 class_id:12;
};

/*
 *	I2O driver structure for OSMs
 */
struct i2o_driver {
	char *name;		/* OSM name */
	int context;		/* Low 8 bits of the transaction info */
	struct i2o_class_id *classes;	/* I2O classes that this OSM handles */

	/* Message reply handler */
	int (*reply) (struct i2o_controller *, u32, struct i2o_message *);

	/* Event handler */
	work_func_t event;

	struct workqueue_struct *event_queue;	/* Event queue */

	struct device_driver driver;

	/* notification of changes */
	void (*notify_controller_add) (struct i2o_controller *);
	void (*notify_controller_remove) (struct i2o_controller *);
	void (*notify_device_add) (struct i2o_device *);
	void (*notify_device_remove) (struct i2o_device *);

	struct semaphore lock;
};

/*
 *	Contains DMA mapped address information
 */
struct i2o_dma {
	void *virt;
	dma_addr_t phys;
	size_t len;
};

/*
 *	Contains slab cache and mempool information
 */
struct i2o_pool {
	char *name;
	struct kmem_cache *slab;
	mempool_t *mempool;
};

/*
 *	Contains IO mapped address information
 */
struct i2o_io {
	void __iomem *virt;
	unsigned long phys;
	unsigned long len;
};

/*
 *	Context queue entry, used for 32-bit context on 64-bit systems
 */
struct i2o_context_list_element {
	struct list_head list;
	u32 context;
	void *ptr;
	unsigned long timestamp;
};

/*
 * Each I2O controller has one of these objects
 */
struct i2o_controller {
	char name[16];
	int unit;
	int type;

	struct pci_dev *pdev;	/* PCI device */

	unsigned int promise:1;	/* Promise controller */
	unsigned int adaptec:1;	/* DPT / Adaptec controller */
	unsigned int raptor:1;	/* split bFifoFreeCount;
    U032 FifoEmptyCount;
    U032 CursorStart;
    U032 flatPanel;
    Bool twoHeads;
    /*
     * Non-FIFO registers.
     */
    volatile U032 __iomem *PCRTC0;
    volatile U032 __iomem *PCRTC;
    volatile U032 __iomem *PRAMDAC0;
    volatile U032 __iomem *PFB;
    volatile U032 __iomem *PFIFO;
    volatile U032 __iomem *PGRAPH;
    volatile U032 __iomem *PEXTDEV;
    volatile U032 __iomem *PTIMER;
    volatile U032 __iomem *PMC;
    volatile U032 __iomem *PRAMIN;
    volatile U032 __iomem *FIFO;
    volatile U032 __iomem *CURSOR;
    volatile U008 __iomem *PCIO0;
    volatile U008 __iomem *PCIO;
    volatile U008 __iomem *PVIO;
    volatile U008 __iomem *PDIO0;
    volatile U008 __iomem *PDIO;
    volatile U032 __iomem *PRAMDAC;
    /*
     * Common chip functions.
     */
    int  (*Busy)(struct _riva_hw_inst *);
    void (*LoadStateExt)(struct _riva_hw_inst *,struct _riva_hw_state *);
    void (*UnloadStateExt)(struct _riva_hw_inst *,struct _riva_hw_state *);
    void (*SetStartAddress)(struct _riva_hw_inst *,U032);
    void (*SetSurfaces2D)(struct _riva_hw_inst *,U032,U032);
    void (*SetSurfaces3D)(struct _riva_hw_inst *,U032,U032);
    int  (*ShowHideCursor)(struct _riva_hw_inst *,int);
    void (*LockUnlock)(struct _riva_hw_inst *, int);
    /*
     * Current extended mode settings.
     */
    struct _riva_hw_state *CurrentState;
    /*
     * FIFO registers.
     */
    RivaRop                 __iomem *Rop;
    RivaPattern             __iomem *Patt;
    RivaClip                __iomem *Clip;
    RivaPixmap              __iomem *Pixmap;
    RivaScreenBlt           __iomem *Blt;
    RivaBitmap              __iomem *Bitmap;
    RivaLine                __iomem *Line;
    RivaTexturedTriangle03  __iomem *Tri03;
    RivaTexturedTriangle05  __iomem *Tri05;
} RIVA_HW_INST;
/*
 * Extended mode state information.
 */
typedef struct _riva_hw_state
{
    U032 bpp;
    U032 width;
    U032 height;
    U032 interlace;
    U032 repaint0;
    U032 repaint1;
    U032 screen;
    U032 scale;
    U032 dither;
    U032 extra;
    U032 pixel;
    U032 horiz;
    U032 arbitration0;
    U032 arbitration1;
    U032 vpll;
    U032 vpll2;
    U032 pllsel;
    U032 general;
    U032 crtcOwner;
    U032 head; 
    U032 head2; 
    U032 config;
    U032 cursorConfig;	
    U032 cursor0;
    U032 cursor1;
    U032 cursor2;
    U032 offset0;
    U032 offset1;
    U032 offset2;
    U032 offset3;
    U032 pitch0;
    U032 pitch1;
    U032 pitch2;
    U032 pitch3;
} RIVA_HW_STATE;

/*
 * function prototypes
 */

extern int CalcStateExt
(
    RIVA_HW_INST  *chip,
    RIVA_HW_STATE *state,
    int            bpp,
    int            width,
    int            hDisplaySize,
    int            height,
    int            dotClock
);

/*
 * External routines.
 */
int RivaGetConfig(RIVA_HW_INST *, unsigned int);
/*
 * FIFO Free Count. Should attempt to yield processor if RIVA is busy.
 */

#define RIVA_FIFO_FREE(hwinst,hwptr,cnt)                            \
{                                                                   \
    while ((hwinst).FifoFreeCount < (cnt)) {                        \
	mb();mb();						    \
        (hwinst).FifoFreeCount = NV_RD32(&(hwinst).hwptr->FifoFree, 0) >> 2;     \
    }								    \
    (hwinst).FifoFreeCount -= (cnt);                                \
}
#endif /* __RIVA_HW_H__ */

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             /*
 * Ultra Wide Band
 * IE Received notification handling.
 *
 * Copyright (C) 2008 Cambridge Silicon Radio Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/bitmap.h>
#include "uwb-internal.h"

/*
 * Process an incoming IE Received notification.
 */
int uwbd_evt_handle_rc_ie_rcv(struct uwb_event *evt)
{
	int result = -EINVAL;
	struct device *dev = &evt->rc->uwb_dev.dev;
	struct uwb_rc_evt_ie_rcv *iercv;
	size_t iesize;

	/* Is there enough data to decode it? */
	if (evt->notif.size < sizeof(*iercv)) {
		dev_err(dev, "IE Received notification: Not enough data to "
			"decode (%zu vs %zu bytes needed)\n",
			evt->notif.size, sizeof(*iercv));
		goto error;
	}
	iercv = container_of(evt->notif.rceb, struct uwb_rc_evt_ie_rcv, rceb);
	iesize = le16_to_cpu(iercv->wIELength);

	dev_dbg(dev, "IE received, element ID=%d\n", iercv->IEData[0]);

	if (iercv->IEData[0] == UWB_RELINQUISH_REQUEST_IE) {
		dev_warn(dev, "unhandled Relinquish Request IE\n");
	}

	return 0;
error:
	return result;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                /*
 * OHCI HCD (Host Controller Driver) for USB.
 *
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2002 David Brownell <dbrownell@users.sourceforge.net>
 * (C) Copyright 2002 Hewlett-Packard Company
 *
 * USB Bus Glue for Samsung S3C2410
 *
 * Written by Christopher Hoover <ch@hpl.hp.com>
 * Based on fragments of previous driver by Russell King et al.
 *
 * Modified for S3C2410 from ohci-sa1111.c, ohci-omap.c and ohci-lh7a40.c
 *	by Ben Dooks, <ben@simtec.co.uk>
 *	Copyright (C) 2004 Simtec Electronics
 *
 * Thanks to basprog@mail.ru for updates to newer kernels
 *
 * This file is licenced under the GPL.
*/

#include <linux/platform_device.h>
#include <linux/clk.h>
#include <plat/usb-control.h>

#define valid_port(idx) ((idx) == 1 || (idx) == 2)

/* clock device associated with the hcd */

static struct clk *clk;
static struct clk *usb_clk;

/* forward definitions */

static void s3c2410_hcd_oc(struct s3c2410_hcd_info *info, int port_oc);

/* conversion functions */

static struct s3c2410_hcd_info *to_s3c2410_info(struct usb_hcd *hcd)
{
	return hcd->self.controller->platform_data;
}

static void s3c2410_start_hc(struct platform_device *dev, struct usb_hcd *hcd)
{
	struct s3c2410_hcd_info *info = dev->dev.platform_data;

	dev_dbg(&dev->dev, "s3c2410_start_hc:\n");

	clk_enable(usb_clk);
	mdelay(2);			/* let the bus clock stabilise */

	clk_enable(clk);

	if (info != NULL) {
		info->hcd	= hcd;
		info->report_oc = s3c2410_hcd_oc;

		if (info->enable_oc != NULL) {
			(info->enable_oc)(info, 1);
		}
	}
}

static void s3c2410_stop_hc(struct platform_device *dev)
{
	struct s3c2410_hcd_info *info = dev->dev.platform_data;

	dev_dbg(&dev->dev, "s3c2410_stop_hc:\n");

	if (info != NULL) {
		info->report_oc = NULL;
		info->hcd	= NULL;

		if (info->enable_oc != NULL) {
			(info->enable_oc)(info, 0);
		}
	}

	clk_disable(clk);
	clk_disable(usb_clk);
}

/* ohci_s3c2410_hub_status_data
 *
 * update the status data from the hub with anything that
 * has been detected by our system
*/

static int
ohci_s3c2410_hub_status_data (struct usb_hcd *hcd, char *buf)
{
	struct s3c2410_hcd_info *info = to_s3c2410_info(hcd);
	struct s3c2410_hcd_port *port;
	int orig;
	int portno;

	orig  = ohci_hub_status_data (hcd, buf);

	if (info == NULL)
		return orig;

	port = &info->port[0];

	/* mark any changed port as changed */

	for (portno = 0; portno < 2; port++, portno++) {
		if (port->oc_changed == 1 &&
		    port->flags & S3C_HCDFLG_USED) {
			dev_dbg(hcd->self.controller,
				"oc change on port %d\n", portno);

			if (orig < 1)
				orig = 1;

			buf[0] |= 1<<(portno+1);
		}
	}

	return orig;
}

/* s3c2410_usb_set_power
 *
 * configure the power on a port, by calling the platform device
 * routine registered with the platform device
*/

static void s3c2410_usb_set_power(struct s3c2410_hcd_info *info,
				  int port, int to)
{
	if (info == NULL)
		return;

	if (info->power_control != NULL) {
		info->port[port-1].power = to;
		(info->power_control)(port-1, to);
	}
}

/* ohci_s3c2410_hub_control
 *
 * look at control requests to the hub, and see if we need
 * to take any action or over-ride the results from the
 * request.
*/

static int ohci_s3c2410_hub_control (
	struct usb_hcd	*hcd,
	u16		typeReq,
	u16		wValue,
	u16		wIndex,
	char		*buf,
	u16		wLength)
{
	struct s3c2410_hcd_info *info = to_s3c2410_info(hcd);
	struct usb_hub_descriptor *desc;
	int ret = -EINVAL;
	u32 *data = (u32 *)buf;

	dev_dbg(hcd->self.controller,
		"s3c2410_hub_control(%p,0x%04x,0x%04x,0x%04x,%p,%04x)\n",
		hcd, typeReq, wValue, wIndex, buf, wLength);

	/* if we are only an humble host without any special capabilities
	 * process the request straight away and exit */

	if (info == NULL) {
		ret = ohci_hub_control(hcd, typeReq, wValue,
				       wIndex, buf, wLength);
		goto out;
	}

	/* check the request to see if it needs handling */

	switch (typeReq) {
	case SetPortFeature:
		if (wValue == USB_PORT_FEAT_POWER) {
			dev_dbg(hcd->self.controller, "SetPortFeat: POWER\n");
			s3c2410_usb_set_power(info, wIndex, 1);
			goto out;
		}
		break;

	case ClearPortFeature:
		switch (wValue) {
		case USB_PORT_FEAT_C_OVER_CURRENT:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: C_OVER_CURRENT\n");

			if (valid_port(wIndex)) {
				info->port[wIndex-1].oc_changed = 0;
				info->port[wIndex-1].oc_status = 0;
			}

			goto out;

		case USB_PORT_FEAT_OVER_CURRENT:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: OVER_CURRENT\n");

			if (valid_port(wIndex)) {
				info->port[wIndex-1].oc_status = 0;
			}

			goto out;

		case USB_PORT_FEAT_POWER:
			dev_dbg(hcd->self.controller,
				"ClearPortFeature: POWER\n");

			if (valid_port(wIndex)) {
				s3c2410_usb_set_power(info, wIndex, 0);
				return 0;
			}
		}
		break;
	}

	ret = ohci_hub_control(hcd, typeReq, wValue, wIndex, buf, wLength);
	if (ret)
		goto out;

	switch (typeReq) {
	case GetHubDescriptor:

		/* update the hub's descriptor */

		desc = (struct usb_hub_descriptor *)buf;

		if (info->power_control == NULL)
			return ret;

		dev_dbg(hcd->self.controller, "wHubCharacteristics 0x%04x\n",
			desc->wHubCharacteristics);

		/* remove the old configurations for power-switching, and
		 * over-current protection, and insert our new configuration
		 */

		desc->wHubCharacteristics &= ~cpu_to_le16(HUB_CHAR_LPSM);
		desc->wHubCharacteristics |= cpu_to_le16(0x0001);

		if (info->enable_oc) {
			desc->wHubCharacteristics &= ~cpu_to_le16(HUB_CHAR_OCPM);
			desc->wHubCharacteristics |=  cpu_to_le16(0x0008|0x0001);
		}

		dev_dbg(hcd->self.controller, "wHubCharacteristics after 0x%04x\n",
			desc->wHubCharacteristics);

		return ret;

	case GetPortStatus:
		/* check port status */

		dev_dbg(hcd->self.controller, "GetPortStatus(%d)\n", wIndex);

		if (valid_port(wIndex)) {
			if (info->port[wIndex-1].oc_changed) {
				*data |= cpu_to_le32(RH_PS_OCIC);
			}

			if (inf