//
//  AUTAPIMobileVehicle.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 24/09/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIVehicle.h>
#import <AUTAPIClient/AUTAPIPage.h>

@class AUTAPIPark;
@class AUTAPILocation;
@class AUTAPIVehicleIgnitionStatus;
@class AUTAPIVehicleSettings;

NS_ASSUME_NONNULL_BEGIN

/// A vehicle and its associated objects as synthesized from the mobile
/// aggregation API.
///
/// Its properties are inherited from AUTAPIVehicle, with additional synthesized
/// relationships to a park, location, and state added by the mobile aggregation
/// API.
@interface AUTAPIMobileVehicle : AUTAPIVehicle

/// The last known park of the receiver.
///
/// Nil if there is no last known park for the receiver.
@property (readonly, nonatomic, strong, nullable) AUTAPIPark *latestPark;

/// The last known location of the receiver.
///
/// Differs from latestPark in that the value could represent the receiver's
/// location while driving.
///
/// Nil if there is no last known location for the receiver.
@property (readonly, nonatomic, strong, nullable) AUTAPILocation *latestLocation;

/// The last known ignition status of the receiver.
@property (readonly, nonatomic, strong, nullable) AUTAPIVehicleIgnitionStatus *latestIgnitionStatus;

/// The client-relevant settings associated with the vehicle.
@property (readonly, nonatomic, nullable) AUTAPIVehicleSettings *settings;

@end

AUTPageSubclassInterface(AUTAPIMobileVehicle)

NS_ASSUME_NONNULL_END
