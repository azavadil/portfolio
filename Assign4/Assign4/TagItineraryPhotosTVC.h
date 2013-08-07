//
//  TagItineraryPhotosTVC.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/20/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Tag.h"
#import "CoreDataTableViewController.h"

@interface TagItineraryPhotosTVC : CoreDataTableViewController

@property (nonatomic, strong) NSString *vacationName; 
@property (nonatomic, strong) Tag *tag; 

@end
