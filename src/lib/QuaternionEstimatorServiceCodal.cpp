/**
 *
 */

// #if MICROBIT_CODAL

/**
 * Class definition for the custom MicroBit Accelerometer Service.
 * Provides a BLE service to remotely read the state of the accelerometer, and configure its behaviour.
 */
#include "MicroBitConfig.h"

// #if CONFIG_ENABLED(DEVICE_BLE)

#include "MicroBitAccelerometerService.h"

using namespace codal;

const uint16_t QuaternionEstimatorService::serviceUUID = 0x0753;
const uint16_t QuaternionEstimatorService::charUUID[mbbs_cIdxCOUNT] = {0xca4b, 0xfb24};

/**
 * Constructor.
 * Create a representation of the AccelerometerService
 * @param _ble The instance of a BLE device that we're running on.
 * @param _accelerometer An instance of MicroBitAccelerometer.
 */
QuaternionEstimatorService::QuaternionEstimatorService(BLEDevice &_ble, codal::Accelerometer &_accelerometer) : accelerometer(_accelerometer)
{
  // Initialise our characteristic values.
  quaternionEstimatorDataCharacteristicBuffer[0] = 0;
  quaternionEstimatorDataCharacteristicBuffer[1] = 0;
  quaternionEstimatorDataCharacteristicBuffer[2] = 0;
  quaternionEstimatorPeriodCharacteristicBuffer = 0;

  // Register the base UUID and create the service.
  RegisterBaseUUID(bs_base_uuid);
  CreateService(serviceUUID);

  // Create the data structures that represent each of our characteristics in Soft Device.
  CreateCharacteristic(mbbs_cIdxDATA, charUUID[mbbs_cIdxDATA],
                       (uint8_t *)quaternionEstimatorDataCharacteristicBuffer,
                       sizeof(quaternionEstimatorDataCharacteristicBuffer), sizeof(quaternionEstimatorDataCharacteristicBuffer),
                       microbit_propREAD | microbit_propNOTIFY);

  CreateCharacteristic(mbbs_cIdxPERIOD, charUUID[mbbs_cIdxPERIOD],
                       (uint8_t *)&quaternionEstimatorPeriodCharacteristicBuffer,
                       sizeof(quaternionEstimatorPeriodCharacteristicBuffer), sizeof(quaternionEstimatorPeriodCharacteristicBuffer),
                       microbit_propREAD | microbit_propWRITE);

  if (getConnected())
    listen(true);
}

void QuaternionEstimatorService::readXYZ()
{
  quaternionEstimatorDataCharacteristicBuffer[0] = accelerometer.getX();
  quaternionEstimatorDataCharacteristicBuffer[1] = accelerometer.getY();
  quaternionEstimatorDataCharacteristicBuffer[2] = accelerometer.getZ();
}

/**
 * Set up or tear down event listers
 */
void QuaternionEstimatorService::listen(bool yes)
{
  if (EventModel::defaultEventBus)
  {
    if (yes)
    {
      // Ensure accelerometer is being updated
      readXYZ();
      quaternionEstimatorPeriodCharacteristicBuffer = accelerometer.getPeriod();
      EventModel::defaultEventBus->listen(MICROBIT_ID_ACCELEROMETER, MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE, this, &QuaternionEstimatorService::quaternionEstimatorUpdate, MESSAGE_BUS_LISTENER_IMMEDIATE);
    }
    else
    {
      EventModel::defaultEventBus->ignore(MICROBIT_ID_ACCELEROMETER, MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE, this, &QuaternionEstimatorService::quaternionEstimatorUpdate);
    }
  }
}

/**
 * Invoked when BLE connects.
 */
void QuaternionEstimatorService::onConnect(const microbit_ble_evt_t *p_ble_evt)
{
  listen(true);
}

/**
 * Invoked when BLE disconnects.
 */
void QuaternionEstimatorService::onDisconnect(const microbit_ble_evt_t *p_ble_evt)
{
  listen(false);
}

/**
 * Callback. Invoked when any of our attributes are written via BLE.
 */
void QuaternionEstimatorService::onDataWritten(const microbit_ble_evt_write_t *params)
{
  if (params->handle == valueHandle(mbbs_cIdxPERIOD) && params->len >= sizeof(quaternionEstimatorPeriodCharacteristicBuffer))
  {
    memcpy(&quaternionEstimatorPeriodCharacteristicBuffer, params->data, sizeof(quaternionEstimatorPeriodCharacteristicBuffer));
    accelerometer.setPeriod(quaternionEstimatorPeriodCharacteristicBuffer);

    // The accelerometer will choose the nearest period to that requested that it can support
    // Read back the ACTUAL period it is using, and store for next read.
    quaternionEstimatorPeriodCharacteristicBuffer = accelerometer.getPeriod();
    setChrValue(mbbs_cIdxPERIOD, (const uint8_t *)&quaternionEstimatorPeriodCharacteristicBuffer, sizeof(quaternionEstimatorPeriodCharacteristicBuffer));
  }
}

/**
 * Accelerometer update callback
 */
void QuaternionEstimatorService::quaternionEstimatorUpdate(MicroBitEvent)
{
  if (getConnected())
  {
    readXYZ();
    notifyChrValue(mbbs_cIdxDATA, (uint8_t *)quaternionEstimatorDataCharacteristicBuffer, sizeof(quaternionEstimatorDataCharacteristicBuffer));
  }
}

// #endif // CONFIG_ENABLED(DEVICE_BLE)

// #endif // MICROBIT_CODAL
