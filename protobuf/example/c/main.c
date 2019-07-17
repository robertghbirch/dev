#include <stdio.h>
#include <stdlib.h>
#include "addressbook.pb-c.h"

#define MAX_MSG_SIZE 1024


static size_t read_buffer (unsigned max_length, uint8_t *out)
{
  size_t cur_len = 0;
  size_t nread;

  while ((nread=fread(out + cur_len, 1, max_length - cur_len, stdin)) != 0)
  {
    cur_len += nread;
    if (cur_len == max_length)
    {
      fprintf(stderr, "max message length exceeded\n");
      exit(1);
    }
  }
  return cur_len;
}

int main (int argc, const char * argv[]) 
{
  Tutorial__AddressBook msg = TUTORIAL__ADDRESS_BOOK__INIT; // AMessage
  Tutorial__AddressBook *msg_in ;
  void *buf;                     // Buffer to store serialized data
  uint8_t buf_in[MAX_MSG_SIZE];  // From HAL
  unsigned len;                  // Length of serialized data


  printf("line [%d]\r\n", __LINE__) ; 
  size_t msg_len = read_buffer(MAX_MSG_SIZE, buf_in) ;

  printf("line [%d]\r\n", __LINE__) ; 
 
  msg_in = tutorial__address_book__unpack(NULL, msg_len, buf_in);
  if (NULL == msg_in) {
    fprintf(stderr, "error unpacking incoming message\n");
    exit(1);
  }

  printf("line [%d] number of n_people[%d]\r\n", __LINE__, (int)msg_in->n_people) ; 

  for (int i = 0 ; i < msg_in->n_people ; i++) {

    Tutorial__Person *person_in = msg_in->people[i];

    
    printf("Name = [%s]\r\n", person_in->name) ;


    for (int j = 0 ; j < person_in->n_phones ; j++) {
	Tutorial__Person__PhoneNumber *person_phone = person_in->phones[j] ;
	printf("\tPhone [%d] = [%s]\r\n", j, person_phone->number) ;
    }   
  } 
  exit (1) ;

  
  if (argc != 2 && argc != 3)
  {   // Allow one or two integers
    fprintf(stderr,"usage: amessage a [b]\n");
    return 1;
  }
  
  len = tutorial__address_book__get_packed_size(&msg);
  buf = malloc(len);
  tutorial__address_book__pack(&msg,buf);

  for (int i = 0 ; i < ((Tutorial__AddressBook *)(buf))->n_people ; i++) {
	//  printf("The name = [%s]\r\n", 
  } 
  
  fprintf(stderr,"Writing %d serialized bytes\n",len); // See the length of message
  fwrite(buf,len,1,stdout); // Write to stdout to allow direct command line piping
  
  free(buf); // Free the allocated serialized buffer

  return 0;
}
