//
//  RACSignal+AUTAuthenticationOperators.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 6/17/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

NS_ASSUME_NONNULL_BEGIN

@interface RACSignal (AUTAuthenticationOperators)

/// Sends an NSError if the receiver sends a username that is invalid, or
/// nil if the username is valid.
///
/// This signal does not error, it just sends NSErrors over its nexts.
///
/// Operates on a signal of strings or nils representing usernames.
- (RACSignal *)aut_usernameErrors;

/// Sends YES if the receiver sends a valid username, and NO if it does not.
///
/// Operates on a signal of strings or nils representing usernames.
- (RACSignal *)aut_isValidUsername;

/// Sends an NSError if the receiver sends a password that is invalid, or
/// nil if the password is valid.
///
/// This signal does not error, it just sends NSErrors over its nexts.
///
/// Operates on a signal of strings or nils representing passwords.
- (RACSignal *)aut_passwordErrors;

/// Sends YES if the receiver sends a valid password, and NO if it does not.
///
/// Operates on a signal of strings or nils representing passwords.
- (RACSignal *)aut_isValidPassword;

@end

NS_ASSUME_NONNULL_END
