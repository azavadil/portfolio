//
//  PhotosForPlaceTVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "PhotosForPlaceTVC.h"
#import "OpenVacationHelper.h"
#import "Photo.h"

@interface PhotosForPlaceTVC()

- (void) setupFetchedResultsController:(UIManagedDocument *)vacation; 
- (void) openDatabase; 

@end




@implementation PhotosForPlaceTVC

@synthesize place = _place; 
@synthesize vacationName = _vacationName; 



/**
 * Instance method: viewDidLoad
 * ----------------------------
 * viewDidLoad is where we do any initial setup. Here we call opendatabase
 */

- (void)viewDidLoad
{
    [super viewDidLoad]; 
    [self openDatabase]; 
}



/**
 * Instance method: setupFetchedResultsController
 * ----------------------------------------------
 * setupFetchedResultsController is passed to the OpenVacationHelper by the openDatabase method. 
 * Because creating/opening documents is done asynchronously, we need a method that we can call 
 * inside a block to be completed (i.e. setupFetchedResultsController is the code that gets put
 * in the block). 
 */

- (void) setupFetchedResultsController:(UIManagedDocument *)vacation
{
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Photo"]; 
    request.predicate = [NSPredicate predicateWithFormat:@"fromPlace.placeName = %@", self.place.placeName]; 
    request.sortDescriptors = [NSArray arrayWithObject:
                               [NSSortDescriptor sortDescriptorWithKey:@"fromPlace.firstVisited" ascending:YES]]; 
    self.fetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:request 
                                                                        managedObjectContext:vacation.managedObjectContext
                                                                          sectionNameKeyPath:nil 
                                                                                   cacheName:nil]; 
}



/** 
 * Instance method: openDatabase 
 * -----------------------------
 * works with setupFetchedResultsController to connect the TVC to a shared managedDocument
 */ 

- (void) openDatabase 
{
    [OpenVacationHelper openVacation:self.vacationName usingBlock:^(UIManagedDocument *vacation){
        [self setupFetchedResultsController:vacation];}];
}





- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return YES;
}



/**
 * Instance method: tableView-cellForRowAtIndexPath
 * ------------------------------------------------
 * tableView-cellForRowAtIndexPath specifies the prototype for a table view cell. 
 */

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"PhotoForPlace Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    // Configure the cell...
    Photo *photo = [self.fetchedResultsController objectAtIndexPath:indexPath]; 
    cell.textLabel.text = photo.title; 
    cell.detailTextLabel.text = photo.subtitle; 
    
    return cell;
}







/** 
 * Instance method: prepareForSegue
 * --------------------------------
 * note the generic version implementation of prepareForSegue. Method checks that the 
 * destination controller responds to setPlace and setVacationName.
 * prepareForSegue will set the @property photo on the topPlacePhotoVC
 */ 

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    NSIndexPath *indexPath = [self.tableView indexPathForCell:sender]; 
    Photo *photo = [self.fetchedResultsController objectAtIndexPath:indexPath]; 
    
    
    if([segue.destinationViewController respondsToSelector:@selector(setPhoto:)]){
        [segue.destinationViewController performSelector:@selector(setPhoto:) withObject:photo]; 
    }
}


@end
