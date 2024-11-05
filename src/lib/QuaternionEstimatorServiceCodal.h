/**
 * https://github.com/lancaster-university/codal-microbit-v2/blob/master/inc/bluetooth/MicroBitAccelerometerService.h
 */


#ifndef QUATERNION_ESTIMATOR_SERVICE_CODAL_H
#define QUATERNION_ESTIMATOR_SERVICE_CODAL_H

// #if MICROBIT_CODAL

#include "MicroBitConfig.h"

// #if CONFIG_ENABLED(DEVICE_BLE)

#include "MicroBitBLEManager.h"
#include "MicroBitBLEService.h"
#include "MicroBitAccelerometer.h"
#include "EventModel.h"

namespace codal
{

/**
  * Class definition for a MicroBit BLE Accelerometer Service.
  * Provides access to live accelerometer data via Bluetooth, and provides basic configuration options.
  */
class MicroBitAccelerometerService : public MicroBitBLEService
{
    public:

    /**
      * Constructor.
      * Create a representation of the AccelerometerService
      * @param _accelerometer An instance of MicroBitAccelerometer.
      */
    MicroBitAccelerometerService( BLEDevice &_ble, codal::Accelerometer &_accelerometer);

    private:

    /**
      * Invoked when BLE connects.
      */
    void onConnect( const microbit_ble_evt_t *p_ble_evt);

    /**
      * Invoked when BLE disconnects.
      */
    void onDisconnect( const microbit_ble_evt_t *p_ble_evt);

    /**
      * Callback. Invoked when any of our attributes are written via BLE.
      */
    void onDataWritten( const microbit_ble_evt_write_t *params);

    /**
      * Helper function to read the values
      */
    void readXYZ();
    
    /**
      * Set up or tear down event listers
      */
    void listen( bool yes);

    /**
     * Accelerometer update callback
     */
    void accelerometerUpdate(MicroBitEvent e);

	codal::Accelerometer	&accelerometer;

    // memory for our 8 bit control characteristics.
    uint16_t            accelerometerDataCharacteristicBuffer[3];
    uint16_t            accelerometerPeriodCharacteristicBuffer;
    
    // Index for each charactersitic in arrays of handles and UUIDs
    typedef enum mbbs_cIdx
    {
        mbbs_cIdxDATA,
        mbbs_cIdxPERIOD,
        mbbs_cIdxCOUNT
    } mbbs_cIdx;
    
    // UUIDs for our service and characteristics
    static const uint16_t serviceUUID;
    static const uint16_t charUUID[ mbbs_cIdxCOUNT];
    
    // Data for each characteristic when they are held by Soft Device.
    MicroBitBLEChar      chars[ mbbs_cIdxCOUNT];

    public:
    
    int              characteristicCount()          { return mbbs_cIdxCOUNT; };
    MicroBitBLEChar *characteristicPtr( int idx)    { return &chars[ idx]; };
};

} // namespace codal

// #endif // CONFIG_ENABLED(DEVICE_BLE)

// #endif // MICROBIT_CODAL

#endif // QUATERNION_ESTIMATOR_SERVICE_CODAL_H
