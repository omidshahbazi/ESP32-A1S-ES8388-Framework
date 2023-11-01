#pragma once
#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include "Common.h"
#include "ESPDebug.h"
#include <driver/i2c.h>

class I2CUtils
{
public:
	static uint8 Read(uint8 BusAddress, uint8 Register)
	{
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();

		// Write the register address to be read
		ESP_CHECK_CALL(i2c_master_start(cmd));
		ESP_CHECK_CALL(i2c_master_write_byte(cmd, BusAddress << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN));
		ESP_CHECK_CALL(i2c_master_write_byte(cmd, Register, ACK_CHECK_EN));

		// Read the data for the register from the slave
		ESP_CHECK_CALL(i2c_master_start(cmd));
		ESP_CHECK_CALL(i2c_master_write_byte(cmd, BusAddress << 1 | I2C_MASTER_READ, ACK_CHECK_EN));

		uint8 value;
		ESP_CHECK_CALL(i2c_master_read_byte(cmd, &value, I2C_MASTER_NACK));
		ESP_CHECK_CALL(i2c_master_stop(cmd));
		ESP_CHECK_CALL(i2c_master_cmd_begin(MASTER_NUM, cmd, 1000 / portTICK_RATE_MS));

		i2c_cmd_link_delete(cmd);

		return value;
	}

	static bool WriteBulk(uint8 BusAddress, uint8 Register, uint8 *Data, uint8 Length)
	{
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();

		ESP_CHECK_CALL(i2c_master_start(cmd));

		ESP_CHECK_CALL(i2c_master_write_byte(cmd, BusAddress << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN));
		ESP_CHECK_CALL(i2c_master_write(cmd, &Register, 1, ACK_CHECK_EN));
		ESP_CHECK_CALL(i2c_master_write(cmd, Data, Length, ACK_CHECK_EN));
		ESP_CHECK_CALL(i2c_master_stop(cmd));
		ESP_CHECK_CALL(i2c_master_cmd_begin(MASTER_NUM, cmd, 1000 / portTICK_RATE_MS));

		i2c_cmd_link_delete(cmd);

		return true;
	}

	static bool Write(uint8 BusAddress, uint8 Register, uint8 Value)
	{
		return WriteBulk(BusAddress, Register, &Value, 1);
	}

private:
	static const uint8 ACK_CHECK_EN;
	static const uint8 MASTER_NUM;
};

const uint8 I2CUtils::ACK_CHECK_EN = 0x1;
const uint8 I2CUtils::MASTER_NUM = 0;

#endif