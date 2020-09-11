#ifndef _Fonts_
#define _Fonts_

#if defined(LCD_ERTFTM0784)
#include "_fonts.h"
#endif

#if TEENSY == 1
#include "font_Arial.h"
#include "font_ArialBold.h"
#include "font_Arial_Digits.h"
#include "font_ArialBold_Digits.h"
typedef const ILI9341_t3_font_t* font_t;
#else
#include <gfxfont.h>
typedef GFXfont font_t;
#endif


#endif

