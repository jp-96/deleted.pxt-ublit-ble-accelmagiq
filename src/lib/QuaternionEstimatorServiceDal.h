/**
 * https://github.com/lancaster-university/microbit-dal/blob/master/inc/bluetooth/MicroBitAccelerometerService.h
 */

#ifndef QUATERNION_ESTIMATOR_SERVICE_DAL_H
#define QUATERNION_ESTIMATOR_SERVICE_DAL_H

#if !MICROBIT_CODAL

#include "MicroBitConfig.h"
#include "ble/BLE.h"
#include "MicroBitAccelerometer.h"
#include "EventModel.h"

// UUIDs for our service and characteristics
extern const uint8_t  MicroBitAccelerometerServiceUUID[];
extern const uint8_t  MicroBitAccelerometerServiceDataUUID[];
extern const uint8_t  MicroBitAccelerometerServicePeriodUUID[];


/**
  * Class definition for a MicroBit BLE Accelerometer Service.
  * Provides access to live accelerometer data via Bluetooth, and provides basic configuration options.
  */
class MicroBitAccelerometerService
{
    public:

    /**
      * Constructor.
      * Create a representation of the AccelerometerService
      * @param _ble The instance of a BLE device that we're running on.
      * @param _accelerometer An instance of MicroBitAccelerometer.
      */
    MicroBitAccelerometerService(BLEDevice &_ble, MicroBitAccelerometer &_acclerometer);


    private:

    /**
      * Callback. Invoked when any of our attributes are written via BLE.
      */
    void onDataWritten(const GattWriteCallbackParams *params);

    /**
     * Accelerometer update callback
     */
    void accelerometerUpdate(MicroBitEvent e);

    // Bluetooth stack we're running on.
    BLEDevice           	&ble;
	MicroBitAccelerometer	&accelerometer;

    // memory for our 8 bit control characteristics.
    uint16_t            accelerometerDataCharacteristicBuffer[3];
    uint16_t            accelerometerPeriodCharacteristicBuffer;

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t accelerometerDataCharacteristicHandle;
    GattAttribute::Handle_t accelerometerPeriodCharacteristicHandle;
};

#endif // !MICROBIT_CODAL

#endif // QUATERNION_ESTIMATOR_SERVICE_DAL_H
