//
//  AUTAPIFirmware.h
//  AUTAPIClient
//
//  Created by Justin Spahr-Summers on 2015-04-17.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

@class AUTAPIFirmwareVersion;

NS_ASSUME_NONNULL_BEGIN

/// Represents the types of firmware that can be downloaded and installed.
typedef NS_ENUM(NSUInteger, AUTAPIFirmwareType) {
    /// Firmware for the Automatic device itself.
    AUTAPIFirmwareTypeAutomatic,

    /// Firmware for the OBD connector.
    AUTAPIFirmwareTypeOBD,

    /// Firmware for the GPS component.
    AUTAPIFirmwareTypeGPS,
};

/// Returns a string description of the provided firmware type.
extern NSString * __nonnull AUTStringFromFirmwareType(AUTAPIFirmwareType type);

/// The versions of a particular type of firmware that are available for update.
@interface AUTAPIFirmware : MTLModel <MTLJSONSerializing>

/// The best version to install for this firmware.
@property (nonatomic, copy, readonly) AUTAPIFirmwareVersion *mainVersion;

/// The minimum version of this firmware that must be installed for basic
/// functionality to work.
@property (nonatomic, copy, readonly) AUTAPIFirmwareVersion *minimumVersion;

/// The firmware type for this firmware.
@property (nonatomic, readonly) AUTAPIFirmwareType firmwareType;

@end

NS_ASSUME_NONNULL_END
