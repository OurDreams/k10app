/**
 ******************************************************************************
 * @file      lcm.c
 * @brief     本文实现了黑白点阵液晶GUI操作基础方法.
 * @details   This file including all API functions's implement of lcm.c.
 *
 * @copyright Copyrigth(C), 2008-2012,Sanxing Electric Co.,Ltd.
 ******************************************************************************
 */
 
/*-----------------------------------------------------------------------------
 Section: Includes
 ----------------------------------------------------------------------------*/
#include <string.h>
#include <types.h>
#include <maths.h>
//#include <zk.h>
#include <lcm.h>
#include <lcddrv.h>

/*-----------------------------------------------------------------------------
 Section: Type Definitions
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Constant Definitions
 ----------------------------------------------------------------------------*/
#ifndef LCM_X_SIZE
#define LCM_X_SIZE      lcddrv_init()->width

#endif
#ifndef LCM_Y_SIZE
#define LCM_Y_SIZE      lcddrv_init()->height
#endif

#ifndef LCM_LINE_WITH_EN
#define LCM_LINE_WITH_EN    (0u)
#endif
#ifndef LCM_CIRCLE_EN
#define LCM_CIRCLE_EN       (0u)
#endif
#ifndef LCM_ELLIPSE_EN
#define LCM_ELLIPSE_EN      (0u)
#endif

/*-----------------------------------------------------------------------------
 Section: Global Variables
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Local Function Prototypes
 ----------------------------------------------------------------------------*/
static status_t
lcm_init(void);

static void
lcm_info(uint8_t *px, uint8_t *py);

static void
lcm_clear(void);

static void
lcm_clear_lines(uint8_t start,
        uint8_t num);

static void
lcm_cross(uint8_t c1, uint8_t c2);

static void
lcm_reverse_area(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1);

static void
lcm_refresh_reg(uint8_t contrast, bool_e isNeedCheck);

static void
lcm_set_contrast(uint8_t value);

static void
lcm_set_backlight(bool_e is_open);

static void
lcm_updt_area(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1);

static void
lcm_updt_all(void);

static void
lcm_draw_bmp(uint8_t xstart,
        uint8_t ystart,
        uint8_t width,
        uint8_t height,
        const uint8_t* pbmp);

static void
lcm_draw_pic(uint8_t x,
        uint8_t y,
        uint8_t w,
        uint8_t h,
        const uint8_t *pic);

static void
lcm_draw_text(uint8_t f_nFont,
         uint8_t x0,
         uint8_t y0,
         uint8_t f_nMaxLen,
         const uint8_t *f_pData,
         uint8_t color);

static void
lcm_line(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t color);

#if (LCM_LINE_WITH_EN == 1u)
static void
lcm_line_width(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t width,
        uint8_t color);
#endif

static void
lcm_rectangle(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t color);

static void
lcm_rectangle_fill(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t color);

static void
lcm_square(uint8_t x0,
        uint8_t y0,
        uint8_t width,
        uint8_t color);

static void
lcm_lines(uint8_t const *points,
        uint8_t no,
        uint8_t color);

#if  (LCM_CIRCLE_EN == 1u)
static void
lcm_circle(uint8_t x0,
        uint8_t y0,
        uint8_t r,
        uint8_t color);

static void
lcm_circle_fill(uint8_t x0,
        uint8_t y0,
        uint8_t r,
        uint8_t color);
#endif

#if  (LCM_ELLIPSE_EN == 1u)
static void
lcm_ellipse(uint8_t x0,
        uint8_t x1,
        uint8_t y0,
        uint8_t y1,
        uint8_t color);

static void
lcm_ellipse_fill(uint8_t x0,
        uint8_t x1,
        uint8_t y0,
        uint8_t y1,
        uint8_t color);
#endif

/*-----------------------------------------------------------------------------
 Section: Local Variables
 ----------------------------------------------------------------------------*/
static const lcm_t the_lcm_opt =
{
        .pfn_init = lcm_init,
        .pfn_info = lcm_info,
        .pfn_clear = lcm_clear,
        .pfn_clear_lines = lcm_clear_lines,
        .pfn_cross = lcm_cross,
        .pfn_reverse_area = lcm_reverse_area,
        .pfn_refresh_reg = lcm_refresh_reg,
        .pfn_set_contrast = lcm_set_contrast,
        .pfn_set_backlight = lcm_set_backlight,
        .pfn_updt_area = lcm_updt_area,
        .pfn_updt_all = lcm_updt_all,
        .pfn_bmp = lcm_draw_bmp,
        .pfn_pic = lcm_draw_pic,
        .pfn_text = lcm_draw_text,
        .pfn_line = lcm_line,
#if (LCM_LINE_WITH_EN == 1u)
        .pfn_line_width = lcm_line_width,
#endif
        .pfn_rectangle = lcm_rectangle,
        .pfn_rectangle_fill = lcm_rectangle_fill,
        .pfn_square = lcm_square,
        .pfn_lines = lcm_lines,
#if  (LCM_CIRCLE_EN == 1u)
        .pfn_circle = lcm_circle,
        .pfn_circle_fill = lcm_circle_fill,
#endif
#if  (LCM_ELLIPSE_EN == 1u)
        .pfn_ellipse = lcm_ellipse,
        .pfn_ellipse_fill = lcm_ellipse_fill,
#endif
};

/*-----------------------------------------------------------------------------
 Section: Global Function Prototypes
 ----------------------------------------------------------------------------*/
/* NONE */

/*-----------------------------------------------------------------------------
 Section: Function Definitions
 ----------------------------------------------------------------------------*/
/**
 ******************************************************************************
 * @brief   获取lcm操作结构
 * @param[in]  None
 *
 * @retval  lcm操作 结构
 ******************************************************************************
 */
const lcm_t *
lcm_opt(void)
{
    return &the_lcm_opt;
}

/**
 ******************************************************************************
 * @brief   lcm初始化
 * @param[in]  type : 液晶模块类型
 *
 * @retval  OK      : 初始化成功
 * @retval  ERROR   : 初始化失败
 ******************************************************************************
 */
static status_t
lcm_init(void)
{
    if (lcddrv_init()->pfn_init != NULL)
    {
        return lcddrv_init()->pfn_init();
    }
    return ERROR;
}

/**
 ******************************************************************************
 * @brief   获取lcm信息
 * @param[out] *px  : 返回width
 * @param[out] *py  : 返回hehight
 *
 * @retval     None
 ******************************************************************************
 */
static void
lcm_info(uint8_t *px, uint8_t *py)
{
    *px = lcddrv_init()->width;
    *py = lcddrv_init()->height;
}

/**
 ******************************************************************************
 * @brief   lcm清除显示缓存
 * @param[in]  None
 *
 * @retval     None
 ******************************************************************************
 */
static void
lcm_clear(void)
{
    if (lcddrv_init()->pfn_clear_layer != NULL)
    {
        lcddrv_init()->pfn_clear_layer();
    }
}

/**
 ******************************************************************************
 * @brief   清空行
 * @param[in]  start    : 起始行
 * @param[in]  num      : 行数
 *
 * @return     None
 ******************************************************************************
 */
static void
lcm_clear_lines(uint8_t start,
        uint8_t num)
{
    if (lcddrv_init()->pfn_clear_lines != NULL)
    {
        lcddrv_init()->pfn_clear_lines(start, num);
    }
}

/**
 ******************************************************************************
 * @brief   lcm画点
 * @param[in]  x    : x坐标
 * @param[in]  y    : y坐标
 * @param[in]  color: 颜色
 *
 * @return NONE
 ******************************************************************************
 */
static inline void
lcm_set_point(uint8_t x,
        uint8_t y,
        uint8_t color)
{
    lcddrv_init()->pfn_set_point(x, y, color);  /* 频繁调用不判断为空 */
}

/**
 ******************************************************************************
 * @brief   lcm获取点颜色
 * @param[in]  x    : x坐标
 * @param[in]  y    : y坐标
 *
 * @retval  0xff : 无效坐标
 * @retval  0    : 白色
 * @retval  1    : 黑色
 ******************************************************************************
 */
static inline uint8_t
lcm_get_point(uint8_t x,
        uint8_t y)
{
    return lcddrv_init()->pfn_get_point(x, y);  /* 频繁调用不判断为空 */
}

/**
 ******************************************************************************
 * @brief   lcm画8*8方格
 * @param[in]  None
 *
 * @retval     None
 ******************************************************************************
 */
static void
lcm_cross(uint8_t c1, uint8_t c2)
{
    c1 = (c1 == 0u) ? 0u : 1u;
    c2 = (c2 == 0u) ? 0u : 1u;

    for (uint8_t i = 0u; i < LCM_Y_SIZE; i++)
    {
        for (uint8_t j = 0u; j < LCM_X_SIZE; j++)
        {
            if (((i / 16 + j / 16) % 2) == 0)
            {
                lcm_set_point(j, i, c1);
            }
            else
            {
                lcm_set_point(j, i, c2);
            }
        }
    }
}

/**
 ******************************************************************************
 * @brief   lcm反选显示
 * @param[in]  x0   : 反选区域左上角x值
 * @param[in]  y0   : 反选区域左上角y值
 * @param[in]  x1   : 反选区域右下角x值
 * @param[in]  y1   : 反选区域右下角y值
 *
 * @return None
 ******************************************************************************
 */
static void
lcm_reverse_area(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1)
{
    uint8_t color;

    for (uint8_t i = y0; i <= y1; i++)
    {
        for (uint8_t j = x0; j <= x1; j++)
        {
            color = (lcm_get_point(j, i) == 0) ? 1u : 0u;
            lcm_set_point(j, i, color);
        }
    }
}

/**
 ******************************************************************************
 * @brief   lcm刷新寄存器
 * @param[in]  contrast : 对比度(0~10)
 * @param[in]  isNeedCheck: 初始化前是否需要检查状态; true: 需要， false: 不需要
 *
 * @return None
 ******************************************************************************
 */
static void
lcm_refresh_reg(uint8_t contrast, bool_e isNeedCheck)
{
    if (lcddrv_init()->pfn_refresh_reg != NULL)
    {
        lcddrv_init()->pfn_refresh_reg(contrast, isNeedCheck);
    }
}

/**
 ******************************************************************************
 * @brief   lcm设置对比度
 * @param[in]  value    : 对比度值(0~10)
 *
 * @return  None
 ******************************************************************************
 */
static void
lcm_set_contrast(uint8_t value)
{
    if (lcddrv_init()->pfn_set_contrast != NULL)
    {
        lcddrv_init()->pfn_set_contrast(value);
    }
}

/**
 ******************************************************************************
 * @brief   lcm设置背光
 * @param[in]  value    : 对比度值(0~100)
 *
 * @return  None
 ******************************************************************************
 */
static void
lcm_set_backlight(bool_e is_open)
{
    if (lcddrv_init()->pfn_set_backlight != NULL)
    {
        lcddrv_init()->pfn_set_backlight(is_open);
    }
}


/**
 ******************************************************************************
 * @brief   lcm更新区域至lcd
 * @param[in]  x0   : 更新区域左上角x值
 * @param[in]  y0   : 更新区域左上角y值
 * @param[in]  x1   : 更新区域右下角x值
 * @param[in]  y1   : 更新区域右下角y值
 *
 * @return  None
 ******************************************************************************
 */
static void
lcm_updt_area(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1)
{
    if (lcddrv_init()->pfn_updt_area != NULL)
    {
        lcddrv_init()->pfn_updt_area(x0, y0, x1, y1);
    }
}

/**
 ******************************************************************************
 * @brief   lcm更新全屏
 * @param[in]  x0   : 更新区域左上角x值
 * @param[in]  y0   : 更新区域左上角y值
 * @param[in]  x1   : 更新区域右下角x值
 * @param[in]  y1   : 更新区域右下角y值
 *
 * @return  None
 ******************************************************************************
 */
static void
lcm_updt_all(void)
{
    if (lcddrv_init()->pfn_updt_all != NULL)
    {
        lcddrv_init()->pfn_updt_all();
    }
}

/**
 ******************************************************************************
 * @brief   LCD显示图片(非压缩)
 * @param[in]  x    : 显示起点x坐标
 * @param[in]  y    : 显示起点y坐标
 * @param[in]  w    : 图片宽度,必须为8的整数倍
 * @param[in]  h    : 图片高度
 * @param[in]  *pbmp: 指向图片数组
 *
 * @return NONE
 ******************************************************************************
 */
static void
lcm_draw_bmp(uint8_t x,
        uint8_t y,
        uint8_t width,
        uint8_t height,
        const uint8_t* pbmp)
{
    /* 判断绘图条件是否合法 */
    if ((width == 0u) || (height == 0u))
    {
        return;
    }
    if ((x >= LCM_X_SIZE) || (y >= LCM_Y_SIZE))
    {
        return;
    }

    if (pbmp == NULL)
    {
        return;
    }
    uint8_t lsize = (width + 7u) / 8u;
    for (uint8_t i = 0u; i < height; i++)
    {
        for (uint8_t j = 0u; j < width; j++)
        {
            lcm_set_point(x + j, y + i,
                    BITS(pbmp[lsize * i + j / 8u], 7u - (j % 8u)));
        }
    }
}

/**
 ******************************************************************************
 * @brief   LCD显示图片(压缩)
 * @param[in]  x    : 显示起点x坐标
 * @param[in]  y    : 显示起点y坐标
 * @param[in]  w    : 图片宽度,必须为8的整数倍
 * @param[in]  h    : 图片高度
 * @param[in]  *pic : 指向图片数组
 *
 * @return NONE
 * @note
 * 注意图片使用压缩格式:
 * 2个以上（包含2个但不超过 255个）连续的00或FF字节可仅用2个连续字节代替，第一
 * 个字节放00或FF,紧跟的第二个字节放连续的00或FF的个数。当连续的00或FF超过255的
 * 处理如下：在00或FF后跟00，表示00或FF的个数超过了255个，然后再跟2个字节，表示
 * 实际的00或FF个数，比如有连续65535个00，则用00，00，ff,ff来代替；再比如有连续
 * 1000个ff，则用ff，00，e8,03来代替；注意高低位的安排
 ******************************************************************************
 */
static void
lcm_draw_pic(uint8_t x,
        uint8_t y,
        uint8_t w,
        uint8_t h,
        const uint8_t *pic)
{
    uint16_t i, j, k;
    uint16_t m, tmp;

    if ((x > LCM_X_SIZE - 1) || (y > LCM_Y_SIZE - 1))
    {
        return;
    }

    for (i = y; i < y + h; i++)
    {
        for (j = x; j < x + w;)
        {
            if ((*pic == 0x00) || (*pic == 0xff))
            {
                if (*(pic + 1) == 0x00)
                {
                    tmp = (*(pic + 3) * 256) + *(pic + 2);
                    for (k = 0; k < tmp; k++)
                    {
                        for (m = 0; m < 8; m++)
                        {
                            lcm_set_point(j++, i, *pic & (0x80 >> m));
                        }
                        if (j == x + w)
                        {
                            j = x;
                            i++;
                            if (i == y + h)
                            {
                                return;
                            }
                        }
                    }
                    pic += 4;
                }
                else
                {
                    tmp = *(pic + 1);
                    for (k = 0; k < tmp; k++)
                    {
                        for (m = 0; m < 8; m++)
                        {
                            lcm_set_point(j++, i, *pic & (0x80 >> m));
                        }
                        if (j == x + w)
                        {
                            j = x;
                            i++;
                            if (i == y + h)
                            {
                                return;
                            }
                        }
                    }
                    pic += 2;
                }

            }
            else
            {
                for (m = 0; m < 8; m++)
                {
                    lcm_set_point(j++, i, (*pic) & (0x80 >> m));
                }
                pic += 1;
            }
        }
    }
}

/**
 ******************************************************************************
 * @brief      在指定区域显示文字信息
 * @param[in]  f_nFont:  字体大小    FONT_SIZE_16: 16*16,16*8,
 *                                      FONT_SIZE_12: 12*12,12*8
 * @param[in]  x0:         显示起始位置横坐标
 * @param[in]  y0:         显示起始位置纵坐标
 * @param[in]  f_nMaxLen:  每一行显示的最大字符数
 * @param[in]  *f_pData:显示内容(字符串)
 * @param[in]  color:      字符颜色, 1: 黑色, 0: 无色
 *
 * @retval     int: 0: 输出成功, -1:最大长度超范围
 *
 * @details
 *      应用指定字体大小，起始位置，每行显示字数,
 *      本方法根据以上信息进行字符输出,
 *      为节省内存，一次只输出一个字符,
 *      取字符，判断当前行已输出字符数，如果当前行>=f_nMaxLen,
 *      在新行输出。
 * @note
 ******************************************************************************
 */
static void
lcm_draw_text(uint8_t f_nFont,
         uint8_t x0,
         uint8_t y0,
         uint8_t f_nMaxLen,
         const uint8_t *f_pData,
         uint8_t color)
{
    int32_t totalLen; /* 总长 */
    int32_t os = 0; /* 偏移位置，指示当前输出字符在数据源中的位置 */
    float32_t cpos = 0; /* 当前行已输出字符数 */
    int32_t rows = 0; /* 当前行序号 */
    int32_t j, latticeLen;
    uint8_t aContent[32];

    /* 取要输出的字符的总长度 */
    totalLen = strlen((char_t*)f_pData);

    /* 无字符输出或者单行输出长度为0，直接返回  */
    if ((totalLen == 0) || (f_nMaxLen == 0))
    {
        return;
    }

    /* 开始输出字符 */
    while (os < totalLen)
    {
        if (f_pData[os] < 0x80)
        {
            /* 字符 */
            if ((cpos + 1) > f_nMaxLen)
            {
                /* 换新行 */
                rows++;
                cpos = 0;
            }
            /* 字符 */
//todo            latticeLen = zk_generate_data(f_nFont, &f_pData[os], 1, aContent,
//                    FALSE);
            if (!color)
            {
                for (j = 0; j < latticeLen; j++)
                {
                    aContent[j] = ~aContent[j];
                }
            }
            lcm_draw_bmp(x0 + cpos * 8, y0 + (f_nFont + 2) * rows, 8, f_nFont,
                    aContent);
            cpos = cpos + 1;
            os++;
        }
        else
        {
            /* 汉字 */
            if ((cpos + f_nFont / 8) > f_nMaxLen)
            {
                /* 换新行 */
                rows++;
                cpos = 0;

            }
            /* 汉字 */
//            latticeLen = zk_generate_data(f_nFont, &f_pData[os], 1, aContent,
//                    FALSE);
            if (!color)
            {
                for (j = 0; j < latticeLen; j++)
                {
                    aContent[j] = ~aContent[j];
                }
            }

            lcm_draw_bmp(x0 + cpos * 8, y0 + f_nFont * rows, f_nFont, f_nFont,
                    aContent);

            cpos = cpos + (float32_t)f_nFont / 8;
            os += 2;
        }
    }

    return;
}

/**
 ******************************************************************************
 * @brief   画水平线
 * @param[in]  x0   : 水平线起点所在列的位置
 * @param[in]  y0   : 水平线起点所在行的位置
 * @param[in]  x1   : 水平线终点所在列的位置
 * @param[in]  color: 显示颜色(对于黑白色LCM，为0时灭，为1时显示)
 *
 * @return     None
 *
 * @note 操作失败原因是指定地址超出缓冲区范围。
 ******************************************************************************
 */
static void
lcm_hline(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t color)
{
    uint8_t bak;

    if (x0 > x1) /* 对x0、x1大小进行排列，以便画图 */
    {
        bak = x1;
        x1 = x0;
        x0 = bak;
    }
    if (x0 == x1)
    {
        lcm_set_point(x0, y0, color);
        return;
    }

    do
    {
        lcm_set_point(x0, y0, color);
        x0++;
    } while (x1 >= x0);
}

/**
 ******************************************************************************
 * @brief   画垂直线
 * @param[in]  x0   : 垂直线起点所在列的位置
 * @param[in]  y0   : 垂直线起点所在行的位置
 * @param[in]  y1   : 垂直线终点所在行的位置
 * @param[in]  color: 显示颜色(对于黑白色LCM，为0时灭，为1时显示)
 *
 * @return     None
 *
 * @note 操作失败原因是指定地址超出缓冲区范围。
 ******************************************************************************
 */
static void
lcm_rline(uint8_t x0,
        uint8_t y0,
        uint8_t y1,
        uint8_t color)
{
    uint32_t bak;

    if (y0 > y1) /* 对y0、y1大小进行排列，以便画图 */
    {
        bak = y1;
        y1 = y0;
        y0 = bak;
    }
    if (y0 == y1)
    {
        lcm_set_point(x0, y0, color);
        return;
    }

    do
    {
        lcm_set_point(x0, y0, color); /* 逐点显示，描出垂直线 */
        y0++;
    } while (y1 > y0);
    lcm_set_point(x0, y0, color);
}

/**
 ******************************************************************************
 * @brief   画任意两点之间的直线
 * @param[in]  x0   : 直线起点的x坐标值
 * @param[in]  y0   : 直线起点的y坐标值
 * @param[in]  x1   : 直线终点的x坐标值
 * @param[in]  y1   : 直线终点的y坐标值
 * @param[in]  color: 显示颜色(对于黑白色LCM，为0时灭，为1时显示)
 *
 * @return None
 *
 * @note 操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_line(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t color)
{
    int16_t dx;       /* 直线x轴差值变量 */
    int16_t dy;       /* 直线y轴差值变量 */
    int8_t dx_sym;    /* x轴增长方向，为-1时减值方向，为1时增值方向 */
    int8_t dy_sym;    /* y轴增长方向，为-1时减值方向，为1时增值方向 */
    int16_t dx_x2;    /* dx*2值变量，用于加快运算速度 */
    int16_t dy_x2;    /* dy*2值变量，用于加快运算速度 */
    int16_t di;       /* 决策变量 */

    dx = x1 - x0;   /* 求取两点之间的差值 */
    dy = y1 - y0;

    /* 判断增长方向，或是否为水平线、垂直线、点 */
    if (dx > 0) // 判断x轴方向
    {
        dx_sym = 1; // dx>0，设置dx_sym=1
    }
    else
    {
        if (dx < 0)
        {
            dx_sym = -1; // dx<0，设置dx_sym=-1
        }
        else
        {
            // dx==0，画垂直线，或一点
            lcm_rline(x0, y0, y1, color);
            return;
        }
    }

    if (dy > 0) // 判断y轴方向
    {
        dy_sym = 1; // dy>0，设置dy_sym=1
    }
    else
    {
        if (dy < 0)
        {
            dy_sym = -1; // dy<0，设置dy_sym=-1
        }
        else
        {
            // dy==0，画水平线，或一点
            lcm_hline(x0, y0, x1, color);
            return;
        }
    }

    /* 将dx、dy取绝对值 */
    dx = dx_sym * dx;
    dy = dy_sym * dy;

    /* 计算2倍的dx及dy值 */
    dx_x2 = dx * 2;
    dy_x2 = dy * 2;

    /* 使用Bresenham法进行画直线 */
    if (dx >= dy) // 对于dx>=dy，则使用x轴为基准
    {
        di = dy_x2 - dx;
        while (x0 != x1)
        {
            lcm_set_point(x0, y0, color);
            x0 += dx_sym;
            if (di < 0)
            {
                di += dy_x2; // 计算出下一步的决策值
            }
            else
            {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        lcm_set_point(x0, y0, color); // 显示最后一点
    }
    else // 对于dx<dy，则使用y轴为基准
    {
        di = dx_x2 - dy;
        while (y0 != y1)
        {
            lcm_set_point(x0, y0, color);
            y0 += dy_sym;
            if (di < 0)
            {
                di += dx_x2;
            }
            else
            {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        lcm_set_point(x0, y0, color); // 显示最后一点
    }
}

#if (LCM_LINE_WITH_EN == 1u)
/**
 ******************************************************************************
 * @brief   画任意两点之间的直线，并且可设置线的宽度
 * @param[in]  x0   : 直线起点的x坐标值
 * @param[in]  y0   : 直线起点的y坐标值
 * @param[in]  x1   : 直线终点的x坐标值
 * @param[in]  y1   : 直线终点的y坐标值
 * @param[in]  width: 线宽(0-50)
 * @param[in]  color: 显示颜色(对于黑白色LCM，为0时灭，为1时显示)
 *
 * @return None
 *
 * @note 操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_line_width(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t width,
        uint8_t color)
{
    int16_t dx; // 直线x轴差值变量
    int16_t dy; // 直线y轴差值变量
    int8_t dx_sym; // x轴增长方向，为-1时减值方向，为1时增值方向
    int8_t dy_sym; // y轴增长方向，为-1时减值方向，为1时增值方向
    int16_t dx_x2; // dx*2值变量，用于加快运算速度
    int16_t dy_x2; // dy*2值变量，用于加快运算速度
    int16_t di; // 决策变量

    int16_t wx, wy; // 线宽变量
    int16_t draw_a, draw_b;

    /* 参数过滤 */
    if (width == 0)
        return;
    if (width > 50)
        width = 50;

    dx = x1 - x0; // 求取两点之间的差值
    dy = y1 - y0;

    wx = width / 2;
    wy = width - wx - 1;

    /* 判断增长方向，或是否为水平线、垂直线、点 */
    if (dx > 0) // 判断x轴方向
    {
        dx_sym = 1; // dx>0，设置dx_sym=1
    }
    else
    {
        if (dx < 0)
        {
            dx_sym = -1; // dx<0，设置dx_sym=-1
        }
        else
        {
            /* dx==0，画垂直线，或一点 */
            wx = x0 - wx;
            if (wx < 0)
                wx = 0;
            wy = x0 + wy;

            while (1)
            {
                x0 = wx;
                lcm_rline(x0, y0, y1, color);
                if (wx >= wy)
                    break;
                wx++;
            }

            return;
        }
    }

    if (dy > 0) // 判断y轴方向
    {
        dy_sym = 1; // dy>0，设置dy_sym=1
    }
    else
    {
        if (dy < 0)
        {
            dy_sym = -1; // dy<0，设置dy_sym=-1
        }
        else
        {
            /* dy==0，画水平线，或一点 */
            wx = y0 - wx;
            if (wx < 0)
                wx = 0;
            wy = y0 + wy;

            while (1)
            {
                y0 = wx;
                lcm_hline(x0, y0, x1, color);
                if (wx >= wy)
                    break;
                wx++;
            }
            return;
        }
    }

    /* 将dx、dy取绝对值 */
    dx = dx_sym * dx;
    dy = dy_sym * dy;

    /* 计算2倍的dx及dy值 */
    dx_x2 = dx * 2;
    dy_x2 = dy * 2;

    /* 使用Bresenham法进行画直线 */
    if (dx >= dy) // 对于dx>=dy，则使用x轴为基准
    {
        di = dy_x2 - dx;
        while (x0 != x1)
        {
            /* x轴向增长，则宽度在y方向，即画垂直线 */
            draw_a = y0 - wx;
            if (draw_a < 0)
                draw_a = 0;
            draw_b = y0 + wy;
            lcm_rline(x0, draw_a, draw_b, color);

            x0 += dx_sym;
            if (di < 0)
            {
                di += dy_x2; // 计算出下一步的决策值
            }
            else
            {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        draw_a = y0 - wx;
        if (draw_a < 0)
            draw_a = 0;
        draw_b = y0 + wy;
        lcm_rline(x0, draw_a, draw_b, color);
    }
    else // 对于dx<dy，则使用y轴为基准
    {
        di = dx_x2 - dy;
        while (y0 != y1)
        {
            /* y轴向增长，则宽度在x方向，即画水平线 */
            draw_a = x0 - wx;
            if (draw_a < 0)
                draw_a = 0;
            draw_b = x0 + wy;
            lcm_hline(draw_a, y0, draw_b, color);

            y0 += dy_sym;
            if (di < 0)
            {
                di += dx_x2;
            }
            else
            {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        draw_a = x0 - wx;
        if (draw_a < 0)
            draw_a = 0;
        draw_b = x0 + wy;
        lcm_hline(draw_a, y0, draw_b, color);
    }
}
#endif

/**
 ******************************************************************************
 * @brief   画矩形
 * @param[in]   x0      : 矩形左上角的x坐标值
 * @param[in]   y0      : 矩形左上角的y坐标值
 * @param[in]   x1      : 矩形右下角的x坐标值
 * @param[in]   y1      : 矩形右下角的y坐标值
 * @param[in]   color   : 显示颜色
 *
 * @return None
 * @note 操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_rectangle(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t color)
{
    lcm_hline(x0, y0, x1, color);
    lcm_hline(x0, y1, x1, color);
    lcm_rline(x0, y0, y1, color);
    lcm_rline(x1, y0, y1, color);
}

/**
 ******************************************************************************
 * @brief   填充矩形。画一个填充的矩形，填充色与边框色一样。
 * @param[in]   x0      : 矩形左上角的x坐标值
 * @param[in]   y0      : 矩形左上角的y坐标值
 * @param[in]   x1      : 矩形右下角的x坐标值
 * @param[in]   y1      : 矩形右下角的y坐标值
 * @param[in]   color   : 填充颜色
 *
 * @return None
 * @note 操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_rectangle_fill(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t color)
{
    uint8_t i;

    /* 先找出矩形左上角与右下角的两个点，保存在(x0,y0)，(x1,y1) */
    if (x0 > x1) // 若x0>x1，则x0与x1交换
    {
        i = x0;
        x0 = x1;
        x1 = i;
    }
    if (y0 > y1) // 若y0>y1，则y0与y1交换
    {
        i = y0;
        y0 = y1;
        y1 = i;
    }

    /* 判断是否只是直线 */
    if (y0 == y1)
    {
        lcm_hline(x0, y0, x1, color);
        return;
    }
    if (x0 == x1)
    {
        lcm_rline(x0, y0, y1, color);
        return;
    }

    while (y0 <= y1)
    {
        lcm_hline(x0, y0, x1, color); // 当前画水平线
        y0++; // 下一行
    }
}

/**
 ******************************************************************************
 * @brief   画正方形
 * @param[in]  x0   : 正方形左上角的x坐标值
 * @param[in]  y0   : 正方形左上角的y坐标值
 * @param[in]  width: 正方形的边长
 * @param[in]  color: 显示颜色
 *
 * @return None
 *
 * @note 操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_square(uint8_t x0,
        uint8_t y0,
        uint8_t width,
        uint8_t color)
{
    if (width == 0)
    {
        return;
    }
    lcm_rectangle(x0, y0, x0 + width, y0 + width, color);
}

/**
 ******************************************************************************
 * @brief   多个点之间的连续连线。从第一点连到第二点，再连到第三点...
 * @param[in]  *points  : 多个点坐标数据的指针，
 *                        数据排列为(x0,y0)、(x1,y1)、(x2,y2)...
 * @param[in]  no       : 点数目，至少要大于1
 * @param[in]  color    : 显示颜色
 *
 * @return None
 *
 * @note 操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_lines(uint8_t const *points,
        uint8_t no,
        uint8_t color)
{
    uint8_t x0, y0;
    uint8_t x1, y1;
    uint8_t i;

    /* 入口参数过滤 */
    if (0 == no)
        return;
    if (1 == no) // 单点
    {
        x0 = *points++;
        y0 = *points;
        lcm_set_point(x0, y0, color);
    }

    /* 画多条线条 */
    x0 = *points++; // 取出第一点坐标值，作为原起点坐标值
    y0 = *points++;
    for (i = 1; i < no; i++)
    {
        x1 = *points++; // 取出下一点坐标值
        y1 = *points++;
        lcm_line(x0, y0, x1, y1, color);
        x0 = x1; // 更新原起点坐标
        y0 = y1;
    }
}

#if  (LCM_CIRCLE_EN == 1u)
/**
 ******************************************************************************
 * @brief   指定圆心位置及半径，画圆
 * @param[in]  x0   : 圆心的x坐标值
 * @param[in]  y0   : 圆心的y坐标值
 * @param[in]  r    : 圆的半径
 * @param[in]  color: 显示颜色
 *
 * @return  None
 * @note 操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_circle(uint8_t x0,
        uint8_t y0,
        uint8_t r,
        uint8_t color)
{
    int16_t draw_x0, draw_y0; // 刽图点坐标变量
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t draw_x4, draw_y4;
    int16_t draw_x5, draw_y5;
    int16_t draw_x6, draw_y6;
    int16_t draw_x7, draw_y7;
    int16_t xx, yy; // 画圆控制变量

    int16_t di; // 决策变量

    /* 参数过滤 */
    if (0 == r)
        return;

    /* 计算出8个特殊点(0、45、90、135、180、225、270度)，进行显示 */
    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < LCM_Y_SIZE)
        lcm_set_point(draw_x0, draw_y0, color); // 90度

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0)
        lcm_set_point(draw_x2, draw_y2, color); // 270度

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < LCM_X_SIZE)
        lcm_set_point(draw_x4, draw_y4, color); // 0度

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5 >= 0)
        lcm_set_point(draw_x5, draw_y5, color); // 180度
    if (1 == r)
        return; // 若半径为1，则已圆画完

    /* 使用Bresenham法进行画圆 */
    di = 3 - 2 * r; // 初始化决策变量

    xx = 0;
    yy = r;
    while (xx < yy)
    {
        if (di < 0)
        {
            di += 4 * xx + 6;
        }
        else
        {
            di += 4 * (xx - yy) + 10;

            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }

        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

        /* 要判断当前点是否在有效范围内 */
        if ((draw_x0 <= LCM_X_SIZE) && (draw_y0 >= 0))
        {
            lcm_set_point(draw_x0, draw_y0, color);
        }
        if ((draw_x1 >= 0) && (draw_y1 >= 0))
        {
            lcm_set_point(draw_x1, draw_y1, color);
        }
        if ((draw_x2 <= LCM_X_SIZE) && (draw_y2 <= LCM_Y_SIZE))
        {
            lcm_set_point(draw_x2, draw_y2, color);
        }
        if ((draw_x3 >= 0) && (draw_y3 <= LCM_Y_SIZE))
        {
            lcm_set_point(draw_x3, draw_y3, color);
        }
        if ((draw_x4 <= LCM_X_SIZE) && (draw_y4 >= 0))
        {
            lcm_set_point(draw_x4, draw_y4, color);
        }
        if ((draw_x5 >= 0) && (draw_y5 >= 0))
        {
            lcm_set_point(draw_x5, draw_y5, color);
        }
        if ((draw_x6 <= LCM_X_SIZE) && (draw_y6 <= LCM_Y_SIZE))
        {
            lcm_set_point(draw_x6, draw_y6, color);
        }
        if ((draw_x7 >= 0) && (draw_y7 <= LCM_Y_SIZE))
        {
            lcm_set_point(draw_x7, draw_y7, color);
        }
    }
}

/**
 ******************************************************************************
 * @brief   指定圆心位置及半径，画圆并填充，填充色与边框色一样
 * @param[in]  x0   : 圆心的x坐标值
 * @param[in]  y0   : 圆心的y坐标值
 * @param[in]  r    : 圆的半径
 * @param[in]  color: 填充颜色
 *
 * @return  None
 * @note 操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_circle_fill(uint8_t x0,
        uint8_t y0,
        uint8_t r,
        uint8_t color)
{
    int16_t draw_x0, draw_y0; // 刽图点坐标变量
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t draw_x4, draw_y4;
    int16_t draw_x5, draw_y5;
    int16_t draw_x6, draw_y6;
    int16_t draw_x7, draw_y7;
    int16_t fill_x0, fill_y0; // 填充所需的变量，使用垂直线填充
    int16_t fill_x1;
    int16_t xx, yy; // 画圆控制变量

    int16_t di; // 决策变量

    /* 参数过滤 */
    if (0 == r)
        return;

    /* 计算出4个特殊点(0、90、180、270度)，进行显示 */
    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < LCM_Y_SIZE)
    {
        lcm_set_point(draw_x0, draw_y0, color); // 90度
    }

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0)
    {
        lcm_set_point(draw_x2, draw_y2, color); // 270度
    }

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < LCM_X_SIZE)
    {
        lcm_set_point(draw_x4, draw_y4, color); // 0度
        fill_x1 = draw_x4;
    }
    else
    {
        fill_x1 = LCM_X_SIZE;
    }
    fill_y0 = y0; // 设置填充线条起始点fill_x0
    fill_x0 = x0 - r; // 设置填充线条结束点fill_y1
    if (fill_x0 < 0)
        fill_x0 = 0;
    lcm_hline(fill_x0, fill_y0, fill_x1, color);

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5 >= 0)
    {
        lcm_set_point(draw_x5, draw_y5, color); // 180度
    }
    if (1 == r)
        return;

    /* 使用Bresenham法进行画圆 */
    di = 3 - 2 * r; // 初始化决策变量

    xx = 0;
    yy = r;
    while (xx < yy)
    {
        if (di < 0)
        {
            di += 4 * xx + 6;
        }
        else
        {
            di += 4 * (xx - yy) + 10;

            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }

        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

        /* 要判断当前点是否在有效范围内 */
        if ((draw_x0 <= LCM_X_SIZE) && (draw_y0 >= 0))
        {
            lcm_set_point(draw_x0, draw_y0, color);
        }
        if ((draw_x1 >= 0) && (draw_y1 >= 0))
        {
            lcm_set_point(draw_x1, draw_y1, color);
        }

        /* 第二点水直线填充(下半圆的点) */
        if (draw_x1 >= 0)
        {
            /* 设置填充线条起始点fill_x0 */
            fill_x0 = draw_x1;
            /* 设置填充线条起始点fill_y0 */
            fill_y0 = draw_y1;
            if (fill_y0 > LCM_Y_SIZE)
                fill_y0 = LCM_Y_SIZE;
            if (fill_y0 < 0)
                fill_y0 = 0;
            /* 设置填充线条结束点fill_x1 */
            fill_x1 = x0 * 2 - draw_x1;
            if (fill_x1 > LCM_X_SIZE)
                fill_x1 = LCM_X_SIZE;
            lcm_hline(fill_x0, fill_y0, fill_x1, color);
        }

        if ((draw_x2 <= LCM_X_SIZE) && (draw_y2 <= LCM_Y_SIZE))
        {
            lcm_set_point(draw_x2, draw_y2, color);
        }

        if ((draw_x3 >= 0) && (draw_y3 <= LCM_Y_SIZE))
        {
            lcm_set_point(draw_x3, draw_y3, color);
        }

        /* 第四点垂直线填充(上半圆的点) */
        if (draw_x3 >= 0)
        {
            /* 设置填充线条起始点fill_x0 */
            fill_x0 = draw_x3;
            /* 设置填充线条起始点fill_y0 */
            fill_y0 = draw_y3;
            if (fill_y0 > LCM_Y_SIZE)
                fill_y0 = LCM_Y_SIZE;
            if (fill_y0 < 0)
                fill_y0 = 0;
            /* 设置填充线条结束点fill_x1 */
            fill_x1 = x0 * 2 - draw_x3;
            if (fill_x1 > LCM_X_SIZE)
                fill_x1 = LCM_X_SIZE;
            lcm_hline(fill_x0, fill_y0, fill_x1, color);
        }

        if ((draw_x4 <= LCM_X_SIZE) && (draw_y4 >= 0))
        {
            lcm_set_point(draw_x4, draw_y4, color);
        }
        if ((draw_x5 >= 0) && (draw_y5 >= 0))
        {
            lcm_set_point(draw_x5, draw_y5, color);
        }

        /* 第六点垂直线填充(上半圆的点) */
        if (draw_x5 >= 0)
        {
            /* 设置填充线条起始点fill_x0 */
            fill_x0 = draw_x5;
            /* 设置填充线条起始点fill_y0 */
            fill_y0 = draw_y5;
            if (fill_y0 > LCM_Y_SIZE)
                fill_y0 = LCM_Y_SIZE;
            if (fill_y0 < 0)
                fill_y0 = 0;
            /* 设置填充线条结束点fill_x1 */
            fill_x1 = x0 * 2 - draw_x5;
            if (fill_x1 > LCM_X_SIZE)
                fill_x1 = LCM_X_SIZE;
            lcm_hline(fill_x0, fill_y0, fill_x1, color);
        }

        if ((draw_x6 <= LCM_X_SIZE) && (draw_y6 <= LCM_Y_SIZE))
        {
            lcm_set_point(draw_x6, draw_y6, color);
        }

        if ((draw_x7 >= 0) && (draw_y7 <= LCM_Y_SIZE))
        {
            lcm_set_point(draw_x7, draw_y7, color);
        }

        /* 第八点垂直线填充(上半圆的点) */
        if (draw_x7 >= 0)
        {
            /* 设置填充线条起始点fill_x0 */
            fill_x0 = draw_x7;
            /* 设置填充线条起始点fill_y0 */
            fill_y0 = draw_y7;
            if (fill_y0 > LCM_Y_SIZE)
                fill_y0 = LCM_Y_SIZE;
            if (fill_y0 < 0)
                fill_y0 = 0;
            /* 设置填充线条结束点fill_x1 */
            fill_x1 = x0 * 2 - draw_x7;
            if (fill_x1 > LCM_X_SIZE)
                fill_x1 = LCM_X_SIZE;
            lcm_hline(fill_x0, fill_y0, fill_x1, color);
        }
    }
}
#endif

#if  (LCM_ELLIPSE_EN == 1u)
/**
 ******************************************************************************
 * @brief   画正椭圆
 *      给定椭圆的四个点的参数，最左、最右点的x轴坐标值为x0、x1，
 *      最上、最下点的y轴坐标为y0、y1
 * @param[in]  x0   : 最左点的x坐标值
 * @param[in]  x1   : 最右点的x坐标值
 * @param[in]  y0   : 最上点的y坐标值
 * @param[in]  y1   : 最下点的y坐标值
 * @param[in]  color: 显示颜色
 *
 * @return   None
 * @note  操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_ellipse(uint8_t x0,
        uint8_t x1,
        uint8_t y0,
        uint8_t y1,
        uint8_t color)
{
    int16_t draw_x0, draw_y0; // 刽图点坐标变量
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t xx, yy; // 画图控制变量

    int16_t center_x, center_y; // 椭圆中心点坐标变量
    int16_t radius_x, radius_y; // 椭圆的半径，x轴半径和y轴半径
    int16_t radius_xx, radius_yy; // 半径乘平方值
    int16_t radius_xx2, radius_yy2; // 半径乘平方值的两倍
    int16_t di; // 定义决策变量

    /* 参数过滤 */
    if ((x0 == x1) || (y0 == y1))
        return;

    /* 计算出椭圆中心点坐标 */
    center_x = (x0 + x1) >> 1;
    center_y = (y0 + y1) >> 1;

    /* 计算出椭圆的半径，x轴半径和y轴半径 */
    if (x0 > x1)
    {
        radius_x = (x0 - x1) >> 1;
    }
    else
    {
        radius_x = (x1 - x0) >> 1;
    }
    if (y0 > y1)
    {
        radius_y = (y0 - y1) >> 1;
    }
    else
    {
        radius_y = (y1 - y0) >> 1;
    }

    /* 计算半径平方值 */
    radius_xx = radius_x * radius_x;
    radius_yy = radius_y * radius_y;

    /* 计算半径平方值乘2值 */
    radius_xx2 = radius_xx << 1;
    radius_yy2 = radius_yy << 1;

    /* 初始化画图变量 */
    xx = 0;
    yy = radius_y;

    di = radius_yy2 + radius_xx - radius_xx2 * radius_y; // 初始化决策变量

    /* 计算出椭圆y轴上的两个端点坐标，作为作图起点 */
    draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
    draw_y0 = draw_y1 = center_y + radius_y;
    draw_y2 = draw_y3 = center_y - radius_y;

    lcm_set_point(draw_x0, draw_y0, color); // 画y轴上的两个端点
    lcm_set_point(draw_x2, draw_y2, color);

    while ((radius_yy * xx) < (radius_xx * yy))
    {
        if (di < 0)
        {
            di += radius_yy2 * (2 * xx + 3);
        }
        else
        {
            di += radius_yy2 * (2 * xx + 3) + 4 * radius_xx
                    - 4 * radius_xx * yy;

            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
        }

        xx++; // x轴加1

        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;

        lcm_set_point(draw_x0, draw_y0, color);
        lcm_set_point(draw_x1, draw_y1, color);
        lcm_set_point(draw_x2, draw_y2, color);
        lcm_set_point(draw_x3, draw_y3, color);
    }

    di = radius_xx2 * (yy - 1) * (yy - 1) + radius_yy2 * xx * xx + radius_yy
            + radius_yy2 * xx - radius_xx2 * radius_yy;
    while (yy >= 0)
    {
        if (di < 0)
        {
            di += radius_xx2 * 3 + 4 * radius_yy * xx + 4 * radius_yy
                    - 2 * radius_xx2 * yy;

            xx++; // x轴加1
            draw_x0++;
            draw_x1--;
            draw_x2++;
            draw_x3--;
        }
        else
        {
            di += radius_xx2 * 3 - 2 * radius_xx2 * yy;
        }

        yy--;
        draw_y0--;
        draw_y1--;
        draw_y2++;
        draw_y3++;

        lcm_set_point(draw_x0, draw_y0, color);
        lcm_set_point(draw_x1, draw_y1, color);
        lcm_set_point(draw_x2, draw_y2, color);
        lcm_set_point(draw_x3, draw_y3, color);
    }
}

/**
 ******************************************************************************
 * @brief   画正椭圆，并填充
 *      给定椭圆的四个点的参数，最左、最右点的x轴坐标值为x0、x1，
 *      最上、最下点的y轴坐标为y0、y1
 * @param[in]  x0   : 最左点的x坐标值
 * @param[in]  x1   : 最右点的x坐标值
 * @param[in]  y0   : 最上点的y坐标值
 * @param[in]  y1   : 最下点的y坐标值
 * @param[in]  color: 填充颜色
 *
 * @return   None
 * @note  操作失败原因是指定地址超出有效范围
 ******************************************************************************
 */
static void
lcm_ellipse_fill(uint8_t x0,
        uint8_t x1,
        uint8_t y0,
        uint8_t y1,
        uint8_t color)
{
    int16_t draw_x0, draw_y0; // 刽图点坐标变量
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t xx, yy; // 画图控制变量

    int16_t center_x, center_y; // 椭圆中心点坐标变量
    int16_t radius_x, radius_y; // 椭圆的半径，x轴半径和y轴半径
    int16_t radius_xx, radius_yy; // 半径乘平方值
    int16_t radius_xx2, radius_yy2; // 半径乘平方值的两倍
    int16_t di; // 定义决策变量

    /* 参数过滤 */
    if ((x0 == x1) || (y0 == y1))
        return;

    /* 计算出椭圆中心点坐标 */
    center_x = (x0 + x1) >> 1;
    center_y = (y0 + y1) >> 1;

    /* 计算出椭圆的半径，x轴半径和y轴半径 */
    if (x0 > x1)
    {
        radius_x = (x0 - x1) >> 1;
    }
    else
    {
        radius_x = (x1 - x0) >> 1;
    }
    if (y0 > y1)
    {
        radius_y = (y0 - y1) >> 1;
    }
    else
    {
        radius_y = (y1 - y0) >> 1;
    }

    /* 计算半径乘平方值 */
    radius_xx = radius_x * radius_x;
    radius_yy = radius_y * radius_y;

    /* 计算半径乘4值 */
    radius_xx2 = radius_xx << 1;
    radius_yy2 = radius_yy << 1;

    /* 初始化画图变量 */
    xx = 0;
    yy = radius_y;

    di = radius_yy2 + radius_xx - radius_xx2 * radius_y; // 初始化决策变量

    /* 计算出椭圆y轴上的两个端点坐标，作为作图起点 */
    draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
    draw_y0 = draw_y1 = center_y + radius_y;
    draw_y2 = draw_y3 = center_y - radius_y;

    lcm_set_point(draw_x0, draw_y0, color); // 画y轴上的两个端点
    lcm_set_point(draw_x2, draw_y2, color);

    while ((radius_yy * xx) < (radius_xx * yy))
    {
        if (di < 0)
        {
            di += radius_yy2 * (2 * xx + 3);
        }
        else
        {
            di += radius_yy2 * (2 * xx + 3) + 4 * radius_xx
                    - 4 * radius_xx * yy;

            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
        }

        xx++; // x轴加1

        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;

        lcm_set_point(draw_x0, draw_y0, color);
        lcm_set_point(draw_x1, draw_y1, color);
        lcm_set_point(draw_x2, draw_y2, color);
        lcm_set_point(draw_x3, draw_y3, color);

        /* 若y轴已变化，进行填充 */
        if (di >= 0)
        {
            lcm_hline(draw_x0, draw_y0, draw_x1, color);
            lcm_hline(draw_x2, draw_y2, draw_x3, color);
        }
    }

    di = radius_xx2 * (yy - 1) * (yy - 1) + radius_yy2 * xx * xx + radius_yy
            + radius_yy2 * xx - radius_xx2 * radius_yy;
    while (yy >= 0)
    {
        if (di < 0)
        {
            di += radius_xx2 * 3 + 4 * radius_yy * xx + 4 * radius_yy
                    - 2 * radius_xx2 * yy;

            xx++; // x轴加1
            draw_x0++;
            draw_x1--;
            draw_x2++;
            draw_x3--;
        }
        else
        {
            di += radius_xx2 * 3 - 2 * radius_xx2 * yy;
        }

        yy--;
        draw_y0--;
        draw_y1--;
        draw_y2++;
        draw_y3++;

        lcm_set_point(draw_x0, draw_y0, color);
        lcm_set_point(draw_x1, draw_y1, color);
        lcm_set_point(draw_x2, draw_y2, color);
        lcm_set_point(draw_x3, draw_y3, color);

        /* y轴已变化，进行填充 */
        lcm_hline(draw_x0, draw_y0, draw_x1, color);
        lcm_hline(draw_x2, draw_y2, draw_x3, color);
    }
}
#endif

/*---------------------------------lcm.c-------------------------------------*/
