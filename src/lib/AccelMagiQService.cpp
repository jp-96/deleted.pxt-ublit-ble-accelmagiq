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

    // Register the base UUID and create the characteristics.
    RegisterBaseUUID(char_base_uuid);
    CreateCharacteristic(mbbs_cIdxRxChar, charUUID[mbbs_cIdxRxChar],
                         (uint8_t *)&rxCharBuffer,
                         sizeof(rxCharBuffer), sizeof(rxCharBuffer),
                         microbit_propREAD | microbit_propNOTIFY);
    CreateCharacteristic(mbbs_cIdxTxChar, charUUID[mbbs_cIdxTxChar],
                         (uint8_t *)&txCharBuffer,
                         sizeof(txCharBuffer), sizeof(txCharBuffer),
                         microbit_propWRITE);

    // Default values.
    writeChrValue(mbbs_cIdxRxChar, (const uint8_t *)&rxCharBuffer, sizeof(rxCharBuffer));
}

void AccelMagiQService::idleCallback()
{
    update();
}

bool AccelMagiQService::getGapStateConnected()
{
    return getConnected();
}

void AccelMagiQService::sendRxCharValue(const uint8_t *data, const uint16_t length)
{
    notifyChrValue(mbbs_cIdxRxChar, data, length);
}

void AccelMagiQService::onDataWritten(const microbit_ble_evt_write_t *params)
{
    microbit_charattr_t type;
    int index = charHandleToIdx(params->handle, &type);

    if (index == mbbs_cIdxTxChar && params->len >= 1)
    {
        onTxCharValueWritten((const uint8_t *)params->data, (const uint16_t)params->len);
    }
}

// base uuid : {00000000-0000-1000-8000-00805F9B34FB}
const uint8_t AccelMagiQService::service_base_uuid[16] =
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb};

// base uuid : {52610000-FA7E-42AB-850B-7C80220097CC}
const uint8_t AccelMagiQService::char_base_uuid[16] =
    {0x52, 0x61, 0x00, 0x00, 0xfa, 0x7e, 0x42, 0xab, 0x85, 0x0b, 0x7c, 0x80, 0x22, 0x00, 0x97, 0xcc};

// uuid
const uint16_t AccelMagiQService::serviceUUID = 0xf005;
const uint16_t AccelMagiQService::charUUID[mbbs_cIdxCOUNT] =
    {
        0xda01, /* mbbs_cIdxRxChar */
        0xda02  /* mbbs_cIdxTxChar */
};

#else // MICROBIT_CODAL

/**
 * NOT MICROBIT_CODAL: v1
 */

AccelMagiQService::AccelMagiQService() : ble(*uBit.ble)
{

    // Caractieristic
    GattCharacteristic rxChar(
        rxCharUUID, (uint8_t *)&rxCharBuffer, 0, sizeof(rxCharBuffer), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
    GattCharacteristic txChar(
        txCharUUID, (uint8_t *)&txCharBuffer, 0, sizeof(txCharBuffer), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

    // Set default security requirements
    rxChar.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
    txChar.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    // Service
    GattCharacteristic *characteristics[] = {
        &rxChar,
        &txChar,
    };
    GattService service(
        serviceUUID, characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));
    ble.addService(service);

    // Characteristic Handle
    rxCharHandle = rxChar.getValueHandle();
    txCharHandle = txChar.getValueHandle();

    // Default values.
    ble.gattServer().write(rxCharHandle, (const uint8_t *)&rxCharBuffer, sizeof(rxCharBuffer));

    // onDataWritten
    ble.onDataWritten(this, &AccelMagiQService::onDataWritten);
}

void AccelMagiQService::idleTick()
{
    update();
}

bool AccelMagiQService::getGapStateConnected()
{
    return ble.getGapState().connected;
}

void AccelMagiQService::sendRxCharValue(const uint8_t *data, const uint16_t length)
{
    ble.gattServer().notify(rxCharHandle, data, length);
}

void AccelMagiQService::onDataWritten(const GattWriteCallbackParams *params)
{
    if (params->handle == txCharHandle && params->len >= 1)
    {
        onTxCharValueWritten((const uint8_t *)params->data, (const uint16_t)params->len);
    }
}

// uuid
const uint16_t AccelMagiQService::serviceUUID = 0xf005; // short: 0xF005
// base uuid : {52610000-FA7E-42AB-850B-7C80220097CC}
const uint8_t AccelMagiQService::rxCharUUID[16] =
    {0x52, 0x61, 0xda, 0x01, 0xfa, 0x7e, 0x42, 0xab, 0x85, 0x0b, 0x7c, 0x80, 0x22, 0x00, 0x97, 0xcc}; // 0xDA01
const uint8_t AccelMagiQService::txCharUUID[16] =
    {0x52, 0x61, 0xda, 0x02, 0xfa, 0x7e, 0x42, 0xab, 0x85, 0x0b, 0x7c, 0x80, 0x22, 0x00, 0x97, 0xcc}; // 0xDA02

#endif // MICROBIT_CODAL
