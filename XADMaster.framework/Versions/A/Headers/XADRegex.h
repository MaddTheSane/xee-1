#import <Foundation/Foundation.h>

#ifdef _WIN32
#import "regex.h"
#else
#import <regex.h>
#endif

@interface XADRegex:NSObject
{
	NSString *patternstring;
	regex_t preg;
	regmatch_t *matches;
	NSRange matchrange;
	NSData *currdata;
}

+(XADRegex *)regexWithPattern:(NSString *)pattern options:(int)options;
+(XADRegex *)regexWithPattern:(NSString *)pattern;

+(NSString *)patternForLiteralString:(NSString *)string;
+(NSString *)patternForGlob:(NSString *)glob;

+(NSString *)null;

-(instancetype)init UNAVAILABLE_ATTRIBUTE;
-(instancetype)initWithPattern:(NSString *)pattern options:(int)options NS_DESIGNATED_INITIALIZER;

-(void)beginMatchingString:(NSString *)string;
//-(void)beginMatchingString:(NSString *)string range:(NSRange)range;
-(void)beginMatchingData:(NSData *)data;
-(void)beginMatchingData:(NSData *)data range:(NSRange)range;
-(void)finishMatching;
@property (NS_NONATOMIC_IOSONLY, readonly) BOOL matchNext;
-(NSString *)stringForMatch:(int)n;
@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSArray<NSString*> *allMatches;

-(BOOL)matchesString:(NSString *)string;
-(NSString *)matchedSubstringOfString:(NSString *)string;
-(NSArray *)capturedSubstringsOfString:(NSString *)string;
-(NSArray *)allMatchedSubstringsOfString:(NSString *)string;
-(NSArray *)allCapturedSubstringsOfString:(NSString *)string;
-(NSArray *)componentsOfSeparatedString:(NSString *)string;

/*
-(NSString *)expandReplacementString:(NSString *)replacement;
*/

@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *pattern;
@property (readonly, copy) NSString *description;

@end

@interface NSString (XADRegex)

-(BOOL)matchedByPattern:(NSString *)pattern;
-(BOOL)matchedByPattern:(NSString *)pattern options:(int)options;

-(NSString *)substringMatchedByPattern:(NSString *)pattern;
-(NSString *)substringMatchedByPattern:(NSString *)pattern options:(int)options;

-(NSArray<NSString*> *)substringsCapturedByPattern:(NSString *)pattern;
-(NSArray<NSString*> *)substringsCapturedByPattern:(NSString *)pattern options:(int)options;

-(NSArray<NSString*> *)allSubstringsMatchedByPattern:(NSString *)pattern;
-(NSArray<NSString*> *)allSubstringsMatchedByPattern:(NSString *)pattern options:(int)options;

-(NSArray<NSString*> *)allSubstringsCapturedByPattern:(NSString *)pattern;
-(NSArray<NSString*> *)allSubstringsCapturedByPattern:(NSString *)pattern options:(int)options;

-(NSArray<NSString*> *)componentsSeparatedByPattern:(NSString *)pattern;
-(NSArray<NSString*> *)componentsSeparatedByPattern:(NSString *)pattern options:(int)options;

@property (NS_NONATOMIC_IOSONLY, readonly, copy) NSString *escapedPattern;

@end

/*@interface NSMutableString (XADRegex)

-(void)replacePattern:(NSString *)pattern with:(NSString *)replacement;
-(void)replacePattern:(NSString *)pattern with:(NSString *)replacement options:(int)options;
-(void)replacePattern:(NSString *)pattern usingSelector:(SEL)selector onObject:(id)object;
-(void)replacePattern:(NSString *)pattern usingSelector:(SEL)selector onObject:(id)object options:(int)options;
-(void)replaceEveryPattern:(NSString *)pattern with:(NSString *)replacement;
-(void)replaceEveryPattern:(NSString *)pattern with:(NSString *)replacement options:(int)options;
-(void)replaceEveryPattern:(NSString *)pattern usingSelector:(SEL)selector onObject:(id)object;
-(void)replaceEveryPattern:(NSString *)pattern usingSelector:(SEL)selector onObject:(id)object options:(int)options;

@end*/
