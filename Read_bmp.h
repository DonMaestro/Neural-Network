#ifndef READ_BMP_H
#define READ_BMP_H

#include <iostream>
#include <fstream>


struct Image
{
	int size;
	int pixels_adress;
	short int bits_per_pixel;

	int width;
	int height;

	float **map;

};


void read_bmp(std::ifstream *file, Image *im)
{
        file->seekg( 2, std::ios::beg);
        file->read ((char*)&im->size, sizeof(int));

        file->seekg( 10, std::ios::beg);
        file->read ((char*)&im->pixels_adress, sizeof(int));


        file->seekg( 18, std::ios::beg);
        file->read ((char*)&im->width, sizeof(int));
        file->read ((char*)&im->height, sizeof(int));

 
        file->seekg( 28, std::ios::beg);
        file->read ((char*)&im->bits_per_pixel, sizeof(short int));
 
        file->seekg(im->pixels_adress, std::ios::beg);
	
	im->map = new float*[im->width];
	for(int number = 0; number < im->width; ++number)
		im->map[number] = new float[im->height];

	
	int bit_p = (int)im->bits_per_pixel / 8;
        unsigned int bgr = 0;

        for(int y = im->height-1; y >= 0; --y)
                for(int x = 0; x < im->width; ++x)
                {
                        file->read ((char*)&bgr, bit_p);
			
                        if ( bgr == 0x000000 )
                                im->map[y][x] = 1.0;
                        else
                                im->map[y][x] = 0.0;

                        bgr = 0;
                }
}


void delete_image(Image *im)
{
	for(int number = 0; number < im->width; ++number)
		delete[] im->map[number];
	delete[] im->map;
}







#endif
