#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main (int argc, char **argv)
{
  int sockfd, new_fd;
  struct sockaddr_in my_addr, their_addr;
  int sin_size, numbytes;
  char buff[100];

  if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
      perror ("socket\n");
      exit (1);
    }

  my_addr.sin_family = AF_INET;
  my_addr.sin_port  = htons (2323);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  bzero (&(my_addr.sin_zero), 8);

  if (-1 == bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)))
    {
      perror ("bind\n");
      exit (1);
    }

  if (-1 == listen(sockfd, 10))
    {
      perror ("listen\n");
      exit (1); 
    }

  while (1)
    {
      sin_size = sizeof(struct sockaddr_in);
      printf ("server %d is run\n", getpid());

      if (-1 == (new_fd = accept(sockfd, (struct sockaddr *)&their_addr, 
				 &sin_size)))
	{
	  perror ("accept\n");
	  exit (1); 
	}

      if (!fork())
	{
	  bzero(buff, 100);
	  if (-1 ==(numbytes = recv(new_fd, buff, 100, 0)))
	    {
	      perror ("recv\n");
	      exit (1); 
	    }
	  printf ("pid %d:%s\n", getpid(), buff);

	  if (-1 == send(new_fd, buff, strlen(buff), 0))
	    {
	      perror ("send\n");
	    }

	  close (new_fd);
	  exit (0); 
	}

      close (new_fd);
    }

}
