//
//  PlaceItineraryTVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/18/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "PlaceItineraryTVC.h"
#import "OpenVacationHelper.h" 
#import "Place.h"


@interface PlaceItineraryTVC()

- (void) openDatabase; 
- (void)testQuery:(UIManagedDocument *)vacationDatabase; 

@end


@implementation PlaceItineraryTVC


@synthesize vacationName = _vacationName; 



/**
 * Instance method: testPressed
 * ----------------------------
 * deprecated: used for debugging
 */
- (IBAction)testPressed:(id)sender {
    NSLog(@"test: self.vacationName = %@", self.vacationName);
    
    [OpenVacationHelper openVacation:self.vacationName usingBlock:^(UIManagedDocument *vacDoc){
                    [self testQuery:vacDoc]; }];  
    
}



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
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Place"]; 
    /* returning all places because we only have 1 vacation. if we had multiple 
     * vacations we would need the predicate
     */ 
    //request.predicate = [NSPredicate predicateWithFormat:@"name = %@", self.vacationName]; 
    
    
    request.sortDescriptors = [NSArray arrayWithObject:[NSSortDescriptor sortDescriptorWithKey:@"firstVisited" ascending:YES]]; 
    
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
    static NSString *CellIdentifier = @"Itinerary Item Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    // Configure the cell...
    
    Place *place = [self.fetchedResultsController objectAtIndexPath:indexPath]; 
    cell.textLabel.text = place.placeName; 
    
    return cell;
}



/** 
 * Instance method: prepareForSegue
 * --------------------------------
 * note the generic version implementation of prepareForSegue. Method checks that the 
 * destination controller responds to setPlace and setVacationName
 */ 

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    NSIndexPath *indexPath = [self.tableView indexPathForCell:sender]; 
    Place *place = [self.fetchedResultsController objectAtIndexPath:indexPath]; 
    
    
    if([segue.destinationViewController respondsToSelector:@selector(setPlace:)] && 
       [segue.destinationViewController respondsToSelector:@selector(setVacationName:)])
    {
        [segue.destinationViewController performSelector:@selector(setPlace:) withObject:place]; 
        [segue.destinationViewController performSelector:@selector(setVacationName:) withObject:self.vacationName];
    }
}








- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return YES;
}



/**
 * Instance method: testQuery
 * --------------------------
 * testQuery is use for testing only
 */

- (void)testQuery:(UIManagedDocument *)vacationDatabase
{
    
    
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Place"]; 
    
    
    // this code builds the query/request
    NSSortDescriptor *sortDescriptor = [NSSortDescriptor sortDescriptorWithKey:@"placeName" ascending:YES]; 
    request.sortDescriptors = [NSArray arrayWithObject:sortDescriptor]; 
    
    
    // this code executes the query/request
    NSError *error = nil;
    NSArray *matches = [vacationDatabase.managedObjectContext executeFetchRequest:request error:&error]; 
    Place *place = [matches lastObject]; 
    NSLog(@"       "); 
    NSLog(@"TEST QUERY"); 
    NSLog(@"testQuery - matches count = %d", [matches count]);
    NSLog(@"testQuery - matches[-1] = %@", place); 
    NSLog(@"testQuery - matches[-1].placeName = %@", place.placeName); 
    NSLog(@"testQuery - matches[-1].firstVisited = %@", place.firstVisited); 
    
}


@end
