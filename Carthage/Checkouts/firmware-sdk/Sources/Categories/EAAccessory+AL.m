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

#import "ALAutomaticAdapter.h"
#import "ALELM327Session.h"
#import "EAAccessory+AL.h"

#ifndef AL_LOG_EAACCESSORY
#if 0
#define AL_LOG_EAACCESSORY AL_LOG
#else
#define AL_LOG_EAACCESSORY(...)
#endif
#endif

@implementation EAAccessory (AL)

#pragma mark - Property accessors

- (BOOL)isAutomaticAdapter {
    NSString *wantProtocol = [ALAutomaticAdapter protocolID];
    NSArray *haveProtocols = [self protocolStrings];
    BOOL isAutomaticAdapter = [haveProtocols containsObject:wantProtocol];

    return isAutomaticAdapter;
}

#pragma mark - EAAccessory (AL) methods

+ (BOOL)authorizeAutomaticAdapterForURL:(NSURL *)inURL withSecret:(NSString *)inClientSecret onStatus:(ALAccessoryOnAuthorizationStatus)inOnStatus onAuthorized:(ALAccessoryOnAdapterAuthorized)inOnAuthorized {
    ALAccessoryOnAdapterAuthorized onAuthorized = [inOnAuthorized copy];
    return [ALAutomaticAdapter
        authorizeAdapterForURL:inURL
                    withSecret:inClientSecret
                      onStatus:inOnStatus
                  onAuthorized:^(ALAutomaticAdapter *inAdapter, NSString *inClientID, ALAutomaticAdapterOnAuthorizationStatus inStatusCallback) {
                      if (onAuthorized) {
                          onAuthorized([inAdapter accessory], inClientID, inStatusCallback);
                      }
                  }];
}

- (ALAutomaticAdapter *)asAutomaticAdapter {
    return [ALAutomaticAdapter adapterWithAccessory:self];
}

- (void)getAuthorizedSessionForAutomaticClient:(NSString *)inClientID
                       allowingUserInteraction:(BOOL)inInteractionAllowed
                                      onStatus:(ALAccessoryOnAuthorizationStatus)inOnStatus
                                  onAuthorized:(ALAccessoryOnAppAuthenticated)inOnAuthorized {
    [self openAuthorizedSessionForAutomaticClient:inClientID allowingUserInteraction:inInteractionAllowed onStatus:inOnStatus onAuthorized:inOnAuthorized];
}

- (BOOL)openAuthorizedSessionForAutomaticClient:(NSString *)inClientID
						allowingUserInteraction:(BOOL)inInteractionAllowed
									   onStatus:(ALAccessoryOnAuthorizationStatus)inOnStatus
                                   onAuthorized:(ALAccessoryOnAppAuthenticated)inOnAuthorized {
    NSAssert(inOnAuthorized, @"nil ALAccessoryOnAppAuthenticated callback");

    ALAutomaticAdapter *adapter = [self asAutomaticAdapter];
    if (adapter == nil) {
        return NO;
    }

    ALAccessoryOnAppAuthenticated onAuthorized = [inOnAuthorized copy];
    [adapter
        openAuthorizedSessionForAutomaticClient:inClientID
                        allowingUserInteraction:inInteractionAllowed
                                       onStatus:inOnStatus
                                   onAuthorized:^(ALELM327Session *inSession, NSString *inGreeting) {
                                       EASession<ALDuplexStream> *eaSession = (EASession<ALDuplexStream> *)[inSession detach];
                                       onAuthorized(eaSession, inGreeting);
                                   }
                                       onClosed:nil];

    return YES;
}

@end
