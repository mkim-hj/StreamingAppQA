//
//  AUTHTTPMultipartRequest.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 11/3/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTHTTPRequest.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTHTTPMultipartRequest : AUTHTTPRequest

/// An optional array of file URLs that will be uploaded by the receiver.
/// Defaults to `nil`.
@property (readonly, nonatomic, copy, nullable) NSArray<NSURL *> *files;

#pragma mark - DSL

/// Builds a new `AUTHTTPMultipartRequest` that uploads the provided file. Can be
/// chained to upload multiple files.
///
/// The name of the file is inferred via the `lastPathComponent` of the provided
/// URL.
- (instancetype)file:(NSURL *)file;

/// Sets the files the receiver is uploading, overwriting any previous ones.
- (instancetype)files:(NSArray<NSURL *> *)files;

@end

NS_ASSUME_NONNULL_END
