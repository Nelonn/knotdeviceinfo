/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#include "target.h"

#if defined(OS_MAC)

#import <Foundation/Foundation.h>
#import <sys/sysctl.h>

NSString* KNDeviceInfo_deviceFromSystemProfiler() {
    // Starting with MacBook M2 the hw.model returns simply Mac[digits],[digits].
    // So we try reading "system_profiler" output.
    NSTask* process = [[NSTask alloc] init];
    NSPipe* pipe = [NSPipe pipe];
    NSData* data = nil;
    NSError* error = nil;

    if (@available(macOS 10.13, *)) {
        process.executableURL = [NSURL fileURLWithPath:@"/usr/sbin/system_profiler"];
    } else {
        process.launchPath = @"/usr/sbin/system_profiler";
    }

    [process setArguments:@[@"-json", @"SPHardwareDataType", @"-detailLevel", @"mini"]];
    [process setStandardOutput:pipe];

    @try {
        /*if (@available(macOS 10.13, *)) {
            [process run];
        } else {
            [process launch];
        }*/
        [process launch];
    } @catch (NSException* exception) {
        return nil;
    }

    [process waitUntilExit];
    data = [[pipe fileHandleForReading] readDataToEndOfFile];

    id document = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingAllowFragments error:&error];

    if (![document isKindOfClass:[NSDictionary class]]) {
        return nil;
    }

    NSArray* fields = document[@"SPHardwareDataType"];

    if (![fields isKindOfClass:[NSArray class]] || fields.count == 0) {
        return nil;
    }

    NSString* result = fields[0][@"machine_name"];

    if (result == nil || [result isEqualToString:@""]) {
        return nil;
    }

    NSString* chip = fields[0][@"chip_type"];

    if ([chip hasPrefix:@"Apple "]) {
        return [result stringByAppendingString:[chip substringFromIndex:5]];
    } else {
        return result;
    }
}

NSString* KNDeviceInfo_fromIdentifier(NSString* model) {
    if (model == nil || [model isEqualToString:@""] || ![model.lowercaseString containsString:@"mac"]) {
        return nil;
    }

    NSMutableArray* words = [NSMutableArray array];
    NSMutableString* word = [NSMutableString string];

    for (NSUInteger i = 0; i < model.length; i++) {
        unichar ch = [model characterAtIndex:i];

        if (![[NSCharacterSet letterCharacterSet] characterIsMember:ch]) {
            continue;
        }

        if ([[NSCharacterSet uppercaseLetterCharacterSet] characterIsMember:ch]) {
            if (![word isEqualToString:@""]) {
                [words addObject:word];
                word = [NSMutableString string];
            }
        }

        [word appendFormat:@"%C", ch];
    }

    if (![word isEqualToString:@""]) {
        [words addObject:word];
    }

    NSMutableString* result = [NSMutableString string];

    for (NSString* word in words) {
        if (![result isEqualToString:@""] && ![word isEqualToString:@"Mac"] && ![word isEqualToString:@"Book"]) {
            [result appendString:@" "];
        }

        [result appendString:word];
    }

    return result;
}

NSString* KNDeviceInfo_deviceModelPretty() {
    NSString* fromSystemProfiler = KNDeviceInfo_deviceFromSystemProfiler();

    if (fromSystemProfiler != nil && ![fromSystemProfiler isEqualToString:@""]) {
        return fromSystemProfiler;
    }

    size_t length = 0;
    sysctlbyname("hw.model", nil, &length, nil, 0);

    if (length > 0) {
        char* bytes = malloc(length);
        sysctlbyname("hw.model", bytes, &length, nil, 0);
        NSString* model = [NSString stringWithCString:bytes encoding:NSUTF8StringEncoding];
        free(bytes);

        NSString* parsed = KNDeviceInfo_fromIdentifier(model);

        if (parsed != nil && ![parsed isEqualToString:@""]) {
            return parsed;
        }
    }

    return @"";
}

const char* KNDeviceInfo_getCurrentHostName() {
    NSHost* currentHost = [NSHost currentHost];
    NSString* localizedName = [currentHost localizedName];

    // Replace special apostrophe with default
    // Michael’s Mac Pro -> Michael's Mac Pro
    localizedName = [localizedName stringByReplacingOccurrencesOfString:@"’" withString:@"'"];

    return [localizedName UTF8String];
}

const char* KNDeviceInfo_getModelName() {
    NSString* model = KNDeviceInfo_deviceModelPretty();
    return [model UTF8String];
}

#endif // OS_MAC
