/*
 * wm_adsp.h  --  Wolfson ADSP support
 *
 * Copyright 2012 Wolfson Microelectronics plc
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __WM_ADSP_H
#define __WM_ADSP_H

#include <sound/soc.h>
#include <sound/soc-dapm.h>

#include "wmfw.h"

struct wm_adsp_region {
	int type;
	unsigned int base;
};

struct wm_adsp_alg_region {
	struct list_head list;
	unsigned int alg;
	int type;
	unsigned int base;
};

struct wm_adsp {
	const char *part;
	int num;
	int type;
	struct device *dev;
	struct regmap *regmap;

	int base;
	int sysclk_reg;
	int sysclk_mask;
	int sysclk_shift;

	struct list_head alg_regions;

	int fw_id;
	int fw_id_version;

	const struct wm_adsp_region *mem;
	int num_mems;

	int fw;
	bool running;

	struct regulator *dvfs;
	int fw_ver;
	u32 running;

	struct mutex ctl_lock;
	struct list_head ctl_list;

	u32 host_buf_ptr;

	int max_dsp_read_bytes;
	u32 dsp_error;
	u32 *raw_capt_buf;
	struct circ_buf capt_buf;
	int capt_buf_size;
	u32 capt_watermark;
	struct wm_adsp_buffer_region *host_regions;
	bool buffer_drain_pending;

	int num_firmwares;
	struct wm_adsp_fw_defs *firmwares;

	struct wm_adsp_fw_features fw_features;

	struct mutex *fw_lock;
	struct work_struct boot_work;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debugfs_root;
	char *wmfw_file_loaded;
	char *bin_file_loaded;
#endif

};

#define WM_ADSP1(wname, num) \
	{ .id = snd_soc_dapm_pga, .name = wname, .reg = SND_SOC_NOPM, \
	.shift = num, .event = wm_adsp1_event, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD }

#define WM_ADSP2(wname, num) \
{	.id = snd_soc_dapm_pga, .name = wname, .reg = SND_SOC_NOPM, \
	.shift = num, .event = wm_adsp2_event, \
	.event_flags = SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD }

extern const struct snd_kcontrol_new wm_adsp1_fw_controls[];
extern const struct snd_kcontrol_new wm_adsp2_fw_controls[];

int wm_adsp1_init(struct wm_adsp *dsp);
int wm_adsp2_init(struct wm_adsp *dsp, struct mutex *fw_lock);

#ifdef CONFIG_DEBUG_FS
void wm_adsp_init_debugfs(struct wm_adsp *dsp, struct snd_soc_codec *codec);
void wm_adsp_cleanup_debugfs(struct wm_adsp *dsp);
#else
static inline void wm_adsp_init_debugfs(struct wm_adsp *dsp,
					struct snd_soc_codec *codec)
{
}

void wm_adsp_cleanup_debugfs(struct wm_adsp *dsp)
{
}
#endif

int wm_adsp1_event(struct snd_soc_dapm_widget *w,
		   struct snd_kcontrol *kcontrol, int event);
int wm_adsp2_event(struct snd_soc_dapm_widget *w,
		   struct snd_kcontrol *kcontrol, int event);

#endif
