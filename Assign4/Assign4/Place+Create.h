//
//  Place+Create.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "Place.h"

/**
 * Implementation note: 
 * --------------------
 * We add a category to the Place. The category has a single method for 
 * creating a photo. We implement the category to maintain clear deliniation
 * of objects (i.e. objects are responsible for creating and deleting themselves). 
 */ 


@interface Place (Create)

+ (Place *)placeWithName:(NSString *)name inManagedObjectContect:(NSManagedObjectContext *)context;

@end
