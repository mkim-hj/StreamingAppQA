//
//  AUTAPITrip.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 25/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <CoreLocation/CoreLocation.h>
#import <MapKit/MapKit.h>
#import <UIKit/UIKit.h>

#import <AUTAPIClient/AUTAPIAddress.h>
#import <AUTAPIClient/AUTAPIObject.h>
#import <AUTAPIClient/AUTAPIPage.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPITrip : AUTAPIObject

@property (readonly, nonatomic, assign) CGFloat distance;

@property (readonly, nonatomic, assign) CGFloat duration;

@property (readonly, nonatomic, assign) NSTimeInterval durationOver70mph;

@property (readonly, nonatomic, strong, nullable) AUTAPIAddress *endAddress;

@property (readonly, nonatomic, strong, nullable) NSDate *endedAt;

@property (readonly, nonatomic, assign) CLLocationCoordinate2D endLocation;

@property (readonly, nonatomic, assign) CLLocationAccuracy endLocationAccuracy;

@property (readonly, nonatomic, assign) NSUInteger hardAccelerations;

@property (readonly, nonatomic, assign) NSUInteger hardBrakes;

@property (readonly, nonatomic, strong, nullable) MKPolyline *path;

/// The path of this trip, encoded as a polyline.
@property (readonly, nonatomic, strong, nullable) NSString *encodedPath;

@property (readonly, nonatomic, assign) CGFloat scoreA;

@property (readonly, nonatomic, assign) CGFloat scoreB;

@property (readonly, nonatomic, strong, nullable) AUTAPIAddress *startAddress;

@property (readonly, nonatomic, strong, nullable) NSDate *startedAt;

@property (readonly, nonatomic, assign) CLLocationCoordinate2D startLocation;

@property (readonly, nonatomic, assign) CLLocationAccuracy startLocationAccuracy;

@property (readonly, nonatomic, strong) NSURL *userURL;

@property (readonly, nonatomic, strong) NSURL *vehicleURL;

/// The fuel cost of this trip in USD.
@property (readonly, nonatomic, copy, nullable) NSNumber *fuelCostUSD;

/// The average fuel efficiency of this trip in kilometers per liter.
@property (readonly, nonatomic, copy, nullable) NSNumber *averageKMPL;

@end

AUTPageSubclassInterface(AUTAPITrip)

NS_ASSUME_NONNULL_END
