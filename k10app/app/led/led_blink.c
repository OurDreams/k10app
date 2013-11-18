/**
 ******************************************************************************
 * @file      led.c
 * @brief     C Source file of led.c.
 * @details   This file including all API functions's 
 *            implement of led.c.	
 *
 * @copyright
 ******************************************************************************
 */
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <devLib.h>
#include <gpio.h>
#include <taskLib.h>
#include <osLib.h>
#include <dmnLib.h>
#include <pit.h>
#include <led.h>
#include <cfg.h>
/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
 #define CS0_START_ADDRESS ((uint8_t*)(0xA0000000))
 #define CS1_START_ADDRESS ((uint8_t*)(0xA1000000))
 #define CS2_START_ADDRESS ((uint8_t*)(0xA2000000))
 #define CS4_START_ADDRESS ((uint8_t*)(0xA4000000))
 #define CS5_START_ADDRESS ((uint8_t*)(0xA5000000))

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */
/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
void pit_test(void)
{
    clear_pitflag(0);
    printf("test\n");
}

uint32_t uart_test1 ()
{
    int32_t _the_tty0_fd, _the_tty1_fd;
    uint8_t i;
    uint8_t s_buff[] = {0x55, 0x55, 0x55, 0x55, 0x55};
    uint8_t r_buff[] = {0x0, 0x0, 0x0, 0x0, 0x0};
    _the_tty0_fd = dev_open("tty0", O_RDWR);
    if(_the_tty0_fd < 0)
    {
        printf("open tty0 error\n");
        return 1;
    }
    _the_tty1_fd = dev_open("tty1", O_RDWR);
    if(_the_tty1_fd < 0)
    {
        printf("open tty1 error\n");
        return 1;
    }
    gpio_init(PORTE, 2, 1, 1);
    gpio_init(PORTA, 16, 1, 1);

    gpio_set(PORTA, 16, 1);
    taskDelay(1);
    dev_write(_the_tty0_fd, s_buff, sizeof(s_buff));
    gpio_set(PORTE, 2, 0);
    taskDelay(1);
    dev_read(_the_tty1_fd, r_buff, sizeof(s_buff));

    for(i = 0; i < 5; i++)
    {
        printf("0x%x ", r_buff[i]);
    }
    printf("\n");
    dev_close(_the_tty0_fd);
    dev_close(_the_tty1_fd);

    return 0;
}
/**
 ******************************************************************************
 * @brief   流水灯任务
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static void ledTask(void *p_arg)
{
    uint8_t led = 0xff;
	DMN_ID dmnid = dmn_register();
	int32_t led_fd;
	led_fd = dev_open("leds", O_RDWR);

	FOREVER
	{
		dmn_sign(dmnid);

#ifdef DTU_LCD
		//*CS0_START_ADDRESS = 0x00;
        *CS1_START_ADDRESS = 0x00;
        *CS2_START_ADDRESS = 0x00;
        *CS4_START_ADDRESS = 0x00;
        *CS5_START_ADDRESS = 0x00;
		taskDelay(osClkRateGet()/4);
        //*CS0_START_ADDRESS = 0xff;
        *CS1_START_ADDRESS = 0xff;
        *CS2_START_ADDRESS = 0xff;
        *CS4_START_ADDRESS = 0xff;
        *CS5_START_ADDRESS = 0xff;
        taskDelay(osClkRateGet()/4);
#endif

#ifdef DTU_YK
        dev_ioctl(led_fd, led_turn, 0);
		taskDelay(osClkRateGet()/4);
#endif

#ifdef DTU_YX
        dev_ioctl(led_fd, led_turn, 0);
        taskDelay(osClkRateGet()/4);
#endif

//		uart_test1();

	}
}

void led_task_init(void)
{
    taskSpawn((const signed char*)"led", 3, 256, (OSFUNCPTR)ledTask,0);
}

