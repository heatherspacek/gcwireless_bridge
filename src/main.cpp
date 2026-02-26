#include "joybus.hpp"
#include "gcReport.hpp"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/spi.h"

#include <cstring>

const int _pinTX  = 27;

union Buttons{
	uint8_t arr[10];
	struct {

		// byte 0
		uint8_t A : 1;
		uint8_t B : 1;
		uint8_t X : 1;
		uint8_t Y : 1;
		uint8_t S : 1;
		uint8_t orig : 1;
		uint8_t errL : 1;
		uint8_t errS : 1;

		// byte 1
		uint8_t Dl : 1;
		uint8_t Dr : 1;
		uint8_t Dd : 1;
		uint8_t Du : 1;
		uint8_t Z : 1;
		uint8_t R : 1;
		uint8_t L : 1;
		uint8_t high : 1;

		//byte 2-7
		uint8_t Ax : 8;
		uint8_t Ay : 8;
		uint8_t Cx : 8;
		uint8_t Cy : 8;
		uint8_t La : 8;
		uint8_t Ra : 8;

		// magic byte 8 & 9 (only used in origin cmd)
		// have something to do with rumble motor status???
		// ignore these, they are magic numbers needed
		// to make a cmd response work
		uint8_t magic1 : 8;
		uint8_t magic2 : 8;
	};
};

Buttons _btn;

GCReport __no_inline_not_in_flash_func(buttonsToGCReport)() {
	GCReport report = {
		.a       = _btn.A,
		.b       = _btn.B,
		.x       = _btn.X,
		.y       = _btn.Y,
		.start   = _btn.S,
		.pad0    = 0,
		.dLeft   = _btn.Dl,
		.dRight  = _btn.Dr,
		.dDown   = _btn.Dd,
		.dUp     = _btn.Du,
		.z       = _btn.Z,
		.r       = _btn.R,
		.l       = _btn.L,
		.pad1    = 1,
		.xStick  = _btn.Ax,
		.yStick  = _btn.Ay,
		.cxStick = _btn.Cx,
		.cyStick = _btn.Cy,
		.analogL = _btn.La,
		.analogR = _btn.Ra
		
	};
	return report;
}

uint8_t BUF_LEN = 10;

void second_core() {
	spi_init(spi_default, 1000*1000);
	spi_set_slave(spi_default, true);
	gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);

	uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];

	while (true)
	{
		sleep_us(10);  // superstition.
		// spi_write_read_blocking(spi0, &NOP, &status1, 1);
		// cs_deselect();
		
		// if ((status1 & 0x40) == 0x40) 
		if (true)
		{
			spi_read_blocking(spi_default, 0x00, in_buf, BUF_LEN);
			_btn.Ax = in_buf[0];
			//printf("\n received one!");
			//sleep_ms(400);
			// uint8_t read_buffer[8];
			// // read_rx_payload(read_buffer);
			// // write_register(0x07, 0b01110000); // clear interrupts
			
			// GCReport destinationreport;
   			// memcpy(&destinationreport, read_buffer , 8); 

   			// _btn.A      =destinationreport.a;
			// _btn.B      =destinationreport.b;
			// _btn.X      =destinationreport.x;
			// _btn.Y      =destinationreport.y;
			// _btn.S      =destinationreport.start;
			// //_btn.orig   =0;
			// //_btn.errL   =0;
			// //_btn.errS   =0;
			// _btn.Dl     =destinationreport.dLeft;
			// _btn.Dr     =destinationreport.dRight;
			// _btn.Dd     =destinationreport.dDown;
			// _btn.Du     =destinationreport.dUp;
			// _btn.Z      =destinationreport.z;
			// _btn.R      =destinationreport.r;
			// _btn.L      =destinationreport.l;
			// //_btn.high   =1;
			// _btn.Ax     =destinationreport.xStick+1;
			// _btn.Ay     =destinationreport.yStick+1;
			// _btn.Cx     =destinationreport.cxStick+1;
			// _btn.Cy     =destinationreport.cyStick+1;
			// _btn.La     =destinationreport.analogL;
			// _btn.Ra     =destinationreport.analogR;
		}
	}
}

int main() {
    multicore_launch_core1(second_core);
    enterMode(_pinTX, buttonsToGCReport);
}