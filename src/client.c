#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{

	if (argc < 3)
	{
		fprintf(stderr, "Port Number not provided");
		exit(1);
	}

	int sockfd, portno, n;
	char buffer[2000];
	char input[100];
	char inputFiles[100];
	char *ptr;
	char dlim[] = " ";
	char actionLenght[2];
	int actionCharNo = 0;

	struct sockaddr_in serv_addr;
	struct hostent *server;

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		error("Error opening socket");
	}

	server = gethostbyname(argv[1]);
	if (server == NULL)
		fprintf(stderr, "Error, no such host");

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("Connection failed");

	printf("▁ ▂ ▄ ▅ ▆ ▇ █  🆂 🆃 🅰 🅽 🅼 🅰 🆁 🅺 1.0 █ ▇ ▆ ▅ ▄ ▂ ▁\n-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶\n⌈ MENU:\n-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶\n⌈ 1.USE file\n-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶\n⌈ 2.SORT file\n-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶\n⌈ *file: SEQUENCE1.txt, SEQUENCE2.txt, SEQUENCE3.txt\n| *Algorithms used: #Bubble, #Shell, #Selection, #Merge\n-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶-̶\n");

	while (1)
	{

		printf("Client: ");
		fgets(input, 100, stdin);

		strcpy(inputFiles, input);
		ptr = strtok(input, dlim);
		actionCharNo = strlen(ptr);
		sprintf(actionLenght, "%d", actionCharNo);

		strcpy(inputFiles, inputFiles + actionCharNo + 1);

		strcpy(buffer, actionLenght);
		strcpy(buffer + 1, ptr);
		strcpy(buffer + actionCharNo + 1, inputFiles);

		n = write(sockfd, buffer, sizeof(buffer));
		if (n < 0)
			error("Error on writing");

		bzero(buffer, 2000);

		n = read(sockfd, buffer, sizeof(buffer));

		if (n < 0)
			error("Error on reading");
		printf("Server says %s\n", buffer);
		bzero(buffer, 2000);
	}

	close(sockfd);
	return 0;
}