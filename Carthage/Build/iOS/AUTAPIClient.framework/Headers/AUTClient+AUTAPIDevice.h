//
//  AUTClient+AUTAPIDevice.h
//  AUTAPIClient
//
//  Created by Justin Spahr-Summers on 2015-04-20.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@import ReactiveCocoa;
@class AUTAPIDevicePage;

@interface AUTClient (AUTAPIDevice)

/// Fetches information for the device that corresponds to the given ID.
/// Internally invokes fetchDeviceWithID:customUserAgent: with nil
/// customUserAgent.
- (RACSignal *)fetchDeviceWithID:(NSString *)deviceID;

/// Fetches information for the device that corresponds to the given ID using a
/// custom HTTP User-Agent header.
///
/// Returns a signal which will send an AUTAPIDevice then complete, or else error.
- (RACSignal *)fetchDeviceWithID:(NSString *)deviceID customUserAgent:(nullable NSString *)customUserAgent;

/// Fetches information for all devices registered by user.
/// Internally invokes fetchDevicesWithCustomUserAgent: with nil
/// customUserAgent.
- (RACSignal *)fetchDevices;

/// Fetches information for all devices registered by user using a custom HTTP
/// User-Agent header.
///
/// Returns a signal which will send an AUTAPIDevicePage then complete, or else
/// error.
///
/// The returned AUTAPIDevicePage can be passed to -fetchNextPageForPage: to
/// retrieve subsequent page if available.
- (RACSignal *)fetchDevicesWithCustomUserAgent:(nullable NSString *)customUserAgent;

@end

NS_ASSUME_NONNULL_END
