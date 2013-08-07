//
//  MapVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/12/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "MapVC.h"
#import <MapKit/MapKit.h> 
#import "PlaceAnnotation.h"
#import "PhotoAnnotation.h"
#import "FlickrFetcher.h" 
#import "PlacePhotosTVC.h" 
#import "TopPlacePhotoVC.h"

@interface MapVC() <MKMapViewDelegate>   //EMD2 denote we implement protocol
@property (weak, nonatomic) IBOutlet MKMapView *mapView;
@end

@implementation MapVC
@synthesize mapView = _mapView;
@synthesize annotations = _annotations; 
@synthesize delegate = _delegate; 


/**
 * Instance method: sychronizeMapView
 * ----------------------------------
 * setMapView, setAnnoations, updateMapView
 * sychronizeMapView is a convience method so to help keep our view and out model 
 * in sync. Whenever our view and our model are setable, we want to make sure that
 * they are always in synch. We implement that in the setters of both the view and the 
 * model.
 * 
 * This way it doens't matter which order the outlets and the model gets set. 
 *
 * see implementation notes. 
 */ 

-(void)sychronizeMapView
{
    if(self.mapView.annotations) [self.mapView removeAnnotations:self.mapView.annotations]; 
    if(self.annotations) [self.mapView addAnnotations:self.annotations]; 
}



/**
 * Instance method: setMapView
 * ---------------------------
 * setMapView calls synchronizeMapView to set the annotations
 * this keeps the controller annotations and the view annotations synchronized
 */

-(void)setMapView:(MKMapView *)mapView
{
    _mapView = mapView; 
    [self sychronizeMapView]; 
}



/**
 * Instance method: setAnnotations
 * ---------------------------
 * setAnnotations calls synchronizeMapView to set the annotations
 * this keeps the controller annotations and the view annotations synchronized
 */

-(void)setAnnotations:(NSArray *)annotations
{
    _annotations = annotations; 
    [self sychronizeMapView]; 
}



- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

/**
 * Implementation note: establish a mapView delegate EMD1
 */



/**
 * Instance method: viewDidLoad
 * ----------------------------
 * viewDidLoad sets the controller as the mapView's delegate (MKMapView has 
 * a built-in delegate method MKMapViewDelegate) and calls sychronizeMapView 
 * to ensure the controller annotations and the view annotations are sychronized.
 * 
 * All the methods in MKMapViewDelegate are optional
 */

-(void)viewDidLoad
{
    [super viewDidLoad]; 
    
    // mapVC gets involved as mapView delegate
    self.mapView.delegate = self;
    [self sychronizeMapView]; 
}

/**
 * Instance method: mapView-viewForAnnotation
 * ------------------------------------------
 * most important method for providing data to the annotation. 
 * mapView-viewForAnnotation is like cellForRowAtIndexPath. mapView-viewForAnnotation
 * only creates the generic pin (it does not download the image until the pin is actually 
 * selected). 
 *
 * We set MapVC as the delegate for the mapView and then MapVC will provide the 
 * data for the annotation. 
 */ 

- (MKAnnotationView *)mapView:(MKMapView *)mapView viewForAnnotation:(id<MKAnnotation>)annotation
{
    MKAnnotationView *aView = [mapView dequeueReusableAnnotationViewWithIdentifier:@"MapPin"]; 
    if(!aView)
    {
        aView = [[MKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"MapPin"]; 
        aView.canShowCallout = YES;     // we want the callout on the annotation
        // add the image and the disclosure button to the annotation
        aView.leftCalloutAccessoryView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 30, 30)];
        aView.rightCalloutAccessoryView = [UIButton buttonWithType:UIButtonTypeDetailDisclosure];
        
    }
    /** Implemenation note: 
     * --------------------
     * set @property annotation for the case were we deque 
     *  (i.e. if we deque if{} block doesn't execute and annotation hasn't be set)
     */ 
    
    aView.annotation = annotation;
    
    /**
     * Implementation note: 
     * --------------------
     * we cast to UIImageView so we can access the setImage method. Also set the image to 
     * nil so if the pin is reused we don't get a legacy image
     */
    
    [(UIImageView *)aView.leftCalloutAccessoryView setImage:nil]; 
    return aView; 
                                          
}


/**
 * Instance method: mapView-didSelectAnnotationView
 * ------------------------------------------------
 * mapView-didSelectAnnotationView is where we provide the image to the annotation when 
 * if a pin is selected. We want mapView-didSelectAnnotationView to get the actual data 
 * from another class so we add a delegate to mapView-didSelectAnnotationView. We then 
 * pass the buck to the delegate (to provide the image). We make the call to the delegate 
 * asynchronous so our app doesn't get blocked while the delegate queries Flickr.
 */ 

- (void)mapView:(MKMapView *)mapView didSelectAnnotationView:(MKAnnotationView *)selectedPin
{
    
    dispatch_queue_t downloadQueue = dispatch_queue_create("mapView delegate image downloader", NULL); 
    dispatch_async(downloadQueue, ^{     
        UIImage *image = [self.delegate provideImageToMapVC:self imageForAnnotation:selectedPin.annotation]; 
        [(UIImageView *)selectedPin.leftCalloutAccessoryView setImage:image];
    }); 
    
    dispatch_release(downloadQueue);
    
}



/**
 * Instance method: mapView-calloutAccessoryControlTapped
 * ------------------------------------------------------
 * mapView-calloutAccessoryControlTapped is the target for the action of an annotation 
 * disclosure button being tapped. Depending on the type of annotation that was tapped
 * the controller either segues to a list of photos or to the photo itself. 
 */

- (void)mapView:(MKMapView *)mapView 
 annotationView:(MKAnnotationView *)view calloutAccessoryControlTapped:(UIControl *)control {
    
    if ([(UIButton*)control buttonType] == UIButtonTypeDetailDisclosure){
        // Do your thing when the detailDisclosureButton is touched
        if([[self.annotations objectAtIndex:0] isKindOfClass:[PlaceAnnotation class]])
        {
            [self performSegueWithIdentifier:@"Show topPlace Photos From Annotation" sender:self]; 
        }
        else if([[self.annotations objectAtIndex:0] isKindOfClass:[PhotoAnnotation class]])
        {
            [self performSegueWithIdentifier:@"Show Photo From Annotation Origin" sender:self]; 
        }
                
    } 
}



/** 
 * Instance method: setRightBarButtonItemInVCToSpinner
 * ---------------------------------------------------
 * setRightBarButtonItemInVCToSpinner takes a controller and sets the rightBarButton
 * on that controller to be a spinner. 
 * 
 * Implementation note: when we segue from a map to a list of topPlaces we have a 2-step
 * dance. In prepareForSegue we immediately use the setRightBarButtonItemInVCToSpinner 
 * method to set the rightBarButton on the successor controller to a spinner. The 2nd 
 * step (still in prepare for segue), we make an asynchronous call that gets both the photos
 * for the place and sets the rightBarButton to a button titled "map" and the selector
 * showMap
 */

- (void)setRightBarButtonItemInVCToSpinner:(UIViewController *)aViewController
{
    
    UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray]; 
    [spinner startAnimating]; 
    
    aViewController.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:spinner]; 
}



/**
 * Instance method: prepareForSegue 
 * --------------------------------
 * prepareForSegue prepares for a segue to either a list of topPlace photos or to a specific photo
 */
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // 'From Annotation' is a reminder that the origin of this action was an annotation
    if([segue.identifier isEqualToString:@"Show topPlace Photos From Annotation"])
        
    {
        
        // we immediately set the rightBarButton on the successor to a spinner
        UIViewController *destinationVC = (UIViewController *)segue.destinationViewController; 
        [self setRightBarButtonItemInVCToSpinner:destinationVC];         
        
        /* there should always be only one annotation selected at a time 
         * to be defensive we pull out the first annotation
         */
        PlaceAnnotation *annotation = [[self.mapView selectedAnnotations] objectAtIndex:0];
        
        /* FlickrFetcher needs a dictionary for a topPlace 
         * For convience we store the dictionary that represents the place with the annotation
         * therefore it's simple to get the dictionary that represents the place 
         */ 
        
        NSDictionary *placeDict = annotation.place;
        
        /**
         * Implemenation note: 
         * -------------------
         * We do two things here. Get the photo data from Flickr and set the data in the successor controller
         * and finish our setting of the rightBarButton
         */ 
        
        dispatch_queue_t download_queue = dispatch_queue_create("topPlacePhotos downloader annotation", NULL); 
        dispatch_async(download_queue, ^{
            NSArray *photoDictionaries = [FlickrFetcher photosInPlace:placeDict maxResults:50];
            dispatch_async(dispatch_get_main_queue(), ^{ 
                UIBarButtonItem *rightBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Map" style:UIBarButtonItemStylePlain target:destinationVC action:@selector(showMap:)];
                destinationVC.navigationItem.rightBarButtonItem = rightBarButtonItem; 
                [segue.destinationViewController setListOfPhotos:photoDictionaries];
            }); 
        }); 
        dispatch_release(download_queue); 
        
    }
    if([segue.identifier isEqualToString:@"Show Photo From Annotation Origin"])
    {
        /* the imageViewVC takes care of downloading the photo
         * all we need to do here is set the photoDictionary @property in the destinationVC
         */ 
        PhotoAnnotation *annotation = [[self.mapView selectedAnnotations] objectAtIndex:0];
        [segue.destinationViewController setPhotoDictionary:annotation.photo];
    }
    
}



- (void)viewDidUnload
{
    [self setMapView:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return YES;
}

@end
