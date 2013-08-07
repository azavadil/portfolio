//
//  Place.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

/**
 * Implementation note: 
 * --------------------
 * Place is a core data object and is used to represent a place
 */ 


@class Photo;

@interface Place : NSManagedObject

@property (nonatomic, retain) NSString * placeName;
@property (nonatomic, retain) NSDate * firstVisited;
@property (nonatomic, retain) NSSet *photos;
@end

@interface Place (CoreDataGeneratedAccessors)

- (void)addPhotosObject:(Photo *)value;
- (void)removePhotosObject:(Photo *)value;
- (void)addPhotos:(NSSet *)values;
- (void)removePhotos:(NSSet *)values;
@end
