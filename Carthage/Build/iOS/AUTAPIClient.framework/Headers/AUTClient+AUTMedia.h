//
//  AUTClient+AUTMedia.h
//  AUTAPIClient
//
//  Created by Eric Horacek on 11/4/15.
//  Copyright © 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

@class AUTMediaCreationResults;

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTMedia)

/// Uploads the media at the provided file URL.
///
/// Sends the remote NSURL of the file that was uploaded, otherwise errors.
- (RACSignal *)createMediaFromFile:(NSURL *)file;

/// Uploads the media at the provided file URLs.
///
/// Sends an NSArray of remote NSURLs in the same order as the provided file
/// URLs if successful, otherwise errors.
- (RACSignal *)createMediaFromFiles:(NSArray<NSURL *> *)files;

@end

NS_ASSUME_NONNULL_END
