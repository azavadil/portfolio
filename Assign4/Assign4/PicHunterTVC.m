//
//  PicHunterTVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/7/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "PicHunterTVC.h"
#import "FlickrFetcher.h"
#import "PlacePhotosTVC.h" 
#import "MapVC.h"
#import "PlaceAnnotation.h"

@interface PicHunterTVC()
- (NSArray *)mapAnnotations; 
- (void)setupVacationsArray;
@end


@implementation PicHunterTVC


@synthesize topPlaces = _topPlaces; 



/** 
 * Instance method: viewDidLoad
 * ----------------------------
 * self-documenting implementation
 */

- (void)viewDidLoad
{
    [super viewDidLoad]; 
    [self setupVacationsArray]; 
}



/** 
 * Instance method: refresh
 * ------------------------
 * refresh is the target for the action of refresh button being pressed on the home
 * screen of the picHunter app. Refresh queries Flickr for the topPlaces and 
 * displays them in a tableViewController
 */ 

- (IBAction)refresh:(id)sender {
    
    UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray]; 
    [spinner startAnimating]; 
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:spinner]; 

    
    dispatch_queue_t download_queue = dispatch_queue_create("topPlaces downloader", NULL); 
    dispatch_async(download_queue, ^{
        NSArray *topPlaces = [FlickrFetcher topPlaces]; 
        dispatch_async(dispatch_get_main_queue(), ^{   
            self.topPlaces = topPlaces;
            self.navigationItem.leftBarButtonItem = sender; 
        });
    }); 
    
    dispatch_release(download_queue);
}



/**
 * Instance method: showMap
 * ------------------------
 * showMap is the target for the action of the 'map' button being pressed on the 
 * home screen of the picHunter app. showMap calls for the segue 
 * "Show topPlaces Map" to be peformed
 */ 

- (IBAction)showMap:(id)sender {
    
    [self performSegueWithIdentifier:@"Show topPlaces Map" sender:self]; 
}



/**
 * Instance method: setTopPlaces
 * -----------------------------
 * setTopPlaces is the setter for the _topPlaces instance variable. 
 * We require the table to reload each time that topPlaces is set
 */

- (void) setTopPlaces:(NSArray *)topPlaces
{
    // defensive: validate that topPlaces has changed before setting
    if (topPlaces != _topPlaces) 
    {
        _topPlaces = topPlaces; 
        if (self.tableView.window) [self.tableView reloadData]; 
    }
}



/**
 * Instance method: prepareForSegue
 * --------------------------------
 * prepareForSegue prepares for the specified segue
 */

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    /**
     * Implementation note:
     * --------------------
     * we have the picHunterTVC set the 'map' button on the successor view. As part of the implementation 
     * we set the selector for the 'map' button to showMap with is one of the PlacePhotosTVC's instance methods
     */
    
    if([segue.identifier isEqualToString:@"Show List of Photos"])
    {
        
        UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray]; 
        [spinner startAnimating]; 
        
        UIViewController *destinationVC = (UIViewController *)segue.destinationViewController; 
        destinationVC.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:spinner]; 
        
        
        NSDictionary *placeDict = [self.topPlaces objectAtIndex:self.tableView.indexPathForSelectedRow.row];
        
        dispatch_queue_t download_queue = dispatch_queue_create("topPlacePhotos downloader", NULL); 
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
    /**
     * Implementation note:
     * --------------------
     * we use the method mapAnnotations to set the mapAnnotations on the successor view controller. 
     */
    if([segue.identifier isEqualToString:@"Show topPlaces Map"])
    {
        NSLog(@"preparing for map = %d", [[self mapAnnotations] count]); 
        [segue.destinationViewController setAnnotations:[self mapAnnotations]]; 
    }
}



/** 
 * Instance method: mapAnnotations
 * -------------------------------
 * mapAnnotations returns an NSArray of mapAnnotations. When we segue to a map displaying
 * all of the places, we set the annotations of the successor view controller to the
 * annotations returned by mapAnnotations. 
 */
 
- (NSArray *)mapAnnotations
{
    
    NSMutableArray *annotations = [NSMutableArray arrayWithCapacity:[self.topPlaces count]]; 
    for (NSDictionary *topPlace in self.topPlaces)
    {
        [annotations addObject:[PlaceAnnotation annotationForPlace:topPlace]]; 
    }
    return annotations; 
}



/** 
 * Instance method: shouldAutorotateToInterfaceOrientation
 * -------------------------------------------------------
 * shouldAutorotateToInterfaceOrientation returns YES for the orientations we 
 * want to rotate to
 */

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - Table view data source


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{

    // Return the number of rows in the section.
    return [self.topPlaces count]; 
}



/**
 * Instance method: tableView-cellForRowAtIndexPath
 * ------------------------------------------------
 * tableView-cellForRowAtIndexPath allocates and initializes the cells for the 
 * PicHunterTVC. 
 * 
 * For the PicHunterTVC the main heading is the city and the subheading
 * is the state, country
 */

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Top Place Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:CellIdentifier];
    }
    
    // Configure the cell...
    NSDictionary *topPlacesEntry = [self.topPlaces objectAtIndex:indexPath.row]; 
    NSString *placeName = [topPlacesEntry objectForKey:FLICKR_PLACE_NAME]; 
        
    // parse the data from Flickr. The data is comma delimited.
    NSMutableArray *tokens = [[placeName componentsSeparatedByString:@","] mutableCopy]; 
    cell.textLabel.text = [tokens objectAtIndex:0];
    NSRange range = NSMakeRange(1, [tokens count] -1 );
    
    //strip off leading whitespace
    [tokens replaceObjectAtIndex:1 withObject:[[tokens objectAtIndex:1] substringFromIndex:1]]; 
 
    cell.detailTextLabel.text = [[tokens objectsAtIndexes:[NSIndexSet indexSetWithIndexesInRange:range]] componentsJoinedByString:@", "]; 

    return cell;
}



#pragma mark - Table view delegate



- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Navigation logic may go here. Create and push another view controller.
    /*
     <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
     [self.navigationController pushViewController:detailViewController animated:YES];
     */
}




/** 
 * Instance method: makeVacationsArrayURL
 * --------------------------------------
 * makeVacationsArrayURL is a helper method to generate the NSDocumentsDirectory URL.
 * the path we create is /NSDocumentDirectory/ListOfVacations
 */ 

- (NSURL *)makeVacationsArrayURL
{
    
    // NSDocumentDirectory/savedVacations   
    NSURL *url = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject]; 
    url = [url URLByAppendingPathComponent:@"ListOfVacations"]; 
    return url; 
}



/**
 * Instance method: setupVacationsArray 
 * ------------------------------------
 * setupVacationsArray is called if we reach 
 * viewDidLoad and self.vacations hasn't been 
 * setup. 
 */ 

- (void)setupVacationsArray
{
    
    // NSDocumentDirectory/ListOfVacations
    NSURL *url = [self makeVacationsArrayURL]; 
    
    if(![[NSFileManager defaultManager] fileExistsAtPath:[url path]])
    {
        NSArray *listingOfVacations = [NSArray arrayWithObject:@"myVacation"]; 
        [listingOfVacations writeToURL:url atomically:YES]; 
    }

}


@end
