#ifndef _CherryPOSController_
#define _CherryPOSController_

#include "USBHost_t36.h"

class CherryPOSController : public USBDriver /* , public USBHIDInput */ {
public:
	CherryPOSController(USBHost &host) { init(); }
	CherryPOSController(USBHost *host) { init(); }
	CherryPOSController() { init(); }
	int      available();
	int      read();
protected:
	virtual bool claim(Device_t *device, int type, const uint8_t *descriptors, uint32_t len);
	virtual void control(const Transfer_t *transfer);
	virtual void disconnect();
	static void callback(const Transfer_t *transfer);
	void new_data(const Transfer_t *transfer);
	void init();
private:
  static const int packetSize = 64;
	void update();
	Pipe_t *datapipe;
	setup_t setup;
	uint8_t report[packetSize];
	uint8_t prev_report[packetSize];
	Pipe_t mypipes[2] __attribute__ ((aligned(32)));
	Transfer_t mytransfers[4] __attribute__ ((aligned(32)));
};

#endif