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
    sockaddr_in * selfAddr = new (sockaddr_in);              
    selfAddr->sin_family = AF_INET;                          
    selfAddr->sin_port = 0;                                   
    selfAddr->sin_addr.s_addr = 0;                           
//подготовка структуры с адресом cервера  
     sockaddr_in remoteAddr{};
        remoteAddr.sin_family = AF_INET;
        remoteAddr.sin_port = htons(13);
        remoteAddr.sin_addr.s_addr = inet_addr("172.16.40.1"); 
     
     
//подготовим буфер для передачи и приема даных 
    char *buf = new char[256]; 
//int msgLen = strlen(buf);                                     //вычисляем длину строки 
   // int msgLen = strlen(buf); 
//создать сокет 
    int mySocket = socket(AF_INET, SOCK_DGRAM, 0);                 //UDP 
    if (mySocket == -1){ 
    print_err("Ошибка открытия сокета", 11); 
    } 
 
//связать сокет с адресом  
    int rc = bind(mySocket, (const sockaddr*) selfAddr, sizeof (sockaddr_in)); 
    if (rc == -1 ){ 
    close(mySocket); 
    print_err("Ошибка: не получилось связать сокет с локальным адресом", 12); 
    } 

//передать данные  
    rc = sendto(mySocket, buf, 256, 0, (struct sockaddr*)&remoteAddr, sizeof(sockaddr));              
    if (rc == -1){ 
        close(mySocket); 
    print_err("Ошибка: не получилось отправить сообщение", 14);} 
         
//принять ответ 
	socklen_t adr_len=sizeof(sockaddr);
    rc = recvfrom(mySocket, buf, 256, 0, (struct sockaddr*)&selfAddr, &adr_len); 
    if (rc == -1){ 
    close(mySocket); 
    print_err("Ошибка: не получилось принять ответ", 15);} 
     
buf[rc]='\0'; 
cout<<"Ответ: "<<buf<<endl; 
 
//закрыть сокет 
close(mySocket); 
 
delete selfAddr; 
delete[] buf; 
return 0; 
    }  
