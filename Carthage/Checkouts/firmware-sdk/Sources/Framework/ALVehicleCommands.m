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

#import "ALVehicleCommands.h"
#import "NSDictionary+AL.h"

@implementation ALVehicleCommands

+ (NSString *)ALVehicleCommandsPath {
	return [[NSBundle bundleForClass:self.class] pathForResource:@"ALVehicleCommands" ofType:@"json"];
}

+ (NSString *)ALVehicleCommandsJSON {
    return [[self ALVehicleCommandsInfo] ALCompactJSONString:nil];
}

+ (NSDictionary *)ALVehicleCommandsInfo {
	static NSDictionary *commandsPlist = nil;
	if (!commandsPlist) {
        NSError *jsonError = nil;
		NSString *commandsPlistPath = [ALVehicleCommands ALVehicleCommandsPath];
        if (!(commandsPlist = [NSDictionary ALDictionaryWithContentsOfJSONFile:commandsPlistPath error:&jsonError])) {
            NSString *msg = [NSString stringWithFormat:@"Cannot parse jason file: %@ %@ %@", commandsPlistPath, jsonError.localizedDescription, jsonError.debugDescription];
            [[NSException exceptionWithName:NSInternalInconsistencyException reason:msg userInfo:@{NSUnderlyingErrorKey: jsonError}] raise];
        }
	}
	
	return commandsPlist;
}

// maps the ELM command string (e.g. 0100) to a command name (e.g. ALODBSupportedPids)
+ (NSDictionary *)ALVehicleCommandsMap {
	static NSDictionary *commandMap = nil;
	if (!commandMap) {
		NSMutableDictionary *map = [NSMutableDictionary new];
		NSDictionary *protocolInfo = [self ALVehicleCommandsInfo];
		
		for (NSString *commandId in protocolInfo.allKeys) {
			NSString *command = protocolInfo[commandId][ALVehicleCommandStringKey];
			if (command) {
				map[command] = commandId;
			}
		}
		
		commandMap = [NSDictionary dictionaryWithDictionary:map];
	}
	return commandMap;
}

@end
