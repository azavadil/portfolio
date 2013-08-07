//
//  Photo+CreatePhotoWithFlickrData.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "Photo.h"

/**
 * Implementation note: 
 * --------------------
 * We add a category to the Photo. The category has two methods, one for 
 * creating a photo and one for deleting a photo. We implement the category 
 * to maintain clear deliniation of objects (i.e. objects are responsible for
 * creating and deleting themselves. 
 */ 


@interface Photo (CreatePhotoWithFlickrData)

/* note that we need to pass the MOC
 */ 

+ (Photo *)photoWithFlickrInfo:(NSDictionary *)flickrInfo inManagedObjectContext:(NSManagedObjectContext *)context; 

+ (void)deletePhotoWithFlickrInfo:(NSDictionary *)flickrInfo inManagedObjectContext:(NSManagedObjectContext *)context; 

@end
