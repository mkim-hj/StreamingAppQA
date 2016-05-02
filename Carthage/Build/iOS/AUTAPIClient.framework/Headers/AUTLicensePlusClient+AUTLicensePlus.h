//
//  AUTLicensePlusClient+AUTLicensePlus.h
//  AUTAPIClient
//
//  Created by James Lawton on 4/7/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTLicensePlusClient.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTLicensePlusPerson;
@class AUTLicensePlusProgram;
@class AUTLicensePlusProgramState;
@class RACSignal;

@interface AUTLicensePlusClient (AUTLicensePlus)

// signal<AUTLicensePlusProgram>
- (RACSignal *)fetchEligibleLicensePlusProgram;

// signal<[AUTLicensePlusBadge]>
- (RACSignal *)fetchBadgesForProgram:(AUTLicensePlusProgram *)program;

// signal<AUTLicensePlusProgramState>
- (RACSignal *)enrollInLicensePlusProgram:(AUTLicensePlusProgram *)program withCoachEmail:(NSString *)coachEmail;

// signal<AUTLicensePlusProgramState?>
- (RACSignal *)fetchLicensePlusProgramState;

// signal<AUTLicensePlusProgram>
- (RACSignal *)fetchLicensePlusProgramForState:(AUTLicensePlusProgramState *)state;

// signal<[AUTLicensePlusBadgeState]>
- (RACSignal *)fetchBadgeStatesForProgramState:(AUTLicensePlusProgramState *)state;

// signal<()>
- (RACSignal *)unenrollInLicensePlusProgramWithState:(AUTLicensePlusProgramState *)state;

// signal<()>
- (RACSignal *)remindLicensePlusCoachForProgramState:(AUTLicensePlusProgramState *)state;

// signal<AUTLicensePlusPerson>
- (RACSignal *)updateLicensePlusCoach:(AUTLicensePlusPerson *)coach phoneNumber:(NSString *)phoneNumber;

// signal<@bool>
// This has to return @YES before `fetchEligibleLicensePlusProgram` is guaranteed
// to return the correct program.
- (RACSignal *)fetchHasVehicle;

@end

NS_ASSUME_NONNULL_END
