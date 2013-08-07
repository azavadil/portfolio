//
//  PlaceAnnotation.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/13/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>

/** 
 * Implementation note: 
 * --------------------
 * The PlaceAnnotation is part of the controller. The PlaceAnnotation is like a bridge 
 * between the model and mapView. Provides a way to tranlate the model in a way the view
 * wants to understand the model. 
 */ 

@interface PlaceAnnotation : NSObject <MKAnnotation> 

+ (PlaceAnnotation *)annotationForPlace:(NSDictionary *)place; 

@property (nonatomic, strong) NSDictionary *place; 
@property (nonatomic, strong) NSString *placeTitle; 
@property (nonatomic, strong) NSString *placeSubtitle; 

@end

