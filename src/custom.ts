
/**
* Use this file to define custom functions and blocks.
* Read more at https://makecode.microbit.org/blocks/custom
*/

enum MyEnum {
    //% block="one-1"
    One,
    //% block="two-2"
    Two
}

/**
 * Custom blocks
 * icon: a Unicode identifier for an icon from the Font Awesome icon set.
 *       http://fontawesome.io/icons
 */
//% block="Custom Blocks"
//% weight=100 color=#696969 icon="\uf1b2"
namespace custom {

    //% block
    //% shim=custom::startService
    export function startService(): void {
        return;
    }

    //% block
    //% shim=custom::notifyData
    export function notifyData(w10000: number, x10000: number, y10000: number, z10000: number): void {
        return;
    }

    //% block
    export function sendQuatArray(q: number[]): void {
        if (4 != q.length) {
            return;
        }
        const w10000 = 10000 * q[0];
        const x10000 = 10000 * q[1];
        const y10000 = 10000 * q[2];
        const z10000 = 10000 * q[3];
        notifyData(w10000, x10000, y10000, z10000);
    }
}
