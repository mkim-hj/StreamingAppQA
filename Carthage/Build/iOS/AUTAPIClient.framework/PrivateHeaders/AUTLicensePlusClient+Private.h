//
//  AUTLicensePlusClient+Private.h
//  AUTAPIClient
//
//  Created by James Lawton on 7/16/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTLicensePlusClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTLicensePlusClient ()

/// Enqueues an `AUTHTTPRequest` request.
///
/// @param request The `AUTHTTPRequest` to perform. This argument must not be nil.
///
/// @return A signal that will send the results of the request, or send an error
///         otherwise.
- (RACSignal *)enqueueRequest:(AUTHTTPRequest *)request;

/// Converts a signal which sends a page of results into a signal which sends a
/// single array, which is an aggregate of all results from that page forward.
/// signal<AUTAPIPage> -> signal<[Model]>
///
/// @param pageSignal  A signal which sends an `AUTAPIPage` or an error.
///
/// @return A signal which sends an array of results collected from all pages
///         from that returned by `pageSignal`, onwards, or an error.
- (RACSignal *)aggregatePages:(RACSignal *)pageSignal;

@end

NS_ASSUME_NONNULL_END
