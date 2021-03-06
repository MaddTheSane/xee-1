#import "CSDesktopServices.h"

__unused
static inline uint16_t CSGetUInt16(const uint8_t *b)
{
	return ((uint16_t)b[0] << 8) | (uint16_t)b[1];
}
static inline uint32_t CSGetUInt32(const uint8_t *b)
{
	return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | (uint32_t)b[3];
}
__unused
static inline int16_t CSGetInt16(const uint8_t *b)
{
	return ((int16_t)b[0] << 8) | (int16_t)b[1];
}
static inline int32_t CSGetInt32(const uint8_t *b)
{
	return ((int32_t)b[0] << 24) | ((int32_t)b[1] << 16) | ((int32_t)b[2] << 8) | (int32_t)b[3];
}
static inline uint64_t CSGetUInt64(const uint8_t *b)
{
	return ((uint64_t)b[0] << 56) | ((uint64_t)b[1] << 48) | ((uint64_t)b[2] << 40) | ((uint64_t)b[3] << 32) | ((uint64_t)b[4] << 24) | ((uint64_t)b[5] << 16) | ((uint64_t)b[6] << 8) | (uint64_t)b[7];
}
static inline int64_t CSGetInt64(const uint8_t *b)
{
	return ((int64_t)b[0] << 56) | ((int64_t)b[1] << 48) | ((int64_t)b[2] << 40) | ((int64_t)b[3] << 32) | ((int64_t)b[4] << 24) | ((int64_t)b[5] << 16) | ((int64_t)b[6] << 8) | (int64_t)b[7];
}

#define USEUTCDATETIMESTRUCT

#ifdef USEUTCDATETIMESTRUCT

static inline UTCDateTime CSGetUTCTime(const uint8_t *b)
{
	union utcHi {
		UTCDateTime utcTime;
		uint64_t theInt;
	} utcUnion;
	uint64_t theNum = CSGetUInt64(b);
	utcUnion.theInt = theNum;
	return utcUnion.utcTime;
}

#else

static const NSTimeInterval MacEpoch = -3061152000;
static const NSTimeInterval utcDateTimeBlah = (1 / (NSTimeInterval)65536);

#endif

// See also http://search.cpan.org/~wiml/Mac-Finder-DSStore/DSStoreFormat.pod

NSDictionary *CSParseDSStore(NSString *filename)
{
	NSData *data = [NSData dataWithContentsOfFile:filename];
	if (!data)
		return nil;

	const unsigned char *bytes = [data bytes];
	NSInteger length = [data length];
	if (length < 20)
		return nil; // Too short for the header.

	int ver = CSGetUInt32(bytes);
	if (ver != 1)
		return nil; // Unsupported version (probably).

	OSType type = CSGetUInt32(bytes + 4);
	if (type != 'Bud1')
		return nil; // Unsupported filetype.

	NSMutableDictionary *dict = [NSMutableDictionary dictionary];

	for (int n = 0;; n++) {
		if (length < 24 + n * 4)
			return nil; // Truncated file.

		int offs = CSGetUInt32(bytes + 20 + n * 4);
		if (offs == 0)
			return dict; // No more chunk sections, parsing is done.

		offs &= ~0x0f; // Chunk sections are 16-byte aligned, but the offsets are not for some reason.

		if (length < offs + 12)
			return nil; // Truncated file.

		//int val1=CSGetUInt32(bytes+offs);
		int val2 = CSGetUInt32(bytes + offs + 4);
		int numchunks = CSGetUInt32(bytes + offs + 8);
		int chunk = offs + 12;

		for (int i = 0; i < numchunks; i++) {
			if (val2 & 2)
				chunk += 4; // Extra four-byte value before each chunk.

			if (length < chunk + 4)
				goto end; // Truncated file.

			int namelen = CSGetUInt32(bytes + chunk);
			if (length < chunk + 12 + namelen * 2)
				goto end; // Truncated file.

			NSString *filename = [[[NSString alloc] initWithBytes:bytes + chunk + 4
														   length:namelen * 2
														 encoding:NSUTF16BigEndianStringEncoding] autorelease];
			chunk += 4 + namelen * 2;

			NSString *attrname = [[[NSString alloc] initWithBytes:bytes + chunk
														   length:4
														 encoding:NSISOLatin1StringEncoding] autorelease];
			OSType type = CSGetUInt32(bytes + chunk + 4);
			chunk += 8;

			id value;
			switch (type) {
				case 'bool': // One-byte boolean.
					if (length < chunk + 1)
						goto end; // Truncated file.
					value = [NSNumber numberWithBool:bytes[chunk]];
					chunk += 1;
					break;

				case 'long': // Four-byte long.
				case 'shor': // Shorts seem to be 4 bytes too.
					if (length < chunk + 4)
						goto end; // Truncated file.
					value = @(CSGetInt32(bytes + chunk));
					chunk += 4;
					break;

				case 'blob': // Binary data.
				{
					if (length < chunk + 4)
						goto end; // Truncated file.
					int len = CSGetUInt32(bytes + chunk);
					if (length < chunk + 4 + len)
						continue; // Truncated file.
					value = [NSData dataWithBytes:bytes + chunk + 4 length:len];
					chunk += 4 + len;
				} break;

				case 'ustr': // UTF16BE string
				{
					if (length < chunk + 4)
						goto end; // Truncated file.
					int len = CSGetUInt32(bytes + chunk);
					if (length < chunk + 4 + len * 2)
						goto end; // Truncated file.
					value = [[[NSString alloc] initWithBytes:bytes + chunk + 2
													  length:len * 2
													encoding:NSUTF16BigEndianStringEncoding] autorelease];
					chunk += 4 + len * 2;
				} break;

				case 'comp': // eight-byte integer
					if (length < chunk + 8)
						goto end; // Truncated file.
					value = @(CSGetInt64(bytes + chunk));
					chunk += 8;
					break;

				case 'dutc': // date stamp
				{
					if (length < chunk + 8)
						goto end; // Truncated file.
#ifdef USEUTCDATETIMESTRUCT
					UTCDateTime newDateTime = CSGetUTCTime(bytes + chunk);
					CFAbsoluteTime theTime;
					UCConvertUTCDateTimeToCFAbsoluteTime(&newDateTime, &theTime);
					value = [NSDate dateWithTimeIntervalSinceReferenceDate:theTime];
#else

#endif
					chunk += 8;
				} break;

				case 'type': // OSType
				{
					if (length < chunk + 4)
						goto end; // Truncated file.
					// Hmm, integer or string...
					// Let's go string.
					OSType type2 = CSGetUInt32(bytes + chunk);
					value = CFBridgingRelease(UTCreateStringForOSType(type2));
				}
					chunk += 4;
					break;

				default:
					goto end; // Unknown chunk type, give up.
			}

			NSMutableDictionary *filedict = [dict objectForKey:filename];
			if (!filedict) {
				filedict = [NSMutableDictionary dictionary];
				[dict setObject:filedict forKey:filename];
			}

			[filedict setObject:value forKey:attrname];
		}
	end:;
	}
}
