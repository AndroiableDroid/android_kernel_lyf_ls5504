/*
<<<<<<< HEAD
*Copyright (c) 2014-2015, The Linux Foundation. All rights reserved.
=======
*Copyright (c) 2014, The Linux Foundation. All rights reserved.
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
*
*This program is free software; you can redistribute it and/or modify
*it under the terms of the GNU General Public License version 2 and
*only version 2 as published by the Free Software Foundation.
*
*This program is distributed in the hope that it will be useful,
*but WITHOUT ANY WARRANTY; without even the implied warranty of
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*GNU General Public License for more details.
*/
#include <linux/debugfs.h>
#include <linux/device.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/msm-bus.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "devfreq_spdm.h"
#include "governor.h"
=======
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <soc/qcom/hvc.h>
#include "devfreq_spdm.h"
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

static int spdm_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}

<<<<<<< HEAD
static char buf[PAGE_SIZE];

static ssize_t enable_write(struct file *file, const char __user *data,
			    size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i;
	int next_idx;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		goto err;
		size = -EINVAL;
	}

	if (sscanf(buf, "%u\n", &i) != 1) {
		size = -EINVAL;
		goto err;
	}
	i = !!i;

	if (i == spdm_data->enabled)
		goto out;

	spdm_data->devfreq->governor->event_handler(spdm_data->devfreq,
						    i ? DEVFREQ_GOV_START :
						    DEVFREQ_GOV_STOP, NULL);

	if (!i) {
		next_idx = spdm_data->cur_idx + 1;
		next_idx = next_idx % 2;

		for (i = 0; i < spdm_data->pdata->usecase[next_idx].num_paths;
		     i++)
			spdm_data->pdata->usecase[next_idx].vectors[i].ab = 0;

		spdm_data->cur_idx = next_idx;
		msm_bus_scale_client_update_request
		    (spdm_data->bus_scale_client_id, spdm_data->cur_idx);
	}

out:
	*offset += size;
err:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t enable_read(struct file *file, char __user *data,
			   size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int len = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	len = scnprintf(buf, size, "%u\n", spdm_data->enabled);
	len = simple_read_from_buffer(data, size, offset, buf, len);

	memset(buf, 0, sizeof(buf));
	return len;
}

static const struct file_operations enable_fops = {
	.open = spdm_open,
	.write = enable_write,
	.read = enable_read,
};

static ssize_t pl_write(struct file *file, const char __user *data,
			size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;
	int i;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}

	if (sscanf(buf, "%u %u\n", &spdm_data->config_data.pl_freqs[0],
	       &spdm_data->config_data.pl_freqs[1]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
static ssize_t pl_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;
	int i;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;

	sscanf(buf, "%u %u\n", &spdm_data->config_data.pl_freqs[0],
		&spdm_data->config_data.pl_freqs[1]);

>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_PL;
	desc.arg[1] = spdm_data->spdm_client;
	for (i = 0; i < SPDM_PL_COUNT - 1; i++)
		desc.arg[i+2] = spdm_data->config_data.pl_freqs[i];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, SPDM_PL_COUNT + 1);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;

}

static ssize_t pl_read(struct file *file, char __user *data,
		       size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n", spdm_data->config_data.pl_freqs[0],
		     spdm_data->config_data.pl_freqs[1]);
	i = simple_read_from_buffer(data, size, offset, buf, i);

	memset(buf, 0, sizeof(buf));
	return i;
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
}

static const struct file_operations pl_fops = {
	.open = spdm_open,
	.write = pl_write,
<<<<<<< HEAD
	.read = pl_read,
};

static ssize_t rejrate_low_write(struct file *file, const char __user *data,
				 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}

	if (sscanf(buf, "%u %u\n", &spdm_data->config_data.reject_rate[0],
	       &spdm_data->config_data.reject_rate[1]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
};

static ssize_t rejrate_low_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;

	sscanf(buf, "%u %u\n", &spdm_data->config_data.reject_rate[0],
		&spdm_data->config_data.reject_rate[1]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_REJRATE_LOW;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.reject_rate[0];
	desc.arg[3] = spdm_data->config_data.reject_rate[1];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t rejrate_low_read(struct file *file, char __user *data,
				size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.reject_rate[0],
		     spdm_data->config_data.reject_rate[1]);

	i = simple_read_from_buffer(data, size, offset, buf, i);

	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations rrl_fops = {
	.open = spdm_open,
	.write = rejrate_low_write,
	.read = rejrate_low_read,
};

static ssize_t rejrate_med_write(struct file *file, const char __user *data,
				 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u\n", &spdm_data->config_data.reject_rate[2],
	       &spdm_data->config_data.reject_rate[3]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations rrl_fops = {
	.open = spdm_open,
	.write = rejrate_low_write,
};

static ssize_t rejrate_med_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u\n", &spdm_data->config_data.reject_rate[2],
		&spdm_data->config_data.reject_rate[3]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_REJRATE_MED;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.reject_rate[2];
	desc.arg[3] = spdm_data->config_data.reject_rate[3];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t rejrate_med_read(struct file *file, char __user *data,
				size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.reject_rate[2],
		     spdm_data->config_data.reject_rate[3]);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations rrm_fops = {
	.open = spdm_open,
	.write = rejrate_med_write,
	.read = rejrate_med_read,
};

static ssize_t rejrate_high_write(struct file *file, const char __user *data,
				  size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u\n", &spdm_data->config_data.reject_rate[4],
	       &spdm_data->config_data.reject_rate[5]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations rrm_fops = {
	.open = spdm_open,
	.write = rejrate_med_write,
};

static ssize_t rejrate_high_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u\n", &spdm_data->config_data.reject_rate[4],
		&spdm_data->config_data.reject_rate[5]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_REJRATE_HIGH;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.reject_rate[4];
	desc.arg[3] = spdm_data->config_data.reject_rate[5];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t rejrate_high_read(struct file *file, char __user *data,
				 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.reject_rate[4],
		     spdm_data->config_data.reject_rate[5]);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations rrh_fops = {
	.open = spdm_open,
	.write = rejrate_high_write,
	.read = rejrate_high_read,
};

static ssize_t resptime_low_write(struct file *file, const char __user *data,
				  size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u\n", &spdm_data->config_data.response_time_us[0],
	       &spdm_data->config_data.response_time_us[1]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations rrh_fops = {
	.open = spdm_open,
	.write = rejrate_high_write,
};

static ssize_t resptime_low_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u\n", &spdm_data->config_data.response_time_us[0],
		&spdm_data->config_data.response_time_us[1]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_RESPTIME_LOW;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.response_time_us[0];
	desc.arg[3] = spdm_data->config_data.response_time_us[1];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t resptime_low_read(struct file *file, char __user *data,
				 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.response_time_us[0],
		     spdm_data->config_data.response_time_us[1]);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations rtl_fops = {
	.open = spdm_open,
	.write = resptime_low_write,
	.read = resptime_low_read,
};

static ssize_t resptime_med_write(struct file *file, const char __user *data,
				  size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u\n", &spdm_data->config_data.response_time_us[2],
	       &spdm_data->config_data.response_time_us[3]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations rtl_fops = {
	.open = spdm_open,
	.write = resptime_low_write,
};

static ssize_t resptime_med_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u\n", &spdm_data->config_data.response_time_us[2],
		&spdm_data->config_data.response_time_us[3]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_RESPTIME_MED;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.response_time_us[2];
	desc.arg[3] = spdm_data->config_data.response_time_us[3];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t resptime_med_read(struct file *file, char __user *data,
				 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.response_time_us[2],
		     spdm_data->config_data.response_time_us[3]);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations rtm_fops = {
	.open = spdm_open,
	.write = resptime_med_write,
	.read = resptime_med_read,
};

static ssize_t resptime_high_write(struct file *file, const char __user *data,
				   size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u\n", &spdm_data->config_data.response_time_us[4],
	       &spdm_data->config_data.response_time_us[5]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations rtm_fops = {
	.open = spdm_open,
	.write = resptime_med_write,
};

static ssize_t resptime_high_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u\n", &spdm_data->config_data.response_time_us[4],
		&spdm_data->config_data.response_time_us[5]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_RESPTIME_HIGH;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.response_time_us[4];
	desc.arg[3] = spdm_data->config_data.response_time_us[5];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t resptime_high_read(struct file *file, char __user *data,
				  size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.response_time_us[4],
		     spdm_data->config_data.response_time_us[5]);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations rth_fops = {
	.open = spdm_open,
	.write = resptime_high_write,
	.read = resptime_high_read,
};

static ssize_t cciresptime_low_write(struct file *file,
				     const char __user *data, size_t size,
				     loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u\n",
		   &spdm_data->config_data.cci_response_time_us[0],
		   &spdm_data->config_data.cci_response_time_us[1]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations rth_fops = {
	.open = spdm_open,
	.write = resptime_high_write,
};

static ssize_t cciresptime_low_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u\n", &spdm_data->config_data.cci_response_time_us[0],
		&spdm_data->config_data.cci_response_time_us[1]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_CCIRESPTIME_LOW;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.cci_response_time_us[0];
	desc.arg[3] = spdm_data->config_data.cci_response_time_us[1];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t cciresptime_low_read(struct file *file, char __user *data,
				    size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.cci_response_time_us[0],
		     spdm_data->config_data.cci_response_time_us[1]);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations ccil_fops = {
	.open = spdm_open,
	.write = cciresptime_low_write,
	.read = cciresptime_low_read,
};

static ssize_t cciresptime_med_write(struct file *file,
				     const char __user *data, size_t size,
				     loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u\n",
		   &spdm_data->config_data.cci_response_time_us[2],
		   &spdm_data->config_data.cci_response_time_us[3]) != 2) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations ccil_fops = {
	.open = spdm_open,
	.write = cciresptime_low_write,
};

static ssize_t cciresptime_med_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u\n", &spdm_data->config_data.cci_response_time_us[2],
		&spdm_data->config_data.cci_response_time_us[3]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_CCIRESPTIME_MED;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.cci_response_time_us[2];
	desc.arg[3] = spdm_data->config_data.cci_response_time_us[3];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t cciresptime_med_read(struct file *file, char __user *data,
				    size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.cci_response_time_us[2],
		     spdm_data->config_data.cci_response_time_us[3]);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations ccim_fops = {
	.open = spdm_open,
	.write = cciresptime_med_write,
	.read = cciresptime_med_read,
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations ccim_fops = {
	.open = spdm_open,
	.write = cciresptime_med_write,
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
};

static ssize_t cciresptime_high_write(struct file *file,
				      const char __user *data,
				      size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
<<<<<<< HEAD
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u\n",
		   &spdm_data->config_data.cci_response_time_us[4],
		   &spdm_data->config_data.cci_response_time_us[5]) != 2){
		size = -EINVAL;
		goto out;
	}
=======
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u\n", &spdm_data->config_data.cci_response_time_us[4],
		&spdm_data->config_data.cci_response_time_us[5]);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_CCIRESPTIME_HIGH;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.cci_response_time_us[4];
	desc.arg[3] = spdm_data->config_data.cci_response_time_us[5];
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 4);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t cciresptime_high_read(struct file *file, char __user *data,
				     size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u\n",
		     spdm_data->config_data.cci_response_time_us[4],
		     spdm_data->config_data.cci_response_time_us[5]);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations ccih_fops = {
	.open = spdm_open,
	.write = cciresptime_high_write,
	.read = cciresptime_high_read,
};

static ssize_t cci_max_write(struct file *file, const char __user *data,
			     size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u\n", &spdm_data->config_data.max_cci_freq) != 1) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations ccih_fops = {
	.open = spdm_open,
	.write = cciresptime_high_write,
};

static ssize_t cci_max_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u\n", &spdm_data->config_data.max_cci_freq);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_MAXCCI;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.max_cci_freq;
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 3);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t cci_max_read(struct file *file, char __user *data,
			    size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u\n", spdm_data->config_data.max_cci_freq);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations ccimax_fops = {
	.open = spdm_open,
	.write = cci_max_write,
	.read = cci_max_read,
};

static ssize_t vote_cfg_write(struct file *file, const char __user *data,
			      size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	struct spdm_args desc = { { 0 } };
	int ext_status = 0;

	if (size > sizeof(buf))
		return -EINVAL;

	if (copy_from_user(buf, data, size)) {
		size = -EINVAL;
		goto out;
	}
	if (sscanf(buf, "%u %u %u %u\n", &spdm_data->config_data.upstep,
	       &spdm_data->config_data.downstep,
	       &spdm_data->config_data.max_vote,
	       &spdm_data->config_data.up_step_multp) != 4) {
		size = -EINVAL;
		goto out;
	}
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations ccimax_fops = {
	.open = spdm_open,
	.write = cci_max_write,
};

static ssize_t vote_cfg_write(struct file *file, const char __user *data,
		 size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	char *buf;
	struct hvc_desc desc;

	buf = kzalloc(size, GFP_KERNEL);

	if (!buf)
		return -ENOMEM;
	if (copy_from_user(buf, data, size))
		return -EINVAL;
	sscanf(buf, "%u %u %u %u\n", &spdm_data->config_data.upstep,
		&spdm_data->config_data.downstep,
		&spdm_data->config_data.max_vote,
		&spdm_data->config_data.up_step_multp);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	desc.arg[0] = SPDM_CMD_CFG_VOTES;
	desc.arg[1] = spdm_data->spdm_client;
	desc.arg[2] = spdm_data->config_data.upstep;
	desc.arg[3] = spdm_data->config_data.downstep;
	desc.arg[4] = spdm_data->config_data.max_vote;
	desc.arg[5] = spdm_data->config_data.up_step_multp;
<<<<<<< HEAD
	ext_status = spdm_ext_call(&desc, 6);
	if (ext_status)
		pr_err("External command %u failed with error %u",
			(int)desc.arg[0], ext_status);
	*offset += size;
out:
	memset(buf, 0, sizeof(buf));
	return size;
}

static ssize_t vote_cfg_read(struct file *file, char __user *data,
			     size_t size, loff_t *offset)
{
	struct spdm_data *spdm_data = file->private_data;
	int i = 32;

	if (size > sizeof(buf))
		return -EINVAL;

	i = scnprintf(buf, size, "%u %u %u %u\n",
		     spdm_data->config_data.upstep,
		     spdm_data->config_data.downstep,
		     spdm_data->config_data.max_vote,
		     spdm_data->config_data.up_step_multp);

	i = simple_read_from_buffer(data, size, offset, buf, i);
	memset(buf, 0, sizeof(buf));
	return i;
}

static const struct file_operations vote_fops = {
	.open = spdm_open,
	.write = vote_cfg_write,
	.read = vote_cfg_read,
=======
	if (hvc(HVC_FN_SIP(SPDM_HYP_FNID), &desc))
		pr_debug("check hvc logs");
	*offset += size;
	kfree(buf);
	return size;
}

static const struct file_operations vote_fops = {
	.open = spdm_open,
	.write = vote_cfg_write,
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
};

void spdm_init_debugfs(struct device *dev)
{
	struct spdm_data *data = 0;

	data = dev_get_drvdata(dev);
	data->debugfs_dir = debugfs_create_dir(dev_name(dev), NULL);

<<<<<<< HEAD
	debugfs_create_file("enable", 0600, data->debugfs_dir, data,
			    &enable_fops);
	debugfs_create_file("pl_freqs", 0600, data->debugfs_dir, data,
			    &pl_fops);
	debugfs_create_file("rej_rate_low", 0600, data->debugfs_dir, data,
			    &rrl_fops);
	debugfs_create_file("rej_rate_med", 0600, data->debugfs_dir, data,
			    &rrm_fops);
	debugfs_create_file("rej_rate_high", 0600, data->debugfs_dir, data,
			    &rrh_fops);
	debugfs_create_file("resp_time_low", 0600, data->debugfs_dir, data,
			    &rtl_fops);
	debugfs_create_file("resp_time_med", 0600, data->debugfs_dir, data,
			    &rtm_fops);
	debugfs_create_file("resp_time_high", 0600, data->debugfs_dir, data,
			    &rth_fops);
	debugfs_create_file("cci_resp_time_low", 0600, data->debugfs_dir, data,
			    &ccil_fops);
	debugfs_create_file("cci_resp_time_med", 0600, data->debugfs_dir, data,
			    &ccim_fops);
	debugfs_create_file("cci_resp_time_high", 0600, data->debugfs_dir,
			    data, &ccih_fops);
	debugfs_create_file("cci_max", 0600, data->debugfs_dir, data,
			    &ccimax_fops);
	debugfs_create_file("vote_cfg", 0600, data->debugfs_dir, data,
			    &vote_fops);
=======
	debugfs_create_file("pl_freqs", 0x700, data->debugfs_dir, data,
			    &pl_fops);
	debugfs_create_file("rej_rate_low", 0x700, data->debugfs_dir, data,
			    &rrl_fops);
	debugfs_create_file("rej_rate_med", 0x700, data->debugfs_dir, data,
			    &rrm_fops);
	debugfs_create_file("rej_rate_high", 0x700, data->debugfs_dir, data,
			    &rrh_fops);
	debugfs_create_file("resp_time_low", 0x700, data->debugfs_dir, data,
			    &rtl_fops);
	debugfs_create_file("resp_time_med", 0x700, data->debugfs_dir, data,
			    &rtm_fops);
	debugfs_create_file("resp_time_high", 0x700, data->debugfs_dir, data,
			    &rth_fops);
	debugfs_create_file("cci_resp_time_low", 0x700, data->debugfs_dir, data,
			    &ccil_fops);
	debugfs_create_file("cci_resp_time_med", 0x700, data->debugfs_dir, data,
			    &ccim_fops);
	debugfs_create_file("cci_resp_time_high", 0x700, data->debugfs_dir,
			data, &ccih_fops);
	debugfs_create_file("cci_max", 0x700, data->debugfs_dir, data,
			&ccimax_fops);
	debugfs_create_file("vote_cfg", 0x700, data->debugfs_dir, data,
			&vote_fops);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
}

void spdm_remove_debugfs(struct spdm_data *data)
{
	debugfs_remove_recursive(data->debugfs_dir);
}

MODULE_LICENSE("GPL v2");
