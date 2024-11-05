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
#include "AccelMagiQServiceBase.h"

AccelMagiQServiceBase::AccelMagiQServiceBase()
{
    // MicroBitComponent/CodalComponent
    id = CUSTOM_EVENT_ID_ACCELMAGIQ_SERVICE;
    status = 0;

    // Sensors
    tiltX = 0;
    tiltY = 0;
    memset(&uBitSensorValueOrCounter, 0, sizeof(uBitSensorValueOrCounter));

    // Display
    DisplayText = "";

    displayTextBufferHead = 0;
    displayTextBufferLength = 0;

    // Initialise our characteristic values.
    memset(&rxCharBuffer, 0, sizeof(rxCharBuffer));
    memset(&txCharBuffer, 0, sizeof(txCharBuffer));

    // Enable callback, idleTick()/idleCallback().
    fiber_add_idle_component(this);
}

void AccelMagiQServiceBase::setTiltX(const int16_t value)
{
    tiltX = value;
}

void AccelMagiQServiceBase::setTiltY(const int16_t value)
{
    tiltY = value;
}

void AccelMagiQServiceBase::setSensor(const uBit_cIdx sensor, const int valueOrCounter)
{
    if (sensor < uBit_cIdxCOUNT)
    {
        uBitSensorValueOrCounter[sensor] = valueOrCounter;
    }
}

bool AccelMagiQServiceBase::nextDisplayText()
{
    if (displayTextBufferLength > 0)
    {
        DisplayText = displayTextBuffers[displayTextBufferHead];
        displayTextBufferLength -= 1;
        displayTextBufferHead = (displayTextBufferHead + 1) % DISPLAY_TEXT_BUFFER_SIZE;
        return true;
    }
    else
    {
        // empty
        return false;
    }
}

void AccelMagiQServiceBase::update(void)
{
    uint64_t currentTime = system_timer_current_time_us();
    if (currentTime < updateSampleTimestamp)
    {
        return;
    }

    // Schedule our next sample.
    updateSampleTimestamp = currentTime + SENSOR_UPDATE_PERIOD_US;

    // RxChar - notify
    if (!getGapStateConnected())
    {
        return;
    }
    rxCharBuffer[0] = (tiltX >> 8) & 0xFF;
    rxCharBuffer[1] = tiltX & 0xFF;
    rxCharBuffer[2] = (tiltY >> 8) & 0xFF;
    rxCharBuffer[3] = tiltY & 0xFF;
    rxCharBuffer[4] = 0 == uBitSensorValueOrCounter[uBit_cIdxButtonA] ? 0 : 1;
    rxCharBuffer[5] = 0 == uBitSensorValueOrCounter[uBit_cIdxButtonB] ? 0 : 1;
    rxCharBuffer[6] = 0 == uBitSensorValueOrCounter[uBit_cIdxTouchPinP0] ? 0 : 1;
    rxCharBuffer[7] = 0 == uBitSensorValueOrCounter[uBit_cIdxTouchPinP1] ? 0 : 1;
    rxCharBuffer[8] = 0 == uBitSensorValueOrCounter[uBit_cIdxTouchPinP2] ? 0 : 1;
    rxCharBuffer[9] = (0 == uBitSensorValueOrCounter[uBit_cIdxGestureShaken] ? 0 : 1)   // bit0(1):shaken
                      | (0 == uBitSensorValueOrCounter[uBit_cIdxGestureJumped] ? 0 : 2) // bit1(2):jumped
                      | (0 == uBitSensorValueOrCounter[uBit_cIdxGestureMoved] ? 0 : 4); // bit2(4):moved
    // rxCharBuffer[19] = rxCharBuffer[19] + 1; // update counter
    sendRxCharValue((const uint8_t *)&rxCharBuffer, (const uint16_t)sizeof(rxCharBuffer));

    // one shot counter, count up to zero.
    for (int i = 0; i < uBit_cIdxCOUNT; i++)
    {
        if (uBitSensorValueOrCounter[i] < 0)
        {
            uBitSensorValueOrCounter[i] += 1;
        }
    }
}

void AccelMagiQServiceBase::onTxCharValueWritten(const uint8_t *data, const uint16_t length)
{
    if (data[0] == 0x81)
    {
        if (displayTextBufferLength < DISPLAY_TEXT_BUFFER_SIZE)
        {
            char text[length];
            memcpy(text, &(data[1]), length - 1);
            text[length - 1] = '\0';
            ManagedString message(text);
            int pos = (displayTextBufferHead + displayTextBufferLength) % DISPLAY_TEXT_BUFFER_SIZE;
            displayTextBuffers[pos] = message;
            displayTextBufferLength += 1;
            MicroBitEvent(id, S3LINK_UDK_EVT_DISPLAY_TEXT);
        }
        else
        {
            // full
            MicroBitEvent(id, S3LINK_UDK_EVT_DISPLAY_TEXT_BUFFER_FULL);
        }
    }
    else if ((data[0] == 0x82) && (length == 6))
    {
        uint8_t matrix[25];
        bool ledClear = true;
        for (int row = 0; row < 5; row++)
        {
            uint8_t rowData = data[row + 1];
            if (rowData > 0)
            {
                ledClear = false;
            }
            for (int col = 0; col < 5; col++)
            {
                matrix[col + row * 5] = (rowData & (0x01 << col)) ? 255 : 0;
            }
        }
        MicroBitImage image(5, 5, matrix);
        uBit.display.stopAnimation();
        uBit.display.print(image);
        MicroBitEvent(id, ledClear ? S3LINK_UDK_EVT_DISPLAY_CLEAR : S3LINK_UDK_EVT_DISPLAY_SYMBOL);
    }
}
