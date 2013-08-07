//
//  PhotosForPlaceTVC.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Place.h"
#import "CoreDataTableViewController.h"

@interface PhotosForPlaceTVC : CoreDataTableViewController

@property (nonatomic, strong) Place *place; 
@property (nonatomic, strong) NSString *vacationName; 

@end
