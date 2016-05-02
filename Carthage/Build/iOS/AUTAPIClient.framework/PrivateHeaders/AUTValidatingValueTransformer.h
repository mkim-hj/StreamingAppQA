//
//  AUTValidatingValueTransformer.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 11/18/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// Describes the nullability behavior of a validating value transformer.
typedef NS_ENUM(NSInteger, AUTValidatingValueTransformerNullabilityBehavior) {
    /// The transformed object must be non-null.
    AUTValidatingValueTransformerNullabilityBehaviorNonnull,

    /// The transformed object may be null.
    AUTValidatingValueTransformerNullabilityBehaviorNullable,
};

/// A value transformer that validates the class and nullability of a
/// transformed object in forward and reverse transformations.
@interface AUTValidatingValueTransformer : NSValueTransformer <MTLTransformerErrorHandling>

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithValueClass:(Class)valueClass nullabilityBehavior:(AUTValidatingValueTransformerNullabilityBehavior)nullabilityBehavior NS_DESIGNATED_INITIALIZER;

/// A value transformer that errors if the transformed value is null or not of
/// the given class.
+ (instancetype)nonnullValidatingTransformerForClass:(Class)valueClass;

/// Errors if the transformed value is not of the given class.
+ (instancetype)nullableValidatingTransformerForClass:(Class)valueClass;

@end

NS_ASSUME_NONNULL_END
