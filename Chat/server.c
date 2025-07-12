#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];

int main()
{
	struct sockaddr_in server_addr;
	int s_fd, new_socket;
	socklen_t addr_len = sizeof(server_addr);
	char buffer[1024];

	fd_set readfds;

	for (int i = 0; i <MAX_CLIENTS; i++)
	{
		client_sockets[i] = 0;
	}

	s_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (s_fd == 0)
	{
		perror("Socket Failed");
		
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if (bind(s_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("Bind Error");
		exit(1);
	}

	listen(s_fd, 5);
	
	printf("Server started on port %d\n", PORT);

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(s_fd, &readfds);
		int max_sd = s_fd;

		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			int sd = client_sockets[i];
			
			if (sd > 0)
				FD_SET(sd, &readfds);
		
			if (sd > max_sd)
				max_sd = sd;
		}

		select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if (FD_ISSET(s_fd, &readfds))
		{
			new_socket = accept(s_fd, (struct sockaddr *)&server_addr, &addr_len);
			
			printf("New connectioon: socket fd %d, IP: %s", new_socket, inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
		
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (client_sockets[i] == 0)
				{
					client_sockets[i] = new_socket;
					
					break;
				}
			}
		}

		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			int sd = client_sockets[i];

			if (FD_ISSET(sd, &readfds))
			{
				int val_read = read(sd, buffer, 1024);

				if (val_read == 0)
				{
					getpeername(sd, (struct sockaddr*)&server_addr, &addr_len);
					printf("Client disconnected: IP %s", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
				
					close(sd);
					client_sockets[i] = 0;
				}

				else
				{
					buffer[val_read] = '\0';
					for (int j = 0; j < MAX_CLIENTS; j++)
					{
						if (client_sockets[j] != 0 && client_sockets[j] != sd)
						{
							send(client_sockets[j], buffer, strlen(buffer), 0);
						}
					}
				}
			}
		}
	}	

	close(s_fd);

	return 0;
}
