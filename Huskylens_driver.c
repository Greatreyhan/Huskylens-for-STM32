/*
 * Huskylens_driver.c
 *
 *  Created on: Janvoid 18, 2022
 *      Author: Maulana Reyhan Savero
 */

#include "Huskylens_driver.h"

static I2C_HandleTypeDef* hi2c;
static uint8_t HUSKY_ADDR = 0;

huskylens_status_t husky_setup(I2C_HandleTypeDef *i2cHandler ){
	HAL_Delay(50);
	hi2c = i2cHandler;
	for(int i =0; i<255;i++){
		if(HAL_I2C_IsDeviceReady(hi2c, i, 1, 10) == HAL_OK){
			 HUSKY_ADDR = i;
		}
	}
	uint8_t knock[] = {0x55, 0xAA, 0x11, 0x00, 0x2C, 0x3C};
	uint8_t rxBuff[6];
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, knock, 6, 100);
	
	// Return OK
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 6, 100);
	
	if((rxBuff[0] == 0x55) && (rxBuff[1] == 0xAA) && (rxBuff[2] == 0x11) && (rxBuff[3] == 0x00) && (rxBuff[4] == 0x2E) && (rxBuff[5] == 0x3E) ){
		return HUSKY_OK;
	}
	else{
		return HUSKY_ERR;
	}
	
	return HUSKY_TIMEOUT;
}

huskylens_all_t husky_getAllArrowBlock(void){
	huskylens_all_t handler;
	// Command Request
	uint8_t req[] = {0x55, 0xAA, 0x11, 0x00, 0x20, 0x30};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, req, 6, 100);
	
	// Return INFO
	uint8_t rxBuff[16];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 16, 100);
	handler.num_block_arr = (rxBuff[6] << 8) | rxBuff[5];
	handler.num_id = (rxBuff[8] << 8) | rxBuff[7];
	handler.current_frame = (rxBuff[10] << 8) | rxBuff[9];
	
	return handler;
}

huskylens_arrow_t husky_getArrows(void){
	huskylens_arrow_t handler;
	// Command Request
	uint8_t reqArr[] = {0x55, 0xAA, 0x11, 0x00, 0x22, 0x32};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, reqArr, 6, 100);
	
	// Return INFO
	uint8_t rxBuff[16];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 16, 100);
	handler.X_origin = (rxBuff[6] << 8) | rxBuff[5];
	handler.Y_origin = (rxBuff[8] << 8) | rxBuff[7];
	handler.X_target = (rxBuff[10] << 8) | rxBuff[9];
	handler.Y_target = (rxBuff[10] << 8) | rxBuff[9];
	handler.id = (rxBuff[12] << 8) | rxBuff[11];
	
	return handler;
}

huskylens_block_t husky_getBlocks(void){
	huskylens_block_t handler;
	uint8_t reqBlock[] = {0x55, 0xAA, 0x11, 0x00, 0x21, 0x31};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, reqBlock, 6, 100);
	
	// Return INFO
	uint8_t rxBuff[16];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 16, 100);
	handler.X_center = (rxBuff[6] << 8) | rxBuff[5];
	handler.Y_center = (rxBuff[8] << 8) | rxBuff[7];
	handler.width = (rxBuff[10] << 8) | rxBuff[9];
	handler.height = (rxBuff[10] << 8) | rxBuff[9];
	handler.id = (rxBuff[12] << 8) | rxBuff[11];
	
	return handler;
}

huskylens_learned_block_t husky_getLearnedBlocks(void){
	huskylens_learned_block_t handler;
	uint8_t reqLearnedBlock[] = {0x55, 0xAA, 0x11, 0x00, 0x24, 0x34};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, reqLearnedBlock, 6, 100);
	
	// Return INFO
	uint8_t rxBuff[16];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 16, 100);
	handler.X_center = (rxBuff[6] << 8) | rxBuff[5];
	handler.Y_center = (rxBuff[8] << 8) | rxBuff[7];
	handler.width = (rxBuff[10] << 8) | rxBuff[9];
	handler.height = (rxBuff[10] << 8) | rxBuff[9];
	handler.id = (rxBuff[12] << 8) | rxBuff[11];
	
	return handler;
}

huskylens_learned_arrow_t husky_getLearnedArrows(void){
	huskylens_learned_arrow_t handler;
	// Command Request
	uint8_t reqArr[] = {0x55, 0xAA, 0x11, 0x00, 0x25, 0x35};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, reqArr, 6, 100);
	
	// Return INFO
	uint8_t rxBuff[16];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 16, 100);
	handler.X_origin = (rxBuff[6] << 8) | rxBuff[5];
	handler.Y_origin = (rxBuff[8] << 8) | rxBuff[7];
	handler.X_target = (rxBuff[10] << 8) | rxBuff[9];
	handler.Y_target = (rxBuff[10] << 8) | rxBuff[9];
	handler.id = (rxBuff[12] << 8) | rxBuff[11];
	
	return handler;
}

huskylens_all_byid_t husky_getAllById(uint16_t id){
	huskylens_all_byid_t handler;
	// Command Request
	uint8_t reqArr[] = {0x55, 0xAA, 0x11, 0x02, 0x26, 0x01, 0x00, 0x39};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, reqArr, 8, 100);
	
	// Return INFO
	uint8_t rxBuff[16];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, handler.buff, 16, 100);
	
	return handler;
}

huskylens_block_t husky_getBlockById(uint16_t id){
	huskylens_block_t handler;
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x02, 0x27, (id & 0xFF), ((id >> 8)&(0xFF)), 0x00 };
	msg[7] = (0x55 + 0xAA + 0x11 + 0x02+ 0x27+ (id & 0xFF) + ((id >> 8)&(0xFF)) + 0x00) & 0xFF;
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 8, 100);
	
	// Return INFO
	uint8_t rxBuff[16];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 16, 100);
	handler.X_center = (rxBuff[6] << 8) | rxBuff[5];
	handler.Y_center = (rxBuff[8] << 8) | rxBuff[7];
	handler.width = (rxBuff[10] << 8) | rxBuff[9];
	handler.height = (rxBuff[10] << 8) | rxBuff[9];
	handler.id = (rxBuff[12] << 8) | rxBuff[11];
	
	return handler;
}

huskylens_arrow_t husky_getArrowById(uint16_t id){
	huskylens_arrow_t handler;
	// Command Request
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x02, 0x28, (id & 0xFF), ((id >> 8)&(0xFF)), 0x00 };
	msg[7] = (0x55 + 0xAA + 0x11 + 0x02+ 0x28+ (id & 0xFF) + ((id >> 8)&(0xFF)) + 0x00) & 0xFF;
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 8, 100);
	
	// Return INFO
	uint8_t rxBuff[16];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 16, 100);
	handler.X_origin = (rxBuff[6] << 8) | rxBuff[5];
	handler.Y_origin = (rxBuff[8] << 8) | rxBuff[7];
	handler.X_target = (rxBuff[10] << 8) | rxBuff[9];
	handler.Y_target = (rxBuff[10] << 8) | rxBuff[9];
	handler.id = (rxBuff[12] << 8) | rxBuff[11];
	
	return handler;
}

huskylens_status_t husky_setAlgorithm(uint16_t algorithm){
	// Command Request
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x02, 0x2D, (algorithm & 0xFF), ((algorithm >> 8)&(0xFF)), 0x00 };
	msg[7] = (0x55 + 0xAA + 0x11 + 0x02+ 0x2D+ (algorithm & 0xFF) + ((algorithm >> 8)&(0xFF)) + 0x00) & 0xFF;
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 8, 100);
	uint8_t rxBuff[6];
	HAL_I2C_Master_Receive(hi2c, HUSKY_ADDR, rxBuff, 6, 100);
	if((rxBuff[0] == 0x55) && (rxBuff[1] == 0xAA) && (rxBuff[2] == 0x11) && (rxBuff[3] == 0x00) && (rxBuff[4] == 0x2E) && (rxBuff[5] == 0x3E) ){
		return HUSKY_OK;
	}
	else{
		return HUSKY_ERR;
	}
	return HUSKY_TIMEOUT;
}

huskylens_status_t husky_savePic(void){
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x00, 0x30, 0x40};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 6, 100);
	
	return HUSKY_OK;
}

huskylens_status_t husky_saveAlgorithm(uint16_t fileNumb){
// Command Request
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x02, 0x32, (fileNumb & 0xFF), ((fileNumb >> 8)&(0xFF)), 0x00 };
	msg[7] = (0x55 + 0xAA + 0x11 + 0x02+ 0x32+ (fileNumb & 0xFF) + ((fileNumb >> 8)&(0xFF)) + 0x00) & 0xFF;
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 8, 100);
	
	return HUSKY_OK;
}

huskylens_status_t husky_loadAlgorithm(uint16_t fileNumb){
// Command Request
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x02, 0x33, (fileNumb & 0xFF), ((fileNumb >> 8)&(0xFF)), 0x00 };
	msg[7] = (0x55 + 0xAA + 0x11 + 0x02+ 0x33+ (fileNumb & 0xFF) + ((fileNumb >> 8)&(0xFF)) + 0x00) & 0xFF;
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 8, 100);
	
	return HUSKY_OK;
}

huskylens_status_t husky_startLearn(uint16_t id){
// Command Request
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x02, 0x36, (id & 0xFF), ((id >> 8)&(0xFF)), 0x00 };
	msg[7] = (0x55 + 0xAA + 0x11 + 0x02+ 0x36+ (id & 0xFF) + ((id >> 8)&(0xFF)) + 0x00) & 0xFF;
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 8, 100);
	
	return HUSKY_OK;
}

huskylens_status_t husky_forget(void){
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x00, 0x37, 0x47};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 6, 100);
	
	return HUSKY_OK;
}

huskylens_status_t husky_saveScreenShoot(void){
	uint8_t msg[] = {0x55, 0xAA, 0x11, 0x00, 0x39, 0x49};
	HAL_I2C_Master_Transmit(hi2c, HUSKY_ADDR, msg, 6, 100);
	
	return HUSKY_OK;
}