#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//<LIBUSB>
#include <libusb-1.0/libusb.h>
//</LIBUSB>

//<OPENCV>
#include <opencv/cv.h>
#include <opencv/highgui.h>
//</OPENCV>

#define FRAME_HEIGHT 128*4
#define FRAME_WIDTH 200
int main(int argc, char *argv[])
{

    CvSize imageSize = {FRAME_WIDTH,FRAME_HEIGHT};
    IplImage* image=cvCreateImage(imageSize, IPL_DEPTH_8U, 1);

    // Create windows
    cvNamedWindow("Parallax Camera", CV_WINDOW_AUTOSIZE);


    //Buffer contenant la ligne
    uint8_t line[256];

    //Buffer USB de reception
    unsigned char answer[512];

    // Touche clavier
    char key= 0;

    //--------------------------------------------------
    //Prepare USB
    libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
    libusb_device_handle *dev_handle; //a device handle
    libusb_context *ctx = NULL; //a libusb session
    int r; //for return values libusb_error
    ssize_t cnt; //holding number of devices in list
    r = (int) libusb_init(&ctx); //initialize the library for the session we just declared
    if(r < 0) {
            printf("Init Error %d \n",r); //there was an error
            return 1;
    }
    libusb_set_debug(ctx, 1); //set verbosity level to 3, as suggested in the documentation

    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if(cnt < 0) {
            printf("Get Device Error\n"); //there was an error
            return 1;
    }
    printf("%d Devices in list.\n",cnt);

    dev_handle = libusb_open_device_with_vid_pid(ctx, 0x04D8, 0x0204); //these are vendorID and productID of USB device
    if(dev_handle == NULL){
            printf("Cannot open device\n");
    }else{
            printf("Device Opened\n");
    }
    libusb_free_device_list(devs, 1); //free the list, unref the devices in it

    unsigned char *data = new unsigned char[2]; //data to write
    data[0]=255;data[1]=1;//some dummy values

    int actual,transferred; //used to find out how many bytes were written
    if(libusb_kernel_driver_active(dev_handle, 0) == 1) { //find out if kernel driver is attached
            printf("Kernel Driver Active\n");
            if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
                    printf("Kernel Driver Detached!\n");
    }
    r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
    if(r < 0) {
            printf("Cannot Claim Interface\n");
            libusb_close(dev_handle); //close the device we opened
            libusb_exit(ctx); //needs to be called to end the
            return 1;
    }
    printf("Claimed Interface\n");
    //USB OK
    //--------------------------------------------------


    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {

        //On envoie la commande
        printf("Writing Data...\n");
        r = libusb_bulk_transfer(dev_handle, (1 | LIBUSB_ENDPOINT_OUT), data, 2, &actual, 0); //my device's out endpoint was 2, found with trial- the device had 2 endpoints: 2 and 129
        if(r == 0 && actual == 2){ //we wrote the 4 bytes successfully
                printf("Write successful\n");
        }else{
                printf("Write Error\n");
        }

        //On lit le retour du device
        r = libusb_bulk_transfer(dev_handle, (1 |LIBUSB_ENDPOINT_IN), answer,512,&transferred, 3000);
        printf("Received= %d bytes\n",transferred);
        if (r < 0) {
                printf( "Bulk read error %d\n", r);
        }

        //On prepare l'affichage
        for(int i =0;i<128;++i)
        {
            line[i]=answer[255-i];
            cvRectangle( image, cvPoint(0,i*FRAME_HEIGHT/128), cvPoint(50,(i+1)*FRAME_HEIGHT/128), cvScalar(line[i]), CV_FILLED);
            //cvRectangle( imageLineRight, cvPoint(i*FRAME_WIDTH/128,0), cvPoint((i+1)*FRAME_WIDTH/128,FRAME_HEIGHT), cvScalar(line[i]), CV_FILLED);
            line[i]=answer[i];
            cvRectangle( image, cvPoint(150,i*FRAME_HEIGHT/128), cvPoint(200,(i+1)*FRAME_HEIGHT/128), cvScalar(line[i]), CV_FILLED);
            //cvRectangle( imageLineLeft, cvPoint(i*FRAME_WIDTH/128,0), cvPoint((i+1)*FRAME_WIDTH/128,FRAME_HEIGHT), cvScalar(line[i]), CV_FILLED);
        }


        //On affiche l'image de la ligne
        cvShowImage( "LineScan", image);


        // On attend 10ms
        key = cvWaitKey(50);
    }







    r = libusb_release_interface(dev_handle, 0); //release the claimed interface
    if(r!=0) {
            printf("Cannot Release Interface\n");
            libusb_close(dev_handle); //close the device we opened
            libusb_exit(ctx); //needs to be called to end the
            return 1;
    }
    printf("Released Interface\n");

    libusb_close(dev_handle); //close the device we opened
    libusb_exit(ctx); //needs to be called to end the

    delete[] data; //delete the allocated memory for data

    return EXIT_SUCCESS;
}
