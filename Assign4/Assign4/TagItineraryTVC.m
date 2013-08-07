//
//  TagItineraryTVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/20/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "TagItineraryTVC.h"
#import "Tag.h"

@implementation TagItineraryTVC





/** 
 * Instance method: setupFetchedResultsController 
 * ----------------------------------------------
 * TagItineraryTVC inherits from PlaceItinearyTVC. we override setupFetchedResultsController so we
 * pull tags instead of places
 */
- (void) setupFetchedResultsController:(UIManagedDocument *)vacation
{
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Tag"]; 
    /* returning all places because we only have 1 vacation. if we had multiple 
     * vacations we would need the predicate
     */ 
    //request.predicate = [NSPredicate predicateWithFormat:@"name = %@", self.vacationName]; 
    
    
    request.sortDescriptors = [NSArray arrayWithObject:[NSSortDescriptor sortDescriptorWithKey:@"numberOfViews" ascending:YES]]; 
    
    self.fetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:request 
                                                                        managedObjectContext:vacation.managedObjectContext
                                                                          sectionNameKeyPath:nil 
                                                                                   cacheName:nil]; 
}



/**
 * Instance method: tableView-cellForRowAtIndexPath
 * ------------------------------------------------
 * tableView-cellForRowAtIndexPath specifies the prototype for a table view cell. 
 */

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"TagItinerary Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    // Configure the cell...
    
    Tag *tag = [self.fetchedResultsController objectAtIndexPath:indexPath]; 
    cell.textLabel.text = tag.tagName; 
    
    return cell;
}



/**
 * Instance method: prepareForSegue 
 * --------------------------------
 * prepareForSegue is a generic implementation of prepareForSegue. 
 * prepareForSegue checks that the destinationViewController responds to 
 * setTag and setVacationName and then sets the tag and the vacation name 
 * on the successor controller
 */ 

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    NSLog(@"TagItineraryTVC - prepareForSegue"); 
    
    NSIndexPath *indexPath = [self.tableView indexPathForCell:sender]; 
    Tag *tag = [self.fetchedResultsController objectAtIndexPath:indexPath]; 
    
    
    if([segue.destinationViewController respondsToSelector:@selector(setTag:)] && 
       [segue.destinationViewController respondsToSelector:@selector(setVacationName:)])
    {
        NSLog(@"TagItineraryTVC - prepareForSegue - conditional"); 
        [segue.destinationViewController performSelector:@selector(setTag:) withObject:tag]; 
        [segue.destinationViewController performSelector:@selector(setVacationName:) withObject:self.vacationName];
    }
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
