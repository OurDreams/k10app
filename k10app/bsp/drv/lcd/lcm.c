/**
 ******************************************************************************
 * @file      lcm.c
 * @brief     ����ʵ���˺ڰ׵���Һ��GUI������������.
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
 * @brief   ��ȡlcm�����ṹ
 * @param[in]  None
 *
 * @retval  lcm���� �ṹ
 ******************************************************************************
 */
const lcm_t *
lcm_opt(void)
{
    return &the_lcm_opt;
}

/**
 ******************************************************************************
 * @brief   lcm��ʼ��
 * @param[in]  type : Һ��ģ������
 *
 * @retval  OK      : ��ʼ���ɹ�
 * @retval  ERROR   : ��ʼ��ʧ��
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
 * @brief   ��ȡlcm��Ϣ
 * @param[out] *px  : ����width
 * @param[out] *py  : ����hehight
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
 * @brief   lcm�����ʾ����
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
 * @brief   �����
 * @param[in]  start    : ��ʼ��
 * @param[in]  num      : ����
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
 * @brief   lcm����
 * @param[in]  x    : x����
 * @param[in]  y    : y����
 * @param[in]  color: ��ɫ
 *
 * @return NONE
 ******************************************************************************
 */
static inline void
lcm_set_point(uint8_t x,
        uint8_t y,
        uint8_t color)
{
    lcddrv_init()->pfn_set_point(x, y, color);  /* Ƶ�����ò��ж�Ϊ�� */
}

/**
 ******************************************************************************
 * @brief   lcm��ȡ����ɫ
 * @param[in]  x    : x����
 * @param[in]  y    : y����
 *
 * @retval  0xff : ��Ч����
 * @retval  0    : ��ɫ
 * @retval  1    : ��ɫ
 ******************************************************************************
 */
static inline uint8_t
lcm_get_point(uint8_t x,
        uint8_t y)
{
    return lcddrv_init()->pfn_get_point(x, y);  /* Ƶ�����ò��ж�Ϊ�� */
}

/**
 ******************************************************************************
 * @brief   lcm��8*8����
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
 * @brief   lcm��ѡ��ʾ
 * @param[in]  x0   : ��ѡ�������Ͻ�xֵ
 * @param[in]  y0   : ��ѡ�������Ͻ�yֵ
 * @param[in]  x1   : ��ѡ�������½�xֵ
 * @param[in]  y1   : ��ѡ�������½�yֵ
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
 * @brief   lcmˢ�¼Ĵ���
 * @param[in]  contrast : �Աȶ�(0~10)
 * @param[in]  isNeedCheck: ��ʼ��ǰ�Ƿ���Ҫ���״̬; true: ��Ҫ�� false: ����Ҫ
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
 * @brief   lcm���öԱȶ�
 * @param[in]  value    : �Աȶ�ֵ(0~10)
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
 * @brief   lcm���ñ���
 * @param[in]  value    : �Աȶ�ֵ(0~100)
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
 * @brief   lcm����������lcd
 * @param[in]  x0   : �����������Ͻ�xֵ
 * @param[in]  y0   : �����������Ͻ�yֵ
 * @param[in]  x1   : �����������½�xֵ
 * @param[in]  y1   : �����������½�yֵ
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
 * @brief   lcm����ȫ��
 * @param[in]  x0   : �����������Ͻ�xֵ
 * @param[in]  y0   : �����������Ͻ�yֵ
 * @param[in]  x1   : �����������½�xֵ
 * @param[in]  y1   : �����������½�yֵ
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
 * @brief   LCD��ʾͼƬ(��ѹ��)
 * @param[in]  x    : ��ʾ���x����
 * @param[in]  y    : ��ʾ���y����
 * @param[in]  w    : ͼƬ���,����Ϊ8��������
 * @param[in]  h    : ͼƬ�߶�
 * @param[in]  *pbmp: ָ��ͼƬ����
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
    /* �жϻ�ͼ�����Ƿ�Ϸ� */
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
 * @brief   LCD��ʾͼƬ(ѹ��)
 * @param[in]  x    : ��ʾ���x����
 * @param[in]  y    : ��ʾ���y����
 * @param[in]  w    : ͼƬ���,����Ϊ8��������
 * @param[in]  h    : ͼƬ�߶�
 * @param[in]  *pic : ָ��ͼƬ����
 *
 * @return NONE
 * @note
 * ע��ͼƬʹ��ѹ����ʽ:
 * 2�����ϣ�����2���������� 255����������00��FF�ֽڿɽ���2�������ֽڴ��棬��һ
 * ���ֽڷ�00��FF,�����ĵڶ����ֽڷ�������00��FF�ĸ�������������00��FF����255��
 * �������£���00��FF���00����ʾ00��FF�ĸ���������255����Ȼ���ٸ�2���ֽڣ���ʾ
 * ʵ�ʵ�00��FF����������������65535��00������00��00��ff,ff�����棻�ٱ���������
 * 1000��ff������ff��00��e8,03�����棻ע��ߵ�λ�İ���
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
 * @brief      ��ָ��������ʾ������Ϣ
 * @param[in]  f_nFont:  �����С    FONT_SIZE_16: 16*16,16*8,
 *                                      FONT_SIZE_12: 12*12,12*8
 * @param[in]  x0:         ��ʾ��ʼλ�ú�����
 * @param[in]  y0:         ��ʾ��ʼλ��������
 * @param[in]  f_nMaxLen:  ÿһ����ʾ������ַ���
 * @param[in]  *f_pData:��ʾ����(�ַ���)
 * @param[in]  color:      �ַ���ɫ, 1: ��ɫ, 0: ��ɫ
 *
 * @retval     int: 0: ����ɹ�, -1:��󳤶ȳ���Χ
 *
 * @details
 *      Ӧ��ָ�������С����ʼλ�ã�ÿ����ʾ����,
 *      ����������������Ϣ�����ַ����,
 *      Ϊ��ʡ�ڴ棬һ��ֻ���һ���ַ�,
 *      ȡ�ַ����жϵ�ǰ��������ַ����������ǰ��>=f_nMaxLen,
 *      �����������
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
    int32_t totalLen; /* �ܳ� */
    int32_t os = 0; /* ƫ��λ�ã�ָʾ��ǰ����ַ�������Դ�е�λ�� */
    float32_t cpos = 0; /* ��ǰ��������ַ��� */
    int32_t rows = 0; /* ��ǰ����� */
    int32_t j, latticeLen;
    uint8_t aContent[32];

    /* ȡҪ������ַ����ܳ��� */
    totalLen = strlen((char_t*)f_pData);

    /* ���ַ�������ߵ����������Ϊ0��ֱ�ӷ���  */
    if ((totalLen == 0) || (f_nMaxLen == 0))
    {
        return;
    }

    /* ��ʼ����ַ� */
    while (os < totalLen)
    {
        if (f_pData[os] < 0x80)
        {
            /* �ַ� */
            if ((cpos + 1) > f_nMaxLen)
            {
                /* ������ */
                rows++;
                cpos = 0;
            }
            /* �ַ� */
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
            /* ���� */
            if ((cpos + f_nFont / 8) > f_nMaxLen)
            {
                /* ������ */
                rows++;
                cpos = 0;

            }
            /* ���� */
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
 * @brief   ��ˮƽ��
 * @param[in]  x0   : ˮƽ����������е�λ��
 * @param[in]  y0   : ˮƽ����������е�λ��
 * @param[in]  x1   : ˮƽ���յ������е�λ��
 * @param[in]  color: ��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
 *
 * @return     None
 *
 * @note ����ʧ��ԭ����ָ����ַ������������Χ��
 ******************************************************************************
 */
static void
lcm_hline(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t color)
{
    uint8_t bak;

    if (x0 > x1) /* ��x0��x1��С�������У��Ա㻭ͼ */
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
 * @brief   ����ֱ��
 * @param[in]  x0   : ��ֱ����������е�λ��
 * @param[in]  y0   : ��ֱ����������е�λ��
 * @param[in]  y1   : ��ֱ���յ������е�λ��
 * @param[in]  color: ��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
 *
 * @return     None
 *
 * @note ����ʧ��ԭ����ָ����ַ������������Χ��
 ******************************************************************************
 */
static void
lcm_rline(uint8_t x0,
        uint8_t y0,
        uint8_t y1,
        uint8_t color)
{
    uint32_t bak;

    if (y0 > y1) /* ��y0��y1��С�������У��Ա㻭ͼ */
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
        lcm_set_point(x0, y0, color); /* �����ʾ�������ֱ�� */
        y0++;
    } while (y1 > y0);
    lcm_set_point(x0, y0, color);
}

/**
 ******************************************************************************
 * @brief   ����������֮���ֱ��
 * @param[in]  x0   : ֱ������x����ֵ
 * @param[in]  y0   : ֱ������y����ֵ
 * @param[in]  x1   : ֱ���յ��x����ֵ
 * @param[in]  y1   : ֱ���յ��y����ֵ
 * @param[in]  color: ��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
 *
 * @return None
 *
 * @note ����ʧ��ԭ����ָ����ַ������Ч��Χ
 ******************************************************************************
 */
static void
lcm_line(uint8_t x0,
        uint8_t y0,
        uint8_t x1,
        uint8_t y1,
        uint8_t color)
{
    int16_t dx;       /* ֱ��x���ֵ���� */
    int16_t dy;       /* ֱ��y���ֵ���� */
    int8_t dx_sym;    /* x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ���� */
    int8_t dy_sym;    /* y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ���� */
    int16_t dx_x2;    /* dx*2ֵ���������ڼӿ������ٶ� */
    int16_t dy_x2;    /* dy*2ֵ���������ڼӿ������ٶ� */
    int16_t di;       /* ���߱��� */

    dx = x1 - x0;   /* ��ȡ����֮��Ĳ�ֵ */
    dy = y1 - y0;

    /* �ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ��� */
    if (dx > 0) // �ж�x�᷽��
    {
        dx_sym = 1; // dx>0������dx_sym=1
    }
    else
    {
        if (dx < 0)
        {
            dx_sym = -1; // dx<0������dx_sym=-1
        }
        else
        {
            // dx==0������ֱ�ߣ���һ��
            lcm_rline(x0, y0, y1, color);
            return;
        }
    }

    if (dy > 0) // �ж�y�᷽��
    {
        dy_sym = 1; // dy>0������dy_sym=1
    }
    else
    {
        if (dy < 0)
        {
            dy_sym = -1; // dy<0������dy_sym=-1
        }
        else
        {
            // dy==0����ˮƽ�ߣ���һ��
            lcm_hline(x0, y0, x1, color);
            return;
        }
    }

    /* ��dx��dyȡ����ֵ */
    dx = dx_sym * dx;
    dy = dy_sym * dy;

    /* ����2����dx��dyֵ */
    dx_x2 = dx * 2;
    dy_x2 = dy * 2;

    /* ʹ��Bresenham�����л�ֱ�� */
    if (dx >= dy) // ����dx>=dy����ʹ��x��Ϊ��׼
    {
        di = dy_x2 - dx;
        while (x0 != x1)
        {
            lcm_set_point(x0, y0, color);
            x0 += dx_sym;
            if (di < 0)
            {
                di += dy_x2; // �������һ���ľ���ֵ
            }
            else
            {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        lcm_set_point(x0, y0, color); // ��ʾ���һ��
    }
    else // ����dx<dy����ʹ��y��Ϊ��׼
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
        lcm_set_point(x0, y0, color); // ��ʾ���һ��
    }
}

#if (LCM_LINE_WITH_EN == 1u)
/**
 ******************************************************************************
 * @brief   ����������֮���ֱ�ߣ����ҿ������ߵĿ��
 * @param[in]  x0   : ֱ������x����ֵ
 * @param[in]  y0   : ֱ������y����ֵ
 * @param[in]  x1   : ֱ���յ��x����ֵ
 * @param[in]  y1   : ֱ���յ��y����ֵ
 * @param[in]  width: �߿�(0-50)
 * @param[in]  color: ��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
 *
 * @return None
 *
 * @note ����ʧ��ԭ����ָ����ַ������Ч��Χ
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
    int16_t dx; // ֱ��x���ֵ����
    int16_t dy; // ֱ��y���ֵ����
    int8_t dx_sym; // x����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
    int8_t dy_sym; // y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
    int16_t dx_x2; // dx*2ֵ���������ڼӿ������ٶ�
    int16_t dy_x2; // dy*2ֵ���������ڼӿ������ٶ�
    int16_t di; // ���߱���

    int16_t wx, wy; // �߿����
    int16_t draw_a, draw_b;

    /* �������� */
    if (width == 0)
        return;
    if (width > 50)
        width = 50;

    dx = x1 - x0; // ��ȡ����֮��Ĳ�ֵ
    dy = y1 - y0;

    wx = width / 2;
    wy = width - wx - 1;

    /* �ж��������򣬻��Ƿ�Ϊˮƽ�ߡ���ֱ�ߡ��� */
    if (dx > 0) // �ж�x�᷽��
    {
        dx_sym = 1; // dx>0������dx_sym=1
    }
    else
    {
        if (dx < 0)
        {
            dx_sym = -1; // dx<0������dx_sym=-1
        }
        else
        {
            /* dx==0������ֱ�ߣ���һ�� */
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

    if (dy > 0) // �ж�y�᷽��
    {
        dy_sym = 1; // dy>0������dy_sym=1
    }
    else
    {
        if (dy < 0)
        {
            dy_sym = -1; // dy<0������dy_sym=-1
        }
        else
        {
            /* dy==0����ˮƽ�ߣ���һ�� */
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

    /* ��dx��dyȡ����ֵ */
    dx = dx_sym * dx;
    dy = dy_sym * dy;

    /* ����2����dx��dyֵ */
    dx_x2 = dx * 2;
    dy_x2 = dy * 2;

    /* ʹ��Bresenham�����л�ֱ�� */
    if (dx >= dy) // ����dx>=dy����ʹ��x��Ϊ��׼
    {
        di = dy_x2 - dx;
        while (x0 != x1)
        {
            /* x����������������y���򣬼�����ֱ�� */
            draw_a = y0 - wx;
            if (draw_a < 0)
                draw_a = 0;
            draw_b = y0 + wy;
            lcm_rline(x0, draw_a, draw_b, color);

            x0 += dx_sym;
            if (di < 0)
            {
                di += dy_x2; // �������һ���ľ���ֵ
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
    else // ����dx<dy����ʹ��y��Ϊ��׼
    {
        di = dx_x2 - dy;
        while (y0 != y1)
        {
            /* y����������������x���򣬼���ˮƽ�� */
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
 * @brief   ������
 * @param[in]   x0      : �������Ͻǵ�x����ֵ
 * @param[in]   y0      : �������Ͻǵ�y����ֵ
 * @param[in]   x1      : �������½ǵ�x����ֵ
 * @param[in]   y1      : �������½ǵ�y����ֵ
 * @param[in]   color   : ��ʾ��ɫ
 *
 * @return None
 * @note ����ʧ��ԭ����ָ����ַ������Ч��Χ
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
 * @brief   �����Ρ���һ�����ľ��Σ����ɫ��߿�ɫһ����
 * @param[in]   x0      : �������Ͻǵ�x����ֵ
 * @param[in]   y0      : �������Ͻǵ�y����ֵ
 * @param[in]   x1      : �������½ǵ�x����ֵ
 * @param[in]   y1      : �������½ǵ�y����ֵ
 * @param[in]   color   : �����ɫ
 *
 * @return None
 * @note ����ʧ��ԭ����ָ����ַ������Ч��Χ
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

    /* ���ҳ��������Ͻ������½ǵ������㣬������(x0,y0)��(x1,y1) */
    if (x0 > x1) // ��x0>x1����x0��x1����
    {
        i = x0;
        x0 = x1;
        x1 = i;
    }
    if (y0 > y1) // ��y0>y1����y0��y1����
    {
        i = y0;
        y0 = y1;
        y1 = i;
    }

    /* �ж��Ƿ�ֻ��ֱ�� */
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
        lcm_hline(x0, y0, x1, color); // ��ǰ��ˮƽ��
        y0++; // ��һ��
    }
}

/**
 ******************************************************************************
 * @brief   ��������
 * @param[in]  x0   : ���������Ͻǵ�x����ֵ
 * @param[in]  y0   : ���������Ͻǵ�y����ֵ
 * @param[in]  width: �����εı߳�
 * @param[in]  color: ��ʾ��ɫ
 *
 * @return None
 *
 * @note ����ʧ��ԭ����ָ����ַ������Ч��Χ
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
 * @brief   �����֮����������ߡ��ӵ�һ�������ڶ��㣬������������...
 * @param[in]  *points  : ������������ݵ�ָ�룬
 *                        ��������Ϊ(x0,y0)��(x1,y1)��(x2,y2)...
 * @param[in]  no       : ����Ŀ������Ҫ����1
 * @param[in]  color    : ��ʾ��ɫ
 *
 * @return None
 *
 * @note ����ʧ��ԭ����ָ����ַ������Ч��Χ
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

    /* ��ڲ������� */
    if (0 == no)
        return;
    if (1 == no) // ����
    {
        x0 = *points++;
        y0 = *points;
        lcm_set_point(x0, y0, color);
    }

    /* ���������� */
    x0 = *points++; // ȡ����һ������ֵ����Ϊԭ�������ֵ
    y0 = *points++;
    for (i = 1; i < no; i++)
    {
        x1 = *points++; // ȡ����һ������ֵ
        y1 = *points++;
        lcm_line(x0, y0, x1, y1, color);
        x0 = x1; // ����ԭ�������
        y0 = y1;
    }
}

#if  (LCM_CIRCLE_EN == 1u)
/**
 ******************************************************************************
 * @brief   ָ��Բ��λ�ü��뾶����Բ
 * @param[in]  x0   : Բ�ĵ�x����ֵ
 * @param[in]  y0   : Բ�ĵ�y����ֵ
 * @param[in]  r    : Բ�İ뾶
 * @param[in]  color: ��ʾ��ɫ
 *
 * @return  None
 * @note ����ʧ��ԭ����ָ����ַ������Ч��Χ
 ******************************************************************************
 */
static void
lcm_circle(uint8_t x0,
        uint8_t y0,
        uint8_t r,
        uint8_t color)
{
    int16_t draw_x0, draw_y0; // ��ͼ���������
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t draw_x4, draw_y4;
    int16_t draw_x5, draw_y5;
    int16_t draw_x6, draw_y6;
    int16_t draw_x7, draw_y7;
    int16_t xx, yy; // ��Բ���Ʊ���

    int16_t di; // ���߱���

    /* �������� */
    if (0 == r)
        return;

    /* �����8�������(0��45��90��135��180��225��270��)��������ʾ */
    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < LCM_Y_SIZE)
        lcm_set_point(draw_x0, draw_y0, color); // 90��

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0)
        lcm_set_point(draw_x2, draw_y2, color); // 270��

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < LCM_X_SIZE)
        lcm_set_point(draw_x4, draw_y4, color); // 0��

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5 >= 0)
        lcm_set_point(draw_x5, draw_y5, color); // 180��
    if (1 == r)
        return; // ���뾶Ϊ1������Բ����

    /* ʹ��Bresenham�����л�Բ */
    di = 3 - 2 * r; // ��ʼ�����߱���

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

        /* Ҫ�жϵ�ǰ���Ƿ�����Ч��Χ�� */
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
 * @brief   ָ��Բ��λ�ü��뾶����Բ����䣬���ɫ��߿�ɫһ��
 * @param[in]  x0   : Բ�ĵ�x����ֵ
 * @param[in]  y0   : Բ�ĵ�y����ֵ
 * @param[in]  r    : Բ�İ뾶
 * @param[in]  color: �����ɫ
 *
 * @return  None
 * @note ����ʧ��ԭ����ָ����ַ������Ч��Χ
 ******************************************************************************
 */
static void
lcm_circle_fill(uint8_t x0,
        uint8_t y0,
        uint8_t r,
        uint8_t color)
{
    int16_t draw_x0, draw_y0; // ��ͼ���������
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t draw_x4, draw_y4;
    int16_t draw_x5, draw_y5;
    int16_t draw_x6, draw_y6;
    int16_t draw_x7, draw_y7;
    int16_t fill_x0, fill_y0; // �������ı�����ʹ�ô�ֱ�����
    int16_t fill_x1;
    int16_t xx, yy; // ��Բ���Ʊ���

    int16_t di; // ���߱���

    /* �������� */
    if (0 == r)
        return;

    /* �����4�������(0��90��180��270��)��������ʾ */
    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < LCM_Y_SIZE)
    {
        lcm_set_point(draw_x0, draw_y0, color); // 90��
    }

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0)
    {
        lcm_set_point(draw_x2, draw_y2, color); // 270��
    }

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < LCM_X_SIZE)
    {
        lcm_set_point(draw_x4, draw_y4, color); // 0��
        fill_x1 = draw_x4;
    }
    else
    {
        fill_x1 = LCM_X_SIZE;
    }
    fill_y0 = y0; // �������������ʼ��fill_x0
    fill_x0 = x0 - r; // �����������������fill_y1
    if (fill_x0 < 0)
        fill_x0 = 0;
    lcm_hline(fill_x0, fill_y0, fill_x1, color);

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5 >= 0)
    {
        lcm_set_point(draw_x5, draw_y5, color); // 180��
    }
    if (1 == r)
        return;

    /* ʹ��Bresenham�����л�Բ */
    di = 3 - 2 * r; // ��ʼ�����߱���

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

        /* Ҫ�жϵ�ǰ���Ƿ�����Ч��Χ�� */
        if ((draw_x0 <= LCM_X_SIZE) && (draw_y0 >= 0))
        {
            lcm_set_point(draw_x0, draw_y0, color);
        }
        if ((draw_x1 >= 0) && (draw_y1 >= 0))
        {
            lcm_set_point(draw_x1, draw_y1, color);
        }

        /* �ڶ���ˮֱ�����(�°�Բ�ĵ�) */
        if (draw_x1 >= 0)
        {
            /* �������������ʼ��fill_x0 */
            fill_x0 = draw_x1;
            /* �������������ʼ��fill_y0 */
            fill_y0 = draw_y1;
            if (fill_y0 > LCM_Y_SIZE)
                fill_y0 = LCM_Y_SIZE;
            if (fill_y0 < 0)
                fill_y0 = 0;
            /* �����������������fill_x1 */
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

        /* ���ĵ㴹ֱ�����(�ϰ�Բ�ĵ�) */
        if (draw_x3 >= 0)
        {
            /* �������������ʼ��fill_x0 */
            fill_x0 = draw_x3;
            /* �������������ʼ��fill_y0 */
            fill_y0 = draw_y3;
            if (fill_y0 > LCM_Y_SIZE)
                fill_y0 = LCM_Y_SIZE;
            if (fill_y0 < 0)
                fill_y0 = 0;
            /* �����������������fill_x1 */
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

        /* �����㴹ֱ�����(�ϰ�Բ�ĵ�) */
        if (draw_x5 >= 0)
        {
            /* �������������ʼ��fill_x0 */
            fill_x0 = draw_x5;
            /* �������������ʼ��fill_y0 */
            fill_y0 = draw_y5;
            if (fill_y0 > LCM_Y_SIZE)
                fill_y0 = LCM_Y_SIZE;
            if (fill_y0 < 0)
                fill_y0 = 0;
            /* �����������������fill_x1 */
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

        /* �ڰ˵㴹ֱ�����(�ϰ�Բ�ĵ�) */
        if (draw_x7 >= 0)
        {
            /* �������������ʼ��fill_x0 */
            fill_x0 = draw_x7;
            /* �������������ʼ��fill_y0 */
            fill_y0 = draw_y7;
            if (fill_y0 > LCM_Y_SIZE)
                fill_y0 = LCM_Y_SIZE;
            if (fill_y0 < 0)
                fill_y0 = 0;
            /* �����������������fill_x1 */
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
 * @brief   ������Բ
 *      ������Բ���ĸ���Ĳ������������ҵ��x������ֵΪx0��x1��
 *      ���ϡ����µ��y������Ϊy0��y1
 * @param[in]  x0   : ������x����ֵ
 * @param[in]  x1   : ���ҵ��x����ֵ
 * @param[in]  y0   : ���ϵ��y����ֵ
 * @param[in]  y1   : ���µ��y����ֵ
 * @param[in]  color: ��ʾ��ɫ
 *
 * @return   None
 * @note  ����ʧ��ԭ����ָ����ַ������Ч��Χ
 ******************************************************************************
 */
static void
lcm_ellipse(uint8_t x0,
        uint8_t x1,
        uint8_t y0,
        uint8_t y1,
        uint8_t color)
{
    int16_t draw_x0, draw_y0; // ��ͼ���������
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t xx, yy; // ��ͼ���Ʊ���

    int16_t center_x, center_y; // ��Բ���ĵ��������
    int16_t radius_x, radius_y; // ��Բ�İ뾶��x��뾶��y��뾶
    int16_t radius_xx, radius_yy; // �뾶��ƽ��ֵ
    int16_t radius_xx2, radius_yy2; // �뾶��ƽ��ֵ������
    int16_t di; // ������߱���

    /* �������� */
    if ((x0 == x1) || (y0 == y1))
        return;

    /* �������Բ���ĵ����� */
    center_x = (x0 + x1) >> 1;
    center_y = (y0 + y1) >> 1;

    /* �������Բ�İ뾶��x��뾶��y��뾶 */
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

    /* ����뾶ƽ��ֵ */
    radius_xx = radius_x * radius_x;
    radius_yy = radius_y * radius_y;

    /* ����뾶ƽ��ֵ��2ֵ */
    radius_xx2 = radius_xx << 1;
    radius_yy2 = radius_yy << 1;

    /* ��ʼ����ͼ���� */
    xx = 0;
    yy = radius_y;

    di = radius_yy2 + radius_xx - radius_xx2 * radius_y; // ��ʼ�����߱���

    /* �������Բy���ϵ������˵����꣬��Ϊ��ͼ��� */
    draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
    draw_y0 = draw_y1 = center_y + radius_y;
    draw_y2 = draw_y3 = center_y - radius_y;

    lcm_set_point(draw_x0, draw_y0, color); // ��y���ϵ������˵�
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

        xx++; // x���1

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

            xx++; // x���1
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
 * @brief   ������Բ�������
 *      ������Բ���ĸ���Ĳ������������ҵ��x������ֵΪx0��x1��
 *      ���ϡ����µ��y������Ϊy0��y1
 * @param[in]  x0   : ������x����ֵ
 * @param[in]  x1   : ���ҵ��x����ֵ
 * @param[in]  y0   : ���ϵ��y����ֵ
 * @param[in]  y1   : ���µ��y����ֵ
 * @param[in]  color: �����ɫ
 *
 * @return   None
 * @note  ����ʧ��ԭ����ָ����ַ������Ч��Χ
 ******************************************************************************
 */
static void
lcm_ellipse_fill(uint8_t x0,
        uint8_t x1,
        uint8_t y0,
        uint8_t y1,
        uint8_t color)
{
    int16_t draw_x0, draw_y0; // ��ͼ���������
    int16_t draw_x1, draw_y1;
    int16_t draw_x2, draw_y2;
    int16_t draw_x3, draw_y3;
    int16_t xx, yy; // ��ͼ���Ʊ���

    int16_t center_x, center_y; // ��Բ���ĵ��������
    int16_t radius_x, radius_y; // ��Բ�İ뾶��x��뾶��y��뾶
    int16_t radius_xx, radius_yy; // �뾶��ƽ��ֵ
    int16_t radius_xx2, radius_yy2; // �뾶��ƽ��ֵ������
    int16_t di; // ������߱���

    /* �������� */
    if ((x0 == x1) || (y0 == y1))
        return;

    /* �������Բ���ĵ����� */
    center_x = (x0 + x1) >> 1;
    center_y = (y0 + y1) >> 1;

    /* �������Բ�İ뾶��x��뾶��y��뾶 */
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

    /* ����뾶��ƽ��ֵ */
    radius_xx = radius_x * radius_x;
    radius_yy = radius_y * radius_y;

    /* ����뾶��4ֵ */
    radius_xx2 = radius_xx << 1;
    radius_yy2 = radius_yy << 1;

    /* ��ʼ����ͼ���� */
    xx = 0;
    yy = radius_y;

    di = radius_yy2 + radius_xx - radius_xx2 * radius_y; // ��ʼ�����߱���

    /* �������Բy���ϵ������˵����꣬��Ϊ��ͼ��� */
    draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
    draw_y0 = draw_y1 = center_y + radius_y;
    draw_y2 = draw_y3 = center_y - radius_y;

    lcm_set_point(draw_x0, draw_y0, color); // ��y���ϵ������˵�
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

        xx++; // x���1

        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;

        lcm_set_point(draw_x0, draw_y0, color);
        lcm_set_point(draw_x1, draw_y1, color);
        lcm_set_point(draw_x2, draw_y2, color);
        lcm_set_point(draw_x3, draw_y3, color);

        /* ��y���ѱ仯��������� */
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

            xx++; // x���1
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

        /* y���ѱ仯��������� */
        lcm_hline(draw_x0, draw_y0, draw_x1, color);
        lcm_hline(draw_x2, draw_y2, draw_x3, color);
    }
}
#endif

/*---------------------------------lcm.c-------------------------------------*/
