//
//  AUTAPIVehicleIgnitionStatus.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 2/19/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

@class AUTAPILocation;

/// Represents an vehicle ignition on or off.
@interface AUTAPIVehicleIgnitionStatus : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic, copy) NSDate *date;

@property (readonly, nonatomic, getter=isIgnitionOn) BOOL ignitionOn;

@property (readonly, nonatomic, nullable) AUTAPILocation *location;

@end

NS_ASSUME_NONNULL_END
