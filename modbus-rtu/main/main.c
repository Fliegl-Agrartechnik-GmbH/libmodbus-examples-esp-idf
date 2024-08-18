#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <modbus.h>


#include "driver/uart_vfs.h"
#include "driver/uart.h"


/* The goal of this program is to check all major functions of
   libmodbus:
   - write_coil
   - read_bits
   - write_coils
   - write_register
   - read_registers
   - write_registers
   - read_registers

   All these functions are called with random values on a address
   range defined by the following defines.
*/
#define SERVER_ID     1

/* At each loop, the program works in the range ADDRESS_START to
 * ADDRESS_END then ADDRESS_START + 1 to ADDRESS_END and so on.
 */

// Timeout threshold for UART = number of symbols (~10 tics) with unchanged state on receive pin
#define ECHO_READ_TOUT          (3) // 3.5T * 8 = 28 ticks, TOUT=3 -> ~24..33 ticks

int app_main(void)
{



    if (uart_driver_install(UART_NUM_1, 2 * 1024, 0, 0, NULL, 0) != ESP_OK) {
        printf("Driver installation failed");
        vTaskDelete(NULL);
    }

    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Configure UART parameters
    uart_param_config(UART_NUM_1, &uart_config);
	
    // Set UART pins
	uart_set_pin(UART_NUM_1, 13, 1, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Set RS485 half duplex mode
    ESP_ERROR_CHECK(uart_set_mode(UART_NUM_1, UART_MODE_RS485_HALF_DUPLEX));

    // Set read timeout of UART TOUT feature
    ESP_ERROR_CHECK(uart_set_rx_timeout(UART_NUM_1, ECHO_READ_TOUT));




    
    modbus_t *ctx;


    /* RTU */
    
        ctx = modbus_new_rtu("/dev/uart/1", uart_config.baud_rate, 'N', 8, 1); //TODO ! sync the params. 
        
        modbus_set_debug(ctx, TRUE);
        
        modbus_set_slave(ctx, SERVER_ID);
    

    /* TCP */
//    ctx = modbus_new_tcp("192.168.178.73", 502);
//    modbus_set_debug(ctx, TRUE);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }



    while (1) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		modbus_write_bit(ctx, 0x0, ON);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
	    modbus_write_bit(ctx, 0x0, OFF);
	}




    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}