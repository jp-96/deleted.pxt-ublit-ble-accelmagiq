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
    // Initialise our characteristic values.
    quaternionDataCharacteristicBuffer[0] = 10000;
    quaternionDataCharacteristicBuffer[1] = 0;
    quaternionDataCharacteristicBuffer[2] = 0;
    quaternionDataCharacteristicBuffer[3] = 0;
}

void AccelMagiQServiceBase::quaternionUpdate(const int16_t w10000, const int16_t x10000, const int16_t y10000, const int16_t z10000)
{
    quaternionDataCharacteristicBuffer[0] = w10000;
    quaternionDataCharacteristicBuffer[1] = x10000;
    quaternionDataCharacteristicBuffer[2] = y10000;
    quaternionDataCharacteristicBuffer[3] = z10000;
    notifyQuaternionData();
}
