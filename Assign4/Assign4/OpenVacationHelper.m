//
//  OpenVacationHelper.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "OpenVacationHelper.h"

@implementation OpenVacationHelper

/**
 * Class method: openVacation
 * --------------------------
 * openVacation opens or creates the UIManagedDocument before handing
 * the document back to the caller. 
 */

+ (void)openVacation:(NSString *)vacationName usingBlock:(completion_block_t)completionBlock
{
    NSURL *url = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject]; 
    NSURL *appDirectoryURL = [url URLByAppendingPathComponent:@"VacationsDirectory"]; 
    
    // if the directory doesn't exist, create it /NSDocumentDirector/MyVacations
    if(![[NSFileManager defaultManager] fileExistsAtPath:[appDirectoryURL path]])
    {
        [[NSFileManager defaultManager] createDirectoryAtPath:[appDirectoryURL path] withIntermediateDirectories:YES attributes:nil error:nil]; 
    }
    
    
    //<NSDocumentsDirectory>/<VacationsDirectory>/myVacation
    url = [appDirectoryURL URLByAppendingPathComponent:vacationName]; 
    
    UIManagedDocument *photoDatabase = [[UIManagedDocument alloc] initWithFileURL:url]; 
    
    //NSLog(@"p2: url = %@", url); 
    //NSLog(@"p2: managedDocument = %@", photoDatabase); 
    
    // if the file doesn't exist, create it
    if(![[NSFileManager defaultManager] fileExistsAtPath:[url path]])
    {
            [photoDatabase saveToURL:photoDatabase.fileURL forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success){ 
            completionBlock(photoDatabase); 
            NSLog(@"openVacation result = %d", success); }]; 
    }
    else if(photoDatabase.documentState == UIDocumentStateClosed)
    {
        [photoDatabase openWithCompletionHandler:^(BOOL success){ 
            completionBlock(photoDatabase);
            NSLog(@"document was closed result = %d", success);
            }]; 
        
    }
    
    
}



@end
