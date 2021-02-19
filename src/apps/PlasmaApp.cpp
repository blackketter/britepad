#include "BritepadShared.h"
#include "ScreensaverApp.h"
// plasma based on DemoSauce example
class PlasmaApp : public ScreensaverApp {
  public:
    void run();
    void begin();

    const char* name() { return "Plasma"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "plas";

    bool canBeMouse() { return true; }

  private:

    float _phase = 0;
    float _colorCycle = 0;
    uint8_t _ditherY = 0;
    color_t _bgColor;

    const float PLASMA_CLOUD_SPEED = 0.02;
    const uint16_t PLASMA_CLOUD_MARGIN = 25;

    const uint8_t PLASMA_CLOUD_LINE_WIDTH = 4;
    const uint8_t PLASMA_CLOUD_STEP_Y = 4;

    const float PLASMA_COLOR_CYCLE_SPEED = 0.003;

    static const uint8_t SQRT_TABLE_LENGTH = 255;
    uint8_t sqrtTable[ SQRT_TABLE_LENGTH ];
    uint8_t sqrtBitShift;  // Shift distances (integers) this many bits. This will give you a lookup index to get the square root.

};

PlasmaApp thePlasmaApp;

void PlasmaApp::begin() {
  ScreensaverApp::begin();
    _bgColor = screen.color565( 0x77, 0, 0xcc );

  float w = (float)screen.clipWidth();
  float h = (float)screen.clipHeight();

  float maxScreenDistance = (w*w) + (h*h);
  uint8_t nextPow2 = ceil( log2f( maxScreenDistance ) );
  float maxTableDistance = pow( 2.0f, nextPow2 );
  sqrtBitShift = nextPow2 - 8;

  float sqrtTableMult = (float)0xff / sqrt(maxScreenDistance);

  for( uint8_t i=0; i<SQRT_TABLE_LENGTH; i++ ) {
    float useDistance = ((float)i / (float)SQRT_TABLE_LENGTH) * maxTableDistance;
    float aSqrt = sqrt( useDistance );
    sqrtTable[i] = (uint8_t)(aSqrt * sqrtTableMult);
    //sqrtTable[i] = random(0xff);
  }
}

void PlasmaApp::run() {
  ScreensaverApp::run();
  coord_t w = screen.clipWidth();
  coord_t h = screen.clipHeight();

  _phase += PLASMA_CLOUD_SPEED;

  _colorCycle -= PLASMA_COLOR_CYCLE_SPEED;
  if( _colorCycle <= 0.0f ) _colorCycle += 1.0f;
  uint8_t colorAdd = _colorCycle * 0xff;

  //py._ditherY = (py._ditherY + 1) % PLASMA_YELLOW_DITHER;

  point_t p0 = (point_t){
    (coord_t)(w/2 + (sin(_phase*0.32f)*(w/2-PLASMA_CLOUD_MARGIN) )),
    (coord_t)(h/2 + (sin(_phase*0.23f)*(h/2-PLASMA_CLOUD_MARGIN) ))
  };
  point_t p1 = (point_t){
    (coord_t)(w/2 + (cos(_phase*1.07f)*(w/2-PLASMA_CLOUD_MARGIN) )),
    (coord_t)(h/2 + (cos(_phase*1.42f)*(h/2-PLASMA_CLOUD_MARGIN) ))
  };

  /*
  point_t p2 = (point_t){
    (coord_t)(w/2 + (cos(_phase*0.57f)*(w/2-PLASMA_CLOUD_MARGIN) )),
    (coord_t)(h/2 + (cos(_phase*0.81f)*(h/2-PLASMA_CLOUD_MARGIN) ))
  };
  */

  if (pad.touched(SCREEN_PAD)) {
    p0.x = pad.x();
    p0.y = pad.y();
  }

  if ((Uptime::millis() - usbMouse.lastMove()) < 500) {
    p1.x = usbMouse.x();
    p1.y = usbMouse.y();
  }


  for( coord_t x=0; x<w; x+=PLASMA_CLOUD_LINE_WIDTH ) {
    launcher.idle();
    for( coord_t y=_ditherY; y<h; y+=PLASMA_CLOUD_STEP_Y ) {
      point_t d0, d1;
      d0.x = abs(p0.x - x);
      d0.y = abs(p0.y - y);
      d1.x = abs(p1.x - x);
      d1.y = abs(p1.y - y);

      //point_t d2 = (point_t){ abs(p2.x - x), abs(p2.y - y) };

      uint8_t lookup0 = (d0.x*d0.x + d0.y*d0.y) >> sqrtBitShift;
      uint8_t lookup1 = (d1.x*d1.x + d1.y*d1.y) >> sqrtBitShift;
      //uint8_t lookup2 = (d2.x*d2.x + d2.y*d2.y) >> sqrtBitShift;

      uint8_t bright = (sqrtTable[ lookup0 ] * sqrtTable[ lookup1 ] ) >> 6;
      bright = (uint16_t)(bright + colorAdd) & 0xff;

      uint16_t color = screen.color565( ((bright&0xfb)<<2), 0, bright );
      screen.drawFastHLine( x, y, PLASMA_CLOUD_LINE_WIDTH, color );
    }
  }

  _ditherY = (_ditherY + 1) % PLASMA_CLOUD_STEP_Y;

}
