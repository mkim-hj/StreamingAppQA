//
//  AUTAPIPage_Private.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 26/05/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

/// Implements a `AUTAPIPage` subclass interface for a given result class.
///
/// The `results` property of the resulting class will employ a Mantle
/// transformer for an array of `RESULT_CLASS` instances populated using the
/// array located at `COLLECTION_KEY` of the input JSON Dictionary
///
/// The name of the new `AUTAPIPage` subclass will be the name of the result class
/// suffixed with the word "Page", e.g.
///
///     AUTPageSubclassInterface(AUTFoo)
///
/// will implement a new subclass `AUTFooPage`.
#define AUTPageSubclassImplementationWithCollectionKey(RESULT_CLASS, COLLECTION_KEY) \
    @implementation RESULT_CLASS ## Page \
    \
    @dynamic results;\
    \
    + (nonnull NSValueTransformer *)resultsJSONTransformer { \
    return [MTLJSONAdapter arrayTransformerWithModelClass:RESULT_CLASS.class]; \
    } \
    \
    + (nonnull NSString *)collectionKey { \
    return COLLECTION_KEY;\
    }\
    @end

/// Convenience macro for backwards compatability with the default `results`
/// key
#define AUTPageSubclassImplementation(RESULT_CLASS) AUTPageSubclassImplementationWithCollectionKey(RESULT_CLASS, @"results")
