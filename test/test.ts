/**
 * tests go here; this will not be compiled when this package is used as an extension.
 */
bluetooth.onBluetoothConnected(function () {
    basic.showIcon(IconNames.Yes)
})
bluetooth.onBluetoothDisconnected(function () {
    basic.showIcon(IconNames.Happy)
})
input.onButtonPressed(Button.A, function () {
    custom.notifyData(
    12345,
    -12345,
    1,
    -1
    )
})
input.onButtonPressed(Button.B, function () {
    custom.sendQuatArray([
    0,
    1,
    2,
    3
    ])
})
custom.startService()
basic.showIcon(IconNames.Heart)
