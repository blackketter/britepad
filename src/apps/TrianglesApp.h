#include "BriteLiteApp.h"

class TrianglesApp : public BriteLiteApp {

  public:
    void begin();

    static constexpr appid_t ID = "tria";
    appid_t id() { return ID; };
    const char* name() { return "Triangles"; };
  protected:
    int getDotsWide() { return screen.width() / 14; }
    int getDotsHigh() { return screen.height() / 24; }

};
