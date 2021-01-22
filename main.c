#include <stdio.h>
#include <stdlib.h>
#include<math.h>

#define CUSTOM_IMG_SIZE   1024*1024
int main()
{
   FILE *streamIn =fopen("fruits.bmp","rb");
   if(streamIn ==(FILE*)0)
    printf("unable to open\n");

    printf("x");
    unsigned char header[54];
    unsigned char colorTable[1024];

    for(int i=0;i<54;i++)
        header[i]=getc(streamIn);

    int width = *(int *)&header[18];
    int height = *(int *)&header[22];
    int bitDepth =*(int *)&header[28];
   // printf("%d %d %d",height,width,bitDepth);
   printf("x");
  // fread(colorTable,sizeof(unsigned char),1024,streamIn);
   unsigned char buf [width*height];
  // fread(buf,sizeof(unsigned char),height*width,streamIn);
   printf("x");
   /*...............................brightness.....................................................
   for(int i=0;i<height*width;i++)/
    {
     int temp =buf[i]+30;
     buf[i]=(temp>255)? 255 :temp;
    }
     */
    /*to make negative..................................................................
    for(int i=0;i<height*width;i++)
        buf[i]=255-buf[i];
     */
// Image Reader till than;.........................................................................

//to make histogram of image..............................................................................
/*
FILE *fptr;
fptr =  fopen("image_histogram.txt","wb");
int x=0,y=0,i=0,j=0;
long int ihist[256],sum=0;
float hist[256];
for(i=0;i<256;i++)
    ihist[i]=0;
printf("ihist\n");

for(i=0;i<height*width;i++)
     {
        // printf()
        j= buf[i];
        ihist[j]+=1;
        sum++;
     }
  printf("ihist\n");
  for(int i=0;i<256;i++)
        hist[i]= (float)ihist[i]/(float)sum;

   for(int i=0;i<256;i++)
          fprintf(fptr,"%f\n",hist[i]);

   fclose(fptr);
 */
//.......................Equalize image..............................
/*
//printf("x");
int mx=INT_MIN,mn=INT_MAX;
for(int i=0;i<height*width;i++)
{// printf("%d \n",buf[i]);
   if(buf[i]>mx)
        mx=buf[i];
   if(buf[i]<mn)
       mn=buf[i];
}

printf("%d %d",mx,mn);
float a=1 ;
float b=1;
float temp=0.0;
unsigned char buf_eq[height*width];
for(int i=0;i<height*width;i++)
{   // printf("%d ",buf[i]);
    if(buf[i]>20)
        buf_eq[i]=50.0;
    else
        buf_eq[i]=8.0;


}


*/
//............Rotate image...................................

/*unsigned char buf_rot[width][height];
unsigned char buffer[width][height];
int selected;
printf("Select for rotation :\n 1 for right \n 2 for left \n 3 for 180");
scanf("%d",&selected);

fread(buffer,sizeof(unsigned char),width*height,streamIn);

if(selected==1)
{
    for(int i=0;i<width;i++){
         for(int j=0;j<height;j++)
    {
        buf_rot[j][height-1-i] = buffer[i][j];
    }
    }
  imagWriter("camera_of_Rotate_right.bmp",header,colorTable,buf_rot,bitDepth,height*width);
}

if(selected ==2)
{
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++)
    {
        buf_rot[j][i]=buffer[i][j];

    }
   imagWriter("camera_of_Rotate_left.bmp",header,colorTable,buf_rot,bitDepth,height*width);

}

if(selected==3)
{
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++)
    {
        buf_rot[width-i][j]=buffer[i][j];

    }
   imagWriter("camera_of_Rotate_180.bmp",header,colorTable,buf_rot,bitDepth,height*width);


}
*/
//............................Blurring by Kernel matrix 3X3......................................

int imgSize =  height * width;
unsigned char buffer[imgSize][3];
unsigned char output_buffer[imgSize][3];
FILE *fOut = fopen("baraba_blur_new.bmp","wb");

fwrite(header,sizeof(unsigned char),54,fOut);

  if(bitDepth <=8)
     {
         fread(colorTable,sizeof(unsigned char),1024,streamIn);
         fwrite(colorTable, sizeof(unsigned char),1024,fOut);

     }

for(int i =0;i<imgSize;i++)
     {
         buffer[i][2] = getc(streamIn);
         buffer[i][1] = getc(streamIn);
         buffer[i][0] = getc(streamIn);
     }
      float kernel[3][3] = {{1.0/9.0, 1.0/9.0,1.0/9.0},  ///to sum the average of neighours pixels
                           {1.0/9.0, 1.0/9.0,1.0/9.0},
                           {1.0/9.0, 1.0/9.0,1.0/9.0}
                            };


     for(int x =1 ;x<height-1;x++)
    {
        for(int y =1;y<width -1;y++)
        {
            float sum0 = 0.0;
            float sum1 = 0.0;
            float sum2 = 0.0;
            for(int i =-1;i<=1;i++)
            {
                for(int j=-1;j<=1;j++)
                {
                    sum0  = sum0 + (float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
                    sum1  = sum1 + (float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
                    sum2  = sum2 + (float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];

                }
            }

            output_buffer[(x)*width+(y)][0] =  sum0;
            output_buffer[(x)*width+(y)][1] =  sum1;
            output_buffer[(x)*width+(y)][2] =  sum2;
        }
    }

    for(int i =0;i<imgSize;i++)
    {
        putc(output_buffer[i][2],fOut);
        putc(output_buffer[i][1],fOut);
        putc(output_buffer[i][0],fOut);
    }
    imagWriter("camera_of_blur.bmp",header,colorTable,fOut,bitDepth,height*width);
    fclose(fOut);

//.......................imagewriter..................................................

    //imagWriter("camera_of_Rotate_right.bmp",header,colorTable,buf_rot,bitDepth,height*width);
     fclose(streamIn);




   return 0;
}
void imagWriter(const char *imgName,
                 unsigned char *header,
                 unsigned char *colortable,
                 unsigned char *buf,
                 int bitdepth,
                 int imgsize)
   {
       FILE *fo=fopen(imgName,"wb");

       fwrite(header,sizeof(unsigned char),54,fo);
       fwrite(colortable,sizeof(unsigned char),1024,fo);
       fwrite(buf,sizeof(unsigned char),imgsize,fo);

       fclose(fo);
        printf("success write");

   }
