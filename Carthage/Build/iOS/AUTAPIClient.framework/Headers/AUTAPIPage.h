//
//  AUTAPIPage.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 25/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <Mantle/Mantle.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIPage : MTLModel <MTLJSONSerializing>

@property (readonly, nonatomic, copy) NSArray *results;

@property (readonly, nonatomic, strong, nullable) NSURL *nextPage;

@property (readonly, nonatomic, strong, nullable) NSURL *previousPage;

@end

NS_ASSUME_NONNULL_END

/// Declares a new `AUTAPIPage` subclass interface for a given result class.
///
/// The name of the new `AUTAPIPage` subclass will be the name of the result class
/// suffixed with the word "Page", e.g.
///
///     AUTPageSubclassInterface(AUTFoo)
///
/// will declare a new subclass `AUTFooPage`.
#define AUTPageSubclassInterface(RESULT_CLASS) \
    @interface RESULT_CLASS ## Page : AUTAPIPage \
    @property (readonly, nonatomic, copy) NSArray <RESULT_CLASS *> *results; \
    @end
