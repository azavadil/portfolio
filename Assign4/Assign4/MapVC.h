//
//  MapVC.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/12/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>

@class MapVC; 

/** 
 * Implementation note: 
 * --------------------
 * The mapView controller needs to provide images to the mapView annoations. We want to keep the 
 * the mapView controller generic (i.e. we don't want the mapView to get images from Flickr, the 
 * controller that is seguing to the photos, PlacePhotosTVC/PicHunterTop20TVC, already does that). 
 * Therefore we create a delegate so the mapView controller can  get data from our main controller. 
 * Specifically, whenever we need to get an image we pass the buck to the delegate to get the image. 
 */

@protocol MapVCImageSource <NSObject>
-(UIImage *)provideImageToMapVC:(MapVC *)sender imageForAnnotation:(id <MKAnnotation>)annotation;
@end

@interface MapVC : UIViewController


/**
 * Implementation note:
 * --------------------
 * convention is to always set the model for a controller. In this case the 
 * model for the controller is the annonations. 
 *
 * The controller will just pass the annotations on to the mapView. 
 * However, since the mapView will not be public, there needs to be 
 * to a public API in the controller so the annotations can be set.
 */ 

@property (nonatomic, strong) NSArray *annotations; 

@property (nonatomic, weak) id<MapVCImageSource> delegate; 

@end
