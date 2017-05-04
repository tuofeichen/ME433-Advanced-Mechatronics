#include "imu.h"



void imu_init() // initialize control registers of IMU
{
    // 1.66 khz, 2g sensitivity, 100 Hz filter
    imu_write(CTRL1_XL, 0x82);
    // 1.66khz, 1000 dps sensitivity
    imu_write(CTRL2_G,  0x88);
    // dis/enable sequential read
    imu_write(CTRL3_C,  0x04);
    
}

void imu_test()
{
    uint8_t c = imu_read(WHO_AM_I);
    if (c == 0b01101001) // default value of who am I
    {
        drawRect(14 ,50,  100 ,  20 , GREEN);
    }
    
    else
    {
        drawRect(14 ,50,  100 ,  20 , RED);
    }
    while (_CP0_GET_COUNT()<12000000)
    {
            ;
    }
    _CP0_SET_COUNT(0);
    drawRect(14 ,50,  100 ,  20 , BACKGROUND);

}

void imu_write(uint8_t reg, uint8_t value)
{
    i2c_master_start();
    i2c_master_send(IMU_ADDR << 1);
    i2c_master_send(reg);  
    i2c_master_send(value); 
    i2c_master_stop();
}


uint8_t imu_read (uint8_t reg)
{
    uint8_t c = 0;
    i2c_master_start();
    i2c_master_send(IMU_ADDR << 1);
    i2c_master_send(reg); 
    i2c_master_restart();
    i2c_master_send((IMU_ADDR << 1)|1);
    c = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    
    return c;
}

void imu_read_seq(uint8_t reg, uint8_t len, uint8_t* out)
{
    
    int i = 0;
    i2c_master_start();
    i2c_master_send(IMU_ADDR << 1);
    i2c_master_send(reg); 
    i2c_master_restart();
    i2c_master_send((IMU_ADDR << 1)|1);
       
    for (i = 0;i<len-1;i++){     
         *out ++= i2c_master_recv();
        i2c_master_ack(0);
    }
    
     *out ++= i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
}

void imu_read_gyro(int16_t* gyro)
{
    uint8_t gyroRaw [12];
    imu_read_seq(OUTX_L_G, 6, gyroRaw);
    
    *gyro++ = (int16_t)(gyroRaw[1]<< 8 | gyroRaw[0]);
    *gyro++ = (int16_t)(gyroRaw[3]<< 8 | gyroRaw[2]);
    *gyro   = (int16_t)(gyroRaw[5]<< 8 | gyroRaw[4]);
       
}
        
void imu_read_acc(int16_t* acc)
{
    
    uint8_t accRaw [12];
    imu_read_seq(OUTX_L_XL, 6, accRaw);
    
    *acc++ = (int16_t)((accRaw[1]<< 8) | accRaw[0]);
    *acc++ = (int16_t)((accRaw[3]<< 8) | accRaw[2]);
    *acc   = (int16_t)((accRaw[5]<< 8) | accRaw[4]);
}
        