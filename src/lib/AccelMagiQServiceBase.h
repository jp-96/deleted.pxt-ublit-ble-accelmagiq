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

/**
 * @class AccelMagiQServiceBase
 * @brief Base class for AccelMagiQ services, providing basic functionalities for managing quaternion updates and BLE notifications.
 */
class AccelMagiQServiceBase
{
public:
  /**
   * @brief Constructor for the AccelMagiQServiceBase class.
   */
  AccelMagiQServiceBase();

  /**
   * @brief Updates the quaternion values.
   * 
   * This method updates the internal quaternion representation with new values.
   * 
   * @param w10000 The W component of the quaternion, scaled by 10000.
   * @param x10000 The X component of the quaternion, scaled by 10000.
   * @param y10000 The Y component of the quaternion, scaled by 10000.
   * @param z10000 The Z component of the quaternion, scaled by 10000.
   */
  void quaternionUpdate(const int16_t w10000, const int16_t x10000, const int16_t y10000, const int16_t z10000);

protected:
  /**
   * @brief Buffer for storing 8-bit control characteristics.
   * 
   * This buffer is used to store temporary data for the control characteristics of the BLE service.
   */
  uint16_t dataBuffer[4];

private:
  /**
   * @brief Virtual method to notify data over BLE.
   * 
   * This pure virtual method must be implemented by derived classes to send the updated data via BLE notifications.
   */
  virtual void notifyData() = 0;
  
};

#endif // ACCELMAGIQ_SERVICE_BASE_H
