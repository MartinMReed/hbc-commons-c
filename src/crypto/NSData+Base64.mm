/**
 * Copyright (c) 2010-2012 Martin M Reed
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#import "base64.h"
#import <Foundation/Foundation.h>
#import "NSData+Base64.h"

@implementation NSData (Base64)

+ (NSData *)base64EncodedWithString:(NSString *)string {
	return [[[NSData alloc] initBase64EncodedWithString:string] autorelease];
}

- (id)initBase64EncodedWithString:(NSString *)string {
	
	if (!(self = [super init])) {
		return self;
	}
	
	if (!string) {
		return self;
	}
	
	NSData *decodedData = [string dataUsingEncoding:NSUTF8StringEncoding];
	const unsigned char *decoded = (const unsigned char *)[decodedData bytes];
	int decodedLength = [decodedData length];
	int encodedLength = hbc::encoded_length(decodedLength);
	char *encoded = (char *)malloc(sizeof(char)*encodedLength);
	hbc::encode(decoded, encoded, decodedLength);
	
	NSData *encodedData = [NSData dataWithBytes:encoded length:encodedLength];
	free(encoded);
 	
	self = [self initWithData:encodedData];
	return self;
}

@end