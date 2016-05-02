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
 **  prior written permission is obtained from AUTOMATIC LABS.  Access to the source code contained
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

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*! @const ALHoldTimeInfinite - -1, not reccomended for production use */
static NSTimeInterval const ALHoldTimeInfinite = -1;

/*! @const ALZeroHoldTime - 0 */
static NSTimeInterval const ALHoldTimeZero = 0;

/*! @const ALHoldTimeOneMinute - 1 minute */
static NSTimeInterval const ALHoldTimeOneMinute = 60;

/*! @const ALHoldTimeOneHour - 1 hour */
static NSTimeInterval const ALHoldTimeOneHour = (ALHoldTimeOneMinute * 60);

/*! @const ALHoldTimeDefault - ALHoldTimeZero */
static NSTimeInterval const ALHoldTimeDefault = ALHoldTimeZero;

#pragma mark -

typedef enum {
    ALDateSearchPast = -1,
    ALDateSearchNearest = 0,
    ALDateSearchFuture = 1
} ALDateSearchDirection;

NSDate* ALDateNearestToDates(NSDate* date, NSDate* earlier, NSDate* later, ALDateSearchDirection direction);

NSDate* ALDateInSortedArrayNearestToDate(NSDate *date, NSArray<NSDate *>*array, ALDateSearchDirection direction, NSUInteger *foundIndex);

#pragma mark -

/*! @abstract a single attribute of an ALVehicleSession with meta-information about the attribute from ALEML327Protocol */
@interface ALVehicleAttribute : NSObject

/*! @abstract attribute name, from ALVehicleSesshion.h */
@property (nonatomic, readonly) NSString *name;

/*! @abstract attribute info, keys from ALELM327Protocol.h */
@property( nonatomic, readonly, nullable) NSDictionary *info;

/*! @abstract the last date a value was recorded for this attribute */
@property (nonatomic, readonly, nullable) NSDate *lastUpdate;

/*! @abstract the last value which was recorded for this attribute */
@property (nonatomic, readonly, nullable) NSValue *lastValue;

/*! @abstract the Dates on which the attribute value was recorded */
@property (nonatomic, readonly, nullable) NSArray<NSDate *>*valueDates;

/*! @abstract is this attribute a series of values? YES if holdTime > 0 */
@property (nonatomic, readonly) BOOL isSeries;

/*! @abstract the hold time for this attribute, the default is '0' which holds a single value */
@property (nonatomic, assign) NSTimeInterval holdTime;

/*! @abstract the value type for this attribute, NSString, NSNumber or NSArray<NSNumber> */
@property (nonatomic, readonly) Class valueClass;

/*! @abstract the JSON string representation of the attribute, and it's associated info dictionary */
@property (nonatomic, readonly, copy) NSString* JSONRepresentation;

#pragma mark -

/** @returns an attribute with the name and info dictionary provided */
+ (instancetype)attributeWithName:(NSString *)name infoDictionary:(nullable NSDictionary *)dictionary;

/** @returns an attribute with the name and info dictionary provided, designated initilizer */
- (instancetype)initWithName:(NSString *)name infoDictionary:(nullable NSDictionary *)dictionary;

#pragma mark - Value Scaling

/*! @param value - the numeric value to be scaled for this property
    @returns a scaled number between 0 and 1 for the value provided */
- (double)scaledValue:(NSNumber *)value;

#pragma mark - Value History

/*! @param value - the value to be recorded
    @returns date - the date on which the value was recorded
    @abstract record a new value for this attribute */
- (NSDate *)recordValue:(NSObject *)value;

/*! @param value - the value to record
    @param date - the date on which the value was originally recorded 
    @returns date - the date on which the value was recorded 
    @abstract allws you to record dates in the past or future */
- (NSDate *)recordValue:(NSObject *)value atDate:(NSDate *)date;

/*! @abstract return the value for this attribute recorded on the date provided
    @param date - the date the original value was recorded
    @returns value - the value for the attribute on that date */
- (NSObject *)valueAtDate:(NSDate *)date;

/*! @abstract return an array of dates that this attribute has recorded values for which fall after the date provided
    @returns dates - an array of dates for which we have values recorded */
- (NSArray<NSDate *>*)historyDatesAfter:(NSDate *)start;

/*! @abstract return an array of dates that this attribute has recorded values for which fall between the dates provided
    @param start - earilest date which we want in the array
    @param end - latest date which we want in the array
    @returns dates - an array of dates for which we have values recorded */
- (NSArray<NSDate *>*)historyDatesBetween:(NSDate *)start and:(NSDate *)end;

/*! @abstract trim the history of the attribute down to the holdTime */
- (void) trimHistory;

@end

NS_ASSUME_NONNULL_END
