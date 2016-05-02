//
//  AUTClient+AUTAPIRealTimeEvent.h
//  AUTAPIClient
//
//  Created by Sylvain Rebaud on 8/2/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIRealTimeEvent.h>
#import <AUTAPIClient/AUTClient.h>

NS_ASSUME_NONNULL_BEGIN

@interface AUTClient (AUTAPIRealTimeEvent)

/// Uploads a real time event.
///
/// Returns a signal which send an AUTAPILegacyResponse and then complete or
/// error.
- (RACSignal *)uploadRealTimeEvent:(AUTAPIRealTimeEvent *)event;

@end

NS_ASSUME_NONNULL_END
