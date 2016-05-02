//
//  AUTAPIFirmwareVersion.h
//  AUTAPIClient
//
//  Created by Justin Spahr-Summers on 2015-04-17.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

#import <AUTAPIClient/AUTAPIFirmware.h>

NS_ASSUME_NONNULL_BEGIN

/// Represents one version of a particular firmware type that is available for
/// download.
@interface AUTAPIFirmwareVersion : MTLModel <MTLJSONSerializing>

/// The URL to the firmware archive.
@property (nonatomic, copy, readonly) NSURL *downloadURL;

/// The version string for this firmware version.
@property (nonatomic, copy, readonly) NSString *version;

/// The minimum app version string for this firmware version.
/// A nil value indicates that there is no minimumAppVersion and all versions
/// support this firmware version.
@property (nonatomic, copy, readonly, nullable) NSString *minimumAppVersion;

/// The firmware type for this firmware version.
@property (nonatomic, readonly) AUTAPIFirmwareType firmwareType;

@end

NS_ASSUME_NONNULL_END
