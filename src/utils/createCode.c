#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <wincrypt.h>
    #pragma comment(lib, "advapi32.lib")
#else
    #include <fcntl.h>
    #include <unistd.h>
#endif

void generateCode(char *code) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int charset_size = sizeof(charset) - 1;
    unsigned char random_bytes[4];
    
#ifdef _WIN32
    HCRYPTPROV hProvider = 0;
    CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT);
    CryptGenRandom(hProvider, 4, random_bytes);
    CryptReleaseContext(hProvider, 0);
#else
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd != -1) {
        read(fd, random_bytes, 4);
        close(fd);
    }
#endif
    
    for (int i = 0; i < 4; i++) {
        code[i] = charset[random_bytes[i] % charset_size];
    }
    code[4] = '\0';
}