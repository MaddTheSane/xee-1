#import "XADArchiveParser.h"

@interface XADArchiveParser (Descriptions)

-(NSString *)descriptionOfValueInDictionary:(NSDictionary<NSString*,id> *)dict key:(NSString *)key;
-(NSString *)descriptionOfKey:(NSString *)key;
-(NSArray<XADArchiveKeys> *)descriptiveOrderingOfKeysInDictionary:(NSDictionary<XADArchiveKeys,id> *)dict;

@end

NSString *XADHumanReadableFileSize(uint64_t size);
NSString *XADShortHumanReadableFileSize(uint64_t size);
NSString *XADHumanReadableBoolean(uint64_t boolean);
NSString *XADHumanReadablePOSIXPermissions(uint64_t permissions);
NSString *XADHumanReadableAmigaProtectionBits(uint64_t protection);
NSString *XADHumanReadableDOSFileAttributes(uint64_t attributes);
NSString *XADHumanReadableWindowsFileAttributes(uint64_t attributes);
NSString *XADHumanReadableOSType(uint32_t ostype);
NSString *XADHumanReadableEntryWithDictionary(NSDictionary<XADArchiveKeys,id> *dict,XADArchiveParser *parser);

NSString *XADHumanReadableObject(id object);
NSString *XADHumanReadableDate(NSDate *date);
NSString *XADHumanReadableData(NSData *data);
NSString *XADHumanReadableArray(NSArray *array);
NSString *XADHumanReadableDictionary(NSDictionary *dict);
NSString *XADHumanReadableList(NSArray<NSString*> *labels,NSArray<NSString*> *values);
NSString *XADIndentTextWithSpaces(NSString *text,NSInteger spaces);
