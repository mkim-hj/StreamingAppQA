//
//  AUTAPITimelineDayGroupTripsItem.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 12/4/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPITimelineDayGroupItem.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTAPITimelineDayGroupTripsItemTrip;

/// A group of trips displayed within the timeline.
@interface AUTAPITimelineDayGroupTripsItem : AUTAPITimelineDayGroupItem

@property (readonly, nonatomic, copy) NSArray<AUTAPITimelineDayGroupTripsItemTrip *> *trips;

@property (readonly, nonatomic, copy) NSString *duration;

@property (readonly, nonatomic, copy) NSString *distance;

@property (readonly, nonatomic, copy) NSString *efficiency;

@end

/// A trip within a timeline trip group.
@interface AUTAPITimelineDayGroupTripsItemTrip : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

/// A string-encoded polyline.
///
/// Nil if a route for the trip could not be determined.
@property (readonly, nonatomic, nullable) NSString *encodedPolyline;

/// An CLLocationCoordinate2D wrapped within an NSValue, accessible via the
/// MKCoordinateValue property.
///
/// Nil if the trip does not have a known start location.
@property (readonly, nonatomic, nullable) NSValue *startCoordinate;

/// An CLLocationCoordinate2D wrapped within an NSValue, accessible via the
/// MKCoordinateValue property.
///
/// Nil if the trip does not have a known end location.
@property (readonly, nonatomic, nullable) NSValue *endCoordinate;

@end

NS_ASSUME_NONNULL_END
