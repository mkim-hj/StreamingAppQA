//
//  AFHTTPRequestOperationManager+AUTReactiveCocoaAdditions.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 24/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import AFNetworking;

#import <AUTAPIClient/RACSignal+AUTRetryRequest.h>

NS_ASSUME_NONNULL_BEGIN

@class RACSignal;

@interface AFHTTPSessionManager (AUTReactiveCocoaAdditions)

/// Returns a cold signal that, upon subscription, enqueues the provided
/// request. If the request is successful, sends the responseObject from
/// AFNetworking, and completes. If an error occurred, errors.
- (RACSignal *)aut_enqueueRequest:(NSURLRequest *)request;

/// Returns a signal of aut_enqueueRequest: with the provided request, chained
/// to aut_retryForHost: for the host of the provided request's URL.
- (RACSignal *)aut_enqueueRetryingRequest:(NSURLRequest *)request;

/// Returns a signal of aut_enqueueRequest: with the provided request, chained
/// to aut_retryForHost:isErrorRecoverable: for the host of the provided
/// request's URL and with the provided AUTIsErrorRecoverableBlock block.
- (RACSignal *)aut_enqueueRetryingRequest:(NSURLRequest *)request isErrorRecoverable:(AUTIsErrorRecoverableBlock)isErrorRecoverable;

/// Returns a signal of aut_enqueueRequest: with the provided request, chained
/// to aut_retryForReachability: with the given reachability signal
- (RACSignal *)aut_enqueueRetryingRequest:(NSURLRequest *)request withReachability:(RACSignal *)reachability;

@end

NS_ASSUME_NONNULL_END
