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

#include "pxt.h"
#include "AccelMagiQService.h"

#if MICROBIT_CODAL

/**
 * MICROBIT_CODAL: v2
 */

AccelMagiQService::AccelMagiQService()
{

    // Register the base UUID and create the service.
    RegisterBaseUUID(service_base_uuid);
    CreateService(serviceUUID);

    // Register the base UUID and create the data structures that represent each of our characteristics in Soft Device.
    RegisterBaseUUID(char_base_uuid);
    CreateCharacteristic(
        mbbs_cIdxDATA, charUUID[mbbs_cIdxDATA],
        (uint8_t *)dataBuffer,
        sizeof(dataBuffer), sizeof(dataBuffer),
        microbit_propREAD | microbit_propNOTIFY);

    // Default values.
    writeChrValue(
        mbbs_cIdxDATA,
        (const uint8_t *)&dataBuffer,
        sizeof(dataBuffer));
}

void AccelMagiQService::notifyData()
{
    if (getConnected())
    {
        notifyChrValue(
            mbbs_cIdxDATA,
            (uint8_t *)dataBuffer,
            sizeof(dataBuffer));
    }
}

// base uuid : {E95D****-251D-470A-A062-FA1922DFA9A8}
const uint8_t AccelMagiQService::service_base_uuid[16] =
    {0xe9, 0x5d, 0x00, 0x00,
     0x25, 0x1d,
     0x47, 0x0a,
     0xa0, 0x62,
     0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8};

// base uuid : {E95D****-251D-470A-A062-FA1922DFA9A8}
const uint8_t AccelMagiQService::char_base_uuid[16] =
    {0xe9, 0x5d, 0x00, 0x00,
     0x25, 0x1d,
     0x47, 0x0a,
     0xa0, 0x62,
     0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8};

// uuid
const uint16_t AccelMagiQService::serviceUUID = 0x0753;
const uint16_t AccelMagiQService::charUUID[mbbs_cIdxCOUNT] = {0xca4b};

#else // MICROBIT_CODAL

/**
 * NOT MICROBIT_CODAL: v1
 */

AccelMagiQService::AccelMagiQService() : ble(*uBit.ble)
{

    // Create the data structures that represent each of our characteristics in Soft Device.
    GattCharacteristic dataChar(
        DataCharUUID,
        (uint8_t *)dataBuffer, 0, sizeof(dataBuffer),
        GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);

    // Set default security requirements
    dataChar.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *chars[] = {&dataChar};
    GattService service(
        ServiceUUID,
        chars, sizeof(chars) / sizeof(GattCharacteristic *));

    ble.addService(service);

    dataHandle = dataChar.getValueHandle();

    ble.gattServer().write(
        dataHandle,
        (uint8_t *)dataBuffer, sizeof(dataBuffer));
}

void AccelMagiQService::notifyData()
{
    if (ble.getGapState().connected)
    {
        ble.gattServer().notify(
            dataHandle,
            (uint8_t *)dataBuffer,
            sizeof(dataBuffer));
    }
}

const uint8_t ServiceUUID[] =
    {0xe9, 0x5d, 0x07, 0x53,
     0x25, 0x1d,
     0x47, 0x0a,
     0xa0, 0x62,
     0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8};

const uint8_t DataCharUUID[] =
    {0xe9, 0x5d, 0xca, 0x4b,
     0x25, 0x1d,
     0x47, 0x0a,
     0xa0, 0x62,
     0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8};

#endif // MICROBIT_CODAL
