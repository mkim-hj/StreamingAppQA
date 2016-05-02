//
//  AUTMediaCreationResults.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 11/4/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

@import Mantle;

NS_ASSUME_NONNULL_BEGIN

/// Describes the result of a media creation request for one or more files.
@interface AUTMediaCreationResults : MTLModel <MTLJSONSerializing>

/// The remote URLs of the files that were uploaded from a media creation
/// request, keyed by the name of the files that were uploaded.
@property (readonly, nonatomic, copy) NSDictionary<NSString *, NSURL *> *URLsByFilename;

@end

NS_ASSUME_NONNULL_END
