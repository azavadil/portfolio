//
//  PlacePhotosTVC.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MapVC.h"



@interface PlacePhotosTVC : UITableViewController <MapVCImageSource> 


//simple model, array of photo dictionaries 
@property (nonatomic, strong) NSArray *listOfPhotos; 

- (NSArray *)mapAnnotations;

@end
