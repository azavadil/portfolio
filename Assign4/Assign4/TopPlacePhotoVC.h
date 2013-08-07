//
//  TopPlacePhotoVC.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/10/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Photo.h"

@interface TopPlacePhotoVC : UIViewController

@property (nonatomic, strong) NSDictionary *photoDictionary;
@property (nonatomic, strong) Photo *photo; 
@property (nonatomic, strong) NSString *vacationName; 

@property (nonatomic, strong) NSFileManager *fileManager; 


@end
