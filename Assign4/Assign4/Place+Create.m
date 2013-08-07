//
//  Place+Create.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "Place+Create.h"

@implementation Place (Create)


/**
 * Class method: placeWithName
 * ----------------------------
 * placeWithName takes a string and a NSManagedObjectContext. placeWithName validates
 * that an object doesn't already exist in the database, if the object doesn't exist
 * adds the object to the database, and returns the object to the client
 */

+ (Place *)placeWithName:(NSString *)name inManagedObjectContect:(NSManagedObjectContext *)context; 
{
    NSLog(@"Place+Create - name = %@", name); 
    Place *place = nil; 
    
    
    /* note that we only want to store unique places
     * therefore we query the database to see if the photo exists
     */ 
    
    // note that @Place matches the Place entity we created 
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Place"]; 
    
    
    request.predicate = [NSPredicate predicateWithFormat:@"placeName = %@", name];   
    NSSortDescriptor *sortDescriptor = [NSSortDescriptor sortDescriptorWithKey:@"firstVisited" ascending:YES]; 
    request.sortDescriptors = [NSArray arrayWithObject:sortDescriptor]; 
    
    
    // this code executes the query/request
    NSError *error = nil;
    NSArray *matches = [context executeFetchRequest:request error:&error]; 
    
    if(!matches || [matches count] > 1) 
    {
        // handle error
    }
    else if([matches count] == 0)
    {
        NSLog(@"Place+Create - placeWithName - count = 0 conditional"); 
        place = [NSEntityDescription insertNewObjectForEntityForName:@"Place" inManagedObjectContext:context]; 
        place.placeName  = name; 
        place.firstVisited = [NSDate date]; 
    }
    else if([matches count] == 1)
    {
        NSLog(@"Place+Create - placeWithName - count = 1 conditional"); 
        place = [matches lastObject]; 
    }
    return place; 

}

@end
