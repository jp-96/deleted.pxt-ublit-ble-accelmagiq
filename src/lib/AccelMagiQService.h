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

#include "AccelMagiQServiceBase.h"

#ifndef ACCELMAGIQ_SERVICE_H
#define ACCELMAGIQ_SERVICE_H

#if MICROBIT_CODAL

/**
 * MICROBIT_CODAL: v2
 */

#include "MicroBitBLEManager.h"
#include "MicroBitBLEService.h"

class AccelMagiQService : public AccelMagiQServiceBase, public MicroBitBLEService
{

public:
    /**
     * Constructor.
     */
    AccelMagiQService();

private:
    // BLE wrapper methods.
    void notifyData();

  private:
    // Index for each charactersitic in arrays of handles and UUIDs
    typedef enum mbbs_cIdx
    {
      mbbs_cIdxDATA,
      mbbs_cIdxCOUNT
    } mbbs_cIdx;

    // UUIDs for our service and characteristics
    static const uint8_t service_base_uuid[16];
    static const uint8_t char_base_uuid[16];
    static const uint16_t serviceUUID;
    static const uint16_t charUUID[mbbs_cIdxCOUNT];

    // Data for each characteristic when they are held by Soft Device.
    MicroBitBLEChar chars[mbbs_cIdxCOUNT];

  public:
    int characteristicCount() { return mbbs_cIdxCOUNT; };
    MicroBitBLEChar *characteristicPtr(int idx) { return &chars[idx]; };
};

#else // MICROBIT_CODAL

/**
 * NOT MICROBIT_CODAL: v1
 */

#include "ble/BLE.h"

// UUIDs for our service and characteristics
extern const uint8_t ServiceUUID[];
extern const uint8_t DataCharUUID[];

class AccelMagiQService : public AccelMagiQServiceBase
{

public:
    /**
     * Constructor.
     */
    AccelMagiQService();

private:
    // BLE wrapper methods.
    void notifyData();

private:
  // Bluetooth stack we're running on.
  BLEDevice &ble;

  // Handles to access each characteristic when they are held by Soft Device.
  GattAttribute::Handle_t dataHandle;
};

#endif // MICROBIT_CODAL

#endif // ACCELMAGIQ_SERVICE_H
