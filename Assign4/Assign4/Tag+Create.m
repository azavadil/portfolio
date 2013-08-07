//
//  Tag+Create.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/20/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "Tag+Create.h"

@implementation Tag (Create)







+ (Tag *)tagWithName:(NSString *)name inManagedObjectContect:(NSManagedObjectContext *)context; 
{
    NSLog(@"Tag+Create - name = %@", name); 
    Tag *tag = nil; 
    
    
    /* note that we only want to store unique tags
     * therefore we query the database to see if the photo exists
     */ 
    
    // note that @Tag matches the Tag entity we created 
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Tag"]; 
    
    
    request.predicate = [NSPredicate predicateWithFormat:@"tagName = %@", name];   
    NSSortDescriptor *sortDescriptor = [NSSortDescriptor sortDescriptorWithKey:@"numberOfViews" ascending:YES]; 
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
        NSLog(@"Tag+Create - tagWithName - count = 0 conditional"); 
        tag = [NSEntityDescription insertNewObjectForEntityForName:@"Tag" inManagedObjectContext:context]; 
        tag.tagName  = name; 
        tag.numberOfViews = [NSNumber numberWithInt:1]; 
    }
    else if([matches count] == 1)
    {
        NSLog(@"Tag+Create - tagWithName - count = 1 conditional"); 
        tag = [matches lastObject]; 
    }
    return tag; 
    
}


@end
