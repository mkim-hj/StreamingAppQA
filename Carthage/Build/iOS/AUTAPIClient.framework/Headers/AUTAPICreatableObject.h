//
//  AUTAPICreatable.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 3/1/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

@import Mantle;
@import ReactiveCocoa;

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPICreatableObject : MTLModel <MTLJSONSerializing>

/// A method to specify if the receiver is being created from a server response
/// or locally.
- (nullable instancetype)initWithDictionary:(NSDictionary *)dictionaryValue createdLocally:(BOOL)createdLocally error:(NSError **)error;

/// The date when the object was first created.
@property (readonly, nonatomic, copy) NSDate *creationDate;

/// The date when the object was last modified.
///
/// This will be `nil` if the object was generated locally.
@property (readonly, nonatomic, copy, nullable) NSDate *updateDate;

/// The unique ID of this object, from the server.
///
/// This will be `nil` if the object was generated locally.
@property (readonly, nonatomic, copy, nullable) NSString *objectID;

/// A Bool indicating whether the object was created from a server response
/// or locally.
@property (readonly, nonatomic, getter=isCreatedLocally) BOOL createdLocally;

@end

NS_ASSUME_NONNULL_END
