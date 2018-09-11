#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//open, write, read

int main() {
  //open returns an int based on whether or not it's opened the filename
  printf("Starting the testing: \n");
  int file;
  file = open("/dev/myRand");
  if (file == -1) {
    printf("Open failed \n");
    exit(1);
  } else {
    printf("Opened successfully \n");
  }
  int numBytesRead;
  char * buf = "Yo";
  size_t count = 1;
  numBytesRead = read(file, buf, count);
  if (numBytesRead == -1) {
    printf("Error on read \n");
  } else {
    printf("Number of bytes read is: " + numBytesRead + "\n");
  }
}
