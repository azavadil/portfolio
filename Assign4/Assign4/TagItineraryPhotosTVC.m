//
//  TagItineraryPhotosTVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/20/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "TagItineraryPhotosTVC.h"
#import "OpenVacationHelper.h"
#import "Photo.h"

@interface TagItineraryPhotosTVC()

- (void) openDatabase; 
- (void) setupFetchedResultsController:(UIManagedDocument *)vacation; 


@end


@implementation TagItineraryPhotosTVC



@synthesize vacationName = _vacationName; 
@synthesize tag = _tag; 





/**
 * Instance method: viewDidLoad
 * ----------------------------
 * viewDidLoad is where we do any initial setup. viewDidLoad makes a call to 
 * openDatabase which in turn sets up the fetchedResultsController 
 */ 
- (void)viewDidLoad
{
    NSLog(@"TagItinearyPhotosTVC"); 
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
    //request.predicate = [NSPredicate predicateWithFormat:@"tags.tagName = %@", self.tag.tagName]; 
    request.sortDescriptors = [NSArray arrayWithObject:
                               [NSSortDescriptor sortDescriptorWithKey:@"title" ascending:YES]]; 
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



/**
 * Instance method: tableView-cellForRowAtIndexPath
 * ------------------------------------------------
 * tableView-cellForRowAtIndexPath specifies the prototype for a table view cell. 
 */

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"TagItineraryPhoto Cell";
    
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
 * prepareForSegue is a generic version of prepareForSegue. prepareForSegue
 * validates that the destination controller responds to the setPhoto method, and 
 * sets the @property photo on the destination controller (topPlacePhotoVC), 
 */ 

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    NSIndexPath *indexPath = [self.tableView indexPathForCell:sender]; 
    Photo *photo = [self.fetchedResultsController objectAtIndexPath:indexPath]; 
    
    
    if([segue.destinationViewController respondsToSelector:@selector(setPhoto:)]){
        [segue.destinationViewController performSelector:@selector(setPhoto:) withObject:photo]; 
    }
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return YES;
}



@end
