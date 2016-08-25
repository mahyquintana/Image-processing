/*Mahy Quintana -> www.mahyquintana.com*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
   char name[100];
   char magicnumberLetter;
   int  magicnumberNumber;
   unsigned int width;
   unsigned int height;
   unsigned int maxGrayVal;
   unsigned int *data;
}PGMImage;



void readPGM(char *name, PGMImage *image){

   int i;
   
   if (strlen(name) >= sizeof( (*image).name)){
      printf("\nError(1): The name of the file is too long.\n");
      exit(-1);
   }
   else{
      strcpy((*image).name, name);
   }


   //Open file
   FILE *fp = fopen((*image).name,"r");
   
   if(fp==NULL) {
      printf("\nError(3): Can not open file %s.\n",(*image).name);
      exit(-3); 
   }
   
   //Reading Header
   
   //Get the first part ("P") of the magic number
   (*image).magicnumberLetter = getc(fp);
   if((*image).magicnumberLetter != 'P')
   {
      printf("\nError(4): Not valid file type.\n");
      exit(-4);
   }

   //Get the secong part ("2|5") of the magic number
   (*image).magicnumberNumber = getc(fp)-48; //Conversion: ascii to int
   
   if(((*image).magicnumberNumber != 2) && ((*image).magicnumberNumber != 5))
   {
      printf("\nError(5): Not valid pgm file type.\n");
      exit(-5);
   }

   while(getc(fp) != '\n');           // Ignore the rest of the line

   while (getc(fp) == '#')            // Ignore comment lines
   {
     while (getc(fp) != '\n');        
   }

   fseek(fp, -1, SEEK_CUR);           // Move 1 position back the "cursor"
   

   fscanf(fp,"%u", &(*image).width);   
   fscanf(fp,"%u", &(*image).height);     
   fscanf(fp,"%u", &(*image).maxGrayVal); 
   fseek(fp, 1, SEEK_CUR);            

   printf("\nImage width=%u, ",(*image).width);
   printf("\nImage height=%u, ",(*image).height);
   printf("\nMaximun gray value=%u\n\n ",(*image).maxGrayVal);


   //Memory alloc for the image data
   (*image).data=(unsigned int*)malloc((*image).width*(*image).height*sizeof(unsigned int));
   
   if((*image).data==NULL) {
      printf("\nError(6): No memory.\n");
      exit(-6);
   }
   
  
   //Reading image data
   for(i=0;i<(*image).width*(*image).height;++i){
      fscanf(fp,"%u",&((*image).data[i]));
   }


   //Close file
   fclose(fp);

}




void savePGM(PGMImage *image){

   int i, j;
   FILE *fp;
   fp = fopen("image_out.pgm","w+");
   
   //Writing header
   fprintf(fp,"P2\n");
   fprintf(fp,"%d %d\n",(*image).width,(*image).height);
   fprintf(fp,"%d\n",(*image).maxGrayVal);
   
   //Writing data
   for (i = 0; i < (*image).height; ++i){
      for (j=0; j < (*image).width;++j)   
         fprintf(fp,"%u ",(*image).data[i*(*image).width+j]);
      fprintf(fp,"\n");
   }

   fclose(fp);

}



void printHistogram(int *histogram, unsigned int maxGrayVal){

   int i, j, maxRepetitions;
   
   //Get the gray tone with more aparitions in the image
   maxRepetitions=0;
   for(i=0;i<maxGrayVal;++i){
      if(histogram[i]>maxRepetitions){
         maxRepetitions=histogram[i];
      }
   }
   
   //Image dimentions:
   // X axis = maxGrayVal ==> maxGrayVal*5 ==> 256*5=1280
   // Y axis = maxRepetitions 
   
   
   FILE *fp;
   fp = fopen("histogram.pgm","w+");
   
   //Writing header
   fprintf(fp,"P2\n");
   fprintf(fp,"%d %d\n",maxGrayVal*5,maxRepetitions);
   fprintf(fp,"%d\n",1);
   
   //Writing data
   for (i = maxRepetitions; i>=0; --i){
      for (j=0; j < maxGrayVal;++j){   
         if(histogram[j]>=i){
            fprintf(fp,"%u %u %u %u %u ",1,1,1,1,1); //Bar width = 5 pixel
         }else{
            fprintf(fp,"%u %u %u %u %u ",0,0,0,0,0);
         }
      }
      fprintf(fp,"\n");
   }

   fclose(fp);
   
   
/*------------------------------------------------------------------------*/   

   fp = fopen("histogram_2.pgm","w+");
   
   
   for(i=0;i<maxGrayVal;++i)
      histogram[i]= (int)((histogram[i]*640)/maxRepetitions);
   
   maxRepetitions = 640;
   
   
   //Writing header
   fprintf(fp,"P2\n");
   fprintf(fp,"%d %d\n",maxGrayVal*5,maxRepetitions);
   fprintf(fp,"%d\n",1);
   
   //Writing data
   for (i = maxRepetitions; i>=0; --i){
      for (j=0; j < maxGrayVal;++j){   
         if(histogram[j]>=i){
            fprintf(fp,"%u %u %u %u %u ",1,1,1,1,1); //Bar width = 5 pixel
         }else{
            fprintf(fp,"%u %u %u %u %u ",0,0,0,0,0);
         }
      }
      fprintf(fp,"\n");
   }

   fclose(fp);
   
   
   
   
   
   
}




int main(int argc, char *argv[]){

   PGMImage image;
   int i;
   
    if(argc > 1){
      readPGM(argv[1],&image);
    }
    else{
      printf("\nError(2): Please especify the file name.\n");
      return -2;
   }


   int *histogram = (int*)calloc(image.maxGrayVal+1,sizeof(unsigned int));
   
   for(i=0;i<image.width*image.height;++i){
      histogram[image.data[i]]++;
   }
   
   printHistogram(histogram, image.maxGrayVal);
  
   savePGM(&image);
   
   //Free memory
   free(image.data);

   return 0;   

}










