//
//  AUTAPIMobileFuelFillUp.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 3/1/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPICreatableObject.h>
#import <AUTAPIClient/AUTAPIMobileFuelVolumeUnitType.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIMobileFuelFillUp : AUTAPICreatableObject

/// The unique ID of the associated vehicle, from the server.
///
/// This can be `nil` if the object was generated locally.
@property (readonly, nonatomic, copy, nullable) NSString *vehicleID;

/// The fill-up fuel amount as a boxed float.
///
/// This must be set if the object is generated locally.
@property (readonly, nonatomic, copy, nullable) NSNumber *volume;

/// The station unique ID where the fill up took place if known.
@property (readonly, nonatomic, copy, nullable) NSString *stationID;

/// The fuel grade unique server ID.
@property (readonly, nonatomic, copy, nullable) NSString *gradeID;

/// The fuel grade display name.
@property (readonly, nonatomic, copy, nullable) NSString *gradeDisplayName;

/// The price per unit of volume for this fuel fill-up.
///
/// This must be set if the object is generated locally.
@property (readonly, nonatomic, copy, nullable) NSNumber *pricePerVolumeUnit;

/// The ISO 4217 currency code for this fill-up.
@property (readonly, nonatomic, copy) NSString *currencyCode;

/// The units of volume for this fill-up.
@property (readonly, nonatomic) AUTAPIMobileFuelVolumeUnitType volumeUnit;

/// The total cost for this fill-up.
///
/// This can be `nil` if the object was generated locally.
@property (readonly, nonatomic, copy, nullable) NSNumber *totalCost;

@end

NS_ASSUME_NONNULL_END
