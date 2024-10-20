#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

struct LocationData {
    time_t currentTime;
    int latitude;      
    int longitude;     
    int altitude;
} typedef LocationData;

char* locationToMessage(LocationData location) {
    char* message = (char*)malloc(100 * sizeof(char));
    snprintf(message, 100, "Time: %ld, Latitude: %d, Longitude: %d, Altitude: %d",
             location.currentTime, location.latitude, location.longitude, location.altitude);
    return message;

const char *encrypt(const char *message, int shift) {
    int len = strlen(message);
    char* encryptMessage = (char*)malloc((len + 1) * sizeof(char));
    int i;
    
    for(i=0 ; message[i] != '\0'; i++){
        char ch = message[i];
        if(ch => 'a' && ch <= 'z'){
            encryptMessage[i] = ((ch - 'a' + shift) %26) + 'a';
        }
        else if(ch => 'A' && ch <= 'Z'){
            encryptMessage[i] = ((ch - 'A' + shift) %26) + 'A';
        }
        else{
            encryptMessage[i] = ch;
        }
    }
}

LocationData getLocation() {
    time_t currentTime = time(NULL);
    int latitude = rand() % 180 - 90;
    int longitude = rand() % 360 - 180;
    int altitude = rand() 5000;
    LocationData location = {currentTime, latitude, longitude, altitude};
    return location;
}

void sendMessage(const char *encryptedMessage,const char *serverIP, int serverPort) {

    int sockfd;
    struct sockaddr_in serverAddr;

    // 1. UDP soketi oluşturun
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Sunucu adres yapısını doldurun
    memset(&serverAddr, 0, sizeof(serverAddr));  // Adres yapısını sıfırlama
    serverAddr.sin_family = AF_INET;             // IPv4
    serverAddr.sin_port = htons(serverPort);     // Sunucu portu
    serverAddr.sin_addr.s_addr = inet_addr(serverIP); // Sunucu IP adresi

    // 3. Şifrelenmiş mesajı UDP ile gönderin
    int n = sendto(sockfd, encryptedMessage, strlen(encryptedMessage), 0, 
                   (const struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if (n < 0) {
        perror("Send failed");
    } else {
        printf("Encrypted message sent to %s:%d\n", serverIP, serverPort);
    }

    // 4. Soketi kapatın
    close(sockfd);
    

}

int main() {
    LocationData location = getLocation();

    char* message= locationToMessage(location);
    printf("Mesaj: %s\n",message);

    int shift = 6;
    char* encryptMessage = encrypt(message,shift);
    printf("Şifreli Mesaj: %s\n",encryptMessage);

    const char *serverIP = "127.0.0.1";
    int serverPort = 7000;

    sendMessage(encryptMessage,serverIP,serverPort);
    free(meesage);
    free(encryptMessage);


    return 0;
}
