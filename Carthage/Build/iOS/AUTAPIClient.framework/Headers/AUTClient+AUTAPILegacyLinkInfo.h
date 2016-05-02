//
//  AUTClient+AUTLinkInfo.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 2015-09-28.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

@import ReactiveCocoa;

#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@class AUTAPILegacyLinkInfo;

@interface AUTClient (AUTAPILegacyLinkInfo)

/// Returns a signal that will update link info with new iOS BT ID,
/// or else error.
///
/// @param linkInfo a AUTAPILegacyLinkInfo to update
- (RACSignal *)updateLinkInfo:(AUTAPILegacyLinkInfo *)linkInfo;

/// Returns a signal that will create a new link info,
/// or else error.
///
/// @param linkInfo a new AUTAPILegacyLinkInfo
- (RACSignal *)createLinkInfo:(AUTAPILegacyLinkInfo *)linkInfo;

/// Returns a signal that will return all link infos for current user.
- (RACSignal *)fetchLinkInfos;

@end

NS_ASSUME_NONNULL_END
