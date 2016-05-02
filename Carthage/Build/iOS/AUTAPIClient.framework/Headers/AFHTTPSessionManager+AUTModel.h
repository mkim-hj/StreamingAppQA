//
//  AFHTTPRequestOperationManager+AUTModel.h
//  AUTAPIClient
//
//  Created by James Lawton on 7/15/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import AFNetworking;

#import "AUTAPIPage+AUTPaginationDirection.h"

@class AUTAPIPage;
@class AUTHTTPRequest;
@class MTLModel;
@class RACSignal;

NS_ASSUME_NONNULL_BEGIN

/// If there is a page to fetch, should return cold signal that upon execution,
/// fetches the next page of the provided AUTAPIPage, then completes, or errors.
///
/// If there is no page to fetch, should return nil.
typedef RACSignal * _Nullable (^AUTFetchNextPage)(AUTAPIPage *page);

@interface AFHTTPSessionManager (AUTModel)

/// Enqueue a request described by an `AUTHTTPRequest`.
///
/// @param request A description of the request.
///
/// @return A cold signal which, on subscription, will enqueue the network request
///         and do subsequent model parsing.
- (RACSignal *)aut_convertAndEnqueueRequest:(AUTHTTPRequest *)request;

/// Enqueues an `NSURLRequest` request.
///
/// @param request     The `NSURLRequest` to perform. This argument must not be
///                    nil.
/// @param resultClass If not nil, the receiver will attempt to parse the
///                    returned JSON data into an instance of this `class`.
///                    If this argument is not nil, it must conform to
///                    `MTLJSONSerializing`.
///
/// @return A signal that will send the results of the request, or send an error
///         otherwise.
- (RACSignal *)aut_enqueueRequest:(NSURLRequest *)request resultClass:(nullable Class)resultClass;

/// Invokes aut_fetchNextPageForPage:inDirection: the default "next" direction.
- (nullable RACSignal *)aut_fetchNextPageForPage:(AUTAPIPage *)page;

/// Get the next page of results.
/// AUTAPIPage -> signal<AUTAPIPage>
///
/// @param page A page of results from the API.
///
/// @param direction The direction to paginate in.
///
/// @return If there is a next page, a signal that will send the next page of
///         results or error. If there is no next page, nil.
- (nullable RACSignal *)aut_fetchNextPageForPage:(AUTAPIPage *)page inDirection:(AUTPaginationDirection)direction;

/// Invokes aut_asSignalOfSignals:inDirection:fetchNextPage: in the default
/// "next" direction.
+ (RACSignal *)aut_asSignalOfSignals:(RACSignal *)pageSignal fetchNextPage:(AUTFetchNextPage)fetchNextPage;

/// Convert a signal of `AUTAPIPage`s to a signal of signals of arrays of the
/// page's respective results.
/// signal<AUTAPIPage> -> signal<signal<[Model]>>
///
/// This signal encapsulates the client and allows streams of pages to be
/// consumed without being aware of the receiver.
///
/// @param pageSignal A signal of `AUTAPIPage` instances. This argument must not be
///                   `nil`.
///
/// @param direction The direction to paginate in.
///
/// @param fetchNextPage A block that when given a page, will return a signal
///        that represents fetching it.
///
/// @return A signal of signals of the page's results. Subscribing to an inner
///         signal and successfully loading its single page will send a new
///         signal over the outer signal.
+ (RACSignal *)aut_asSignalOfSignals:(RACSignal *)pageSignal inDirection:(AUTPaginationDirection)direction fetchNextPage:(AUTFetchNextPage)fetchNextPage;

/// Invokes aut_aggregatePages:inDirection:fetchNextPage: in the default "next"
/// direction.
+ (RACSignal *)aut_aggregatePages:(RACSignal *)pageSignal fetchNextPage:(AUTFetchNextPage)fetchNextPage;

/// Converts a signal which sends a page of results into a signal which sends a
/// single array, which is an aggregate of all results from that page forward.
/// signal<AUTAPIPage> -> signal<[Model]>
///
/// @param pageSignal  A signal which sends an `AUTAPIPage` or an error.
///
/// @param direction The direction to paginate in.
///
/// @param fetchNextPage A block that when given a page, will return a signal
///        that represents fetching it.
///
/// @return A signal which sends an array of results collected from all pages
///         from that returned by `pageSignal`, onwards, or an error.
+ (RACSignal *)aut_aggregatePages:(RACSignal *)pageSignal inDirection:(AUTPaginationDirection)direction fetchNextPage:(AUTFetchNextPage)fetchNextPage;

/// Parse a model of the given class from a JSON object.
///
/// @param class  The class of model to create.
/// @param object The JSON object to parse.
/// @param error  Outputs the parsing error, if `nil` is returned.
///
/// @return A parsed model, or `nil`.
+ (nullable MTLModel *)aut_modelOfClass:(Class)class fromJSONObject:(nullable id)object error:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
