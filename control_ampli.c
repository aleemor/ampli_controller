// control_ampli

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <stdbool.h>
#include <netdb.h>

#define serial_baudrate B115200


int serial_file_descriptor,bytes_read;
int comando;

struct state{
  bool powerState;
  bool relay_state;
}stato;

unsigned char read_buffer[sizeof(stato)]; 

//char serial_port[] = "/dev/cu.usbmodem14101";
//char serial_port[] = "/dev/cu.usbserial-FT2K0SI2";
char serial_port[] = "/dev/cu.usbserial-FTHFGYDY";

//char serial_port[] = "/dev/cu.usbserial-1410";
//char serial_port[] = "/dev/ttyACM0";

void serial_initialize() {
	serial_file_descriptor = open(serial_port,O_RDWR );
    if(serial_file_descriptor == -1) {
        printf("Serial port error\n");
    }
    else {
        printf("Serial port opened\n");
    }
    struct termios SerialPortSettings;
    tcgetattr(serial_file_descriptor, &SerialPortSettings);
    cfsetispeed(&SerialPortSettings,serial_baudrate);
    cfsetospeed(&SerialPortSettings,serial_baudrate);
    SerialPortSettings.c_cflag &= ~PARENB;
    SerialPortSettings.c_cflag &= ~CSTOPB;
    SerialPortSettings.c_cflag &= ~CSIZE;
    SerialPortSettings.c_cflag |=  CS8;
    SerialPortSettings.c_cflag &= ~CRTSCTS;
    SerialPortSettings.c_cflag |= CREAD | CLOCAL;
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
    SerialPortSettings.c_iflag &= ~(  ECHO | ECHOE | ISIG);
    SerialPortSettings.c_oflag &= ~OPOST;
    SerialPortSettings.c_cc[VMIN] = 100;
    SerialPortSettings.c_cc[VTIME] = 1;
    if((tcsetattr(serial_file_descriptor,TCSANOW,&SerialPortSettings)) != 0) {
        printf("ERROR ! in Setting attributes\n");
    }
}

void serial_listen(){
    //printf("Serial listening\n");
    bytes_read = read(serial_file_descriptor,&read_buffer,sizeof(stato));
    printf("Serial received: size: %d\n",bytes_read);
    printf("power_state: %d \n", read_buffer[0]);
    printf("relay_state: %d \n", read_buffer[1]);
    if(bytes_read == sizeof(stato) ) {
        memcpy(&stato,read_buffer,sizeof(stato));
    }
}

void show_commands(){
    printf("commands: [1] - State request\n");
    printf("          [2] - Reset\n");
    printf("          [3] - turn ON  power pin\n");
    printf("          [4] - turn OFF power pin\n");
    printf("          [5] - turn ON  relay pin\n");
    printf("          [6] - turn OFF relay pin\n");    
    printf("          [7] - turn ON  sequence\n");    
    printf("          [8] - turn OFF sequence\n");    

}

void command(){
    //uint8_t buff[3];
    show_commands();   
    while (true){
        printf("enter the command: ");
        scanf("%d", &comando);
        switch (comando){
            case 1:
            printf("State request\n");             // non è consentito fare uno statement dopo un label!!!!
            uint8_t buff1[3] = {0x25, 0xaa, 0x26};
            write(serial_file_descriptor,(char *)&buff1,sizeof(buff1));
            serial_listen();
            break;
            case 2:
            printf("Reset\n");
            uint8_t buff2[3] = {0x25, 0xab, 0x26};
            write(serial_file_descriptor,(char *)&buff2,sizeof(buff2));
            break;
            case 3:
            printf("turn ON power pin\n");
            uint8_t buff3[3] = {0x25, 0xac, 0x26};
            write(serial_file_descriptor,(char *)&buff3,sizeof(buff3));
            serial_listen();
            break;
            case 4:
            printf("turn OFF power pin\n");
            uint8_t buff4[3] = {0x25, 0xad, 0x26};
            write(serial_file_descriptor,(char *)&buff4,sizeof(buff4));
            serial_listen();
            break;
            case 5:
            printf("turn ON relay pin\n");
            uint8_t buff5[3] = {0x25, 0xae, 0x26};
            write(serial_file_descriptor,(char *)&buff5,sizeof(buff5));
            serial_listen();
            break;
            case 6:
            printf("turn OFF relay pin\n");
            uint8_t buff6[3] = {0x25, 0xaf, 0x26};
            write(serial_file_descriptor,(char *)&buff6,sizeof(buff6));
            serial_listen();
            break;  
            case 7:
            printf("turn ON  sequence\n");
            uint8_t buff7[3] = {0x25, 0xba, 0x26};
            write(serial_file_descriptor,(char *)&buff7,sizeof(buff7));
            serial_listen();
            break;
            case 8:
            printf("turn OFF  sequence\n");
            uint8_t buff8[3] = {0x25, 0xbb, 0x26};
            write(serial_file_descriptor,(char *)&buff8,sizeof(buff8));
            serial_listen();
            break;   
            default:
            printf("Insert a valid command: \n"); 
            show_commands();


        }
    }
}


int main(void) {

    printf("control_ampli \n");
    serial_initialize();
    command();

    return 0;
}
