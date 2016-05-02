//
//  AUTAPIDeletable.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 1/8/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Foundation;

NS_ASSUME_NONNULL_BEGIN

/// A box representing a deletable object within an object graph.
///
/// For use in requests where a deletion is communicated as a subset of an
/// object graph, where the DELETE HTTP verb is not correct. An example is would
/// be if an object's deletion is communicated within the context of a PATCH to
/// its parent.
///
/// Within the context of JSON requests, allows consumers to specify the tri-
/// state values of:
/// - Omitted from JSON entirely (key is not present, box is nil)
/// - Deletion communicated in JSON as null (key has a value of null, box is
///   non-nil but contains nil value)
/// - The boxed value itself as a JSON value (key: value, box contains a value)
@interface AUTAPIDeletable <__covariant ObjectType: id<MTLJSONSerializing>> : MTLModel

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithDictionary:(NSDictionary *)dictionaryValue error:(NSError **)error NS_UNAVAILABLE;
+ (instancetype)modelWithDictionary:(NSDictionary *)dictionaryValue error:(NSError **)error NS_UNAVAILABLE;

- (instancetype)initWithValue:(nullable ObjectType)value NS_DESIGNATED_INITIALIZER;

@property (nonatomic, strong, nullable) ObjectType value;

@end

@interface MTLJSONAdapter (AUTAPIDeletable)

/// Creates a value transformer suitable for boxing and unboxing the provided
/// deletable object in JSON.
///
/// @param modelClass The MTLModel subclass to attempt to parse from the JSON.
///        This class must conform to <MTLJSONSerializing>. This argument must
///        not be nil.
+ (NSValueTransformer *)aut_deletableDictionaryTransformerWithModelClass:(Class)modelClass;

@end

NS_ASSUME_NONNULL_END
