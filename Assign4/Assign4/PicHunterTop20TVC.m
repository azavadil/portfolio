//
//  PicHunterTop20TVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "PicHunterTop20TVC.h"
#import "FlickrFetcher.h"
#import "TopPlacePhotoVC.h" 
#import "PhotoAnnotation.h"
#import "MapVC.h"

@interface PicHunterTop20TVC()

@end


@implementation PicHunterTop20TVC 




#define MOST_RECENT @"20_MostRecentPhotos"

#pragma mark - View lifecycle



/**
 * Instance method: viewDidLoad
 * ----------------------------
 * viewDidLoad ensures that the listOfPhotos is set whenever the view loads 
 */

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.listOfPhotos = [[NSUserDefaults standardUserDefaults] objectForKey:MOST_RECENT]; 
}




- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - Table view data source



/**
 * Instance method: tableView-numberOfRowsInSection
 * ------------------------------------------------
 * tableView-numberOfRowsInSection returns the number of rows in a section. In this
 * case its easy: the number of entries in the photoDictionaries
 */

// implemented in parent PlacePhotosTVC


/**
 * Instance method: tableView-cellForRowAtIndexPath
 * ------------------------------------------------
 * tableView-cellForRowAtIndexPath overrides the parent method. The method initializes 
 * the cells for the PicHunterTop20TVC. 
 * 
 * For the PicHunterTop20TVC the main heading is the photo title and the subheading
 * is the photo description. If the photo doesn't have a title we set the main
 * heading to be "Unknown". 
 */

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Recently Viewed Photo";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
    }
    
    // Configure the cell...
    NSDictionary *photoDict = [self.listOfPhotos objectAtIndex:indexPath.row]; 
    NSString *photoTitle = [photoDict objectForKey:FLICKR_PHOTO_TITLE];
    NSString *photoDescription = [photoDict objectForKey:FLICKR_PHOTO_DESCRIPTION];
    
    if(photoTitle && ![photoTitle isEqualToString:@""]) 
    {
        cell.textLabel.text = photoTitle;    
    } 
    else 
    {
        if(photoDescription && ![photoDescription isEqualToString:@""]) 
        {
            cell.textLabel.text = photoDescription;
        }
        else
        {
            cell.textLabel.text = @"Unknown"; 
        }
    }
    
    cell.detailTextLabel.text = photoDescription; 
    return cell;
    
}



/**
 * Instance method: prepareForSegue
 * --------------------------------
 * Override the parent class prepareForSegue method
 */

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if([segue.identifier isEqualToString:@"Show Recent Photo"])
    {
        
        NSDictionary *photoDict = [self.listOfPhotos objectAtIndex:self.tableView.indexPathForSelectedRow.row];
        [segue.destinationViewController setPhotoDictionary:photoDict];
    }
    if([segue.identifier isEqualToString:@"Show recentPhotos Map"])
    {
        MapVC *destinationMapVC = segue.destinationViewController; 
        destinationMapVC.delegate = self; 
        [segue.destinationViewController setAnnotations:[self mapAnnotations]]; 
    }
    
}


/** 
 * Instance method: mapAnnotations
 * -------------------------------
 * mapAnnotations returns an NSArray of mapAnnotations. When we segue to a map displaying
 * all the photos on map, we set the annotations of the successor view controller to the
 * annotations returned by mapAnnotations. 
 */

// implemented in parent PlacePhotoTVC


/**
 * Instance method: provideImageToMapVC 
 * ------------------------------------
 * PlacePhotosTVC is the datasource for a mapView controller (i.e. PicHunterTop20TVC gets images
 * from Flickr on the mapView controllers behalf). provideImageToMapVC gets an image on behalf
 * of the mapView (the mapView subsequently passes the image to an annotation
 */

// implemented in parent PlacePhotoTVC





#pragma mark - Table view delegate


@end
