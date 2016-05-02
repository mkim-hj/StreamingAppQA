//
//  AUTClient+AUTLogChunkUpload.h
//  AUTAPIClient
//
//  Created by Justin Spahr-Summers on 2015-06-18.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

@class AUTAPILogChunk;

NS_ASSUME_NONNULL_BEGIN

/// The maximum accepted size for log chunks by the API, in bytes.
extern const NSUInteger AUTMaximumLogChunkSize;

@interface AUTClient (AUTLogChunkUpload)

/// Uploads one chunk from a batch of log data.
///
/// Returns a signal which will send an AUTAPILegacyResponse and then complete or
/// error.
- (RACSignal *)uploadLogChunk:(AUTAPILogChunk *)logChunk;

/// Returns a signal that internally automatically retries uploading the
/// specified log chunk until it is successful or an unrecoverable error
/// occurs.
///
/// If the upload was successful, sends a next of AUTAPILegacyResponse and
/// completes. If an unrecoverable error occurs, errors with the unrecoverable
/// error. If a recoverable error occurs, internally retries the upload after a
/// back-off interval.
- (RACSignal *)autoretryingUploadLogChunk:(AUTAPILogChunk *)logChunk;

/// @see autoretryingUploadLogChunk: for signal contract.
///
/// @param reachabilityStatus A signal that sends AFNetworkReachabilityStatus,
///        typically for use when testing.
///
/// @param errors A subject that sends the errors that occur during internal
///        retries.
- (RACSignal *)autoretryingUploadLogChunk:(AUTAPILogChunk *)logChunk reachabilityStatus:(RACSignal *)reachabilityStatus errors:(nullable RACSubject *)errors;

@end

/// The amount of time than an autoretrying log chunk upload should wait before
/// retrying after a recoverable error, given the number of retries that have
/// occurred so far.
NSTimeInterval AUTLogChunkUploadAutoretryBackOffInterval(NSUInteger retry);

NS_ASSUME_NONNULL_END
