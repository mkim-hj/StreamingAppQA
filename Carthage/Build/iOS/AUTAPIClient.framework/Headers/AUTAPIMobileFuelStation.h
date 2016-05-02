//
//  AUTAPIMobileFuelStation.h
//  AUTAPIClient
//
//  Created by Westin Newell on 2/25/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import CoreLocation;

#import <AUTAPIClient/AUTAPIObject.h>
#import <AUTAPIClient/AUTAPIPage.h>

@class AUTAPIMobileFuelGrade;

NS_ASSUME_NONNULL_BEGIN

/// A fuel station from the mobile API.
@interface AUTAPIMobileFuelStation : AUTAPIObject

/// The brand name of the fuel station.
@property (readonly, nonatomic, copy) NSString *name;

/// The address of the fuel station.
@property (nullable, readonly, nonatomic, copy) NSString *address;

/// The location of the fuel station.
@property (readonly, nonatomic) CLLocationCoordinate2D coordinate;

/// The NSURL for the fuel station's icon.
@property (nullable, readonly, nonatomic, copy) NSURL *iconURL;

/// The ISO 4217 currency code used by the fuel station.
@property (readonly, nonatomic, copy) NSString *currencyCode;

/// The fuel grades available at the fuel station.
///
/// This array can be empty.
@property (readonly, nonatomic, copy) NSArray<AUTAPIMobileFuelGrade *> *fuelGrades;

@end

AUTPageSubclassInterface(AUTAPIMobileFuelStation)

NS_ASSUME_NONNULL_END
