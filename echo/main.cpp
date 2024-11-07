#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cstdlib> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;
//-----------------------------------------------------------------------------------------
void print_err(const char* why, const int exitCode=1){      //для вывода, обработки ошибок
    cerr<< why<<endl;                                                   
    exit(exitCode);}
//-----------------------------------------------------------------------------------------

int main(int argc, char **argv){
    
//подготовка структуры с адресом нашей программы
    sockaddr_in * selfAddr = new (sockaddr_in);              //указатель на объект структуры
    
    selfAddr->sin_family = AF_INET;                          //в 1 поле записывается инет протокол
    selfAddr->sin_port = 0;                                  //типа тут любой порт выбирается на усмотрение операционки
    selfAddr->sin_addr.s_addr = 0;                           // все адреса компа
//подготовка структуры с адресом cервера 
    sockaddr_in * remoteAddr = new (sockaddr_in);            //указатель на объект структуры
    remoteAddr->sin_family = AF_INET;                        //в 1 поле записывается инет протокол
    remoteAddr->sin_port = htons(7);                         //порт 7
    remoteAddr->sin_addr.s_addr = inet_addr("172.16.40.2"); //адрес локальный 172.16.40.1
    
    
//подготовим буфер для передачи и приема даных
    char *buf = new char[256];
    strcpy(buf, "Привет\n");                                      //скопировали строку в буфер
    int msgLen = strlen(buf);                                     //вычисляем длину строки

//создать сокет
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);                 //UDP
    if (mySocket == -1){
    print_err("Ошибка открытия сокета", 11);
    }

//связать сокет с адресом 
    int rc = bind(mySocket, (const sockaddr*) selfAddr, sizeof (sockaddr_in));
    if (rc == -1 ){
    close(mySocket);
    print_err("Ошибка: не получилось связать сокет с локальным адресом", 12);
    }
    
//установить соединение 
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if  (rc == -1 ){
    close(mySocket);
    print_err("Ошибка: не получилось установить соединение", 13);
     }
     
//передать данные 
    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1){
        close(mySocket);
    print_err("Ошибка: не получилось отправить сообщение", 14);}
        
//принять ответ
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1){
    close(mySocket);
    print_err("Ошибка: не получилось принять ответ", 15);}
    
buf[rc]='\0';
cout<<"Ответ: "<<buf<<endl;

//закрыть сокет
close(mySocket);

delete selfAddr;
delete remoteAddr;
delete[] buf;
return 0;
    }  
