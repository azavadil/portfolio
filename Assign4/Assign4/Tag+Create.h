//
//  Tag+Create.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/20/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "Tag.h"

@interface Tag (Create)

+ (Tag *)tagWithName:(NSString *)name inManagedObjectContect:(NSManagedObjectContext *)context;

@end
