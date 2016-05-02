//
//  AutomaticAdapterTests.m
//  AutomaticAdapterTests
//
//  Created by Alf Watt on 2/4/16.
//  Copyright © 2016 Automatic Labs, Inc. All rights reserved.
//

#import <XCTest/XCTest.h>
#import <AutomaticAdapter/AutomaticAdapter.h>


static NSArray<NSDate *>* ALGenerateDateArray(NSDate *start, NSTimeInterval interval, NSUInteger count) {
    NSMutableArray* dates = [NSMutableArray arrayWithCapacity:count];
    NSUInteger index = 0;
    while (index < count) {
        [dates addObject:[NSDate dateWithTimeInterval:(interval*index) sinceDate:start]];
        index++;
    }
    
    return [NSArray arrayWithArray:dates];
}

#pragma mark -

@interface AutomaticAdapterTests : XCTestCase

@end

#pragma mark -

@implementation AutomaticAdapterTests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)testDateSearchNearestSingleCase {
    NSArray* oneDate = @[[NSDate date]];
    NSDate* target = [NSDate dateWithTimeIntervalSinceNow:60];
    NSUInteger foundIndex = NSNotFound;
    NSDate* nearest = ALDateInSortedArrayNearestToDate(target, oneDate, ALDateSearchNearest, &foundIndex);

    XCTAssert(nearest && (foundIndex != NSNotFound), @"oneDate: %@ target: %@ nearest: %@ foundIndex: %lu",
        oneDate, target, nearest, foundIndex);
}

- (void)testDateSearchNearestInTwoCase {
    NSArray* twoDates = @[[NSDate date], [NSDate dateWithTimeIntervalSinceNow:60]];
    NSDate* target = [NSDate dateWithTimeIntervalSinceNow:45];
    NSUInteger foundIndex = NSNotFound;
    NSDate* nearest = ALDateInSortedArrayNearestToDate(target, twoDates, ALDateSearchNearest, &foundIndex);
    XCTAssert(nearest && (foundIndex == 1),  @"twoDates: %@ target: %@ nearest: %@ foundIndex: %lu",
        twoDates, target, nearest, foundIndex);
}

- (void)testDateSearchPastInTwoCase {
    NSArray* twoDates = @[[NSDate date], [NSDate dateWithTimeIntervalSinceNow:60]];
    NSDate* target = [NSDate dateWithTimeIntervalSinceNow:45];
    NSUInteger foundIndex = NSNotFound;
    NSDate* nearest = ALDateInSortedArrayNearestToDate(target, twoDates, ALDateSearchPast, &foundIndex);
    XCTAssert(nearest && (foundIndex == 0),  @"twoDates: %@ target: %@ nearest: %@ foundIndex: %lu",
        twoDates, target, nearest, foundIndex);
}

- (void)testDateSearchFutureInTwoCase {
    NSArray* twoDates = @[[NSDate date], [NSDate dateWithTimeIntervalSinceNow:60]];
    NSDate* target = [NSDate dateWithTimeIntervalSinceNow:15];
    NSUInteger foundIndex = NSNotFound;
    NSDate* nearest = ALDateInSortedArrayNearestToDate(target, twoDates, ALDateSearchFuture, &foundIndex);
    XCTAssert(nearest && (foundIndex == 1),  @"twoDates: %@ target: %@ nearest: %@ foundIndex: %lu",
        twoDates, target, nearest, foundIndex);
}

- (void)testDateSearchNearestInThreeCase {
    NSArray* threeDates = @[[NSDate dateWithTimeIntervalSinceNow:-60], [NSDate date], [NSDate dateWithTimeIntervalSinceNow:60]];
    NSDate* target = [NSDate dateWithTimeIntervalSinceNow:15];
    NSUInteger foundIndex = NSNotFound;
    NSDate* nearest = ALDateInSortedArrayNearestToDate(target, threeDates, ALDateSearchNearest, &foundIndex);
    XCTAssert(nearest && (foundIndex == 1),  @"threeDates: %@ target: %@ nearest: %@ foundIndex: %lu",
        threeDates, target, nearest, foundIndex);
}

- (void)testDateSearchPastInThreeCase {
    NSArray* threeDates = @[[NSDate dateWithTimeIntervalSinceNow:-60], [NSDate date], [NSDate dateWithTimeIntervalSinceNow:60]];
    NSDate* target = [NSDate dateWithTimeIntervalSinceNow:-1];
    NSUInteger foundIndex = NSNotFound;
    NSDate* nearest = ALDateInSortedArrayNearestToDate(target, threeDates, ALDateSearchPast, &foundIndex);
    XCTAssert(nearest && (foundIndex == 0),  @"threeDates: %@ target: %@ nearest: %@ foundIndex: %lu",
        threeDates, target, nearest, foundIndex);
}

- (void)testDateSearchFutureInThreeCase {
    NSArray* threeDates = @[[NSDate dateWithTimeIntervalSinceNow:-60], [NSDate date], [NSDate dateWithTimeIntervalSinceNow:60]];
    NSDate* target = [NSDate dateWithTimeIntervalSinceNow:1];
    NSUInteger foundIndex = NSNotFound;
    NSDate* nearest = ALDateInSortedArrayNearestToDate(target, threeDates, ALDateSearchFuture, &foundIndex);
    XCTAssert(nearest && (foundIndex == 2),  @"threeDates: %@ target: %@ nearest: %@ foundIndex: %lu",
        threeDates, target, nearest, foundIndex);
}

- (void)testDateSearchNearestInOneHundredCase {
    NSDate *fiveMinutesAgo = [NSDate dateWithTimeIntervalSinceNow:-(60 * 5)]; //
    NSArray<NSDate *>*hundredDates = ALGenerateDateArray(fiveMinutesAgo, 5, 100); // every five seconds, over the last five minutes, and into the future a bit
    NSDate *target = [NSDate date];
    NSUInteger foundIndex = NSNotFound;
    NSDate *nearest = ALDateInSortedArrayNearestToDate(target, hundredDates, ALDateSearchNearest, &foundIndex);
    NSTimeInterval nearInterval = fabs([nearest timeIntervalSinceDate:target]);
    NSTimeInterval nextInterval = fabs([hundredDates[foundIndex+1] timeIntervalSinceDate:target]);
    NSTimeInterval prevInterval = fabs([hundredDates[foundIndex-1] timeIntervalSinceDate:target]);
    XCTAssert(nearest && foundIndex != 0 && foundIndex != NSNotFound && nearInterval < nextInterval && nearInterval < prevInterval,
        @"near: %f next: %f prev: %f foundIndex: %lu", nearInterval, nextInterval, prevInterval, foundIndex);
}

- (void)testDateSearchFutureInOneHundred {
    NSDate *fiveMinutesAgo = [NSDate dateWithTimeIntervalSinceNow:-(60 * 5)]; //
    NSArray<NSDate *>*hundredDates = ALGenerateDateArray(fiveMinutesAgo, 5, 100); // every five seconds, over the last five minutes, and into the future a bit
    NSDate *target = [NSDate date];
    NSUInteger foundIndex = NSNotFound;
    NSDate *nearest = ALDateInSortedArrayNearestToDate(target, hundredDates, ALDateSearchFuture, &foundIndex);
    NSTimeInterval nearInterval = [nearest timeIntervalSinceDate:target];
    NSTimeInterval prevInterval = [hundredDates[foundIndex-1] timeIntervalSinceDate:target];
    XCTAssert(nearest && foundIndex != 0 && foundIndex != NSNotFound && nearInterval > 0 && prevInterval < 0,
        @"near: %f prev: %f foundIndex: %lu", nearInterval, prevInterval, foundIndex);
}

- (void)testDateSearchPastInOneHundred {
    NSDate *fiveMinutesAgo = [NSDate dateWithTimeIntervalSinceNow:-(60 * 5)]; //
    NSArray<NSDate *>*hundredDates = ALGenerateDateArray(fiveMinutesAgo, 5, 100); // every five seconds, over the last five minutes, and into the future a bit
    NSDate *target = [NSDate date];
    NSUInteger foundIndex = NSNotFound;
    NSDate *nearest = ALDateInSortedArrayNearestToDate(target, hundredDates, ALDateSearchPast, &foundIndex);
    NSTimeInterval nearInterval = [nearest timeIntervalSinceDate:target];
    NSTimeInterval nextInterval = [hundredDates[foundIndex+1] timeIntervalSinceDate:target];
    XCTAssert(nearest && foundIndex != 0 && foundIndex != NSNotFound && nearInterval < 0 && nextInterval > 0,
        @"near: %f next: %f foundIndex: %lu", nearInterval, nextInterval, foundIndex);
}

- (void)testDateSearchInOneThousandCase {
    NSDate *target = [NSDate date];
    NSTimeInterval year = 60*60*24*365.24;
    NSArray<NSDate *>*thousandYears = ALGenerateDateArray([NSDate dateWithTimeIntervalSinceNow:-(year*500)], year, 1000);
    NSUInteger foundIndex = NSNotFound;
    NSDate *nearest = ALDateInSortedArrayNearestToDate(target, thousandYears, ALDateSearchNearest, &foundIndex);
    NSTimeInterval nearInterval = fabs([nearest timeIntervalSinceDate:target]);
    NSTimeInterval nextInterval = fabs([thousandYears[foundIndex+1] timeIntervalSinceDate:target]);
    NSTimeInterval prevInterval = fabs([thousandYears[foundIndex-1] timeIntervalSinceDate:target]);
    XCTAssert(nearest && foundIndex != 0 && foundIndex != NSNotFound && nearInterval < nextInterval && nearInterval < prevInterval,
        @"thousandYears: %lu target: %@ nearest: %@ foundIndex: %lu", thousandYears.count, target, nearest, foundIndex);
}

- (void)testDateSearchTimeOneHundredThousandLookups {
    // This is an example of a performance test case.
    NSTimeInterval year = 60*60*24*365.24;
    NSArray<NSDate *>*thousandYears = ALGenerateDateArray([NSDate dateWithTimeIntervalSinceNow:-(year*500)], year, 1000);
    srandomdev();

    [self measureBlock:^{
        // Put the code you want to measure the time of here.
        NSUInteger counter = 100000;
        while (counter-- > 0) {
            NSDate *target = [NSDate dateWithTimeIntervalSinceNow:(random()/RAND_MAX)*(year*400)];
            NSUInteger foundIndex = NSNotFound;
            ALDateInSortedArrayNearestToDate(target, thousandYears, ALDateSearchNearest, &foundIndex);
        }
    }];
}

@end
