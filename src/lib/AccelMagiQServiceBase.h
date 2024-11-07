/*
MIT License

Copyright (c) 2022 jp-rad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ACCELMAGIQ_SERVICE_BASE_H
#define ACCELMAGIQ_SERVICE_BASE_H

#include "pxt.h"

// AccelMagiQ Service custom Event Bus ID. (base:32768)
#define CUSTOM_EVENT_ID_ACCELMAGIQ_SERVICE (32768 + 5)

// Event Value
#define S3LINK_UDK_EVT_DISPLAY_TEXT_BUFFER_FULL 0x01
#define S3LINK_UDK_EVT_DISPLAY_CLEAR 0x02
#define S3LINK_UDK_EVT_DISPLAY_TEXT 0x81
#define S3LINK_UDK_EVT_DISPLAY_SYMBOL 0x82

#define DISPLAY_TEXT_BUFFER_SIZE 5

class AccelMagiQServiceBase : MicroBitComponent
{

public:
  /**
   * Constructor.
   */
  AccelMagiQServiceBase();

  /**
   * Setters.
   */
  void setTiltX(const int16_t value);
  void setTiltY(const int16_t value);

  typedef enum uBit_cIdx
  {
    uBit_cIdxButtonA,
    uBit_cIdxButtonB,
    uBit_cIdxTouchPinP0,
    uBit_cIdxTouchPinP1,
    uBit_cIdxTouchPinP2,
    uBit_cIdxGestureShaken,
    uBit_cIdxGestureJumped,
    uBit_cIdxGestureMoved,
    uBit_cIdxCOUNT
  } uBit_cIdx;

  void setSensor(const uBit_cIdx sensor, const int valueOrCounter);

  bool nextDisplayText();

protected:
  /**
   * Notify sensors data.
   */
  void update();

private:
  // next sample timestamp scheduled
  static const uint64_t SENSOR_UPDATE_PERIOD_US = 24000; // 24ms
  uint64_t updateSampleTimestamp = 0;

private:
  int16_t tiltX;
  int16_t tiltY;

  int uBitSensorValueOrCounter[uBit_cIdxCOUNT]; // zero, positive, negative. negative is to count up to zero.

  int displayTextBufferHead;
  int displayTextBufferLength;
  ManagedString displayTextBuffers[DISPLAY_TEXT_BUFFER_SIZE];

public:
  ManagedString DisplayText;

protected:
  // Characteristic buffer
  uint8_t rxCharBuffer[20]; // display command (from S3 to device)
  uint8_t txCharBuffer[20]; // sensors data (from device to S3)

  /**
   * Callback. Invoked when any of our attributes are written via BLE, CODAL/DAL.
   */
  void onTxCharValueWritten(const uint8_t *data, const uint16_t length);

private:
  // BLE wrapper methods.
  virtual bool getGapStateConnected() = 0;
  virtual void sendRxCharValue(const uint8_t *data, const uint16_t length) = 0;
};

#endif // ACCELMAGIQ_SERVICE_BASE_H
