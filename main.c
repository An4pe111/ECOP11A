#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
//o html é no formato latino, por isso é necessário usar o charset=iso-8859-1
const char *html_content = 
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=utf8\r\n\r\n"
"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Servidor Anape</title></head>"
"<body>"
"<p>Este é um servidor web simples.</p>"
"<p>Ele foi criado para testar a comunicação entre um cliente e um servidor.</p>"
"<p>Para testar, abra um navegador e acesse o endereço <strong>http://localhost:8080</strong>.</p>"
"<p>Se tudo estiver funcionando corretamente, você verá a mensagem <strong>Hello, World!</strong> em negrito.</p>"
"<p>Este é um exemplo de resposta HTTP.</p>"
"<h1>Hello, World!</h1>"
"</body>"
"</html>";

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    read(client_socket, buffer, BUFFER_SIZE);
    write(client_socket, html_content, strlen(html_content));
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        handle_client(client_socket);
    }

    return 0;
}