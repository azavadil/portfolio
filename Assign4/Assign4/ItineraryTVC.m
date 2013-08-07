//
//  ItineraryTVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "ItineraryTVC.h"


@implementation ItineraryTVC

@synthesize itineraryOptions = _itineraryOptions; 
@synthesize vacationName = _vacationName; 


/** 
 * Instance method: setupItineraryOptionsArray 
 * -------------------------------------------
 * setupItineraryOptionsArray is called if we reach viewDidLoad and 
 * self.itineraryOptions hasn't been setup. 
 */ 

- (void)setupItineraryOptionsArray
{
    self.itineraryOptions = [[NSArray alloc] initWithObjects:@"Itinerary", @"Tags", nil]; 
}



/**
 * Instance method: viewDidLoad
 * ----------------------------
 * self-documenting
 */
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSLog(@"viewDidLoad/vacationName = %@", self.vacationName); 

    if(!self.itineraryOptions)
    {
        [self setupItineraryOptionsArray]; 
    }

}



/**
 * Instance method: prepareForSegue
 * --------------------------------
 * self-documenting
 */

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if([segue.identifier isEqualToString:@"Show PlaceItinerary Option"])
    {
        NSLog(@"Prepare for PlaceItinerary Option"); 
        if([segue.destinationViewController respondsToSelector:@selector(setVacationName:)])
        {
            [segue.destinationViewController setVacationName:self.vacationName]; 
        }
    }
    else if ( [segue.identifier isEqualToString:@"Show TagItinerary Option"])
    {
        NSLog(@"Prepare for TagItinerary Option"); 
        if([segue.destinationViewController respondsToSelector:@selector(setVacationName:)])
        {
            [segue.destinationViewController setVacationName:self.vacationName]; 
        }
    }
}



/**
 * Instance method: tableView-didSelectRowAtIndexPath
 * --------------------------------------------------
 * There are only two options from the Itinerary view. Show an itinerary by place
 * or show an itinerary by tag. 
 */

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(indexPath.row == 0) 
    {
        NSLog(@"Show PlaceItinerary Option"); 
        [self performSegueWithIdentifier:@"Show PlaceItinerary Option" sender:self]; 
    }
    else if(indexPath.row == 1)
    {
        NSLog(@"Show TagItinerary Options"); 
        [self performSegueWithIdentifier:@"Show TagItinerary Option" sender:self];
    }


}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}



/**
 * Instance method: tableView-numberOfRowsInSection
 * ------------------------------------------------
 * tableView-numberOfRowsInSection returns an NSInteger specifying the number of rows in the 
 * the section. Required method for table view.
 */

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.itineraryOptions count];
}



/**
 * Instance method: tableView-cellForRowAtIndexPath
 * ------------------------------------------------
 * self-documenting
 */

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Itinerary Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    // Configure the cell...
    
    
    cell.textLabel.text = [self.itineraryOptions objectAtIndex:indexPath.row]; 
    return cell;
}




@end
