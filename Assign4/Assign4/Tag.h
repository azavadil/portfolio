//
//  Tag.h
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
 * Tag is a core data object and is used to represent a tag
 */ 



@class Photo;

@interface Tag : NSManagedObject

@property (nonatomic, retain) NSNumber * numberOfViews;
@property (nonatomic, retain) NSString * tagName;
@property (nonatomic, retain) NSSet *photos;
@end

@interface Tag (CoreDataGeneratedAccessors)

- (void)addPhotosObject:(Photo *)value;
- (void)removePhotosObject:(Photo *)value;
- (void)addPhotos:(NSSet *)values;
- (void)removePhotos:(NSSet *)values;
@end
