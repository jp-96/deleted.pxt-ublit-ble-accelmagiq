/**
 * https://github.com/lancaster-university/microbit-dal/blob/master/source/bluetooth/MicroBitAccelerometerService.cpp
 */

// #if !MICROBIT_CODAL

/**
 * Class definition for the custom MicroBit Accelerometer Service.
 * Provides a BLE service to remotely read the state of the accelerometer, and configure its behaviour.
 */
#include "MicroBitConfig.h"
#include "ble/UUID.h"

#include "QuaternionEstimatorServiceDal.h"

/**
 * Constructor.
 * Create a representation of the AccelerometerService
 * @param _ble The instance of a BLE device that we're running on.
 * @param _accelerometer An instance of MicroBitAccelerometer.
 */
QuaternionEstimatorService::QuaternionEstimatorService(BLEDevice &_ble, MicroBitAccelerometer &_accelerometer) : ble(_ble), accelerometer(_accelerometer)
{
  // Create the data structures that represent each of our characteristics in Soft Device.
  GattCharacteristic quaternionEstimatorDataCharacteristic(QuaternionEstimatorServiceDataUUID, (uint8_t *)quaternionEstimatorDataCharacteristicBuffer, 0,
                                                     sizeof(quaternionEstimatorDataCharacteristicBuffer), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);

  GattCharacteristic quaternionEstimatorPeriodCharacteristic(QuaternionEstimatorServicePeriodUUID, (uint8_t *)&quaternionEstimatorPeriodCharacteristicBuffer, 0,
                                                       sizeof(quaternionEstimatorPeriodCharacteristicBuffer),
                                                       GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE);

  // Initialise our characteristic values.
  quaternionEstimatorDataCharacteristicBuffer[0] = 0;
  quaternionEstimatorDataCharacteristicBuffer[1] = 0;
  quaternionEstimatorDataCharacteristicBuffer[2] = 0;
  quaternionEstimatorPeriodCharacteristicBuffer = accelerometer.getPeriod();

  // Set default security requirements
  quaternionEstimatorDataCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);
  quaternionEstimatorPeriodCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  GattCharacteristic *characteristics[] = {&quaternionEstimatorDataCharacteristic, &quaternionEstimatorPeriodCharacteristic};
  GattService service(QuaternionEstimatorServiceUUID, characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));

  ble.addService(service);

  quaternionEstimatorDataCharacteristicHandle = quaternionEstimatorDataCharacteristic.getValueHandle();
  quaternionEstimatorPeriodCharacteristicHandle = quaternionEstimatorPeriodCharacteristic.getValueHandle();

  ble.gattServer().write(quaternionEstimatorDataCharacteristicHandle, (uint8_t *)quaternionEstimatorDataCharacteristicBuffer, sizeof(quaternionEstimatorDataCharacteristicBuffer));
  ble.gattServer().write(quaternionEstimatorPeriodCharacteristicHandle, (const uint8_t *)&quaternionEstimatorPeriodCharacteristicBuffer, sizeof(quaternionEstimatorPeriodCharacteristicBuffer));

  ble.onDataWritten(this, &QuaternionEstimatorService::onDataWritten);

  if (EventModel::defaultEventBus)
    EventModel::defaultEventBus->listen(MICROBIT_ID_ACCELEROMETER, MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE, this, &QuaternionEstimatorService::quaternionEstimatorUpdate, MESSAGE_BUS_LISTENER_IMMEDIATE);
}

/**
 * Callback. Invoked when any of our attributes are written via BLE.
 */
void QuaternionEstimatorService::onDataWritten(const GattWriteCallbackParams *params)
{
  if (params->handle == quaternionEstimatorPeriodCharacteristicHandle && params->len >= sizeof(quaternionEstimatorPeriodCharacteristicBuffer))
  {
    quaternionEstimatorPeriodCharacteristicBuffer = *((uint16_t *)params->data);
    accelerometer.setPeriod(quaternionEstimatorPeriodCharacteristicBuffer);

    // The accelerometer will choose the nearest period to that requested that it can support
    // Read back the ACTUAL period it is using, and report this back.
    quaternionEstimatorPeriodCharacteristicBuffer = accelerometer.getPeriod();
    ble.gattServer().write(quaternionEstimatorPeriodCharacteristicHandle, (const uint8_t *)&quaternionEstimatorPeriodCharacteristicBuffer, sizeof(quaternionEstimatorPeriodCharacteristicBuffer));
  }
}

/**
 * Accelerometer update callback
 */
void QuaternionEstimatorService::quaternionEstimatorUpdate(MicroBitEvent)
{
  if (ble.getGapState().connected)
  {
    quaternionEstimatorDataCharacteristicBuffer[0] = accelerometer.getX();
    quaternionEstimatorDataCharacteristicBuffer[1] = accelerometer.getY();
    quaternionEstimatorDataCharacteristicBuffer[2] = accelerometer.getZ();

    ble.gattServer().notify(quaternionEstimatorDataCharacteristicHandle, (uint8_t *)quaternionEstimatorDataCharacteristicBuffer, sizeof(quaternionEstimatorDataCharacteristicBuffer));
  }
}

const uint8_t QuaternionEstimatorServiceUUID[] = {
    0xe9, 0x5d, 0x07, 0x53, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8};

const uint8_t QuaternionEstimatorServiceDataUUID[] = {
    0xe9, 0x5d, 0xca, 0x4b, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8};

const uint8_t QuaternionEstimatorServicePeriodUUID[] = {
    0xe9, 0x5d, 0xfb, 0x24, 0x25, 0x1d, 0x47, 0x0a, 0xa0, 0x62, 0xfa, 0x19, 0x22, 0xdf, 0xa9, 0xa8};

// #endif // !MICROBIT_CODAL
