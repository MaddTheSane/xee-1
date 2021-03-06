#import "XeeSampleSet.h"

#include <math.h>
#include <tgmath.h>

@interface XeeBestCandidateSamples : XeeSampleSet {
}
- (id)initWithCount:(NSInteger)count;
@end

@interface XeeNRooksSamples : XeeSampleSet {
}
- (id)initWithCount:(NSInteger)count;
@end

#define PI ((CGFloat)(M_PI))

static float XeeBoxFilter(float u, float v)
{
	if (fabs(u) > 0.5 || fabs(v) > 0.5)
		return 0;
	return 1;
}

static float XeeSincFilter(float u, float v)
{
	return sin(2 * u * PI) * sin(2 * v * PI) / (4 * u * v);
}

/*static float XeeWindowedSinc3Filter(float u,float v)
{
	if(fabs(u)>3.0/2.0||fabs(v)>3.0/2.0) return 0;
	return sin(2*u*PI)*sin(2*v*PI)*sin(2*u*PI/3)*sin(2*v*PI/3)/(4*u*u*v*v/9);
}*/

@implementation XeeSampleSet
@synthesize count = num;

- (id)initWithCount:(NSInteger)count
{
	if (self = [super init]) {
		if (!(samples = calloc(count, sizeof(XeeSamplePoint)))) {
			[self release];
			return nil;
		}
		num = count;
	}
	return self;
}

- (void)dealloc
{
	free(samples);
	[super dealloc];
}

- (void)filterWithFunction:(XeeFilterFunction)filter
{
	for (int i = 0; i < num; i++)
		samples[i].weight = filter(samples[i].u, samples[i].v);
}

static int XeeSamplePointSorter(const void *a, const void *b)
{
	return ((XeeSamplePoint *)a)->weight < ((XeeSamplePoint *)b)->weight ? -1 : 1;
}

- (void)sortByWeight
{
	qsort(samples, num, sizeof(XeeSamplePoint), XeeSamplePointSorter);
	//	for(int i=0;i<num;i++) NSLog(@"%f %f %f",samples[i].u,samples[i].v,samples[i].weight);
}

- (XeeSamplePoint *)samples
{
	return samples;
}

+ (XeeSampleSet *)sampleSetWithCount:(NSInteger)count distribution:(NSString *)distname filter:(NSString *)filtername;
{
	static NSMutableDictionary *setdict = nil;
	if (!setdict)
		setdict = [[NSMutableDictionary alloc] init];

	NSString *name = [NSString stringWithFormat:@"%@-%@-%ld", distname, filtername, (long)count];
	XeeSampleSet *set = [setdict objectForKey:name];
	if (!set) {
		if ([distname isEqual:@"bestCandidate"])
			set = [[[XeeBestCandidateSamples alloc] initWithCount:count] autorelease];
		else if ([distname isEqual:@"nRooks"])
			set = [[[XeeNRooksSamples alloc] initWithCount:count] autorelease];
		else
			return nil;

		XeeFilterFunction filter;
		if ([filtername isEqual:@"sinc"])
			filter = XeeSincFilter;
		else if ([filtername isEqual:@"box"])
			filter = XeeBoxFilter;
		else
			return nil;

		[set filterWithFunction:filter];
		[set sortByWeight];

		[setdict setObject:set forKey:name];
	}

	return set;
}

@end

#define drand() ((double)random() / 2147483648.0)

@implementation XeeBestCandidateSamples

- (id)initWithCount:(NSInteger)count
{
	if (self = [super initWithCount:count]) {
		samples[0].u = drand() - 0.5;
		samples[0].v = drand() - 0.5;

		for (NSInteger i = 1; i < num; i++) {
			float maxdist = 0;
			XeeSamplePoint maxpoint = {0};

			for (int j = 0; j < 100; j++) // should adapt the count
			{
				float u = drand() - 0.5;
				float v = drand() - 0.5;
				float mindist = 1;

				for (int k = 0; k < i; k++) {
					XeeSamplePoint curr = samples[k];
					mindist = fmin(mindist, (u - curr.u) * (u - curr.u) + (v - curr.v) * (v - curr.v));
					mindist = fmin(mindist, (u - curr.u + 1) * (u - curr.u + 1) + (v - curr.v) * (v - curr.v));
					mindist = fmin(mindist, (u - curr.u - 1) * (u - curr.u - 1) + (v - curr.v) * (v - curr.v));
					mindist = fmin(mindist, (u - curr.u) * (u - curr.u) + (v - curr.v + 1) * (v - curr.v + 1));
					mindist = fmin(mindist, (u - curr.u) * (u - curr.u) + (v - curr.v - 1) * (v - curr.v - 1));
					mindist = fmin(mindist, (u - curr.u + 1) * (u - curr.u + 1) + (v - curr.v + 1) * (v - curr.v + 1));
					mindist = fmin(mindist, (u - curr.u + 1) * (u - curr.u + 1) + (v - curr.v - 1) * (v - curr.v - 1));
					mindist = fmin(mindist, (u - curr.u - 1) * (u - curr.u - 1) + (v - curr.v + 1) * (v - curr.v + 1));
					mindist = fmin(mindist, (u - curr.u - 1) * (u - curr.u - 1) + (v - curr.v - 1) * (v - curr.v - 1));
				}

				if (mindist > maxdist) {
					maxdist = mindist;
					maxpoint.u = u;
					maxpoint.v = v;
				}
			}

			samples[i] = maxpoint;
		}
	}
	return self;
}

@end

@implementation XeeNRooksSamples

- (id)initWithCount:(NSInteger)count
{
	if (self = [super initWithCount:count]) {
		for (int i = 0; i < num; i++) {
			samples[i].u = (float)i / (float)num + drand() / (float)num - 0.5;
			samples[i].v = (float)i / (float)num + drand() / (float)num - 0.5;
		}

		for (NSInteger i = num - 1; i >= 1; i--) {
			int n = rand() % (i + 1);
			if (n != i) {
				float tmp = samples[i].u;
				samples[i].u = samples[n].u;
				samples[n].u = tmp;
			}
		}
	}
	return self;
}

@end
