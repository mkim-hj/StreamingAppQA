//
//  AUTAPILogChunk.h
//  AUTAPIClient
//
//  Created by Justin Spahr-Summers on 2015-06-19.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// Represents a portion of trip data to be uploaded to the server.
///
/// For an AUTAPILogChunk to be valid, at least one of the array properties must
/// be set before serialization.
///
/// See
/// https://phabricator.automatic.co/w/projects/java_api/user_related_apis/#upload-data
/// for more information about the format of this data.
@interface AUTAPILogChunk : MTLModel <MTLJSONSerializing>

/// Initializes a chunk that has the given index inside the given batch ID.
- (instancetype)initWithBatchID:(NSString *)batchID chunkIndex:(NSUInteger)chunkIndex totalChunks:(NSUInteger)totalChunks;

/// The ID of the trip (or "batch") that this chunk is associated with.
@property (nonatomic, copy, readonly) NSString *batchID;

/// This chunk's index in the batch.
///
/// This must be less than or equal to `totalChunks`.
@property (nonatomic, readonly) NSUInteger chunkIndex;

/// The total number of chunks in the batch.
@property (nonatomic, readonly) NSUInteger totalChunks;

/// An array of NSDictionary objects representing location updates during the
/// trip.
///
/// The default value for this property is an empty array.
@property (atomic, copy) NSArray *locationDictionaries;

/// An array of NSDictionary objects representing drive logs read during the
/// trip.
///
/// The default value for this property is an empty array.
@property (atomic, copy) NSArray *driveLogDictionaries;

/// An array of NSDictionary objects representing certain events that occurred
/// during the trip.
///
/// The default value for this property is an empty array.
@property (atomic, copy) NSArray *eventDictionaries;

/// An array of NSDictionary objects representing debug logs read or created
/// during the trip.
///
/// The default value for this property is an empty array.
@property (atomic, copy) NSArray *debugLogDictionaries;

@end

NS_ASSUME_NONNULL_END
