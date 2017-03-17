#import "CSHandle.h"

#define CSSubHandle XADSubHandle

@interface CSSubHandle:CSHandle
{
	CSHandle *parent;
	off_t start,end;
}

// Initializers
-(instancetype)initWithHandle:(CSHandle *)handle from:(off_t)from length:(off_t)length;
-(instancetype)initAsCopyOf:(CSSubHandle *)other;

// Public methods
@property (readonly, retain) CSHandle *parentHandle;
@property (readonly) off_t startOffsetInParent;

// Implemented by this class
@property (NS_NONATOMIC_IOSONLY, readonly) off_t fileSize;
@property (NS_NONATOMIC_IOSONLY, readonly) off_t offsetInFile;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL atEndOfFile;

-(void)seekToFileOffset:(off_t)offs;
-(void)seekToEndOfFile;
-(int)readAtMost:(int)num toBuffer:(void *)buffer;

@end
