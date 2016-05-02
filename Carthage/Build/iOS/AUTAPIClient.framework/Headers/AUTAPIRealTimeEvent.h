//
//  AUTAPIRealTimeEvent.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 8/2/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// A real time event mapped from a drive event recorded on the device.
@interface AUTAPIRealTimeEvent : MTLModel <MTLJSONSerializing>

/// Returns the class of AUTAPIRealTimeEvent corresponding to the type
/// Only used by Tests.
+ (Class)realTimeEventClassForType:(NSString *)type;

/// Creates a new instance from values
+ (instancetype)realTimeEventWithDictionary:(NSDictionary *)dictionaryValue error:(NSError **)error;

/// A unique ID for this event.
@property (nonatomic, copy, readonly) NSUUID *uuid;

/// The date recorded for this event.
@property (nonatomic, copy, readonly) NSDate *date;

/// The type of this event.
@property (nonatomic, copy, readonly) NSString *type;

/// The timezone where this event occurred.
@property (nonatomic, copy, readonly) NSString *timezone;

/// A NSDictionary representing the location of when this event happened.
@property (nonatomic, copy, readonly, nullable) NSDictionary *locationDictionary;

/// The vin number associated with the vehicle.
@property (nonatomic, copy, readonly) NSString *VIN;

/// The type associated with a given class of AUTAPIRealTimeEvent
+ (NSString *)type;

/// A method returning a copy updating properties using passed values
- (instancetype)realtimeEventAddingVIN:(NSString *)VIN locationDictionary:(nullable NSDictionary *)location timezone:(NSString *)timezone error:(NSError **)error;

@end

@interface AUTIgnitionOnRealTimeEvent : AUTAPIRealTimeEvent
@end

@interface AUTIgnitionOffRealTimeEvent : AUTAPIRealTimeEvent
@end

@interface AUTHardRealTimeEvent : AUTAPIRealTimeEvent

/// The force recorded for the event, in Gs.
@property (nonatomic, readonly) double force;

@end

@interface AUTHardBrakeRealTimeEvent : AUTHardRealTimeEvent
@end

@interface AUTHardAccelerationRealTimeEvent : AUTHardRealTimeEvent
@end

@interface AUTSpeedingRealTimeEvent : AUTAPIRealTimeEvent

/// The velocity of this event, in miles per hour.
@property (nonatomic, readonly) NSInteger speed;

@end

@interface AUTLocationUpdateRealTimeEvent : AUTAPIRealTimeEvent

/// An array representing the location of the vehicle as seen by the phone
/// and the adapter
@property (nonatomic, readonly) NSArray<NSDictionary *> *locations;

/// A method returning an instance of AUTLocationUpdateRealTimeEvent
+ (AUTLocationUpdateRealTimeEvent *)eventWithLocations:(NSArray<NSDictionary *> *)locations date:(NSDate *)date error:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
