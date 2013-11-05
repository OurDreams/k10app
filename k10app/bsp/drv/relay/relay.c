/**
 ******************************************************************************
 * @file       relay.c
 * @version    V1.1.4
 * @brief      API C source file of relay.c
 *             Created on: 2013-9-9
 *             Author: daixuyi
 * @details    This file including all API functions's implement of dps.
 * @copy       Copyrigth(C).
 *
 ******************************************************************************
 */

#include <relay.h>
#include <flexbus.h>
#include <stdio.h>
#include <types.h>
#include <devLib.h>

/*合闸执行*/
void close_action()
{
    *CS0_START_ADDRESS &= 0xf8;
    taskDelay(1);
//    printf("合闸执行\n");
}
/*分闸执行*/
void open_action()
{
    *CS0_START_ADDRESS &= 0xf4;
    taskDelay(1);
//    printf("分闸执行\n");
}

typedef struct
{
    uint8_t relay_num;  //
    uint8_t *paddr;     //
    uint8_t open_val;   //
    uint8_t close_val;  //
//    void (*p_func)(void);
}relay_status_t;

static const relay_status_t yaokong[] =
{
        {relay_o1, CS0_START_ADDRESS, 0xef, 0x10},
        {relay_o2, CS0_START_ADDRESS, 0xbf, 0x40},
        {relay_o3, CS1_START_ADDRESS, 0xfe, 0x01},
        {relay_o4, CS1_START_ADDRESS, 0xfb, 0x04},
        {relay_o5, CS1_START_ADDRESS, 0xef, 0x10},
        {relay_o6, CS1_START_ADDRESS, 0xbf, 0x40},
        {relay_o7, CS2_START_ADDRESS, 0xfe, 0x01},
        {relay_o8, CS2_START_ADDRESS, 0xfb, 0x04},
        {relay_o9, CS2_START_ADDRESS, 0xef, 0x10},
        {relay_o10, CS2_START_ADDRESS, 0xbf, 0x40},

        {relay_c1, CS0_START_ADDRESS, 0xdf, 0x20},
        {relay_c2, CS0_START_ADDRESS, 0x7f, 0x80},
        {relay_c3, CS1_START_ADDRESS, 0xfd, 0x02},
        {relay_c4, CS1_START_ADDRESS, 0xf7, 0x08},
        {relay_c5, CS1_START_ADDRESS, 0xdf, 0x20},
        {relay_c6, CS1_START_ADDRESS, 0x7f, 0x80},
        {relay_c7, CS2_START_ADDRESS, 0xfd, 0x02},
        {relay_c8, CS2_START_ADDRESS, 0xf7, 0x08},
        {relay_c9, CS2_START_ADDRESS, 0xdf, 0x20},
        {relay_c10, CS2_START_ADDRESS, 0x7f, 0x80},

};

/**
 ******************************************************************************
 * @brief      设备初始化
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t dev_relay_init(struct device* dev)
{
    return OK;
}
/**
 ******************************************************************************
 * @brief      设备释放
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t dev_relay_release(struct device* dev)
{
//    printf("relay device release\n");
    return OK;
}
/**
 ******************************************************************************
 * @brief      打开设备
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t  dev_relay_open(struct device* dev)
{
//    printf("relay device open\n");
    return OK;
}
/**
 ******************************************************************************
 * @brief      关闭设备
 * @param[in]  None
 * @param[out] None
 *
 * @retval     None
 ******************************************************************************
 */
static status_t  dev_relay_close(struct device* dev)
{
//    printf("relay device close\n");
    return OK;
}
/**
 ******************************************************************************
 * @brief      读GPIO
 * @param[in]  device* dev : 设备描述
 * @param[in]  *buffer     : 写缓存地址
 * @param[in]  pinno       : 读取的引脚

 * @retval     OK          : 成功
 * @retval     FALSE       : 失败
 ******************************************************************************
 */
static size_t dev_relay_read (struct device* dev, int32_t pos, void *buffer, size_t pinno)
{

    return OK;
}

/**
 ******************************************************************************
 * @brief
 * @param[in]  device* dev : 设备描述
 * @param[in]  cmd         ：0，置低；1，置高
 * @param[in]  *arg        ：
 * @retval     OK          : 成功
 * @retval     FALSE       : 失败
 ******************************************************************************
 */
static int32_t dev_relay_ioctl (struct device* dev, uint32_t cmd, void* arg)
{
    uint8_t i;
    uint8_t temp;

    relay_param_t *p = (relay_param_t*)arg;

    if ((p == NULL) || (p->num > relay_c10))
    {
        return FALSE;
    }

    if (p->onoff == 0)
    {
        *(yaokong[p->num].paddr) &= yaokong[p->num].open_val;
    }
    else
    {
        *(yaokong[p->num].paddr) |= yaokong[p->num].close_val;
    }
    taskDelay(1);
//    if (yaokong[p->num].paddr == CS0_START_ADDRESS)
//    {
//        cs0 =
//    }

//    temp = *CS0_START_ADDRESS;
//    printf("tempis 0x%x\n", temp);
    if (p->num <= relay_o10)
    {
        temp = *CS0_START_ADDRESS;
        if ((temp & 0x08) == 0x08)
        {

            *CS0_START_ADDRESS = (temp | 0x04)& 0xf4;
            taskDelay(1);
        }
        else
        {
            open_action();
        }
    }
    else
    {
        temp = *CS0_START_ADDRESS;
//        printf("tempis 0x%x\n", temp);
        if ((temp & 0x04) == 0x04)
        {

            *CS0_START_ADDRESS = (temp | 0x08)& 0xf8;
//            printf("temp&0x04 is 0x%x\n", (temp | 0x08)& 0xf8);
            taskDelay(1);
        }
        else
        {
            close_action();
        }
    }
    taskDelay(1);
#if 0
    for(i = 0; i < ARRAY_SIZE(yaokong); i++)
    {
        if(yaokong[i].relay_num == ((uint8_t)(*((uint8_t*)arg))))
        {
            if(cmd == 1u)
            {
                *(yaokong[i].paddr) &= yaokong[i].close_val;
                if(yaokong[i].relay_num < 10)
                {
                    open_action();
                }
                else
                {
                    close_action();
                }
            }
            else if(cmd == 0u)
            {
                *(yaokong[i].paddr) &= yaokong[i].open_val;
                taskDelay(1);

                if(yaokong[i].relay_num < 10)
                {
//                    temp = *CS0_START_ADDRESS;
//                    temp &= 0x08;
//                    if(temp == 0x08)
//                    {
//                        *CS0_START_ADDRESS |= 0x08;
//                        taskDelay(1);
//                    }
                    open_action();
                }
                else
                {
                    temp = *CS0_START_ADDRESS;
                    if ((temp & 0x04) == 0x04)
                    {
                        *CS0_START_ADDRESS = temp | 0x04;
                        taskDelay(1);
                    }
                    close_action();
                }
//                printf("0x%x = 0x%x\n", yaokong[i].paddr, *(yaokong[i].paddr));/*没有的话，1~4号继电器不跳，why？*/
            }
            else
            {
                printf("Wrong command %d!\n\r", cmd);
            }
        }
    }
#endif
    return OK;
}

const static fileopt_t relay_opt =
{
        .init = dev_relay_init,
        .release = dev_relay_release,
        .open = dev_relay_open,
        .read = dev_relay_read,
        .close = dev_relay_close,
        .ioctl = dev_relay_ioctl,
};

status_t relay_device_init(void)
{
    if (OK != dev_create("relays", &relay_opt, MKDEV(2, 0), NULL))
    {
        printf("dev_create err\n");
        return ERROR;
    }
    return OK;
}

