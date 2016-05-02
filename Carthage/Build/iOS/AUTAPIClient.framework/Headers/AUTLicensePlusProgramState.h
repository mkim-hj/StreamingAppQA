//
//  AUTLicensePlusProgramState.h
//  AUTAPIClient
//
//  Created by James Lawton on 6/16/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>
#import <CoreLocation/CoreLocation.h>
#import <AUTAPIClient/AUTAPIPage.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTLicensePlusCoachInvitation;
@class AUTLicensePlusPerson;

typedef enum {
    AUTLicensePlusMedalNone,
    AUTLicensePlusMedalBronze,
    AUTLicensePlusMedalSilver,
    AUTLicensePlusMedalGold,
} AUTLicensePlusMedal;

@interface AUTLicensePlusProgramState : MTLModel <MTLJSONSerializing>

@property (nonatomic, readonly) NSTimeInterval timeDriven;
@property (nonatomic, readonly) CLLocationDistance distanceDriven;

@property (nonatomic, readonly) double progress; // [0, 1]
@property (nonatomic, readonly, getter=isComplete) BOOL complete;

@property (nonatomic, readonly) double score;
@property (nonatomic, readonly) AUTLicensePlusMedal medal;

@property (nonatomic, readonly) NSDate *startedAt;
@property (nonatomic, readonly, nullable) NSDate *endedAt;

@property (nonatomic, readonly) NSUInteger hardAccelerations;
@property (nonatomic, readonly) NSUInteger hardBrakes;
@property (nonatomic, readonly) NSTimeInterval durationOver70mph;

@property (nonatomic, readonly) AUTLicensePlusPerson *student;
@property (nonatomic, readonly, nullable) AUTLicensePlusPerson *coach;
@property (nonatomic, readonly, nullable) AUTLicensePlusCoachInvitation *coachInvitation;

// References
@property (nonatomic, readonly) NSURL *URL;
@property (nonatomic, readonly) NSURL *programURL;
@property (nonatomic, readonly) NSURL *badgesURL;

@end

AUTPageSubclassInterface(AUTLicensePlusProgramState)

NS_ASSUME_NONNULL_END
