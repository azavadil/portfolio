//
//  TopPlacePhotoVC.m
//  Assign4
//
//  Created by Anthony Zavadil on 6/10/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "TopPlacePhotoVC.h"
#import "FlickrFetcher.h" 

@interface TopPlacePhotoVC() <UIScrollViewDelegate>          //all methods in <UIScrollViewDelegate> are optional

@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UIImageView *imageView;

- (UIImage *)fetchImageWithDictionary; 
- (NSURL*)makeCacheURL; 
- (UIImage *)fetchImageWithDatabase;


@end

@implementation TopPlacePhotoVC
@synthesize scrollView = _scrollView;
@synthesize imageView = _imageView;


@synthesize  photoDictionary = _photoDictionary;
@synthesize photo = _photo; 
@synthesize fileManager = _fileManager; 
@synthesize vacationName = _vacationName; 

- (NSFileManager*)fileManager
{
    if (!_fileManager) _fileManager = [[NSFileManager alloc] init]; 
    return _fileManager; 
}



/** 
 * Instance method: viewForZoomingInScrollView
 * -------------------------------------------
 * viewForZoomingInScrollView specifies the view that we want zoom-in/zoom-out to apply to 
 * viewForZoomingInScrollView is required if we want zooming to work. We simply return 
 * the subview we want to zoom. 
 * 
 * In this case (and most cases) you only have 1 subview so this is trivial
 */ 

- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView   
{
    return self.imageView; 
}



/** 
 * Instance method: sizeOfCacheContainer
 * -------------------------------------
 * deprecated. Returns the size of the container but on the cumulative size of the elements
 * 
 * sizeOfCacheContainer returns the size of the container at the specified file path 
 */ 

-(unsigned long long)sizeOfCacheContainer
{
    
    NSFileManager *fileManager = [[NSFileManager alloc] init];
    NSString *filePath = [[[self makeCacheURL]URLByAppendingPathComponent:@"photoCache"] path]; 
    NSDictionary *fileAttributes = [fileManager attributesOfItemAtPath:filePath error:NULL]; 
	unsigned long long cacheSize = [fileAttributes fileSize]; 
    return cacheSize; 
}



/**
 * Instance method: getSizeOfCache
 * -------------------------------
 * getSizeOfCache returns the size of the photos we have caches (we cache recent photos 
 * instead of constantly hitting Flickr). 
 */ 

- (int)getSizeOfCache
{
    NSURL *filePath = [[self makeCacheURL]URLByAppendingPathComponent:@"photoCache"];
    NSDictionary *cachedData = [[NSDictionary alloc] initWithContentsOfURL:filePath]; 
    
    int fileSize = 0; 
    
    for (id item in cachedData)
    {
        if([item isKindOfClass:[NSData class]])
        {
            fileSize += [item length];
        }
    }
    return fileSize; 
}



/**
 * Instance method: getSizeOfDictionary
 * ------------------------------------
 * getSizeOfDictionary takes a dictionary and returns the size of the 
 * items in the dictionary
 */

-(int)getSizeOfDictionary:(NSDictionary *)dictionaryOfImageData
{
    int totalSize = 0; 
    for(id item in dictionaryOfImageData)
    {
        NSData *imageValue = [dictionaryOfImageData valueForKey:item]; 
        totalSize += [imageValue length]; 
    }
    return totalSize; 
}


#define MAXIMUM_CACHE_SIZE 10485760


/**
 * Instance method: makeCacheURL
 * ----------------------------- 
 * makeCacheURL is a convience method to autogenerate the path where the default 
 * cache directory is located
 */

- (NSURL*)makeCacheURL
{
    NSArray *urlsArray = [self.fileManager URLsForDirectory:NSCachesDirectory 
                                                  inDomains:NSUserDomainMask];  
    
    return [urlsArray lastObject]; 
}



/**
 * Instance method: cachePhoto
 * ---------------------------
 * cachePhoto takes an NSString and an image. The cache is an NSDictionary. The dictionary
 * holds 2 types of values: UIImages with uniqueID as keys and an NSMutableArray with 
 * @"chronology" as key. We use the chronology as a queue to keep track of the order that 
 * images are added to the cache. When the cache is over the size limit we take the uniqueID
 * at position 0 of the chronology (i.e. the oldest uniqueID) and delete that entry from the 
 * cache.
 */

- (void)cachePhoto:(NSString *)uniqueID imageToCache:(UIImage *)image
{
    
    if(!image) return; 
    
    NSURL *filePath = [[self makeCacheURL]URLByAppendingPathComponent:@"photoCache"];
    
    
    // read the cache
    NSMutableDictionary *cachedImages = [[NSMutableDictionary alloc] initWithContentsOfURL:filePath]; 
    if(!cachedImages) cachedImages = [[NSMutableDictionary alloc] init]; 
    
    
    // extract the chronological order from the cache 
    NSMutableArray *chronology = [cachedImages objectForKey:@"chronology"];
    [cachedImages removeObjectForKey:@"chronology"];
    if(!chronology) chronology = [[NSMutableArray alloc] init]; 
    
    while( [self getSizeOfDictionary:cachedImages] > MAXIMUM_CACHE_SIZE )
    {
        
        if([chronology count] > 0)
        {
            NSString *currKey = [chronology objectAtIndex:0]; 
            [chronology removeObjectAtIndex:0];
            [cachedImages removeObjectForKey:currKey];
            
        }
    }
        
    // check that the image isn't already in the cache
    id obj = [cachedImages objectForKey:uniqueID]; 
    if(!obj)
    {
    
        [chronology addObject:uniqueID]; 
        [cachedImages setObject:chronology forKey:@"chronology"]; 
        NSData *pngImage = UIImagePNGRepresentation(image); 
        [cachedImages setObject:pngImage forKey:uniqueID];

        [cachedImages writeToURL:filePath atomically:YES]; 
    }
    //NSLog(@"cachePhoto = %d, %d", [cachedImages count], [self sizeOfCacheContainer]); 
    
}



/**
 * Instance method: fetchImage
 * ---------------------------
 * fetchImage just calls the helper methods fetchImageWithDictionary and
 * fetchImageWithDatabase. 
 * 
 * Implementation note: 
 * --------------------
 * We use a single view controller to display photos that are fetched from Flickr
 * or photos that are fetched out of the database. fetchImage is just a wrapper that 
 * directs us to the appropriate method. 
 */ 
 
- (UIImage *)fetchImage
{
    UIImage *image = nil;
    
    if(self.photoDictionary)
    {
        image = [self fetchImageWithDictionary]; 
    }
    else if(self.photo)
    {
        image = [self fetchImageWithDatabase]; 
    }
    return image; 
}



/**
 * Instance method: fetchImageWithDictionary
 * -----------------------------------------
 * fetchImageWithDictionary first checks whether the specified photo is in the cache. 
 * If the photo is in the cache it retrieves the photo data from the cache. Otherwise
 * fetchImageWithDictionary queries Flickr for the photo data. 
 */ 

- (UIImage *)fetchImageWithDictionary
{
    
    UIImage *image = nil; 
    
    NSURL *url = [self makeCacheURL]; 
    NSURL *filePath = [url URLByAppendingPathComponent:@"photoCache"]; 
    NSDictionary *cachedImages = [[NSDictionary alloc] initWithContentsOfURL:filePath]; 
    UIImage *cachedPhotoImage = [UIImage imageWithData:[cachedImages valueForKey:[self.photoDictionary valueForKey:FLICKR_PHOTO_ID]]]; 
    
        
    if(cachedPhotoImage)
    {
        image = cachedPhotoImage; 
    }
    else
    {
        image = [UIImage imageWithData:
                           [NSData dataWithContentsOfURL:
                            [FlickrFetcher urlForPhoto:self.photoDictionary format:FlickrPhotoFormatLarge]]];
    }
    return image; 
}



/**
 * Instance method: fetchImageWithDatabase
 * -----------------------------------------
 * fetchImageWithDatabase first checks whether the specified photo is in the cache. 
 * If the photo is in the cache it retrieves the photo data from the cache. Otherwise
 * fetchImageWithDatabase queries the database for the photo data. 
 */ 

- (UIImage *)fetchImageWithDatabase
{
    UIImage *image = nil; 
    
    NSURL *url = [self makeCacheURL]; 
    
    NSURL *filePath = [url URLByAppendingPathComponent:@"photoCache"]; 
    NSDictionary *cachedImages = [[NSDictionary alloc] initWithContentsOfURL:filePath]; 
    UIImage *cachedPhotoImage = [UIImage imageWithData:[cachedImages valueForKey:self.photo.unique]];  
    
    if(cachedPhotoImage)
    {
        image = cachedPhotoImage; 
    }
    else
    {
        image = [UIImage imageWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:self.photo.imageURL]]];
    }
    return image; 
}





/** 
 * Instance method: viewDidLoad
 * ----------------------------
 * Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
 * viewDidLoad is the first place we want to think about putting things. 
 *
 * In this case we set the spinner animating and then asynchronously fetch photo data. 
 */ 

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSLog(@"TopPlacePhotoVC - viewDidLoad"); 

    UIActivityIndicatorView *spinner = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray]; 
    [spinner startAnimating]; 
    
    
    spinner.center = self.view.center;  
    [self.view addSubview:spinner]; 
    
    dispatch_queue_t downloadQueue = dispatch_queue_create("topPlaceImage downloader", NULL); 
    dispatch_async(downloadQueue, ^{ 
    
        UIImage *currImage = [self fetchImage]; 
        dispatch_async(dispatch_get_main_queue(), ^{ 
            [spinner removeFromSuperview]; 
            self.imageView.image = currImage; 
            self.scrollView.delegate = self; 
            self.scrollView.contentSize = self.imageView.image.size; 
            self.imageView.frame = CGRectMake(0, 0, self.imageView.image.size.width, self.imageView.image.size.height);
            
            
            //set the zoomRatio
            
            float imageWidth = self.imageView.image.size.width; 
            float imageHeight = self.imageView.image.size.height; 
            float viewWidth = self.view.bounds.size.width; 
            float viewHeight = self.view.bounds.size.height;
            
            float widthRatio = viewWidth / imageWidth; 
            float heightRatio = viewHeight / imageHeight; 
            
            self.scrollView.zoomScale = MAX(widthRatio, heightRatio); 
            
            
            self.title = [self.photoDictionary objectForKey:FLICKR_PHOTO_TITLE];
        }); 
        if(self.photoDictionary)
        {
            [self cachePhoto:[self.photoDictionary objectForKey:FLICKR_PHOTO_ID] imageToCache:currImage];
        }
        else if(self.photo)
        {
            [self cachePhoto:self.photo.unique imageToCache:currImage];
        }
        
    }); 
    
    dispatch_release(downloadQueue); 
    
}



- (void)viewDidUnload
{
    [self setImageView:nil];
    [self setScrollView:nil];
    [self setScrollView:nil];
    [self setImageView:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}



- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return YES;
}



- (void)debugLogging
{
    // implement
}

@end
