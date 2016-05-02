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
#import "ALELM327Session.h"
#import "EAAccessory+AL.h"

@implementation ALAutomaticAdapter

+ (instancetype)adapterWithAccessory:(EAAccessory *)inAccessory {
    return [[self alloc] initWithAccessory:inAccessory];
}

- (instancetype)initWithAccessory:(EAAccessory *)inAccessory {
    if (![inAccessory isAutomaticAdapter]) {
        return nil;
    }

    if ((self = [super init])) {
        _accessory = inAccessory;
    }

    return self;
}

+ (BOOL)authorizeAdapterForURL:(NSURL *)inURL withSecret:(NSString *)inClientSecret onStatus:(ALAutomaticAdapterOnAuthorizationStatus)inOnStatus onAuthorized:(ALAutomaticAdapterOnAdapterAuthorized)inOnAuthorized {
    return [ALAuthorization
        installAuthorizationFromURL:inURL
                         withSecret:inClientSecret
                           onStatus:inOnStatus
                       onAuthorized:inOnAuthorized];
}

- (BOOL)authorizeWithOAuthToken:(NSString *)inToken tokenType:(NSString *)inTokenType onStatus:(ALAutomaticAdapterOnAuthorizationStatus)inOnStatus onAuthorized:(ALAutomaticAdapterOnAdapterAuthorized)inOnAuthorized {
    ALAuthorization *authorization = [ALAuthorization authorizationForAdapter:self];
    return [authorization installAuthorizationWithOAuthToken:inToken tokenType:inTokenType onStatus:inOnStatus onAuthorized:inOnAuthorized];
}

+ (ALAutomaticAdapter *)connectedAdapterWithID:(NSString *)inAdapterID {
    return [ALAdapterDetector connectedAdapterWithID:inAdapterID];
}

- (void)openAuthorizedSessionForAutomaticClient:(NSString *)inClientID
                        allowingUserInteraction:(BOOL)inInteractionAllowed
                                       onStatus:(ALAutomaticAdapterOnAuthorizationStatus)inOnStatus
                                   onAuthorized:(ALAutomaticAdapterOnAppAuthenticated)inOnAuthorized
                                       onClosed:(ALAutomaticAdapterOnSessionClosed)inOnClosed {
    ALAutomaticAdapterOnSessionClosed onClosed = [inOnClosed copy];
    [ALELM327Session
        openAuthorizedSessionWithAdapter:self
                      forAutomaticClient:inClientID
                 allowingUserInteraction:inInteractionAllowed
                                onStatus:inOnStatus
                            onAuthorized:inOnAuthorized
                                onClosed:^(NSError *inError, BOOL inWasOpen) {
                                    if (onClosed) {
                                        onClosed(inError);
                                    }
                                }];
}

+ (NSString *)protocolID {
    return @"com.automatic.link.protocol.v2";
}

+ (ALAdapterDetector *)watchForAdapterWithPIN:(NSString *)inAdapterPIN onDetected:(ALAutomaticAdapterOnDetected)inOnDetected {
    return [ALAdapterDetector watchForPIN:inAdapterPIN onDetected:inOnDetected];
}

@end
