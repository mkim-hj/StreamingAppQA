//
//  AUTAPITripDetails.h
//  AUTAPIClient
//
//  Created by Engin Kurutepe on 15/01/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Mantle;
@import MapKit;

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPITripDetails : MTLModel <MTLJSONSerializing>

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic, copy) NSString *objectID;

@property (readonly, nonatomic, copy) NSString *distance;

@property (readonly, nonatomic, copy) NSString *duration;

/// The average fuel efficiency of this trip in localized units
@property (readonly, nonatomic, copy) NSString *efficiency;

/// This property is non-null even when the end coordinate is nil.
@property (readonly, nonatomic, copy) NSString *endAddress;

@property (readonly, nonatomic, copy) NSDate *endDate;

/// An CLLocationCoordinate2D wrapped within an NSValue, accessible via the
/// MKCoordinateValue property.
///
/// Nil if the trip does not have a known end location.
@property (readonly, nonatomic, copy, nullable) NSValue *endCoordinate;

/// A string-encoded polyline.
///
/// Nil if a route for the trip could not be determined.
@property (readonly, nonatomic, copy, nullable) NSString *encodedPolyline;

/// A parsed polyline.
///
/// Nil if a route for the trip could not be determined.
@property (readonly, nonatomic, copy, nullable) MKPolyline *polyline;

/// This property is non-null even when the start coordinate is nil.
@property (readonly, nonatomic, copy) NSString *startAddress;

@property (readonly, nonatomic, copy) NSDate *startDate;

/// An CLLocationCoordinate2D wrapped within an NSValue, accessible via the
/// MKCoordinateValue property.
///
/// Nil if the trip does not have a known start location.
@property (readonly, nonatomic, copy, nullable) NSValue *startCoordinate;

@end

NS_ASSUME_NONNULL_END
