//
//  VacationSelectorTVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "VacationSelectorTVC.h"

@interface VacationSelectorTVC()

- (NSURL *)makeVacationsArrayURL; 

@end




@implementation VacationSelectorTVC



@synthesize vacations = _vacations; 




/** 
 * Instance method: makeVacationsArrayURL 
 * --------------------------------------
 * makeVacationsArrayURL is a convience method to generate the NSDocumentsDirectory URL
 * path is at: /NSDocumentDirectory/ListOfVacations
 */ 

- (NSURL *)makeVacationsArrayURL
{
    
    // NSDocumentDirectory/savedVacations   
    NSURL *url = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject]; 
    url = [url URLByAppendingPathComponent:@"ListOfVacations"]; 
    return url; 
}



/** 
 * Instance method: setVacations
 * -----------------------------
 * setVacations sets the instance variable _vacations. We also write to the 
 * NSDocumentsDirectory in the setter to keep the TVC and the document synchronized
 */ 


- (void)setVacations:(NSArray *)vacations
{
    if(_vacations != vacations)
    {
        _vacations = vacations; 
        [vacations writeToURL:[self makeVacationsArrayURL] atomically:YES]; 
    }
}



/**
 * Instance method: viewDidLoad
 * ----------------------------
 * viewDidLoad initializes setup that needs to be completed when the view goes 
 * onscreen. If we reach viewDidLoad and self.vacations hasn't been set up, 
 * then we need to setup self.vacations. 
 */ 

-(void)viewDidLoad
{
    [super viewDidLoad]; 
    
    if(!self.vacations)
    {
        self.vacations = [[NSArray alloc] initWithContentsOfURL:[self makeVacationsArrayURL]];  
    }
}



/**
 * Instance method: tableView-numberOfRowsInSection
 * ------------------------------------------------
 * tableView-numberOfRowsInSection returns an NSInteger specifying the number of rows in the 
 * the section. Required method for table view.
 */

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.vacations count]; 
}



/**
 * Instance method: prepareForSegue
 * --------------------------------
 * note the generic implementation of the prepareForSegue. prepareForSegue only checks the
 * the destination controller responds to the method setVactionName. As long as the destination 
 * controller responds to setVacationName we set the vacation name and segue. 
 */

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender 
{
    if([segue.destinationViewController respondsToSelector:@selector(setVacationName:)])
    {
        NSLog(@"cell title = %@", [[sender textLabel] text]); 
        [segue.destinationViewController performSelector:@selector(setVacationName:) withObject:[[sender textLabel] text]]; 
    }
}




- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Vacation Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    // Configure the cell...
    
    cell.textLabel.text = [self.vacations objectAtIndex:indexPath.row]; 
    
    return cell;
}





/** 
 * Implementation note:
 * --------------------------------------------------
 * Instead of using a segue we could also use didSelectRowAtIndexPath to identify the vacation and 
 * trigger a segue. 
 * 
 * The drawback is the sender is technically the controller. Instead of using self 
 * as the sender, we would have to get the selected vacation 
 * with [self.vacation objectAtIndex.indexPath.row] and send the NSString
 * 
 *
 * - (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
 * {
 *    [self performSegueWithIdentifier:@"Show ItineraryTVC" sender:[self.vacations objectAtIndex:indexPath.row]]; 
 * }
 */



@end
