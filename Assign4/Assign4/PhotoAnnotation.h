//
//  PhotoAnnotation.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/12/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h> 

/** 
 * Implementation note: 
 * --------------------
 * The PhotoAnnotation is part of the controller. The PhotoAnnotation is like a bridge 
 * between the model and mapView. Provides a way to tranlate the model in a way the view
 * wants to understand the model. 
 * 
 * Model = NSArray of annotations
 * View = mapView
 */ 

@interface PhotoAnnotation : NSObject <MKAnnotation> 

+ (PhotoAnnotation *)annotationForPhoto:(NSDictionary *)photo; 

@property (nonatomic, strong) NSDictionary *photo; 

@end
