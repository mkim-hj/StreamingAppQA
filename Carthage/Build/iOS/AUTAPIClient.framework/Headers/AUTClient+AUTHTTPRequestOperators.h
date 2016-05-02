//
//  AUTClient+AUTHTTPRequestOperators.h
//  AUTAPIClient
//
//  Created by Westin Newell on 1/5/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

#import "AUTAPIPage+AUTPaginationDirection.h"

NS_ASSUME_NONNULL_BEGIN

/// A result type that encapsulates a pagination signal in either direction of
/// pagination.
@interface AUTBidirectionalPaginationResult : NSObject

- (instancetype)init NS_UNAVAILABLE;

@property (readonly, nonatomic) RACSignal *previousSignalOfSignals;
@property (readonly, nonatomic) RACSignal *nextSignalOfSignals;

@end

@interface AUTClient (AUTHTTPRequestOperators)

/// Invokes asSignalOfSignals:inDirection: in the default "next" direction.
- (RACSignal *)asSignalOfSignals:(RACSignal *)pageSignal;

/// Convert a signal of `AUTAPIPage`s to a signal of signals of arrays of the
/// page's respective results.
///
/// This signal encapsulates the client and allows streams of pages to be
/// consumed without being aware of the receiver.
///
/// @param pageSignal A signal of `AUTAPIPage` instances. This argument must not be
///        `nil`.
///
/// @return A signal of signals of the page's results. Subscribing to an inner
///         signal and successfully loading its single page will send a new
///         signal over the outer signal.
- (RACSignal *)asSignalOfSignals:(RACSignal *)pageSignal inDirection:(AUTPaginationDirection)direction;

/// Convert a signal of `AUTAPIPage`s into an AUTBidrectionalPaginationResult by
/// creating a signal for both directions using asSignalOfSignals:inDirection:.
///
/// @warning The nextSignalOfSignals is responsbile for fetching the first page
///          to ensure that results to not overlap between both signals. As
///          such, you must request at least one page form the
///          AUTBidirectionalPaginationResult's nextSignalOfSignals before you
///          can paginate using the previousSignalOfSignals.
///
/// @param pageSignal A signal of `AUTAPIPage` instances. This argument must not
///        be `nil`.
///
/// @return An AUTBidirectionalPaginationResult that contains two signal of
///         signals, one for paginating in the next direction and one for
///         paginating in the previous direction.
- (AUTBidirectionalPaginationResult *)bidirectionalSignalOfSignalsFromPageSignal:(RACSignal *)pageSignal;

/// Invokes aggregatePages:inDirection: in the default "next" direction.
- (RACSignal *)aggregatePages:(RACSignal *)pageSignal;

/// Converts a signal of `AUTAPIPage`s to a signal which sends a single array,
/// which is an aggregate of all results from that page forward, including that
/// page.
///
/// @param pageSignal A signal which sends an `AUTAPIPage` or an error.
///
/// @param direction The direction to paginate in.
///
/// @return A signal which sends an NSArray of results collected from all pages
///         from that returned by `pageSignal`, onwards, or an error.
- (RACSignal *)aggregatePages:(RACSignal *)pageSignal inDirection:(AUTPaginationDirection)direction;

/// Fetches the next page for a given `AUTAPIPage`.
///
/// @param direction The direction to paginate in.
///
/// @param page The page to fetch. This argument must not be nil. If the
///        provided page has no next page, the returned signal completes
///        immediately.
///
/// @return If there is a next page, a signal that sends the next page for
///         `page` and then completes, or sends an error otherwise. If there is
///         no next page, nil.
- (nullable RACSignal *)fetchPageInDirection:(AUTPaginationDirection)direction forPage:(AUTAPIPage *)page;

/// Retries the provided request with an exponential backoff until successful.
///
/// If the receiver is unable to perform the request due the to host of the
/// receiver being unavailable, waits until it becomes available before trying
/// the request.
///
/// @param A cold signal that represents performing a network request with the
///        receiver.
///
/// @return A signal that forwards the events of the provided signal, catching
///         and internally retrying in the case of recoverable network errors,
///         erroring otherwise.
- (RACSignal *)retryRequest:(RACSignal *)request;

@end

NS_ASSUME_NONNULL_END
