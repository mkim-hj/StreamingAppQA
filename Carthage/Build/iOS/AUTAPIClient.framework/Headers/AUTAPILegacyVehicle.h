//
//  AUTAPILegacyVehicle.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 5/12/15.
//  Copyright (c) 2015 Automatic. All rights reserved.
//

#import <AUTAPIClient/AUTAPILegacyResponse.h>

NS_ASSUME_NONNULL_BEGIN

/// A Legacy Vehicle information as returned by /car legacy API
/// when registering a vehicle at the end of setup
@interface AUTAPILegacyVehicle : AUTAPILegacyResponse <MTLJSONSerializing>

@property (readonly, nonatomic, copy) NSString *vin;

@property (readonly, nonatomic, copy) NSString *model;

@property (readonly, nonatomic, copy) NSString *make;

@property (readonly, nonatomic, copy) NSString *displayName;

@property (readonly, nonatomic) NSNumber *year;

@property (readonly, nonatomic) NSNumber *fuelEconomyCity;

@property (readonly, nonatomic) NSNumber *fuelType;

@property (readonly, nonatomic) NSNumber *fuelPercent;

@property (readonly, nonatomic) NSNumber *fuelTankCapacity;

@property (readonly, nonatomic) NSNumber *fuelEconomyHighway;

@property (readonly, nonatomic, copy) NSString *fuelGrade;

@property (readonly, nonatomic) NSNumber *lastTripEndTime;

@property (readonly, nonatomic) NSNumber *ignitionOffOnRPMZero;

@property (readonly, nonatomic) NSArray *supportedPids;

@end

NS_ASSUME_NONNULL_END
