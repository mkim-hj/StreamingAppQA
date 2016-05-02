//
//  AUTLicensePlusClient+AUTSession.h
//  AUTAPIClient
//
//  Created by James Lawton on 7/16/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTLicensePlusClient.h>

NS_ASSUME_NONNULL_BEGIN

@class RACSignal;

@interface AUTLicensePlusClient (AUTSession)

/// Convert a grant token into a session with the License+ server.
///
/// @param grantToken Grant token from the core API.
///
/// @return A signal sending a session ID (string), after a session has been
///         created and the session ID has been updated on the receiver.
- (RACSignal *)authenticateSessionWithGrantToken:(NSString *)grantToken;

@end

NS_ASSUME_NONNULL_END
