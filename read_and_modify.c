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




int main(int argc, char *argv[]){

   PGMImage image;
   int i,j;
   
    if(argc > 1){
      readPGM(argv[1],&image);
    }
    else{
      printf("\nError(2): Please especify the file name.\n");
      return -2;
   }

   
   
   //Getting the min gray value in the image
   int imageMinGrayVal=image.maxGrayVal;
   for(i = 0; i < image.width*image.height;++i){
      if(image.data[i] < imageMinGrayVal){
         imageMinGrayVal = image.data[i];
      }
   }
   
   
   int diff = image.maxGrayVal - imageMinGrayVal;
   
   
   printf("\nTo darken the image pulse 1, to lighten it pulse 2:\n");
   int var = 0;
   
again:
   scanf("%d",&var);
   
   
   int factor, percent;
   
   switch(var){
      case 1:
         printf("\nOption 1 selected: Darken image");
         printf("\nPlease insert the factor in percent that you want to darken the image\n");

         scanf("%d",&factor);
         percent=(factor*diff)/100;
         printf("\nDarken %d%% the image ...",factor);
         for(i=0; i<image.width*image.height; ++i){
            image.data[i]=(int)image.data[i]-percent <= 0 ? 0 : image.data[i]-percent;
         } 
         //If we modify the max gray value, we are modifiying the image contrast too. 
         //image.maxGrayVal=(int)image.maxGrayVal-percent <= 0 ? 0 : image.maxGrayVal-percent ;   
         printf("\nDone!\n");
         break;
      case 2:
         printf("\nOption 2: Lightem image");
         printf("\nPlease insert the factor in percent that you want to lighten the image\n");
         scanf("%d",&factor);
         percent=(factor*diff)/100;
         printf("\nLighten %d%% the image ...",factor);
         for(i=0; i<image.width*image.height; ++i){
            image.data[i]=(int)image.data[i]+percent > image.maxGrayVal ? image.maxGrayVal : image.data[i]+percent;
         } 
         //If we modify the max gray value, we are modifiying the image contrast too. 
         //image.maxGrayVal=(int)image.maxGrayVal+percent > 255 ? 255 : image.maxGrayVal+percent ;   
         printf("\nDone!\n");
         break;
         break;
      default:
         printf("\n Unrecognized option, try again:\n");
         goto again;
         break;
   }
   
   savePGM(&image);
   
   //Free memory
   free(image.data);

   return 0;   

}










