#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libpnm.h"

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) // macro to return the sign of a number

int program1 (int width, int height, char *imageName, bool formatCode);
int program2 (int width, int height, char *imageName, bool formatCode);
int program3 (int width, int height, char *imageName, bool formatCode);

void draw_line(struct PBM_Image * pbmImage, int x1, int y1, int x2, int y2);
void draw_line_gray(struct PGM_Image * pgmImage, int x1, int y1, int x2, int y2);

int main (int argc, char *argv[]){
    if (argc != 6){
        printf("Error with arguments\n");
        printf("Usage: %s typeCode width height imageName formatCode\n", argv[1]);
        return -1;
    }
    //specified through argv
    int typeCode, width, height;
    bool formatCode = false; //ASCII by default
    char *imageName, *pnmExt;
    // Checking for typeCode and width, run the appropiate function if valid
    typeCode = atoi(argv[1]);
    width = atoi(argv[2]);
    if (typeCode == 3){
        if (width < 6 || width % 6 != 0) {
            printf("width error for ppm image. Conditions not satisfied.\n");
            printf("Conditions: width >= 6\nwidth must be divisible by 6\n");
            return -1;                
        }
        pnmExt = "ppm";
    }
    else {
        if (typeCode == 1){
            pnmExt = "pbm";
        }
        else if (typeCode == 2){
            pnmExt = "pgm";
        }
        else {
            printf("typeCode error\nRange: 0 < typeCode <= 3\n");
            return -1;
        }
        if (width < 4 || width%4 != 0){
            printf("width error for %s image. Conditions not satisfied.\n", pnmExt);
            printf("Conditions: width >= 4\nwidth must be divisible by 4\n");
            return -1;
        }
    }
    printf("%d typeCode: %s\n",typeCode,pnmExt);
    // Checking for height of image
    height = atoi(argv[3]);
    if (height < 4 || height % 4 != 0) {
        printf("height error for %s image. Conditions not satisfied.\n", pnmExt);
        printf("Conditions: height >= 4\nheight must be divisible by 4\n");
        return -1;
    }
    imageName = argv[4];
    printf("%s is the name of my image\n",imageName);
    // how to debug image name for bad characters here
    int format = atoi(argv[5]);
    if (format == 0)
        formatCode = false; //ASCII
    else if (format==1)
        formatCode = true; //raw
    else {
        printf("Invalid Format Code. Usage:\nformatCode = 0, ASCII\nformatCode = 1, raw\n");
        return -1;
    }
    // run appropiate program based on typeCode
    if (typeCode == 1)
        program1(width, height, imageName, formatCode); /*****ARGS will be needed*****/
    else if (typeCode == 2)
        program2(width, height, imageName, formatCode); /*****ARGS will be needed*****/
    else
        program3(width, height, imageName, formatCode); /*****ARGS will be needed*****/
    return 0;
}

int program1 (int width, int height, char *imageName, bool formatCode) {
    struct PBM_Image rectangle;
    // This should create a new image
    printf("Creating new PGM image: %d\n", create_PBM_Image(&rectangle, width, height));
    int row, col, // row and column counters for loop
        w = width/2,
        h = height/2, // width and height for inner rectangle
        x1, x2, y1, y2; // coordinate variables for drawing inner square and lines
    x1 = (width-w)/2;
    y1 = (height-h)/2;
    x2 = (width+w)/2;
    y2 = (height+h)/2;

    /* Things I'd like to improve
     * - Find a way to plot the pixels here in these loops as the square is being drawn
     * At least with Bresenham's the line gets drawn faster
     */
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            if (row >= y1 && row < y2) {
                if (col >= x1 && col < x2){
                    rectangle.image[row][col] = 0;
                }
                else
                    rectangle.image[row][col] = 1;
            }
            else
                rectangle.image[row][col] = 1;
        }
    }
    // going from top left to bottom right
    draw_line(&rectangle, x1, y1, x2-1, y2-1);
    // going from bottom left to top right, dx is the same
    draw_line(&rectangle, x1, y2-1, x2-1, y1);
    
    save_PBM_Image(&rectangle, imageName, formatCode);
    free_PBM_Image(&rectangle);
    return 0;
}

int program2 (int width, int height, char *imageName, bool formatCode) {
    struct PGM_Image rectangle;
    // This should create a new image
    printf("Creating new image: %d\n", create_PGM_Image(&rectangle, width, height, MAX_GRAY_VALUE));
    int row, col, // row and column counters for loop
        w = width/2,
        h = height/2, // width and height for inner rectangle
        x1, x2, y1, y2; // coordinate variables for drawing inner square and lines
    x1 = (width-w)/2;
    y1 = (height-h)/2;
    x2 = (width+w)/2;
    y2 = (height+h)/2;
    
    // Draw black and white rectangles:
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            if (row >= y1 && row < y2) {
                if (col >= x1 && col < x2){
                    rectangle.image[row][col] = rectangle.maxGrayValue;
                }
                else
                    rectangle.image[row][col] = 0;
            }
            else
                rectangle.image[row][col] = 0;
        }
    }
    // First line
    draw_line_gray(&rectangle, x1, y1, x2-1, y2-1);
    // Second line
    draw_line_gray(&rectangle, x1, y2-1, x2-1, y1);
    
    /*
     * I realize that this entire block of code could be broken down into different functions
     * Or even put in its own function altoghether.
     *
     * However, I'm running out of time to finish the assignment, so if this comment is still
     * here, then it means I did not have time to refactor this part.
     */
    if (width > 4 && height > 4) { // this isn't necessary when one of the values is 4
        int startx, endx, starty, endy, pixel,m[2];
        m[0]=(x1+(x2-1))/2; m[1]=(y1+(y2-1))/2;// middle points (m[0],m[1])
        // Fill in each line
        int nextPix, countx, county;
        starty = y1;
        for (startx = x1; startx != m[0]; startx++) {
            county = starty+1;
            do {
                nextPix = rectangle.image[starty+1][startx]; //Get next pixel value
                starty++;
                if (nextPix == 0) {
                    starty=county;
                    break; // Do not plot if next pixel encountered is pure black (border or middle)
                }
            } while (nextPix != rectangle.maxGrayValue);
            starty--; // offset position to current pixel
            pixel = rectangle.image[starty][startx]; // pixel to plot
            county = starty;
            while (nextPix == rectangle.maxGrayValue){
                rectangle.image[county][startx] = pixel; //plot pixel
                county++; //shift pixel position by 1 down
                nextPix = rectangle.image[county][startx]; //shift down one to next pixel to compare 
            }
        }
        starty = y1;
        for (endx = x2-1; endx != m[0]+1; endx--) { //m+1 is the "second pixel half" of middle point
            county = starty+1;
            do {
                nextPix = rectangle.image[starty+1][endx]; //Get next pixel value
                starty++;
                if (nextPix <= 0) {
                    starty=county;
                    break;
                }
            } while(nextPix != rectangle.maxGrayValue);
            starty--; // offset position to current pixel
            pixel = rectangle.image[starty][endx];
            county = starty;
            while (nextPix == rectangle.maxGrayValue){
                rectangle.image[county][endx] = pixel; //plot pixel
                county++; //shift pixel position by 1 down
                nextPix = rectangle.image[county][endx]; //shift down one pixel to compare        
            }
        }
        startx = x1;
        for (starty = y1; starty != m[1]; starty++) {
            countx = startx+1;
            do {
                nextPix = rectangle.image[starty][startx+1]; // shift right one pixel to compare
                startx++;
                if (nextPix <= 0) {
                    startx = countx;
                    break;
                }
            } while (nextPix!=rectangle.maxGrayValue);
            startx--; // offset position
            pixel = rectangle.image[starty][startx];
            countx = startx;
            while (nextPix >= rectangle.maxGrayValue){
                rectangle.image[starty][countx] = pixel; //plot pixel
                countx++; //shift pixel position by 1 right
                nextPix = rectangle.image[starty][countx]; //shift right one pixel to compare
            }
        }
        startx = x1;
        for (endy = y2-1; endy != m[1]+1; endy--) {// m[1]+1 to offset position in pixel grid
            countx = startx+1;
            do {
                nextPix = rectangle.image[endy][startx+1]; // shift right one pixel to compare
                startx++;
                if (nextPix <= 0) {
                    startx = countx;
                    break;
                }
            } while (nextPix!=rectangle.maxGrayValue);
            startx--; // offset position
            pixel = rectangle.image[endy][startx];
            countx = startx;
            while (nextPix >= rectangle.maxGrayValue){
                rectangle.image[endy][countx] = pixel; //plot pixel
                countx++; //shift pixel position by 1 right
                nextPix = rectangle.image[endy][countx]; //shift right one pixel to compare
            }
        }//*/
    } // end if
    save_PGM_Image(&rectangle, imageName, formatCode);
    free_PGM_Image(&rectangle);
    return 0;
}

int program3 (int width, int height, char *imageName, bool formatCode) {
    struct PPM_Image rectangle;
    // This should create a new image
    printf("Creating new PPM image: %d\n", create_PPM_Image(&rectangle, width, height, MAX_GRAY_VALUE));
    
    int row, col, // row and column counters for loop
        wRGB = width/3,
        wGray = width/2,
        h = height/2, // middle point of image = h
        x1r = 0, x2r = wRGB-1, // red,
        x1g = wRGB, x2g = (wRGB*2)-1, // green, and
        x1b = wRGB*2, x2b = width-1, // blue coordinate variables for drawing inner squares
        y1rgb = 0, y2rgb=h-1, // heights remain the same
        x1btow = 0, x2btow = wGray-1, // black to white,
        x1wtob = wGray, x2wtob = width-1, // white to black, and
        y1gray = h, y2gray = height-1; // the height of the entire grayscale area
        
    // Program 3 - I'm having trouble understanding how to assign color values
    // I'm submitting what I've done so far; it's late.
      
      // A function to draw a color line here
      
      // Then draw a gradient on each area here
      rectangle.image[0][0][2] = 255;
      rectangle.image[0][0][1] = 0;
      rectangle.image[0][0][0] = 0;
      
      save_PPM_Image(&rectangle, imageName, formatCode);
      free_PPM_Image(&rectangle);
    return 0;
}

//void draw_color_line(struct PPM_Image * ppmImage)
// I would have drawn a line using a function like this
// Then found the slope above and calculated the gradient

//Again, I will try to do it tomorrow; better to lose 10% for being late than 1 function (25% of the assignment)

void draw_line_gray(struct PGM_Image * pgmImage, int x1, int y1, int x2, int y2) {
    int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py,m[2];
    float currentPixel;

    dx=x2-x1;       // the horizontal distance of the line
    dy=y2-y1;       // the vertical distance of the line
    dxabs=abs(dx);  
    dyabs=abs(dy);  // absolute values for the above, used to determine steepness
    sdx=sgn(dx);    
    sdy=sgn(dy);    // signs of the delta values (+ or -)
    
    x=dyabs>>1;     // x relative to the absolute delta of y
    y=dxabs>>1;     // y relative to absolute delta of x
    px=x1;
    py=y1;          //the first points in the line
    
    m[0]=(x1+x2)/2; m[1]=(y1+y2)/2;// middle points (m[0],m[1])
    float dist = sqrt( pow((x2-x1),2) + pow((y2-y1),2) ); // distance
    float distB = dist/2; // distance to Black
    
    int maxGray = pgmImage->maxGrayValue;
    pgmImage->image[py][px] = maxGray; //plot the first pixel
    
    float curdist;
    printf("line\n");
    if (dxabs>=dyabs) { /* the line is more horizontal than vertical */
        for(i=0;i!=dxabs;i+= sdx) {
            y+=dyabs;
            if (y>=dxabs) {
                y-=dxabs;
                py+=sdy;
            }
            px+=sdx;
            curdist = sqrt( pow((m[0]-px),2) + pow((m[1]-py),2) );
            currentPixel = fabs(curdist/distB)*(float)maxGray;
            if (currentPixel > maxGray)
                currentPixel = (float) maxGray;
            pgmImage->image[py][px] = (int) round(currentPixel);
        }
    }
    else { /* the line is more vertical than horizontal */
        for(i=0;i<dyabs;i++) {
            x+=dxabs;
            if (x>=dyabs) {
                x-=dyabs;
                px+=sdx;
            }
            py+=sdy;
            // nextPixel = (float)i*((float)dx / (float)dy);
            curdist = sqrt( pow((m[0]-px),2) + pow((m[1]-py),2) );
            currentPixel = fabs(curdist/distB)*(float)maxGray;
            if (currentPixel > maxGray)
                currentPixel = (float) maxGray;
            pgmImage->image[py][px] = (int) round(currentPixel);
        }
    }
}

/****************************************************************************
 *  draw_line                                                               *
 *    draws a line using Bresenham's line-drawing algorithm, which uses     *
 *    no multiplication or division.                                        *
 *                                                                          *
 *  Code taken from here:                                                   *
 * http://www.brackeen.com/vga/source/djgpp20/lines.c.html                  *
 *                                                                          *
 ****************************************************************************/
void draw_line(struct PBM_Image * pbmImage, int x1, int y1, int x2, int y2)
{
    int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

    dx=x2-x1;       // the horizontal distance of the line
    dy=y2-y1;       // the vertical distance of the line
    dxabs=abs(dx);  
    dyabs=abs(dy);  // absolute values for the above, used to determine steepness
    sdx=sgn(dx);    
    sdy=sgn(dy);    // signs of the delta values (+ or -)
    x=dyabs>>1;     // x relative to the absolute delta of y
    y=dxabs>>1;     // y relative to absolute delta of x
    px=x1;
    py=y1;          //the first points in the line
    
    pbmImage->image[py][px] = 1; //plot the first pixel
    
    if (dxabs>=dyabs) { /* the line is more horizontal than vertical */
        for(i=0;i!=dxabs;i+= sdx) {
            y+=dyabs;
            if (y>=dxabs) {
                y-=dxabs;
                py+=sdy;
            }
            px+=sdx;
            pbmImage->image[py][px] = 1;
        }
    }
    else { /* the line is more vertical than horizontal */
        for(i=0;i<dyabs;i++) {
            x+=dxabs;
            if (x>=dyabs) {
                x-=dyabs;
                px+=sdx;
            }
            py+=sdy;
            pbmImage->image[py][px] = 1;
        }
    }
}
