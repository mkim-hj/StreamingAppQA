//
//  AUTAPILocation.h
//  Automatic
//
//  Created by Robert Böhnke on 24/04/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import CoreLocation;
@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// An `AUTAPILocation` encapulates a location.
@interface AUTAPILocation : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

/// The receiver as represented by Core Location.
@property (readonly, nonatomic, copy, nonnull) CLLocation *location;

@end

NS_ASSUME_NONNULL_END
