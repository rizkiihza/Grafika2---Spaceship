#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

int main() {
	int fbfd = 0;
  	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
  	char *fbp = 0;
  	int x = 0, y = 0;
  	long int location = 0;

  	// Open the file for reading and writing
  	fbfd = open("/dev/fb0", O_RDWR);
  	if (fbfd == -1) {
  		perror("Error: cannot open framebuffer device");
		exit(1);
	}
	
	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
			fbfd, 0);
	if ((int)fbp == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");

	//read char
	int charlength = 120;
	int charheight = 60;

	char **pixelmap = (char **)malloc(charheight * sizeof(char *));
	for (int i=0; i<charheight; i++)
		pixelmap[i] = (char *)malloc(charlength * sizeof(char));
	
	FILE *charmap;

	//baca map untuk pixel karakter
	charmap = fopen("pesawat.txt", "r");
	for (int i = 0; i < charheight; i++) {
			fscanf (charmap, "%s", pixelmap[i]);
		}
	fclose;

	// Figure out where in memory to put the pixel
	int first_y_pesawat = 100; //y awal;
	int first_x_pesawat = (int)(vinfo.xres)-200;
	int first_y_blok1 = (int)(vinfo.yres)-200;
	int first_x_blok1 = (int)(vinfo.xres)/2;
	int height_blok1 = 80;
	int width_blok1 = 20;
	int current_y_pesawat = first_y_pesawat; //y untuk karakter sementara
	int current_x_pesawat = first_x_pesawat; //x untuk karakter sementara
	int current_y_blok1 = first_y_blok1;
	int current_x_blok1 = first_x_blok1;
	for (int i = 0; i < 25; i++) {
		//menghitamkan layar
		for (y = 0; y < vinfo.yres; y++) {
			for (x = 0; x < vinfo.xres; x++) {
				location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
						(y+vinfo.yoffset) * finfo.line_length;
				/*
				if ((y >= (current_y_blok1 - height_blok1)) || (y <= (current_y_blok1 + height_blok1))) {
					if ((x >= (current_x_blok1 - width_blok1)) || ((x <= (current_x_blok1 + width_blok1)))) {
						*(fbp + location) = 0;        // kuning
						*(fbp + location + 1) = 255;     // kuning
						*(fbp + location + 2) = 255;    // kuning
						*(fbp + location + 3) = 0;      // No transparency
					}			
				} else {
					*/
					*(fbp + location) = 0;        // hitam
					*(fbp + location + 1) = 0;     // hitam
					*(fbp + location + 2) = 0;    // hitam
					*(fbp + location + 3) = 0;      // No transparency	
				//}
			}
		}
		//menggambar blok1
		for (y = current_y_blok1; y < current_y_blok1+height_blok1; y++) {
			if (y >= 0) {
				for (x = current_x_blok1; x < current_x_blok1+width_blok1; x++) {
					location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
							(y+vinfo.yoffset) * finfo.line_length;
					if (vinfo.bits_per_pixel == 32) {
						*(fbp + location) = 0;        // putih
						*(fbp + location + 1) = 255;     // putih
						*(fbp + location + 2) = 255;    // putih
						*(fbp + location + 3) = 0;      // No transparency
					}
				}
			}
		}
		current_x_pesawat -= 60;
		current_y_blok1 -= 60;
		usleep(500000);
	}

	munmap(fbp, screensize);

	close(fbfd);
	
	return 0;

}