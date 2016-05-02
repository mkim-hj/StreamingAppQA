//
//  AUTAPIPage+AUTPaginationDirection.h
//  AUTAPIClient
//
//  Created by Westin Newell on 1/5/16.
//  Copyright © 2016 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTAPIPage.h>

/// The direction to paginate.
typedef NS_ENUM(NSUInteger, AUTPaginationDirection) {
    /// Paginate by repeatedly following the "next" page.
    AUTPaginationDirectionNext,
    
    /// Paginate by repeatedly following the "previous" page.
    AUTPaginationDirectionPrevious,
};

NS_ASSUME_NONNULL_BEGIN

@interface AUTAPIPage (AUTPaginationDirection)

/// The URL to paginate in the provided direction, or nil if there are no more
/// pages.
- (nullable NSURL *)URLForDirection:(AUTPaginationDirection)direction;

@end

NS_ASSUME_NONNULL_END
