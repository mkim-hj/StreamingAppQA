/*****************************************************************************
**
**  Automatic Labs - CONFIDENTIAL
**
**  Unpublished Copyright (c) 2009-2016 AUTOMATIC LABS, All Rights Reserved.
**
**  NOTICE:
**
**  All information contained herein is, and remains the property of AUTOMATIC LABS.
**  The intellectual and technical concepts contained herein are proprietary to AUTOMATIC LABS
**  and may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade
**  secret or copyright law.
**
**  Dissemination of this information or reproduction of this material is strictly forbidden unless
**  prior written permission is obtained from AUTOMATIC LABS.  Access to the source code contained
**  herein is hereby forbidden to anyone except current AUTOMATIC LABS employees, managers or
**  contractors who have executed Confidentiality and Non-disclosure agreements explicitly covering
**  such access.
**
**  The copyright notice above does not evidence any actual or intended publication or disclosure of
**  this source code, which includes information that is confidential and/or proprietary, and is a
**  trade secret, of AUTOMATIC LABS. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC PERFORMANCE,
**  OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF
**  AUTOMATIC LABS IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL TREATIES.
**  THE RECEIPT OR POSSESSION OF THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY
**  ANY RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL
**  ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
**
******************************************************************************/

#import "ALAdapterDetector.h"
#import "ALAutomaticAdapter.h"
#import "EAAccessory+AL.h"
#import <CoreBluetooth/CoreBluetooth.h>
#import <ExternalAccessory/ExternalAccessory.h>

#ifndef AL_LOG_ADAPTERDETECTOR
#if 0
#define AL_LOG_ADAPTERDETECTOR AL_LOG
#else
#define AL_LOG_ADAPTERDETECTOR(...)
#endif
#endif

static NSPredicate *_anyAdapterFilter;
typedef void (^_OnEAAccessoryDidConnect_t)(EAAccessory *inAccessory);

@implementation ALAdapterDetector {
  @private
    NSPredicate *_nameStringFilter;
    NSPredicate *_pinFilter;
    ALAdapterDetectorOnPINDetected _onDetected;
    NSMutableArray *_pendingAccessories;
}

+ (void)initialize {
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        _anyAdapterFilter = [NSPredicate predicateWithBlock:^BOOL(EAAccessory *inEachAccessory, NSDictionary *inBindings) {
            return [inEachAccessory isAutomaticAdapter];
        }];
    });
}

#pragma mark - Lifecycle

- (void)dealloc {
    [self _unwatchAccessories];
}

+ (instancetype)watchForPIN:(NSString *)inDevicePIN onDetected:(ALAdapterDetectorOnPINDetected)inOnDetected {
    return [[[self class] alloc] initWithPIN:inDevicePIN onDetected:inOnDetected];
}

- (instancetype)initWithPIN:(NSString *)inPIN onDetected:(ALAdapterDetectorOnPINDetected)inOnDetected {
    if ((self = [super init])) {
        NSAssert(inOnDetected, @"nil ALAdapterDetectorOnPINDetected");
        _onDetected = [inOnDetected copy];

        NSString *pinPlus2 = [inPIN stringByAppendingString:@"##"];
        NSString *pinPrefix = [pinPlus2 substringToIndex:2];
        NSPredicate *nameFilter = [NSPredicate predicateWithBlock:^BOOL(NSString *inEachName, NSDictionary *inBindings) {
            NSString *namePlus2 = [inEachName stringByAppendingString:@"??"];
            NSString *namePrefix = [namePlus2 substringToIndex:2];
            NSComparisonResult comparison = [namePrefix caseInsensitiveCompare:pinPrefix];
            AL_LOG_ADAPTERDETECTOR(@"%@ ? %@ = %d", namePlus2, pinPlus2, int(comparison));
            return (comparison == NSOrderedSame);
        }];
        _nameStringFilter = nameFilter;
        _pinFilter = [NSPredicate predicateWithBlock:^BOOL(EAAccessory *inEachAccessory, NSDictionary *inBindings) {
            if (![_anyAdapterFilter evaluateWithObject:inEachAccessory]) {
                return NO;
            }

            NSString *name = [inEachAccessory name];
            return [nameFilter evaluateWithObject:name];
        }];

        ALAdapterDetector *__weak weakSelf = self;
        dispatch_async(dispatch_get_main_queue(), ^{
            [weakSelf _watchAccessories];
            [weakSelf _check];
        });
    }

    return self;
}

#pragma mark - ALAdapterDetector methods

+ (ALAutomaticAdapter *)connectedAdapterWithID:(NSString *)inAdapterID {
    EAAccessoryManager *accessoryMgr = [EAAccessoryManager sharedAccessoryManager];
    NSArray *allConnecteds = [accessoryMgr connectedAccessories];
    NSArray *allAdapters = [allConnecteds filteredArrayUsingPredicate:_anyAdapterFilter];
    NSArray *matchingAdapters = [allAdapters filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(EAAccessory *inEachAdapter, NSDictionary *inBindings) {
                                                 NSString *serialNumber = [inEachAdapter serialNumber];
                                                 return [serialNumber isEqualToString:inAdapterID];
                                             }]];
    ALAutomaticAdapter *primeAdapter = [ALAutomaticAdapter adapterWithAccessory:[matchingAdapters firstObject]];

    return primeAdapter;
}

- (BOOL)_check {
    EAAccessoryManager *accessoryMgr = [EAAccessoryManager sharedAccessoryManager];
    NSArray *connecteds = [accessoryMgr connectedAccessories];
    if ([self _gotAccessories:connecteds]) {
        return YES;
    }

    [self _interact];

    return NO;
}

- (BOOL)_gotAccessories:(NSArray *)inAccessories {
    AL_LOG_ADAPTERDETECTOR(@"%@", inAccessories);
    NSArray *pinMatches = [inAccessories filteredArrayUsingPredicate:_pinFilter];
    if ([pinMatches count] == 0) {
        return NO;
    }

    NSArray *asAdapters = [pinMatches valueForKey:@"asAutomaticAdapter"];
    _onDetected(asAdapters);

    return YES;
}

- (void)_interact {
    _pendingAccessories = [NSMutableArray new];

    // The bluetooth picker won't appear if summoned in or before the first -viewDidAppear call
    // upon launch.  Defer until the next iteration of the run loop:
    dispatch_async(dispatch_get_main_queue(), ^{
        [[EAAccessoryManager sharedAccessoryManager]
            showBluetoothAccessoryPickerWithNameFilter:_nameStringFilter
                                            completion:^(NSError *inError) {
                                                AL_LOG_ADAPTERDETECTOR(@"%@", inError);
                                                if (inError) {
                                                }
                                                [self _gotAccessories:_pendingAccessories];
                                                _pendingAccessories = nil;
                                            }];
    });
}

- (void)_unwatchAccessories {
    NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
    [nc removeObserver:self name:EAAccessoryDidConnectNotification object:nil];
}

- (void)_watchAccessories {
    NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self selector:@selector(_onEAAccessoryDidConnectNotification:) name:EAAccessoryDidConnectNotification object:nil];
}

#pragma mark - Notifications

- (void)_onEAAccessoryDidConnectNotification:(NSNotification *)inNotification {
    AL_LOG_ADAPTERDETECTOR(@"%@", inNotification);
    dispatch_async(dispatch_get_main_queue(), ^{
        EAAccessory *accessory = [inNotification userInfo][EAAccessoryKey];
        if (_pendingAccessories) {
            [_pendingAccessories addObject:accessory];
        } else {
            [self _gotAccessories:@[ accessory ]];
        }
    });
}

@end
