//
//  AUTAPIVehicleSettings.h
//  AUTAPIClient
//
//  Created by James Lawton on 12/9/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, AUTAPIVehicleNotificationTrigger) {
    AUTAPIVehicleNotificationTriggerNever,
    AUTAPIVehicleNotificationTriggerWhenConnected,
    AUTAPIVehicleNotificationTriggerAlways,
};

/// Represents per-vehicle settings.
@interface AUTAPIVehicleSettings : MTLModel <MTLJSONSerializing>

@property (nonatomic, readonly) BOOL adapterAudioOnConnectedDisconnected;

@property (nonatomic, readonly) AUTAPIVehicleNotificationTrigger notificationOnParked;

@end

NS_ASSUME_NONNULL_END
