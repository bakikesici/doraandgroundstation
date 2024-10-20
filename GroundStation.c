#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

// Mesajı almak için bir fonksiyon
const char* receiveMessage(int port) {
    static char buffer[1024];
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Socket oluştur
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket oluşturma başarisiz");
        exit(EXIT_FAILURE);
    }

    // Sunucu adresini ayarla
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Tüm arayüzlerden gelen bağlantıları kabul et
    serverAddr.sin_port = htons(port);  // Port numarasını ayarla

    // Socket'i bağla
    if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind başarısız");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Mesajı al
    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, 
                     (struct sockaddr *) &clientAddr, &addrLen);
    if (n < 0) {
        perror("Alma başarısız");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';  // Null karakter ile sonlandır

    close(sockfd);  // Soketi kapat

    return buffer;  // Alınan mesajı döndür
}

// Mesajı şifre çözmek için bir fonksiyon
const char* decrypt(const char* encryptedMessage) {
    int shift = 6;  // Şifre çözme kaydırma miktarı
    int len = strlen(encryptedMessage);
    char* decryptMessage = (char*)malloc((len + 1) * sizeof(char));
    if (decryptMessage == NULL) {
        perror("Bellek ayırma başarısız");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < len; i++) {
        char ch = encryptedMessage[i];
        if (ch >= 'a' && ch <= 'z') {
            decryptMessage[i] = ((ch - 'a' - shift + 26) % 26 + 'a');  // Küçük harfler için
        } else if (ch >= 'A' && ch <= 'Z') {
            decryptMessage[i] = ((ch - 'A' - shift + 26) % 26 + 'A');  // Büyük harfler için
        } else {
            decryptMessage[i] = ch;  // Diğer karakterler için
        }
    }
    decryptMessage[len] = '\0';  // Null karakter ile sonlandır
    return decryptMessage;  // Çözülen mesajı döndür
}

// Mesajı ekrana yazdıran bir fonksiyon
void displayMessage(const char* message) {
    printf("Çözülmüş şifre: %s\n", message);
}

int main() {
    int serverPort = 7000;  // Sunucu portu
    const char* encryptedMessage = receiveMessage(serverPort);  // Mesajı al
    printf("Alınan şifreli mesaj: %s\n", encryptedMessage);  // Alınan mesajı yazdır

    const char* decryptedMessage = decrypt(encryptedMessage);  // Şifreyi çöz
    displayMessage(decryptedMessage);  // Çözülmüş mesajı göster

    free((void*)decryptedMessage);  // Çözülmüş mesaj için belleği serbest bırak

    return 0;
}
