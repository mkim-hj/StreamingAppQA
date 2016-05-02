//
//  AUTAPIMobileFuelGrade.h
//  AUTAPIClient
//
//  Created by Westin Newell on 2/25/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIMobileFuelVolumeUnitType.h>

NS_ASSUME_NONNULL_BEGIN

/// A fuel grade for a fuel station returned from the mobile API.
@interface AUTAPIMobileFuelGrade : MTLModel <MTLJSONSerializing>

/// The unique server ID for this fuel grade.
@property (readonly, nonatomic, copy) NSString *objectID;

/// The display name for this fuel grade.
@property (readonly, nonatomic, copy) NSString *displayName;

/// The price per unit of this fuel grade.
@property (readonly, nonatomic, copy) NSNumber *price;

/// The ISO 4217 currency code for this fuel grade.
@property (readonly, nonatomic, copy) NSString *currencyCode;

/// The units of volume for this grade.
@property (readonly, nonatomic) AUTAPIMobileFuelVolumeUnitType volumeUnit;

@end

NS_ASSUME_NONNULL_END
