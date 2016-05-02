//
//  AUTClient+AUTTimeline.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 12/4/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

@class AUTAPITimelineDayGroup;
@class AUTAPITimelineDayGroupPage;

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPITimeline)

/// Fetches the first page of the current user's timeline for the given vehicle
/// ID as synthesized by the view server.
///
/// Returns a signal which will send an AUTAPITimelineDayGroupPage then complete,
/// or else error.
///
/// The returned AUTAPITimelineDayGroupPage can be passed to -fetchNextPageForPage:
/// to retrieve subsequent pages if available.
- (RACSignal *)fetchTimelineForVehicleWithID:(NSString *)vehicleID;

@end

NS_ASSUME_NONNULL_END
