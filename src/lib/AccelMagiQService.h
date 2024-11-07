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
 * @file AccelMagiQService.h
 * @brief Defines the AccelMagiQService class for the micro:bit CODAL platform (v2).
 */

#include "MicroBitBLEManager.h"
#include "MicroBitBLEService.h"

/**
 * @class AccelMagiQService
 * @brief Extends AccelMagiQServiceBase and MicroBitBLEService to provide BLE services for AccelMagiQ on MICROBIT_CODAL (v2).
 */
class AccelMagiQService : public AccelMagiQServiceBase, public MicroBitBLEService
{

public:
    /**
     * @brief Constructor to initialize the AccelMagiQService.
     */
    AccelMagiQService();

private:
    /**
     * @brief BLE interface method to notify data.
     * 
     * This method sends the updated data via BLE notifications.
     */
    void notifyData();

  private:
    /**
     * @enum mbbs_cIdx
     * @brief Index for each characteristic in arrays of handles and UUIDs.
     */
    typedef enum mbbs_cIdx
    {
        mbbs_cIdxDATA, /**< Index for data characteristic */
        mbbs_cIdxCOUNT /**< Total number of characteristics */
    } mbbs_cIdx;

    // UUIDs for our service and characteristics
    /**
     * @brief UUID for the service base.
     */
    static const uint8_t service_base_uuid[16];

    /**
     * @brief UUID for the characteristic base.
     */
    static const uint8_t char_base_uuid[16];

    /**
     * @brief UUID for the service.
     */
    static const uint16_t serviceUUID;

    /**
     * @brief UUIDs for the characteristics.
     */
    static const uint16_t charUUID[mbbs_cIdxCOUNT];

    /**
     * @brief Data for each characteristic when they are held by Soft Device.
     */
    MicroBitBLEChar chars[mbbs_cIdxCOUNT];

public:
    /**
     * @brief Gets the total number of characteristics.
     * @return The total number of characteristics.
     */
    int characteristicCount() { return mbbs_cIdxCOUNT; };

    /**
     * @brief Gets a pointer to the characteristic at the given index.
     * @param idx The index of the characteristic.
     * @return A pointer to the characteristic.
     */
    MicroBitBLEChar *characteristicPtr(int idx) { return &chars[idx]; };
};

#else // MICROBIT_CODAL

/**
 * @file AccelMagiQService.h
 * @brief Defines the AccelMagiQService class for micro:bit DAL platforms (v1).
 */

#include "ble/BLE.h"

// UUIDs for our service and characteristics
extern const uint8_t ServiceUUID[];
extern const uint8_t DataCharUUID[];

/**
 * @class AccelMagiQService
 * @brief Extends AccelMagiQServiceBase to provide BLE services for AccelMagiQ on non-MICROBIT_CODAL platforms (v1).
 */
class AccelMagiQService : public AccelMagiQServiceBase
{

public:
    /**
     * @brief Constructor to initialize the AccelMagiQService.
     */
    AccelMagiQService();

private:
    /**
     * @brief BLE interface method to notify data.
     * 
     * This method sends the updated data via BLE notifications.
     */
    void notifyData();

private:
    /**
     * @brief The Bluetooth stack we are running on.
     */
    BLEDevice &ble;

    /**
     * @brief Handle to access the data characteristic when held by Soft Device.
     */
    GattAttribute::Handle_t dataHandle;
    
};

#endif // MICROBIT_CODAL

#endif // ACCELMAGIQ_SERVICE_H
