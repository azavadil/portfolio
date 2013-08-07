//
//  PhotoAnnotation.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/12/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "PhotoAnnotation.h"
#import "FlickrFetcher.h"

@implementation PhotoAnnotation

@synthesize photo = _photo; 




/**
 * Class method: annotationForPhoto
 * --------------------------------
 * annotationForPhoto takes an NSDictionary of photo data and returns a 
 * a PhotoAnnotation. 
 */

+ (PhotoAnnotation *)annotationForPhoto:(NSDictionary *)photo
{
    PhotoAnnotation *annotation = [[PhotoAnnotation alloc] init]; 
    annotation.photo = photo; 
    return annotation; 
}



/* MKAnnotation methods: title, subtitle, coordinate */ 



/**
 * Instance method: title
 * ----------------------
 * title returns the title of a photo
 */ 

- (NSString *)title
{
    return [self.photo objectForKey:FLICKR_PHOTO_TITLE]; 
}



/**
 * Instance method: subtitle
 * ----------------------
 * subtitle returns the description of a photo
 */ 

- (NSString *)subtitle
{
    return [self.photo valueForKeyPath:FLICKR_PHOTO_DESCRIPTION]; 
}



/**
 * Instance method: coordinate
 * ----------------------
 * coordinate returns the coordinate of a photo
 */ 

- (CLLocationCoordinate2D)coordinate
{
    CLLocationCoordinate2D coordinate; 
    coordinate.latitude = [[self.photo objectForKey:FLICKR_LATITUDE] doubleValue]; 
    coordinate.longitude = [[self.photo objectForKey:FLICKR_LONGITUDE] doubleValue]; 
    return coordinate; 
}

@end

