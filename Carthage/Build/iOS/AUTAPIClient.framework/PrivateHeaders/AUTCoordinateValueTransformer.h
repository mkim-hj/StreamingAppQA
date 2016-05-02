//
//  AUTCoordinateValueTransformer.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 25/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

#import <AUTAPIClient/AUTValidatingValueTransformer.h>

NS_ASSUME_NONNULL_BEGIN

/// The key in the transformed value dictionary for latitude.
extern NSString * const AUTCoordinateValueTransformerLatitudeKey;

/// The key in the transformed value dictionary for longitude.
extern NSString * const AUTCoordinateValueTransformerLongitudeKey;

/// Transforms between NSValue<CLLocationCoordinate2D> and an NSDictionary of:
///
/// {
///     AUTCoordinateValueTransformerLatitudeKey: NSNumber,
///     AUTCoordinateValueTransformerLongitudeKey: NSNumber,
/// }
@interface AUTCoordinateValueTransformer : NSValueTransformer <MTLTransformerErrorHandling>

- (instancetype)init NS_UNAVAILABLE;

/// @param nullabilityBehavior Whether an error should be generated if the
///        transformed or reversed-transformed input value is nil.
- (instancetype)initWithNullabilityBehavior:(AUTValidatingValueTransformerNullabilityBehavior)nullabilityBehavior NS_DESIGNATED_INITIALIZER;

/// Creates a value transformer that errors if its transformed or reverse-
/// transformed values are nil.
+ (instancetype)nonnullValueTransformer;

/// Creates a value transformer that allows its transformed or reverse-
/// transformed values to be nil.
+ (instancetype)nullableValueTransformer;

@end

NS_ASSUME_NONNULL_END
