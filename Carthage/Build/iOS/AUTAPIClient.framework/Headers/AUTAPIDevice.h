//
//  AUTAPIDevice.h
//  AUTAPIClient
//
//  Created by Justin Spahr-Summers on 2015-04-17.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIPage.h>
#import <AUTAPIClient/AUTAPIObject.h>

NS_ASSUME_NONNULL_BEGIN

/// Represents a registered device.
@interface AUTAPIDevice : AUTAPIObject

/// A dictionary mapping zero or more `AUTAPIFirmwareType` keys to the
/// `AUTAPIFirmware` values that correspond to them.
@property (nonatomic, copy, readonly) NSDictionary *firmwareByType;

/// The integral version of this Automatic device.
@property (nonatomic, assign, readonly) NSUInteger linkVersion;

/// The encryption key for communicating with this device using
/// the `com.automatic.link.protocol.v1` protocol.
@property (nonatomic, copy, readonly) NSString *encryptionKey;

/// The MAC address for this device.
@property (nonatomic, copy, readonly) NSString *MACAddress;

/// The short lived authorization for this device to be able to establish
/// streaming data connections using the `com.automatic.link.protocol.v2`
/// protocol.
@property (nonatomic, copy, readonly, nullable) NSString *directAccessToken;

/// The shared secret used for authentication with this device to be able to
/// establish streaming data connections using the
/// `com.automatic.link.protocol.v2` protocol.
@property (nonatomic, copy, readonly, nullable) NSString *directAccessSecret;

/// Convenience method mapping firmware string to type
///
/// returns @{
///     @"obd": @(AUTAPIFirmwareTypeOBD),
///     @"gps": @(AUTAPIFirmwareTypeGPS),
///     @"automatic": @(AUTAPIFirmwareTypeAutomatic),
/// }
+ (NSDictionary *)firmwareTypeByString;

@end

AUTPageSubclassInterface(AUTAPIDevice)

NS_ASSUME_NONNULL_END
