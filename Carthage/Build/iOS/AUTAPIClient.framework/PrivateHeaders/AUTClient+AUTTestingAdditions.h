//
//  AUTClient+AUTTestingAdditions.h
//  AUTAPIClient
//
//  Created by Robert Böhnke on 25/02/15.
//  Copyright (c) 2015 Automatic Labs. All rights reserved.
//

#import <AUTAPIClient/AUTClient.h>

@interface AUTClient (AUTTestingAdditions)

+ (instancetype)aut_authenticatedClientForTesting;

+ (instancetype)aut_unauthenticatedClientForTesting;

- (void)aut_becomeAuthenticated;

@end
