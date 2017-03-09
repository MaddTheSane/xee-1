#import "XeeImageSource.h"

@class XeeListEntry;

@interface XeeListSource:XeeImageSource
{
	NSMutableArray *entries;
	NSRecursiveLock *listlock,*loadlock;
	NSArray *types;

	XeeListEntry *currentry,*nextentry,*preventry;
	NSInteger changes,oldindex;

	BOOL loader_running,exiting;
	XeeImage *loadingimage;
}

-(id)init;
-(void)dealloc;

-(void)stop;

@property (readonly) NSInteger numberOfImages;
@property (readonly) NSInteger indexOfCurrentImage;
-(NSString *)descriptiveNameOfCurrentImage;

-(void)pickImageAtIndex:(NSInteger)index next:(NSInteger)next;
-(void)pickImageAtIndex:(NSInteger)index;

-(void)startListUpdates;
-(void)endListUpdates;

-(void)addEntry:(XeeListEntry *)entry;
-(void)addEntryUnlessExists:(XeeListEntry *)entry;
-(void)removeEntry:(XeeListEntry *)entry;
-(void)removeEntryMatchingObject:(id)obj;
-(void)removeAllEntries;

-(void)setCurrentEntry:(XeeListEntry *)entry;
-(void)setPreviousEntry:(XeeListEntry *)entry;
-(void)setNextEntry:(XeeListEntry *)entry;

-(void)launchLoader;
-(void)loader;

@end



@interface XeeListEntry:NSObject <NSCopying>
{
	XeeImage *savedimage;
	int imageretain;
}

-(id)init;
-(id)initAsCopyOf:(XeeListEntry *)other;
-(void)dealloc;

-(NSString *)descriptiveName;
-(BOOL)matchesObject:(id)obj;

-(void)retainImage;
-(void)releaseImage;
-(XeeImage *)image;
-(XeeImage *)produceImage;

-(BOOL)isEqual:(id)other;
-(unsigned long)hash;

-(id)copyWithZone:(NSZone *)zone;

@end
