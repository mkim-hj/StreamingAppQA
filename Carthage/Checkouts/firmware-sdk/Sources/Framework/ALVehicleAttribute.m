/*****************************************************************************
 **
 **  Automatic Labs - CONFIDENTIAL
 **
 **  Unpublished Copyright (c) 2009-2016 AUTOMATIC LABS, All Rights Reserved.
 **
 **  NOTICE:
 **
 **  All information contained herein is, and remains the property of AUTOMATIC LABS.
 **  The intellectual and technical concepts contained herein are proprietary to AUTOMATIC LABS
 **  and may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade
 **  secret or copyright law.
 **
 **  Dissemination of this information or reproduction of this material is strictly forbidden unless
 **  prior written permission is obtained from AUTOMATIC LABS. Access to the source code contained
 **  herein is hereby forbidden to anyone except current AUTOMATIC LABS employees, managers or
 **  contractors who have executed Confidentiality and Non-disclosure agreements explicitly covering
 **  such access.
 **
 **  The copyright notice above does not evidence any actual or intended publication or disclosure of
 **  this source code, which includes information that is confidential and/or proprietary, and is a
 **  trade secret, of AUTOMATIC LABS. ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC PERFORMANCE,
 **  OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF
 **  AUTOMATIC LABS IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE LAWS AND INTERNATIONAL TREATIES.
 **  THE RECEIPT OR POSSESSION OF THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT CONVEY OR IMPLY
 **  ANY RIGHTS TO REPRODUCE, DISCLOSE OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL
 **  ANYTHING THAT IT MAY DESCRIBE, IN WHOLE OR IN PART.
 **
 ******************************************************************************/

#import "ALVehicleAttribute.h"
#import "ALVehicleCommands.h"

NS_ASSUME_NONNULL_BEGIN

NSDate* ALDateNearestToDates(NSDate* date, NSDate* earlier, NSDate* later, ALDateSearchDirection direction) {
    NSCAssert([earlier timeIntervalSinceDate:later] < 0, @"earlier must be before later");
    NSDate* nearest = nil;
    NSTimeInterval earlierInterval = [date timeIntervalSinceDate:earlier];
    NSTimeInterval laterInterval = [date timeIntervalSinceDate:later];

    if ((earlierInterval < 0 && laterInterval < 0) // both are in the past
    ||  (earlierInterval > 0 && laterInterval > 0) // both are in the future
    ||  (direction == ALDateSearchNearest)) {      // we just want the nearest in time
        nearest = (fabs(earlierInterval)<fabs(laterInterval)?earlier:later); // compare the absolute intervals
    }
    else if (direction == ALDateSearchFuture) { // choose the earlier date
        nearest = later;
    }
    else { // direction == ALDateSearchPast
        nearest = earlier;
    }

    return nearest;
}

NSDate* ALDateInSortedArrayNearestToDate(NSDate *date, NSArray<NSDate *>*array, ALDateSearchDirection direction, NSUInteger *foundIndex) {
    NSDate* nearbyDate = nil;
    NSUInteger nearbyIndex = 0;
    NSTimeInterval interval = date.timeIntervalSinceReferenceDate;

    // check some simple cases: short arrays, and edge conditions
    if (array.count == 1) {
        nearbyDate = array[nearbyIndex];
    }
    else if (array.count == 2) {
        nearbyDate = ALDateNearestToDates(date, array[0], array[1], direction);
        nearbyIndex = [array indexOfObject:nearbyDate];
    }
    else if (array[0].timeIntervalSinceReferenceDate >= interval) { // date was before the earliet date
        nearbyDate = array[0];
        nearbyIndex = 0;
    }
    else if (array.lastObject.timeIntervalSinceReferenceDate <= interval) { // date was after the latest date
        nearbyDate = array.lastObject;
        nearbyIndex = (array.count-1);
    }
    else { // binary search of the array
        NSUInteger first = 0;
        NSUInteger last = array.count-1;
        NSUInteger middle = (first+last)/2;
        NSDate *candidate = nil;

        while (middle > 0 && first <= last) { // binary search of valueTimes
            candidate = array[middle];
            NSTimeInterval candidateInterval = candidate.timeIntervalSinceReferenceDate;
//            NSLog(@"ALDateInSortedArrayNearestToDate: %f candidate: %f %4lu - %4lu - %4lu",
//                interval, candidate.timeIntervalSinceReferenceDate, (unsigned long)first, (unsigned long)middle, (unsigned long)last);

            if (candidateInterval == interval) { // we got lucky
                nearbyDate = candidate;
                nearbyIndex = middle;
                break; // while
            }
            else if (candidateInterval < interval) {
                first = middle + 1;
            }
            else if (last > 0) { // don't let it underflow
                last = middle - 1;
            }

            middle = (first+last)/2;
        }

        nearbyIndex = middle;

        // if we don't find an exact match, then search adjacent dates for the best fit for the direction we're searching
        if (!nearbyDate) {
            NSUInteger nearestIndex = nearbyIndex;
            NSUInteger earlierIndex = nearbyIndex - 1;
            NSUInteger laterIndex = nearbyIndex + 1;
            NSUInteger lastIndex = array.count - 1;
            if (nearbyIndex == 0) {
                if( direction == ALDateSearchFuture) { // pick the next later date
                    nearestIndex = laterIndex;
                }
            }
            else if (nearbyIndex == lastIndex) {
                if (direction == ALDateSearchPast) { // pick the earlier date
                    nearestIndex = earlierIndex;
                }
            }
            else { // could go either way
                NSTimeInterval nearbyInterval = [array[nearbyIndex] timeIntervalSinceDate:date];
                NSTimeInterval earlierInterval = [array[earlierIndex] timeIntervalSinceDate:date];
                NSTimeInterval laterInterval = [array[laterIndex] timeIntervalSinceDate:date];
                if ((direction == ALDateSearchPast && nearbyInterval > 0)
                 || fabs(earlierInterval) < fabs(nearbyInterval)) {
                    nearestIndex = earlierIndex;
                }
                else if ((direction == ALDateSearchFuture && nearbyInterval < 0)
                 || fabs(laterInterval) < fabs(nearbyInterval)) {
                    nearestIndex = laterIndex;
                }
                else {
                    nearestIndex = nearbyIndex;
                }
            }
            // update the index and date
            nearbyIndex = nearestIndex;
            nearbyDate = array[nearbyIndex];
        }
    }

    /* report back the index of the date we found if a pointer was provided */
    if (foundIndex) {
        *foundIndex = nearbyIndex;
    }

//    NSLog(@"ALDateInSortedArrayNearestToDate: %f count: %lu nearby: %f index: %lu",
//        date.timeIntervalSinceReferenceDate, array.count, nearbyDate.timeIntervalSinceReferenceDate, nearbyIndex);

    return nearbyDate;
}

#pragma mark - Private

@interface ALVehicleAttribute ()
@property (nonatomic, strong) NSMutableArray<NSDate *>*valueTimes;
@property (nonatomic, strong) NSMutableArray<NSObject *>*valueHistory;
@end

#pragma mark -

@implementation ALVehicleAttribute

+ (instancetype) attributeWithName:(NSString *)name infoDictionary:(nullable NSDictionary *)dictionary {
    return [[ALVehicleAttribute alloc] initWithName:name infoDictionary:dictionary];
}

- (instancetype) initWithName:(NSString *)name infoDictionary:(nullable NSDictionary *)dictionary {
    if (self = [super init]) {
        self.valueTimes = [NSMutableArray new];
        self.valueHistory = [NSMutableArray new];
        self.holdTime = ALHoldTimeZero;
        _name = name;
        _info = [dictionary copy];
    }
    return self;
}

#pragma mark - Properties

- (nullable NSDate *)lastUpdate {
    return [self.valueTimes lastObject];
}

- (nullable NSObject *)lastValue {
    return [self.valueHistory lastObject];
}

- (nullable NSArray<NSDate *>*)valueDates {
    return [self.valueTimes copy];
}

- (BOOL)isSeries {
    return (self.holdTime != ALHoldTimeZero);
}

- (Class)valueClass {
    static NSDictionary *valueMap = nil;
    if (!valueMap) {
        valueMap = [NSDictionary dictionaryWithObjectsAndKeys:
            [NSString class], @"string",
            [NSNumber class], @"number",
            [NSNumber class], @"bitfield",
            [NSArray class],  @"array",
            nil];
    }

    Class valueClass = valueMap[self.info[ALVehicleCommandResponseTypeKey]];
    if (!valueClass) {
        valueClass = [NSObject class];
    }
    return valueClass;
}

- (NSString *)JSONRepresentation {
    NSError *jsonError = nil;
    NSDictionary *representation = nil;

    if (self.valueHistory.lastObject) {
        representation = @{
            @"name": self.name,
            @"value": self.valueHistory.lastObject, // must be either a String, Number or Array
            @"time": @(self.valueTimes.lastObject.timeIntervalSince1970*1000) // provide miliseconds for JavaScript
        };
    }
    else {
        representation = @{
            @"name": self.name
        };
    }
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:representation options:0 error:&jsonError];
    NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    return jsonString;
}

#pragma mark - Recording and Reading Values

- (void) trimHistory {
    if (self.isSeries) {
        NSUInteger trimIndex = 0;
        NSDate *trimDate = [NSDate dateWithTimeIntervalSinceNow:-self.holdTime];
        ALDateInSortedArrayNearestToDate(trimDate, self.valueTimes, ALDateSearchPast, &trimIndex);
        NSRange trimmed = NSMakeRange(trimIndex, self.valueTimes.count-trimIndex);
        [self.valueTimes setArray:[self.valueTimes subarrayWithRange:trimmed]];
        [self.valueHistory setArray:[self.valueHistory subarrayWithRange:trimmed]];
    }
    else {
        [self.valueTimes setArray:@[self.valueTimes.lastObject]];
        [self.valueHistory setArray:@[self.valueHistory.lastObject]];
    }
}

- (NSDate *)recordValue:(NSObject *)value {
    NSDate *recorded = nil;
    if (value) {
        recorded = [self recordValue:value atDate:[NSDate date]];
    }
    return recorded;
}

- (NSDate *)recordValue:(NSObject *)value atDate:(NSDate *)date {
    if (self.holdTime > ALHoldTimeZero) {
        [self.valueTimes addObject:date];
        [self.valueHistory addObject:value];
        [self trimHistory];
    }
    else {
        self.valueTimes[0] = date;
        self.valueHistory[0] = value;
    }
    return date;
}

- (NSObject *)valueAtDate:(NSDate *)date {
    NSUInteger valueIndex = [self indexOfDateInHistory:date];
    return self.valueHistory[valueIndex];
}

- (double)scaledValue:(NSNumber *)value {
    double doubleValue = value.doubleValue;
    double minValue = [self.info[ALVehicleCommandResponseLimitsKey][0] doubleValue];
    double maxValue = [self.info[ALVehicleCommandResponseLimitsKey][1] doubleValue];
    if (minValue != maxValue) { // they might both be 0, and we can't scale aginst 0
        doubleValue = (doubleValue-minValue)/maxValue;
    }
    return doubleValue;
}

#pragma mark - Time Series Methods

- (NSDate *)historyDateNearestTo:(NSDate *)date searchDirection:(ALDateSearchDirection) direction foundAtIndex:(NSUInteger *)foundIndex {
    NSUInteger nearbyIndex = 0;
    NSDate* nearbyDate = ALDateInSortedArrayNearestToDate(date, self.valueTimes, direction, &nearbyIndex);

    if (foundIndex) {
        *foundIndex = nearbyIndex;
    }

    return nearbyDate;
}

- (NSInteger)indexOfDateInHistory:(NSDate *)date {
    NSRange searchRange = NSMakeRange(0, self.valueTimes.count);
    NSCalendar *gregorianCalendar = [NSCalendar calendarWithIdentifier:NSCalendarIdentifierGregorian];
    NSUInteger nearestIndex = [self.valueTimes
        indexOfObject:date
        inSortedRange:searchRange
        options:NSBinarySearchingFirstEqual
        usingComparator:^(id obj1, id obj2) {
            return [gregorianCalendar
                compareDate:(NSDate *)obj1
                toDate:(NSDate *)obj2
                toUnitGranularity:NSCalendarUnitNanosecond];
        }];
    return nearestIndex;
}

- (NSArray<NSDate *>*)historyDatesAfter:(NSDate *)start {
    NSUInteger startIndex = 0;
    [self historyDateNearestTo:start searchDirection:ALDateSearchFuture foundAtIndex:&startIndex];
    return [self.valueTimes subarrayWithRange:NSMakeRange(startIndex, self.valueTimes.count-startIndex)];
}

- (NSArray<NSDate *>*)historyDatesBetween:(NSDate *)start and:(NSDate *)end {
    NSUInteger startIndex = 0;
    NSUInteger endIndex = 0;
    [self historyDateNearestTo:start searchDirection:ALDateSearchFuture foundAtIndex:&startIndex];
    [self historyDateNearestTo:end searchDirection:ALDateSearchPast foundAtIndex:&endIndex];
    return [self.valueTimes subarrayWithRange:NSMakeRange(startIndex, endIndex-startIndex)];
}

- (NSArray<NSDate *>*)historyDatesIncluding:(NSDate *)start and:(NSDate *)end {
    NSUInteger startIndex = 0;
    NSUInteger endIndex = 0;
    [self historyDateNearestTo:start searchDirection:ALDateSearchPast foundAtIndex:&startIndex];
    [self historyDateNearestTo:end searchDirection:ALDateSearchFuture foundAtIndex:&endIndex];
    return [self.valueTimes subarrayWithRange:NSMakeRange(startIndex, endIndex-startIndex)];
}

#pragma mark - NSObject

- (NSString *)description {
    return [NSString stringWithFormat:@"<%@: %p name: %@ value: %@ update: %@ hold time: %f>",
        self.class, self, self.name, self.lastValue, self.lastUpdate, self.holdTime];
}

@end

NS_ASSUME_NONNULL_END
