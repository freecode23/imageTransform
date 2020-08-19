#include <iostream>
#include <cmath>
#include <cstdlib>

#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include "ImageTransform.h"

/*******************

Name: Sherly Hartono  
Email: sharton1@asu.edu

*********************/

using uiuc::PNG;
using uiuc::HSLAPixel;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {

  // for every row of pixels
  for (unsigned x = 0; x < image.width(); x++) {
    // for each column in the row
    for (unsigned y = 0; y < image.height(); y++) {

      // get the address of this pixel in memory, make alias name pixel.
      // get pixel return HSLAPixel HSLAPixel & getPixel(unsigned int x, unsigned int y) const;
      HSLAPixel & pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }
  return image;
}

/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 * 
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {
  // 1. get every pixel
  // for every row
  for (unsigned x = 0; x < image.width(); x++) {
  // for every column
  // 2. create spotlight depending on distance
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);   
      // 2.1 determine luminance of each pixel 
      // 2.1.1 calculate horizontal distance x, and vertical distance y
      // doesn't work not absolute value.
      // double distanceX = x-centerX;
      // double distanceY = y-centerY;

      // 2.1.2 calculate "normal" distance

      double euclideanDistance = abs(sqrt(((x-centerX)*(x-centerX)) + ((y-centerY)*(y-centerY))));
      double percentageDecrease;

      // 2.1.1.1 if distance is greater than 160 px, luminance decreased by 80% 
      if(euclideanDistance >= 160){
        percentageDecrease = 80.0;
      }else{
        // 2.1.1.2 else calculate percentage decrease
        // double percentage decrease = distance * 0.5s
        percentageDecrease = euclideanDistance * 0.5;
      }
      // set luminance for this pixel
      pixel.l = (1-percentageDecrease/100) * pixel.l;
    }    
  }
  /**
    std::cout<< "center: "<< "( "<< centerX << ", "<< centerY << " )"<< endl;
    std::cout<< "x distance: "<< 446-centerX << " ,y distance: "<< 147-centerY<< endl;
    std::cout<< "euclideanDistance: "<< abs(sqrt((446-centerX)*(446-centerX) + ((147-centerY)*(147-centerY))))<< endl;
    std::cout<< "percentage decrease: "<< abs(sqrt((446-centerX)*(446-centerX) + ((147-centerY)*(147-centerY)))) * 0.5 << endl;
    std::cout<< image.width() << endl;
    std::cout<< image.height() << endl;
  **/
  return image;
  
}
 
/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
**/
PNG illinify(PNG image) {
  // 1. get every pixel 
  // for each row
  for (unsigned x = 0; x < image.width(); x++) {
    // for each column in that row
    for (unsigned y = 0; y < image.height(); y++) {
      // 2. compare distance
      // 2.1) get pixel
      HSLAPixel & pixel = image.getPixel(x, y);

      // 2.2) if pixel's hue is within orange range
      // (0.0 <= pixel.h <= 113.5 || 293.5<= pixel.h <= 360.0)
      if((pixel.h >= 0.0 && pixel.h <=113.5) || (pixel.h < 360 && pixel.h >=293.5)){
        // change hue to orange
        pixel.h = 11;
      }else{
        // 2.3) else
        // change hue to blue
        pixel.h = 216;
      }
    }
  }
return image;
}
 
/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs, which is the base image.
* @param secondImage The second of the two PNGs, which acts as the stencil.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {
  // check luminance of stencel
  for (unsigned x = 0; x < firstImage.width(); x++) {
    // for each column in the row
    for (unsigned y = 0; y < firstImage.height(); y++) {
      HSLAPixel & basePixel = firstImage.getPixel(x, y);
      HSLAPixel & stencilPixel = secondImage.getPixel(x, y);
      if(stencilPixel.l == 1){
        basePixel.l += 0.2; 
      }
    }
  }

  return firstImage;
}
