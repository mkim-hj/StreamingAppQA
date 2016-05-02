//
//  AUTClient+AUTMobileAppInstance.h
//  AUTAPIClient
//
//  Created by James Lawton on 11/12/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTMobileAppInstance;

@interface AUTClient (AUTMobileAppInstance)

/// Returns a signal which sends a `AUTMobileAppInstancePage` and completes,
/// or errors.
- (RACSignal *)fetchMobileAppInstances;

/// Returns a signal which sends a `AUTMobileAppInstance` or `nil` and completes,
/// or errors.
- (RACSignal *)fetchMobileAppInstanceWithPhoneIdentifier:(NSString *)phoneIdentifier;

/// Upload a newly-created mobile app instance.
///
/// Returns the `AUTMobileAppInstance` from the server and completes, or errors.
- (RACSignal *)createMobileAppInstance:(AUTMobileAppInstance *)mobileAppInstance;

/// Modify an existing mobile app instance.
///
/// Returns the updated `AUTMobileAppInstance` from the server and completes, or errors.
- (RACSignal *)updateMobileAppInstance:(AUTMobileAppInstance *)mobileAppInstance;

@end

NS_ASSUME_NONNULL_END
