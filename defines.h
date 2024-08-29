 /*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * General stdiodemo defines
 *
 * $Id: defines.h,v 1.2.2.1 2009/06/25 20:21:15 joerg_wunsch Exp $
 */

#define GLUE(a, b)     a##b

/* single-bit macros, used for control bits */
#define SET_(what, p, m) GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m) GLUE(what, p) &= ~(1 << (m))
#define GET_(/* PIN, */ p, m) GLUE(PIN, p) & (1 << (m))
#define SET(what, x) SET_(what, x)
#define CLR(what, x) CLR_(what, x)
#define GET(/* PIN, */ x) GET_(x)

/* nibble macros, used for data path */
#define ASSIGN_(what, p, m, v) GLUE(what, p) = (GLUE(what, p) & \
~((1 << (m)) | (1 << ((m) + 1)) | \
(1 << ((m) + 2)) | (1 << ((m) + 3)))) | \
((v) << (m))
#define READ_(what, p, m) (GLUE(what, p) & ((1 << (m)) | (1 << ((m) + 1)) | \
(1 << ((m) + 2)) | (1 << ((m) + 3)))) >> (m)
#define ASSIGN(what, x, v) ASSIGN_(what, x, v)
#define READ(what, x) READ_(what, x)

#define _BITPOS(what, bit) (1 << (bit))
#define BITPOS(what) _BITPOS(what) 

/* HD44780 LCD port connections */
#define HD44780_RS D, 3 //4
#define HD44780_RW D, 2 //3
#define HD44780_E  D, 1 //2
/* The data bits have to be in ascending order. */
#define HD44780_D4 C, 0

/* Whether to read the busy flag, or fall back to
   worst-time delays. */
#define USE_BUSY_BIT 1



#define LCD_ROWS    2        //Liczba wyœwietlanych na LCD rzêdów
#define LCD_COLUMNS 15       //Liczba wyœwietlanych na LCD kolumn