/* USB EHCI Host for Teensy 3.6
 * Copyright 2017 Paul Stoffregen (paul@pjrc.com)
 * Copyright 2017 Dean Blackketter (dean@blackketter.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// Derived from KeyboardController from USBHost_t3

#include <Arduino.h>
#include "CherryPOSController.h"

#define print   USBHost::print_
#define println USBHost::println_

void CherryPOSController::init()
{
	contribute_Pipes(mypipes, sizeof(mypipes)/sizeof(Pipe_t));
	contribute_Transfers(mytransfers, sizeof(mytransfers)/sizeof(Transfer_t));
	driver_ready_for_device(this);
}

bool CherryPOSController::claim(Device_t *dev, int type, const uint8_t *descriptors, uint32_t len)
{
	println("CherryPOSController claim this=", (uint32_t)this, HEX);
return false;
	// only claim at interface level
	if (type != 1) return false;
	if (len < 9+9+7) return false;

	uint32_t numendpoint = descriptors[4];
	if (numendpoint < 1) return false;
	if (descriptors[5] != 3) return false; // bInterfaceClass, 3 = HID
	if (descriptors[6] != 0) return false; // bInterfaceSubClass
	if (descriptors[7] != 0) return false; // bInterfaceProtocol
	if (descriptors[9] != 9) return false;
	if (descriptors[10] != 33) return false; // HID descriptor (ignored, Boot Protocol)
	if (descriptors[18] != 7) return false;
	if (descriptors[19] != 5) return false; // endpoint descriptor
	uint32_t endpoint = descriptors[20];
	println("ep = ", endpoint, HEX);
	if ((endpoint & 0xF0) != 0x80) return false; // must be IN direction
	endpoint &= 0x0F;
	if (endpoint == 0) return false;
	if (descriptors[21] != 3) return false; // must be interrupt type
	uint32_t size = descriptors[22] | (descriptors[23] << 8);
	println("packet size = ", size);
	if (size != packetSize) return false; // must be packetSize bytes
	uint32_t interval = descriptors[24];
	println("polling interval = ", interval);
	datapipe = new_Pipe(dev, 3, endpoint, 1, packetSize, interval);
	datapipe->callback_function = callback;
	queue_Data_Transfer(datapipe, report, packetSize, this);

#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_RECIP_INTERFACE		0x01

	mk_setup(setup, USB_TYPE_CLASS | USB_RECIP_INTERFACE, 0x09, 0x200, 1, 1);  // values from cherry usbkbd.c
//	mk_setup(setup, 0x21, 10, 0, 0, 0); // 10=SET_IDLE
	queue_Control_Transfer(dev, &setup, NULL, this);
	println("CLAIMED by CherryPOSController:", (uint32_t)this, HEX);

	return true;
}

void CherryPOSController::control(const Transfer_t *transfer)
{
	println("CherryPOSController control");
}

void CherryPOSController::callback(const Transfer_t *transfer)
{
	println("CherryPOSController callback");
	if (transfer->driver) {
		((CherryPOSController *)(transfer->driver))->new_data(transfer);
	}
}

void CherryPOSController::disconnect()
{
	println("CherryPOSController disconnect");
	// TODO: free resources
}


void CherryPOSController::new_data(const Transfer_t *transfer)
{
	println("CherryPOSController Callback (member)");
	print("  KB Data: ");
	print_hexbytes(transfer->buffer, packetSize);
	memcpy(prev_report, report, packetSize);
	queue_Data_Transfer(datapipe, report, packetSize, this);
}












