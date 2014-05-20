#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc, char **argv[])
{
  int sockfd, numbytes;
  char buf[100];
  struct sockaddr_in their_addr;
  int i = 0;
  char *he = "hello";
  char *message = NULL;
  int idx = 2;

  if (argc < 2)
    {	
      perror ("Usage:./a.out <server IP>");
      exit (0);
    }
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("socket\n");
      exit (1);
    }

  their_addr.sin_family = AF_INET;
  their_addr.sin_port = htons(2323);
  if (inet_pton(AF_INET, argv[1], &their_addr.sin_addr) <= 0)
    {
      perror ("inet_pton\n");
      exit(0);
    }

  bzero(&(their_addr.sin_zero), 8);

  if (connect(sockfd, (struct sockaddr *)&their_addr,
	      sizeof(struct sockaddr)) == -1)
    {
      perror("connect\n");
      exit (1);	
    } 	
	
  if (argc <= 2)
    {
      message = he;
    }

  do
    {
      if (-1 == send(sockfd, "hello!", 6, 0))
	{	
	  perror ("send\n");
	  exit (1);
	}

    }while((message != he) && (argv[idx]));

  if (-1 == (numbytes = recv(sockfd, buf, 100, 0)))
    {
      perror ("recv\n");
      exit (1);
    }

  buf[numbytes] = '\0';
  printf ("result:%s\n", buf);
  close(sockfd);
  return 0;
}
