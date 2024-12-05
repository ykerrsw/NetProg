#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno> // для perror


void print_err(const char* why, int exitCode = 1) {
    perror(why); // используем perror для вывода ошибки
    exit(exitCode);
}

int main(int argc, char** argv) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        print_err("Ошибка открытия сокета");
    }

    sockaddr_in srv_addr; // Убрали new
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(7);
    srv_addr.sin_addr.s_addr = inet_addr("172.16.40.1");

    int rc;
    rc = connect(s, (const sockaddr*)&srv_addr, sizeof(srv_addr));
    if (rc == -1) {
        close(s);
        print_err("Ошибка соединения");
    }

    const char* msg = "привет";
    rc = send(s, msg, strlen(msg) + 1, 0); // +1 для завершающего нуля
    if (rc == -1) {
        close(s);
        print_err("Ошибка отправки");
    }

    char returned_msg[50];
    rc = recv(s, returned_msg, sizeof(returned_msg), 0);
    if (rc <= 0) { // проверяем на 0 и -1
        close(s);
        print_err("Ошибка приема");
    }
    returned_msg[rc] = '\0'; // добавляем завершающий нуль


    std::cout << "Сообщение: \"" << returned_msg << "\" было получено!\n";

    close(s);
    return 0;
}

