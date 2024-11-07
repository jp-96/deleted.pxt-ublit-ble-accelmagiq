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

class AccelMagiQServiceBase
{

public:
  /**
   * Constructor.
   */
  AccelMagiQServiceBase();

  void quaternionUpdate(const int16_t w10000, const int16_t x10000, const int16_t y10000, const int16_t z10000);

protected:
  // memory for our 8 bit control characteristics.
  uint16_t dataBuffer[4];

private:
  // BLE wrapper methods.
  virtual void notifyData() = 0;
};

#endif // ACCELMAGIQ_SERVICE_BASE_H
