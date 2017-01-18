#ifndef _Fonts_
#define _Fonts_

#if TEENSY == 1
#include <ILI9341_t3.h>
#include "font_Arial.h"
#include "font_ArialBold.h"
#include "font_Arial_Digits.h"
#include "font_ArialBold_Digits.h"
typedef ILI9341_t3_font_t font_t;
#else
#include <gfxfont.h>
typedef GFXfont font_t;
#endif


#endif

