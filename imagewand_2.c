//
//  main.c
//  testing
//
//  Created by Sourabh Kulhare on 11/19/15.
//  Copyright © 2015 Sourabh Kulhare. All rights reserved.
//


#include <stdio.h>
#include <wand/MagickWand.h>// ImageMagick's C framework for image processing.
#include <math.h>

struct image{
    unsigned char *gray_Data;
    int height;
    int width;
};

struct image get_Image(char name[])
{

    MagickWandGenesis();
    MagickBooleanType img_Check;
    MagickWand *wand_ip;
    
    wand_ip = NewMagickWand();
    img_Check =  MagickReadImage(wand_ip, name);
    if(img_Check==MagickFalse)
    {
        printf("ERROR : - Image not found. ");
    }

    int height1,width1;
    
    
    // get the height and width
    
    width1 =  (int) MagickGetImageWidth(wand_ip);
    //printf("Width is %lu\n",width1);
    
    height1 = (int) MagickGetImageHeight(wand_ip);
    //printf("Height is %lu",height1);
    
    size_t total_gray_pixels =  height1*width1;
    
    
    unsigned char * blob = malloc(total_gray_pixels);
    
    MagickExportImagePixels(wand_ip,      // Image instance
                            0,         // Start X
                            0,         // Start Y
                            width1,     // End X
                            height1,    // End Y
                            "I",       // Value where "I" = intensity = gray value
                            CharPixel, // Storage type where "unsigned char == (0 ~ 255)
                            blob);     // Destination pointer
    
    

    struct image image1 = {blob, height1, width1};

    return image1;
}










int main(int argc, const char * argv[])
{
    
    struct image get_Image(char []);
    
    char filename[] = "/Users/SK_Mac/Downloads/image4.jpg";
    
    struct image image_IP = get_Image(filename);
    
    
    unsigned char img_IP[image_IP.height][image_IP.width];
    
    for (int i = 0; i<image_IP.height; i++)
    {
        for (int j = 0; j<image_IP.width; j++)
        {
            img_IP[i][j] = (int) image_IP.gray_Data[i*(int)image_IP.width+j];
        }
    }
    
    
    int height1 =image_IP.height;
    int width1 = image_IP.width;
    free(image_IP.gray_Data);
    

    //unsigned long height2 = strtol(argv[1], NULL, 0);
    //unsigned long width2 = strtol(argv[2], NULL, 0);
    
    unsigned long height2 = 2000;
    unsigned long width2 = 2000;
    
    MagickWandGenesis();
    
    // Declearation of variables
    // Wand variables

    char hex[128];

    MagickWand *wand_op;

    
    PixelIterator *iterator_op;

    
    

    // Bilineat linterpolation.
    
    unsigned char img_op[height2][width2];
    
    float scale_h = height1/(float)height2;
    float scale_w = width1/(float)width2;

    
    
    for (int i = 0 ; i <height2 ; i++)
    {
        for (int j = 0 ; j< width2 ; j++)
        {
            float height_Frac_Idx = i*scale_h;
            float width_Frac_Idx = j*scale_w;
            int height_idx, width_idx;
            height_idx = (int) height_Frac_Idx;
            width_idx =  (int) width_Frac_Idx;
            float del_h = height_Frac_Idx - height_idx;
            float del_w = width_Frac_Idx - width_idx;
            
            
            if(height_idx>=height1 || width_idx>=width1)
            {
                height_idx = (int) height1-2;
                width_idx = (int) width1-2;
                
            }

            img_op[i][j] = ((img_IP[height_idx][width_idx])*(1 - del_h)*(1-del_w)) + ((img_IP[height_idx+1][width_idx])*(del_h)*(1-del_w)) + ((img_IP[height_idx][width_idx+1])*(1 - del_h)*(del_w)) + ((img_IP[height_idx+1][width_idx+1])*(del_h)*(del_w));
            
        }
        
        
    }
    
    
    // Initialize the wand for final image based one the given height and width.
    
    //Initialize the wand for output window.
    wand_op = NewMagickWand();
    
    PixelWand *p_wand = NULL;
    p_wand = NewPixelWand();
    
    //Set the initial color, it is necessary in MagicWand to have it.
    PixelSetColor(p_wand,"white");
    
    //Configure the output image wand.
    MagickNewImage(wand_op,width2,height2,p_wand);
    iterator_op=NewPixelIterator(wand_op);
    PixelWand **pixels_op;
    for(int i=0;i<height2;i++)
    {
        // Get the next row of the image as an array of PixelWands
        pixels_op= PixelGetNextIteratorRow(iterator_op,&width2);
        // Set the row of wands to a simple gray scale gradient
        for(int j=0;j<width2;j++)
        {
            //PixelSetColorFromWand(wand_op, pixel_op[j1]);
            //unsigned char temp = img_op[i1][j1];
            sprintf(hex,"#%02x%02x%02x",img_op[i][j],img_op[i][j],img_op[i][j]);
            PixelSetColor(pixels_op[j],hex);
        }
        // Sync writes the pixels back to the m_wand
        PixelSyncIterator(iterator_op);
    }
    
    
    
    MagickWriteImage(wand_op,"bits_demo.gif");
    //Display the input image.
    //MagickDisplayImage(wand_ip, ":0");
    
    //Display the output image.
    MagickDisplayImage(wand_op, ":0");
    //wand_ip = DestroyMagickWand(wand_ip);
    wand_op = DestroyMagickWand(wand_op);
    MagickWandTerminus();
    return 0;
}


















