//
//  DisplayPhotoVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/19/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "DisplayPhotoVC.h"
#import "FlickrFetcher.h"
#import "OpenVacationHelper.h"
#import "Photo+CreatePhotoWithFlickrData.h"

@interface DisplayPhotoVC()

- (void)setupVacationDocument:(UIManagedDocument *)vacation; 
- (void)openDatabase; 
- (void)setupRightBarButtonItem;

@end


@implementation DisplayPhotoVC

@synthesize vacationDocument = _vacationDocument; 




/**
 * Instance method: testQuery 
 * --------------------------
 * deprecated: used for early debugging
 */

- (void)testQuery
{
    
    NSString *uniqueID = self.photoDictionary ? [self.photoDictionary objectForKey:FLICKR_PHOTO_ID] : self.photo.unique; 
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Photo"]; 
    
    
    // this code builds the query/request
    request.predicate = [NSPredicate predicateWithFormat:@"unique = %@", uniqueID];  
    NSSortDescriptor *sortDescriptor = [NSSortDescriptor sortDescriptorWithKey:@"title" ascending:YES]; 
    request.sortDescriptors = [NSArray arrayWithObject:sortDescriptor]; 
    
    
    // this code executes the query/request
    NSError *error = nil;
    NSArray *matches = [self.vacationDocument.managedObjectContext executeFetchRequest:request error:&error]; 
    NSLog(@"DisplayPhotoVC - testQuery matches count = %d", [matches count]); 
    NSLog(@"DisplayPhotoVC - testQuery - uniqueID = %@", uniqueID); 

}




/** 
 * Instance method: setVacationDocument
 * ------------------------------------ 
 * setVacationDocument is the setter for the vacation document (i.e. the coreData document)
 */

- (void)setVacationDocument:(UIManagedDocument *)vacationDocument
{
    if(_vacationDocument != vacationDocument)
    {
        _vacationDocument = vacationDocument;
    }
    [self setupRightBarButtonItem]; 
    [self.view setNeedsDisplay]; 
     
}



/**
 * Instance method: viewDidLoad
 * ---------------------------- 
 * viewDidLoad calls openDatabase. This ensures that whenever the view is opened the
 * _vacationDocument instance variable is also set (via the call to 
 * setVacationDocument in openDatabase. 
 */

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSLog(@"DisplayPhotoVC - viewDidLoad");
    [self openDatabase]; 
    NSLog(@"DisplayPhotoVC - dictionary = %@", self.photoDictionary); 
    
}



/**
 * Instance method: openDatabase
 * -----------------------------
 * openDatabase uses the OpenVacationHelper to open the UIManagedDocument
 */

- (void)openDatabase
{
    
    NSString *currVacation = nil;
    currVacation = self.vacationName; 
    if(!currVacation) 
    {
        
        /* currently we have only one vacation so this is a kludge if self.vacationName 
         * hasn't been set then we read the file at /NSDocumentsDirectory/ListOfVacations 
         * which keeps a list of all the vacations. Because we know there's only one vacation 
         * we can just take the vacation at index 0
         */ 
        
        NSURL *url = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject]; 
        url = [url URLByAppendingPathComponent:@"ListOfVacations"];
        NSArray *vacations = [NSArray arrayWithContentsOfURL:url]; 
        currVacation = [vacations objectAtIndex:0];
        //NSLog(@"openDatabase, ListOfVacations = %@", vacations); 
        //NSLog(@"openDatabase vacationName = %@", currVacation); 
    }
    
    [OpenVacationHelper openVacation:currVacation usingBlock:^(UIManagedDocument *vacationDoc)
     {   
         [self setupVacationDocument:vacationDoc];  }]; 
    
}



/**
 * Instance method: setupVacationDocument
 * --------------------------------------
 * setupVacationDocument works in conjunction with 
 * openDatabase to access the sharedManagedDocument
 */ 

- (void)setupVacationDocument:(UIManagedDocument *)vacation
{
    //NSLog(@"DisplayPhotoVC - setupVacationDocument"); 
    self.vacationDocument = vacation;               
}



/**
 * Instance method: photoExistsInDatabase
 * --------------------------------------
 * photoExistsInData takes an NSSstring representing a unique identifier and returns 
 * YES if the that unique identifier is found in the database and false otherwise 
 */

- (BOOL)photoExistsInDatabase:(NSString *)uniqueID 
{
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Photo"]; 
    
    
    // this code builds the query/request
    request.predicate = [NSPredicate predicateWithFormat:@"unique = %@", uniqueID];  
    NSSortDescriptor *sortDescriptor = [NSSortDescriptor sortDescriptorWithKey:@"title" ascending:YES]; 
    request.sortDescriptors = [NSArray arrayWithObject:sortDescriptor]; 
    
    
    // this code executes the query/request
    NSError *error = nil;
    NSArray *matches = [self.vacationDocument.managedObjectContext executeFetchRequest:request error:&error]; 
    
    
    BOOL result; 
    
    if (!matches || [matches count] > 1) {  
        NSLog(@"ERROR: nil or non-unique photos in database");
        NSLog(@"ERROR: numMatches = %@", [matches count]); 
        // return YES, stop addToDatabase from attemping to add the photo
        result = YES; 
    }
    else if([matches count] == 0)
    {
        result = NO; 
    }
    else 
    {
        result = YES; 
    }
    return result; 
}



/** 
 * Instance method: setupRightBarButtonItem
 * ------------------------------
 * setupRightBarButtonItem sets the rightBarButton when a photo is viewed. The rightBarButton
 * can either say "visit" and trigger the method addToVaction or it can say "unvisit" 
 * and trigger the method removeFromVacation.
 */ 


- (void)setupRightBarButtonItem
{
    NSString *uniqueID = self.photoDictionary ? [self.photoDictionary objectForKey:FLICKR_PHOTO_ID] : self.photo.unique; 
        
    UIBarButtonItem *rightBarButton = nil;  
    
    BOOL existsInDatabase = [self photoExistsInDatabase:uniqueID]; 
        
    
    /** 
     * Implementation note: 
     * --------------------
     * if the photo isn't in the database then user has the option of visiting the photo which.
     * The title of the button is set to "Visit" and the action is set to method addToVacation
     */
    if(!existsInDatabase) 
    {
        rightBarButton = [[UIBarButtonItem alloc] initWithTitle:@"Visit" style:UIBarButtonItemStylePlain target:self action:@selector(addToVacation:)];
    }
    else     
    {
        rightBarButton = [[UIBarButtonItem alloc] initWithTitle:@"Unvisit" style:UIBarButtonItemStylePlain target:self action:@selector(removeFromVacation:)];
    }
    self.navigationItem.rightBarButtonItem = rightBarButton;
}



/** 
 * Instance method: addToVacation
 * ------------------------------
 * addToVacation is the target for the action of a rightBarButton that's set to "visit" being pressed. The 
 * button can either be "visit" or "unvisit" so we can't hardwire the button to 
 * the addToVacation or removeFromVacation methods. Instead, we implement the functionality 
 * the setupRightBarButtonItem method. The action for the button is set as either 
 * addToVacation or removeFromVacation depending on whether the photo is already in the 
 * vaction. 
 *
 * It should be the case that this is only triggered when we have self.photoDictionary
 */ 

- (void)addToVacation:(id)sender
{
    NSLog(@"DisplayPhotoVC - addToVacation - self.photoDictionary = %@", self.photoDictionary); 
    [Photo photoWithFlickrInfo:self.photoDictionary inManagedObjectContext:self.vacationDocument.managedObjectContext]; 
    [self.vacationDocument saveToURL:self.vacationDocument.fileURL forSaveOperation:UIDocumentSaveForOverwriting completionHandler:^(BOOL success){
        if(success){ NSLog(@"DisplayPhotoVC - addToVacation - NSManagedDocument saved");}} ]; 
    [self setupRightBarButtonItem]; 
    [self.view setNeedsDisplay]; 
}



/** 
 * Instance method: removeFromVacation
 * ------------------------------
 * removeFromVacation is the target for the action of a rightBarButton that's set to "unvisit" 
 * being pressed. The button can either be "visit" or "unvisit" so we can't hardwire the button to 
 * the addToVacation or removeFromVacation methods. Instead, we implement the functionality 
 * the setupRightBarButtonItem method. The action for the button is set as either 
 * addToVacation or removeFromVacation depending on whether the photo is already in the 
 * vaction. 
 *
 * It should be the case that this is only triggered when we have self.photoDictionary
 */ 

- (void)removeFromVacation:(id)sender
{
    [Photo deletePhotoWithFlickrInfo:self.photoDictionary inManagedObjectContext:self.vacationDocument.managedObjectContext]; 
    [self.vacationDocument saveToURL:self.vacationDocument.fileURL forSaveOperation:UIDocumentSaveForOverwriting completionHandler:^(BOOL success){
        if(success){ NSLog(@"DisplayPhotoVC - removeFromVacation - NSManagedDocument saved");}} ];
    [self setupRightBarButtonItem]; 
    [self.view setNeedsDisplay]; 
}




- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
