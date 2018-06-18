/*
 * Synaptics DSX touchscreen driver
 *
<<<<<<< HEAD
=======
 * Copyright (c) 2014, The Linux Foundation.  All rights reserved.
 *
 * Linux foundation chooses to take subject only to the GPLv2 license terms,
 * and distributes only under these terms.
 *
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
 * Copyright (C) 2012 Synaptics Incorporated
 *
 * Copyright (C) 2012 Alexandra Chin <alexandra.chin@tw.synaptics.com>
 * Copyright (C) 2012 Scott Lin <scott.lin@tw.synaptics.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/input/synaptics_dsx.h>
#include "synaptics_dsx_core.h"

#define SYN_I2C_RETRY_TIMES 10

/*
#define I2C_BURST_LIMIT 255
*/
/*
#define XFER_MSGS_LIMIT 8
*/

static struct synaptics_dsx_hw_interface hw_if;

static struct platform_device *synaptics_dsx_i2c_device;

#ifdef CONFIG_OF
static int parse_dt(struct device *dev, struct synaptics_dsx_board_data *bdata)
{
	int retval;
	u32 value;
	const char *name;
	struct property *prop;
	struct device_node *np = dev->of_node;

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);

	bdata->irq_gpio = of_get_named_gpio_flags(np,
			"synaptics,irq-gpio", 0, NULL);

	retval = of_property_read_u32(np, "synaptics,irq-on-state",
			&value);
	if (retval < 0)
		bdata->irq_on_state = 0;
	else
		bdata->irq_on_state = value;
	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);

	retval = of_property_read_u32(np, "synaptics,irq-flags", &value);
	if (retval < 0)
		return retval;
	else
		bdata->irq_flags = value;

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	retval = of_property_read_string(np, "synaptics,pwr-reg-name", &name);
	if (retval == -EINVAL)
		bdata->pwr_reg_name = NULL;
	else if (retval < 0)
		return retval;
	else
		bdata->pwr_reg_name = name;

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	retval = of_property_read_string(np, "synaptics,bus-reg-name", &name);
	if (retval == -EINVAL)
		bdata->bus_reg_name = NULL;
	else if (retval < 0)
		return retval;
	else
		bdata->bus_reg_name = name;

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	if (of_property_read_bool(np, "synaptics,power-gpio")) {
		bdata->power_gpio = of_get_named_gpio_flags(np,
				"synaptics,power-gpio", 0, NULL);
		retval = of_property_read_u32(np, "synaptics,power-on-state",
				&value);
		if (retval < 0)
			return retval;
		else
			bdata->power_on_state = value;
	} else {
		bdata->power_gpio = -1;
	}

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	if (of_property_read_bool(np, "synaptics,power-delay-ms")) {
		retval = of_property_read_u32(np, "synaptics,power-delay-ms",
				&value);
		if (retval < 0)
			return retval;
		else
			bdata->power_delay_ms = value;
	} else {
		bdata->power_delay_ms = 0;
	}

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	if (of_property_read_bool(np, "synaptics,reset-gpio")) {
		bdata->reset_gpio = of_get_named_gpio_flags(np,
				"synaptics,reset-gpio", 0, NULL);
		retval = of_property_read_u32(np, "synaptics,reset-on-state",
				&value);
		if (retval < 0)
			return retval;
		else
			bdata->reset_on_state = value;
		retval = of_property_read_u32(np, "synaptics,reset-active-ms",
				&value);
		if (retval < 0)
			return retval;
		else
			bdata->reset_active_ms = value;
	} else {
		bdata->reset_gpio = -1;
	}

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	if (of_property_read_bool(np, "synaptics,reset-delay-ms")) {
		retval = of_property_read_u32(np, "synaptics,reset-delay-ms",
				&value);
		if (retval < 0)
			return retval;
		else
			bdata->reset_delay_ms = value;
	} else {
		bdata->reset_delay_ms = 0;
	}

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	if (of_property_read_bool(np, "synaptics,max-y-for-2d")) {
		retval = of_property_read_u32(np, "synaptics,max-y-for-2d",
				&value);
		if (retval < 0)
			return retval;
		else
			bdata->max_y_for_2d = value;
	} else {
		bdata->max_y_for_2d = -1;
	}

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	bdata->swap_axes = of_property_read_bool(np, "synaptics,swap-axes");

	bdata->x_flip = of_property_read_bool(np, "synaptics,x-flip");

	bdata->y_flip = of_property_read_bool(np, "synaptics,y-flip");

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	if (of_property_read_bool(np, "synaptics,ub-i2c-addr")) {
		retval = of_property_read_u32(np, "synaptics,ub-i2c-addr",
				&value);
		if (retval < 0)
			return retval;
		else
			bdata->ub_i2c_addr = (unsigned short)value;
	} else {
		bdata->ub_i2c_addr = -1;
	}

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	prop = of_find_property(np, "synaptics,cap-button-codes", NULL);
	if (prop && prop->length) {
		bdata->cap_button_map->map = devm_kzalloc(dev,
				prop->length,
				GFP_KERNEL);
		if (!bdata->cap_button_map->map)
			return -ENOMEM;
		bdata->cap_button_map->nbuttons = prop->length / sizeof(u32);
		retval = of_property_read_u32_array(np,
				"synaptics,cap-button-codes",
				bdata->cap_button_map->map,
				bdata->cap_button_map->nbuttons);
		if (retval < 0) {
			bdata->cap_button_map->nbuttons = 0;
			bdata->cap_button_map->map = NULL;
		}
	} else {
		bdata->cap_button_map->nbuttons = 0;
		bdata->cap_button_map->map = NULL;
	}

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	prop = of_find_property(np, "synaptics,vir-button-codes", NULL);
	printk(KERN_ERR "%s:JackChen-----%d----- prop = %p\n", __func__, __LINE__, prop);
	if (prop && prop->length) {
		bdata->vir_button_map->map = devm_kzalloc(dev,
				prop->length,
				GFP_KERNEL);
		if (!bdata->vir_button_map->map)
			return -ENOMEM;
		bdata->vir_button_map->nbuttons = prop->length / sizeof(u32);
		bdata->vir_button_map->nbuttons /= 5;
		retval = of_property_read_u32_array(np,
				"synaptics,vir-button-codes",
				bdata->vir_button_map->map,
				bdata->vir_button_map->nbuttons * 5);
		if (retval < 0) {
			bdata->vir_button_map->nbuttons = 0;
			bdata->vir_button_map->map = NULL;
		}
	} else {
		printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
		bdata->vir_button_map->nbuttons = 0;
		bdata->vir_button_map->map = NULL;
	}

	printk(KERN_ERR "%s:JackChen-----%d-----\n", __func__, __LINE__);
	return 0;
}
#endif

static void synaptics_rmi4_i2c_check_addr(struct synaptics_rmi4_data *rmi4_data,
		struct i2c_client *i2c)
{
	if (hw_if.board_data->ub_i2c_addr == -1)
		return;

	if (hw_if.board_data->i2c_addr == i2c->addr)
		hw_if.board_data->i2c_addr = hw_if.board_data->ub_i2c_addr;
	else
		hw_if.board_data->i2c_addr = i2c->addr;

	return;
}
=======
#include <linux/platform_device.h>
#include <linux/input/synaptics_dsx_v2.h>
#include "synaptics_dsx_core.h"
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#if defined(CONFIG_SECURE_TOUCH)
#include <linux/pm_runtime.h>
#endif

#define SYN_I2C_RETRY_TIMES 10
#define RESET_DELAY 100
#define DSX_COORDS_ARR_SIZE	4
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

static int synaptics_rmi4_i2c_set_page(struct synaptics_rmi4_data *rmi4_data,
		unsigned short addr)
{
	int retval;
	unsigned char retry;
	unsigned char buf[PAGE_SELECT_LEN];
	unsigned char page;
	struct i2c_client *i2c = to_i2c_client(rmi4_data->pdev->dev.parent);
<<<<<<< HEAD
	struct i2c_msg msg[1];

	msg[0].addr = hw_if.board_data->i2c_addr;
	msg[0].flags = 0;
	msg[0].len = PAGE_SELECT_LEN;
	msg[0].buf = buf;

	page = ((addr >> 8) & MASK_8BIT);
	buf[0] = MASK_8BIT;
	buf[1] = page;

	if (page != rmi4_data->current_page) {
		for (retry = 0; retry < SYN_I2C_RETRY_TIMES; retry++) {
			if (i2c_transfer(i2c->adapter, msg, 1) == 1) {
				rmi4_data->current_page = page;
				retval = PAGE_SELECT_LEN;
				break;
			}
			dev_err(rmi4_data->pdev->dev.parent,
					"%s: I2C retry %d\n",
					__func__, retry + 1);
			msleep(20);

			if (retry == SYN_I2C_RETRY_TIMES / 2) {
				synaptics_rmi4_i2c_check_addr(rmi4_data, i2c);
				msg[0].addr = hw_if.board_data->i2c_addr;
			}
=======

	page = ((addr >> 8) & MASK_8BIT);
	if (page != rmi4_data->current_page) {
		buf[0] = MASK_8BIT;
		buf[1] = page;
		for (retry = 0; retry < SYN_I2C_RETRY_TIMES; retry++) {
			retval = i2c_master_send(i2c, buf, PAGE_SELECT_LEN);
			if (retval != PAGE_SELECT_LEN) {
				dev_err(rmi4_data->pdev->dev.parent,
						"%s: I2C retry %d\n",
						__func__, retry + 1);
				msleep(20);
			} else {
				rmi4_data->current_page = page;
				break;
			}
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
		}
	} else {
		retval = PAGE_SELECT_LEN;
	}

	return retval;
}

static int synaptics_rmi4_i2c_read(struct synaptics_rmi4_data *rmi4_data,
		unsigned short addr, unsigned char *data, unsigned short length)
{
	int retval;
	unsigned char retry;
	unsigned char buf;
<<<<<<< HEAD
#ifdef I2C_BURST_LIMIT
	unsigned char ii;
	unsigned char rd_msgs = ((length - 1) / I2C_BURST_LIMIT) + 1;
#else
	unsigned char rd_msgs = 1;
#endif
	unsigned char index = 0;
	unsigned char xfer_msgs;
	unsigned char remaining_msgs;
	unsigned short i2c_addr;
	unsigned short data_offset = 0;
	unsigned short remaining_length = length;
	struct i2c_client *i2c = to_i2c_client(rmi4_data->pdev->dev.parent);
	struct i2c_adapter *adap = i2c->adapter;
	struct i2c_msg msg[rd_msgs + 1];
=======
	struct i2c_client *i2c = to_i2c_client(rmi4_data->pdev->dev.parent);
	struct i2c_msg msg[] = {
		{
			.addr = i2c->addr,
			.flags = 0,
			.len = 1,
			.buf = &buf,
		},
		{
			.addr = i2c->addr,
			.flags = I2C_M_RD,
			.len = length,
			.buf = data,
		},
	};

	buf = addr & MASK_8BIT;
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	mutex_lock(&rmi4_data->rmi4_io_ctrl_mutex);

	retval = synaptics_rmi4_i2c_set_page(rmi4_data, addr);
	if (retval != PAGE_SELECT_LEN) {
		retval = -EIO;
		goto exit;
	}

<<<<<<< HEAD
	msg[0].addr = hw_if.board_data->i2c_addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &buf;

#ifdef I2C_BURST_LIMIT
	for (ii = 0; ii < (rd_msgs - 1); ii++) {
		msg[ii + 1].addr = hw_if.board_data->i2c_addr;
		msg[ii + 1].flags = I2C_M_RD;
		msg[ii + 1].len = I2C_BURST_LIMIT;
		msg[ii + 1].buf = &data[data_offset];
		data_offset += I2C_BURST_LIMIT;
		remaining_length -= I2C_BURST_LIMIT;
	}
#endif

	msg[rd_msgs].addr = hw_if.board_data->i2c_addr;
	msg[rd_msgs].flags = I2C_M_RD;
	msg[rd_msgs].len = remaining_length;
	msg[rd_msgs].buf = &data[data_offset];

	buf = addr & MASK_8BIT;

	remaining_msgs = rd_msgs + 1;

	while (remaining_msgs) {
#ifdef XFER_MSGS_LIMIT
		if (remaining_msgs > XFER_MSGS_LIMIT)
			xfer_msgs = XFER_MSGS_LIMIT;
		else
			xfer_msgs = remaining_msgs;
#else
		xfer_msgs = remaining_msgs;
#endif
		for (retry = 0; retry < SYN_I2C_RETRY_TIMES; retry++) {
			retval = i2c_transfer(adap, &msg[index], xfer_msgs);
			if (retval == xfer_msgs)
				break;

			dev_err(rmi4_data->pdev->dev.parent,
					"%s: I2C retry %d\n",
					__func__, retry + 1);
			msleep(20);

			if (retry == SYN_I2C_RETRY_TIMES / 2) {
				synaptics_rmi4_i2c_check_addr(rmi4_data, i2c);
				i2c_addr = hw_if.board_data->i2c_addr;
				msg[0].addr = i2c_addr;
#ifdef I2C_BURST_LIMIT
				for (ii = 0; ii < (rd_msgs - 1); ii++)
					msg[ii + 1].addr = i2c_addr;
#endif
				msg[rd_msgs].addr = i2c_addr;
			}
		}

		if (retry == SYN_I2C_RETRY_TIMES) {
			dev_err(rmi4_data->pdev->dev.parent,
					"%s: I2C read over retry limit\n",
					__func__);
			retval = -EIO;
			goto exit;
		}

		remaining_msgs -= xfer_msgs;
		index += xfer_msgs;
	}

	retval = length;
=======
	for (retry = 0; retry < SYN_I2C_RETRY_TIMES; retry++) {
		if (i2c_transfer(i2c->adapter, msg, 2) == 2) {
			retval = length;
			break;
		}
		dev_err(rmi4_data->pdev->dev.parent,
				"%s: I2C retry %d\n",
				__func__, retry + 1);
		msleep(20);
	}

	if (retry == SYN_I2C_RETRY_TIMES) {
		dev_err(rmi4_data->pdev->dev.parent,
				"%s: I2C read over retry limit\n",
				__func__);
		retval = -EIO;
	}
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

exit:
	mutex_unlock(&rmi4_data->rmi4_io_ctrl_mutex);

	return retval;
}

static int synaptics_rmi4_i2c_write(struct synaptics_rmi4_data *rmi4_data,
		unsigned short addr, unsigned char *data, unsigned short length)
{
	int retval;
	unsigned char retry;
	unsigned char buf[length + 1];
	struct i2c_client *i2c = to_i2c_client(rmi4_data->pdev->dev.parent);
<<<<<<< HEAD
	struct i2c_msg msg[1];
=======
	struct i2c_msg msg[] = {
		{
			.addr = i2c->addr,
			.flags = 0,
			.len = length + 1,
			.buf = buf,
		}
	};
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	mutex_lock(&rmi4_data->rmi4_io_ctrl_mutex);

	retval = synaptics_rmi4_i2c_set_page(rmi4_data, addr);
	if (retval != PAGE_SELECT_LEN) {
		retval = -EIO;
		goto exit;
	}

<<<<<<< HEAD
	msg[0].addr = hw_if.board_data->i2c_addr;
	msg[0].flags = 0;
	msg[0].len = length + 1;
	msg[0].buf = buf;

	buf[0] = addr & MASK_8BIT;
	retval = secure_memcpy(&buf[1], length, &data[0], length, length);
	if (retval < 0) {
		dev_err(rmi4_data->pdev->dev.parent,
				"%s: Failed to copy data\n",
				__func__);
		goto exit;
	}
=======
	buf[0] = addr & MASK_8BIT;
	memcpy(&buf[1], &data[0], length);
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	for (retry = 0; retry < SYN_I2C_RETRY_TIMES; retry++) {
		if (i2c_transfer(i2c->adapter, msg, 1) == 1) {
			retval = length;
			break;
		}
		dev_err(rmi4_data->pdev->dev.parent,
				"%s: I2C retry %d\n",
				__func__, retry + 1);
		msleep(20);
<<<<<<< HEAD

		if (retry == SYN_I2C_RETRY_TIMES / 2) {
			synaptics_rmi4_i2c_check_addr(rmi4_data, i2c);
			msg[0].addr = hw_if.board_data->i2c_addr;
		}
=======
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
	}

	if (retry == SYN_I2C_RETRY_TIMES) {
		dev_err(rmi4_data->pdev->dev.parent,
				"%s: I2C write over retry limit\n",
				__func__);
		retval = -EIO;
	}

exit:
	mutex_unlock(&rmi4_data->rmi4_io_ctrl_mutex);

	return retval;
}

<<<<<<< HEAD
=======
#if defined(CONFIG_SECURE_TOUCH)
static int synaptics_rmi4_clk_prepare_enable(
		struct synaptics_rmi4_data *rmi4_data)
{
	int ret;
	ret = clk_prepare_enable(rmi4_data->iface_clk);
	if (ret) {
		dev_err(rmi4_data->pdev->dev.parent,
			"error on clk_prepare_enable(iface_clk):%d\n", ret);
		return ret;
	}

	ret = clk_prepare_enable(rmi4_data->core_clk);
	if (ret) {
		clk_disable_unprepare(rmi4_data->iface_clk);
		dev_err(rmi4_data->pdev->dev.parent,
			"error clk_prepare_enable(core_clk):%d\n", ret);
	}
	return ret;
}

static void synaptics_rmi4_clk_disable_unprepare(
		struct synaptics_rmi4_data *rmi4_data)
{
	clk_disable_unprepare(rmi4_data->core_clk);
	clk_disable_unprepare(rmi4_data->iface_clk);
}

static int synaptics_rmi4_i2c_get(struct synaptics_rmi4_data *rmi4_data)
{
	int retval;
	struct i2c_client *i2c = to_i2c_client(rmi4_data->pdev->dev.parent);

	mutex_lock(&rmi4_data->rmi4_io_ctrl_mutex);
	retval = pm_runtime_get_sync(i2c->adapter->dev.parent);
	if (retval >= 0) {
		retval = synaptics_rmi4_clk_prepare_enable(rmi4_data);
		if (retval)
			pm_runtime_put_sync(i2c->adapter->dev.parent);
	}
	mutex_unlock(&rmi4_data->rmi4_io_ctrl_mutex);

	return retval;
}

static void synaptics_rmi4_i2c_put(struct synaptics_rmi4_data *rmi4_data)
{
	struct i2c_client *i2c = to_i2c_client(rmi4_data->pdev->dev.parent);

	mutex_lock(&rmi4_data->rmi4_io_ctrl_mutex);
	synaptics_rmi4_clk_disable_unprepare(rmi4_data);
	pm_runtime_put_sync(i2c->adapter->dev.parent);
	mutex_unlock(&rmi4_data->rmi4_io_ctrl_mutex);
}
#endif

>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
static struct synaptics_dsx_bus_access bus_access = {
	.type = BUS_I2C,
	.read = synaptics_rmi4_i2c_read,
	.write = synaptics_rmi4_i2c_write,
<<<<<<< HEAD
};

=======
#if defined(CONFIG_SECURE_TOUCH)
	.get = synaptics_rmi4_i2c_get,
	.put = synaptics_rmi4_i2c_put,
#endif
};

static struct synaptics_dsx_hw_interface hw_if;

static struct platform_device *synaptics_dsx_i2c_device;

>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
static void synaptics_rmi4_i2c_dev_release(struct device *dev)
{
	kfree(synaptics_dsx_i2c_device);

	return;
}
<<<<<<< HEAD
=======
#ifdef CONFIG_OF
int synaptics_dsx_get_dt_coords(struct device *dev, char *name,
				struct synaptics_dsx_board_data *pdata,
				struct device_node *node)
{
	u32 coords[DSX_COORDS_ARR_SIZE];
	struct property *prop;
	struct device_node *np = (node == NULL) ? (dev->of_node) : (node);
	int coords_size, rc;

	prop = of_find_property(np, name, NULL);
	if (!prop)
		return -EINVAL;
	if (!prop->value)
		return -ENODATA;

	coords_size = prop->length / sizeof(u32);
	if (coords_size != DSX_COORDS_ARR_SIZE) {
		dev_err(dev, "invalid %s\n", name);
		return -EINVAL;
	}

	rc = of_property_read_u32_array(np, name, coords, coords_size);
	if (rc && (rc != -EINVAL)) {
		dev_err(dev, "Unable to read %s\n", name);
		return rc;
	}

	if (strcmp(name, "synaptics,panel-coords") == 0) {
		pdata->panel_minx = coords[0];
		pdata->panel_miny = coords[1];
		pdata->panel_maxx = coords[2];
		pdata->panel_maxy = coords[3];
	} else if (strcmp(name, "synaptics,display-coords") == 0) {
		pdata->disp_minx = coords[0];
		pdata->disp_miny = coords[1];
		pdata->disp_maxx = coords[2];
		pdata->disp_maxy = coords[3];
	} else {
		dev_err(dev, "unsupported property %s\n", name);
		return -EINVAL;
	}

	return 0;
}

static int synaptics_dsx_parse_dt(struct device *dev,
				struct synaptics_dsx_board_data *rmi4_pdata)
{
	struct device_node *np = dev->of_node;
	struct property *prop;
	u32 temp_val, num_buttons;
	u32 button_map[MAX_NUMBER_OF_BUTTONS];
	int rc, i;

	rmi4_pdata->x_flip = of_property_read_bool(np, "synaptics,x-flip");
	rmi4_pdata->y_flip = of_property_read_bool(np, "synaptics,y-flip");

	rmi4_pdata->disable_gpios = of_property_read_bool(np,
			"synaptics,disable-gpios");

	rmi4_pdata->reset_delay_ms = RESET_DELAY;
	rc = of_property_read_u32(np, "synaptics,reset-delay-ms", &temp_val);
	if (!rc)
		rmi4_pdata->reset_delay_ms = temp_val;
	else if (rc != -EINVAL) {
		dev_err(dev, "Unable to read reset delay\n");
		return rc;
	}

	rmi4_pdata->fw_name = "PRXXX_fw.img";
	rc = of_property_read_string(np, "synaptics,fw-name",
					&rmi4_pdata->fw_name);
	if (rc && (rc != -EINVAL)) {
		dev_err(dev, "Unable to read fw name\n");
		return rc;
	}

	/* reset, irq gpio info */
	rmi4_pdata->reset_gpio = of_get_named_gpio_flags(np,
			"synaptics,reset-gpio", 0, &rmi4_pdata->reset_flags);
	rmi4_pdata->irq_gpio = of_get_named_gpio_flags(np,
			"synaptics,irq-gpio", 0, &rmi4_pdata->irq_flags);

	rc = synaptics_dsx_get_dt_coords(dev, "synaptics,display-coords",
				rmi4_pdata, NULL);
	if (rc && (rc != -EINVAL))
		return rc;

	rc = synaptics_dsx_get_dt_coords(dev, "synaptics,panel-coords",
				rmi4_pdata, NULL);
	if (rc && (rc != -EINVAL))
		return rc;

	rmi4_pdata->detect_device = of_property_read_bool(np,
				"synaptics,detect-device");

	if (rmi4_pdata->detect_device)
		return 0;

	prop = of_find_property(np, "synaptics,button-map", NULL);
	if (prop) {
		num_buttons = prop->length / sizeof(temp_val);

		rmi4_pdata->cap_button_map = devm_kzalloc(dev,
			sizeof(*rmi4_pdata->cap_button_map),
			GFP_KERNEL);
		if (!rmi4_pdata->cap_button_map)
			return -ENOMEM;

		rmi4_pdata->cap_button_map->map = devm_kzalloc(dev,
			sizeof(*rmi4_pdata->cap_button_map->map) *
			MAX_NUMBER_OF_BUTTONS, GFP_KERNEL);
		if (!rmi4_pdata->cap_button_map->map)
			return -ENOMEM;

		if (num_buttons <= MAX_NUMBER_OF_BUTTONS) {
			rc = of_property_read_u32_array(np,
				"synaptics,button-map", button_map,
				num_buttons);
			if (rc) {
				dev_err(dev, "Unable to read key codes\n");
				return rc;
			}
			for (i = 0; i < num_buttons; i++)
				rmi4_pdata->cap_button_map->map[i] =
					button_map[i];
			rmi4_pdata->cap_button_map->nbuttons =
				num_buttons;
		} else {
			return -EINVAL;
		}
	}
	return 0;
}
#else
static inline int synaptics_dsx_parse_dt(struct device *dev,
				struct synaptics_dsx_board_data *rmi4_pdata)
{
	return 0;
}
#endif
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

static int synaptics_rmi4_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *dev_id)
{
	int retval;
<<<<<<< HEAD
=======
	struct synaptics_dsx_board_data *platform_data;
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	if (!i2c_check_functionality(client->adapter,
			I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_err(&client->dev,
				"%s: SMBus byte data commands not supported by host\n",
				__func__);
		return -EIO;
	}

<<<<<<< HEAD
=======
	if (client->dev.of_node) {
		platform_data = devm_kzalloc(&client->dev,
			sizeof(struct synaptics_dsx_board_data),
			GFP_KERNEL);
		if (!platform_data) {
			dev_err(&client->dev, "Failed to allocate memory\n");
			return -ENOMEM;
		}

		retval = synaptics_dsx_parse_dt(&client->dev, platform_data);
		if (retval)
			return retval;
	} else {
		platform_data = client->dev.platform_data;
	}

	if (!platform_data) {
		dev_err(&client->dev,
				"%s: No platform data found\n",
				__func__);
		return -EINVAL;
	}

>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
	synaptics_dsx_i2c_device = kzalloc(
			sizeof(struct platform_device),
			GFP_KERNEL);
	if (!synaptics_dsx_i2c_device) {
		dev_err(&client->dev,
				"%s: Failed to allocate memory for synaptics_dsx_i2c_device\n",
				__func__);
		return -ENOMEM;
	}

<<<<<<< HEAD
#ifdef CONFIG_OF
	if (client->dev.of_node) {
		hw_if.board_data = devm_kzalloc(&client->dev,
				sizeof(struct synaptics_dsx_board_data),
				GFP_KERNEL);
		if (!hw_if.board_data) {
			dev_err(&client->dev,
					"%s: Failed to allocate memory for board data\n",
					__func__);
			return -ENOMEM;
		}
		hw_if.board_data->cap_button_map = devm_kzalloc(&client->dev,
				sizeof(struct synaptics_dsx_button_map),
				GFP_KERNEL);
		if (!hw_if.board_data->cap_button_map) {
			dev_err(&client->dev,
					"%s: Failed to allocate memory for 0D button map\n",
					__func__);
			return -ENOMEM;
		}
		hw_if.board_data->vir_button_map = devm_kzalloc(&client->dev,
				sizeof(struct synaptics_dsx_button_map),
				GFP_KERNEL);
		if (!hw_if.board_data->vir_button_map) {
			dev_err(&client->dev,
					"%s: Failed to allocate memory for virtual button map\n",
					__func__);
			return -ENOMEM;
		}
		parse_dt(&client->dev, hw_if.board_data);
	}
#else
	hw_if.board_data = client->dev.platform_data;
#endif

	hw_if.bus_access = &bus_access;
	hw_if.board_data->i2c_addr = client->addr;
=======
	hw_if.board_data = platform_data;
	hw_if.bus_access = &bus_access;
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130

	synaptics_dsx_i2c_device->name = PLATFORM_DRIVER_NAME;
	synaptics_dsx_i2c_device->id = 0;
	synaptics_dsx_i2c_device->num_resources = 0;
	synaptics_dsx_i2c_device->dev.parent = &client->dev;
	synaptics_dsx_i2c_device->dev.platform_data = &hw_if;
	synaptics_dsx_i2c_device->dev.release = synaptics_rmi4_i2c_dev_release;

	retval = platform_device_register(synaptics_dsx_i2c_device);
	if (retval) {
		dev_err(&client->dev,
				"%s: Failed to register platform device\n",
				__func__);
		return -ENODEV;
	}

	return 0;
}

static int synaptics_rmi4_i2c_remove(struct i2c_client *client)
{
	platform_device_unregister(synaptics_dsx_i2c_device);

	return 0;
}

static const struct i2c_device_id synaptics_rmi4_id_table[] = {
	{I2C_DRIVER_NAME, 0},
	{},
};
MODULE_DEVICE_TABLE(i2c, synaptics_rmi4_id_table);

#ifdef CONFIG_OF
<<<<<<< HEAD
static struct of_device_id synaptics_rmi4_of_match_table[] = {
	{
		.compatible = "synaptics,dsx-i2c",
	},
	{},
};
MODULE_DEVICE_TABLE(of, synaptics_rmi4_of_match_table);
#else
#define synaptics_rmi4_of_match_table NULL
=======
static struct of_device_id dsx_match_table[] = {
	{ .compatible = "synaptics,dsx",},
	{ },
};
#else
#define dsx_match_table NULL
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
#endif

static struct i2c_driver synaptics_rmi4_i2c_driver = {
	.driver = {
		.name = I2C_DRIVER_NAME,
		.owner = THIS_MODULE,
<<<<<<< HEAD
		.of_match_table = synaptics_rmi4_of_match_table,
=======
		.of_match_table = dsx_match_table,
>>>>>>> 87066d33ef6e4347ea24108260bbbe3b944ef130
	},
	.probe = synaptics_rmi4_i2c_probe,
	.remove = synaptics_rmi4_i2c_remove,
	.id_table = synaptics_rmi4_id_table,
};

int synaptics_rmi4_bus_init(void)
{
	return i2c_add_driver(&synaptics_rmi4_i2c_driver);
}
EXPORT_SYMBOL(synaptics_rmi4_bus_init);

void synaptics_rmi4_bus_exit(void)
{
	i2c_del_driver(&synaptics_rmi4_i2c_driver);

	return;
}
EXPORT_SYMBOL(synaptics_rmi4_bus_exit);

MODULE_AUTHOR("Synaptics, Inc.");
MODULE_DESCRIPTION("Synaptics DSX I2C Bus Support Module");
MODULE_LICENSE("GPL v2");
