//
//  AUTLocationValueTransformer.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 11/12/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// The key in the transformed value dictionary for timestamp.
extern NSString * const AUTLocationValueTransformerTimestampKey;

/// The key in the transformed value dictionary for horizontal accuracy.
extern NSString * const AUTLocationValueTransformerHorizontalAccuracyKey;

/// Transforms between CLLocation and an NSDictionary of:
///
/// {
///     AUTCoordinateValueTransformerLatitudeKey: NSNumber,
///     AUTCoordinateValueTransformerLongitudeKey: NSNumber,
///     AUTLocationValueTransformerTimestampKey: NSString,
///     AUTLocationValueTransformerHorizontalAccuracyKey: NSNumber,
/// }
@interface AUTLocationValueTransformer : NSValueTransformer <MTLTransformerErrorHandling>

@end

NS_ASSUME_NONNULL_END
