//
//  AUTClient+AUTAPIUser.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 25/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@class RACSignal;

@interface AUTClient (AUTAPIUser)

/// Fetches the currently authenticated user.
///
/// @return A signal that sends the currently authenticated `AUTAPIUser`  and then
///         completes, or sends an error otherwise.
- (RACSignal *)fetchCurrentUser;

/// Fetches a user by their ID.
///
/// @param objectID The ID of the user to fetch. This argument must not be nil.
///
/// @return A signal that sends the `AUTAPIUser` matching the given ID and then
///         completes, or sends an error otherwise.
- (RACSignal *)fetchUserWithID:(NSString *)objectID;

@end

NS_ASSUME_NONNULL_END
