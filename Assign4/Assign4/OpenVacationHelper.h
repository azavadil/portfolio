//
//  OpenVacationHelper.h
//  Assign4
//
//  Created by Anthony Zavadil on 6/17/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h> 

 /** 
 * Implementation note: 
 * -------------------
 * The approach of this program is to have a single UIManagedDocument instance
 * per document on disk. This approach is implemented by have the openVacationHelper
 * class which opens and creates the document. 
 */ 



@interface OpenVacationHelper : NSObject

typedef void(^completion_block_t)(UIManagedDocument *vacation); 

/** 
 * Class method: openVacation
 * --------------------------
 * openVacation opens or creates the UIManagedDocument before handing
 * the document back to the caller. 
 */

+ (void)openVacation:(NSString *)vacationName usingBlock:(completion_block_t)completionBlock; 

@end
