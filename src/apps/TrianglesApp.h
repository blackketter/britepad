#include "BriteLiteApp.h"

class TrianglesApp : public BriteLiteApp {

  public:
    void begin(AppMode asMode);

    static constexpr appid_t ID = "tria";
    appid_t id() { return ID; };
    const char* name() { return "Triangles"; };
  protected:
    int getDotsWide() { return 24; }  // approximately equalateral triangles
    int getDotsHigh() { return 9; }

};