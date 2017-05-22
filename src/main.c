/*
 *  Copyright 2017 Justin Watson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file Sample application using the MCP4725 DAC over I2C/TWI interface.
 */

#include <zephyr.h>
#include <device.h>
#include <misc/printk.h>
#include <i2c.h>
#include <kernel.h>

/*
 * Look at Figure 6-1. The address bits A2 and A1 are programmed at the factoy.
 * To determine your A2 and A1 values read section 7.2 Device Addressing.
 */
#define DAC_I2C_ADDR 0xc0

void main(void)
{
	printk("MCP4725 Zephyr Sample Project.\n");

	struct device *i2c_dev;
	int result;

	i2c_dev = device_get_binding("I2C_0");
	if (!i2c_dev) {
		printk("I2C: Device driver not found.");
		return;
	}

	/*
	 * The MCP4725 can run at 1 of 3 speeds.
	 * 100 kHz, 400 kHz, 3.4 MHz
	 * TWI0, PA18 CK0, PA17 D0
	 */
	result = i2c_configure(i2c_dev, I2C_SPEED_FAST | I2C_MODE_MASTER);
	if (!result) {
		printk("I2C: Could not be configured.");
		return;
	}

	/* Send state changes each iteration so we make a square wave. */
	int send_state;
	struct i2c_msg msg;
	u8_t i2c_data[2];

	send_state = 0;

	while (1) {
		if (send_state) {
			/*
			 * Packet structure is on page 24 of MCP4725 datasheet.
			 * Use Fast Mode packet.
			 *
			 * Set DAC to max voltage.
			 */
			i2c_data[0] = 0x0f;
			i2c_data[1] = 0xff;
			msg.buf = i2c_data;
			msg.len = 2;
			msg.flags = I2C_MSG_WRITE;
			i2c_transfer(i2c_dev, msg, 2, DAC_I2C_ADDR);

			/* Go to send low state. */
			send_state = 0;
		} else {
			/* Set DAC to minimum voltage. */
			i2c_data[0] = 0;
			i2c_data[1] = 0;
			msg.buf = i2c_data;
			msg.len = 2;
			msg.flags = I2C_MSG_WRITE;
			i2c_transfer(i2c_dev, msg, 2, DAC_I2C_ADDR);

			/* Go to send high state. */
			send_state = 1;
		}
		k_sleep(K_MSEC(250));
	}
}
