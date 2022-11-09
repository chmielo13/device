#ifndef LLBLE_H
#define LLBLE_H



#include <ArduinoBLE.h>
#include "BLETypedCharacteristics.h"


enum BleStatus {
    BLE_STATUS_UNKNOWN,
    BLE_STATUS_CENTRAL_FOUND,
    BLE_STATUS_CENTRAL_NOT_FOUND,
    BLE_STATUS_CENTRAL_CONNECTED,
    BLE_STATUS_CENTRAL_DISCONNECTED,
    BLE_STATUS_CENTRAL_NO_LONGER_FOUND
};



char * bleStatusString(BleStatus status);


class LongLongCharacteristic : public BLETypedCharacteristic<long long> {
public:
  LongLongCharacteristic(const char* uuid, unsigned char properties);
};

LongLongCharacteristic::LongLongCharacteristic(const char* uuid, unsigned char properties) :
  BLETypedCharacteristic<long long>(uuid, properties)
{
}


BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

LongLongCharacteristic triggerTimeCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
BLEIntCharacteristic inflationDurationCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite);



// // Assumes SIGNED long long value!
// class LongLongCharacteristic {
//
//     // Long Long Version 1 (used when WHICH = 1)
//     BLELongCharacteristic* longCharacteristicPart1A;
//     BLELongCharacteristic* longCharacteristicPart1B;
//
//     // Long Long Version 2 (used when WHICH = 2)
//     BLELongCharacteristic* longCharacteristicPart2A;
//     BLELongCharacteristic* longCharacteristicPart2B;
//
//     // Specifies which stored Long Long version to use.
//     BLECharCharacteristic* charCharacteristicWhich;
//
//  
//
//     public:
//
//     static void splitLongLong(long long value, long result[]) {
//         // Big Endian, most significant byte first.
//         unsigned long longs[2];
//         unsigned long long n = value;
//         longs[0] = (n >> 32) & 0xFFFFFFFF;
//         longs[1] = n & 0xFFFFFFFF;
//         result[0] = longs[0];
//         result[1] = longs[1];
//     }
//
//     static long long composeLongLong(long long0, long long1) {
//         // Serial.println("Recomposing...");
//         // Serial.println(((long long) long0) << 32, BIN);
//         // Serial.println( long1, BIN);
//         // Serial.println((unsigned long long) ((unsigned) long1), BIN);
//         long long result = ((unsigned long long) ((unsigned) long1)) | ((long long) long0) << 32;
//         // long long result = long0 | long1 << 32;
//         return result;
//     }
//
//     // TODO: (Use Serial print?)
//     // static void printLongBinary
//     // static void printLongLongBinary
//
//     // static long long 
//
//     // LongLongCharacteristic(int a) {
//
//     // }
//
//
//     LongLongCharacteristic(
//             BLELongCharacteristic* part1A,
//             BLELongCharacteristic* part1B,
//             BLELongCharacteristic* part2A, 
//             BLELongCharacteristic* part2B, 
//             BLECharCharacteristic* which
//         ) {
//         longCharacteristicPart1A = part1A;
//         longCharacteristicPart1B = part1B;
//         longCharacteristicPart2A = part2A;
//         longCharacteristicPart2B = part2B;
//         charCharacteristicWhich = which;
//     }
//
//     void writeValue(long long value) {
//         long longs[2];
//         splitLongLong(value, longs);
//
//         if ((charCharacteristicWhich->value() == 0) || (charCharacteristicWhich->value() == 2)) {
//             // Uninitialzed, or Version 2 currently used. Write to Version 1.
//             longCharacteristicPart1A->writeValue(longs[0]);
//             longCharacteristicPart1B->writeValue(longs[1]);
//             charCharacteristicWhich->writeValue(1);
//         } else if (charCharacteristicWhich->value() == 1) {
//             // Version 1 currently used. Write to Version 2.
//             longCharacteristicPart2A->writeValue(longs[0]);
//             longCharacteristicPart2B->writeValue(longs[1]);
//             charCharacteristicWhich->writeValue(2);
//         } else {
//             Serial.println('Error, unrecognized Long Version.');
//         }
//     }
//
//     long long value() {
//         long long0, long1;
//         if (charCharacteristicWhich->value() == 0) {
//             // Uninitialized. Return 0.
//             return 0;
//         } else if (charCharacteristicWhich->value() == 1) {
//             // Stored in Version 1.
//
//         }
//     }
//
// };



#endif
