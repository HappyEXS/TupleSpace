#include <iostream>
#include <csignal>
#include <sys/stat.h>
#include <fcntl.h>

#include "Tuple.h"
#include "TupleTemplate.h"

using namespace std;

string g_ServerFifoPath = "fifo-server";

void printInfo(int pid) {
    cout << "Witaj w przestrzeni TupleSpace"<< pid << endl
         << "Proces klienta PID: "<< pid << endl
         << "Możliwe do wykanania operacje:" << endl
         << "1) TSoutput -> (krotka) - umieszczenie krotki w przestrzeni" << endl
         << "2) TSinput -> (wzorzec) - pobiera i usuwa krotke z przestrzeni" << endl
         << "3) TSread -> (wzorzec) - pobiera krotke z przestrzeni" << endl
         << "4) exit" << endl;
}

void handleTSoutput(int pid) {
    string tupleStr;
    cout << "-> Wpisz krotkę do przesłania: ";
    getline(cin, tupleStr);

    int server_fd = open(g_ServerFifoPath.c_str(), O_WRONLY);
    if ( server_fd == -1) {
        perror("Błąd przy otwieraniu potoku");
        return;
    }
    Tuple tuple = Tuple(tupleStr);
    if (tuple.isValid) {
        cout << "przesłano do sewera: " << tuple.toString() << endl;
        string message = "1~" + to_string(pid) + "~" + tuple.toString();
        write(server_fd, message.c_str(), message.length());
    }
    else {
        cout << "Błąd przy parsowaniu krotki. Niepoprawny format" << endl;
    }
    close(server_fd);
}

void handleTSinputTSread(int pid, bool isInput ) {
    string tupleTemplateStr;
    cout << "-> Wpisz wzorzec krotki do pobrania: ";
    getline(cin, tupleTemplateStr);

    int server_fd = open(g_ServerFifoPath.c_str(), O_WRONLY);
    if ( server_fd == -1) {
        perror("Błąd przy otwieraniu potoku serwera");
        return;
    }
    TupleTemplate tupleTemplate = TupleTemplate(tupleTemplateStr);
    if (tupleTemplate.isValid) {
        string com;
        if (isInput) com = "2~"; else com = "3~";
        string message = com + to_string(pid) + "~" + tupleTemplate.toString();
        write(server_fd, message.c_str(), message.length());
    }
    else {
        cout << "Błąd przy parsowaniu wzorca. Niepoprawny format" << endl;
        return;
    }
    close(server_fd);

    const string fifoPath = "fifo-client" + to_string(pid);
    int fifo_fd = open(fifoPath.c_str(), O_RDONLY);
    if ( fifo_fd == -1) {
        perror("Błąd przy otwieraniu potoku klienta");
        return;
    }
    char buffer[256];
    ssize_t bytesRead = read(fifo_fd, buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';
    string data(buffer, bytesRead);
    cout << "server: " << data.substr(2) << endl;
    close(fifo_fd);
}


void handleServerShutDown(int pid) {
    int server_fd = open(g_ServerFifoPath.c_str(), O_WRONLY);
    if ( server_fd == -1) {
        perror("Błąd przy otwieraniu potoku");
        return;
    }
    string message = "4~" + to_string(pid);
    write(server_fd, message.c_str(), message.length());
    close(server_fd);
}


int main() {
    pid_t pid = getpid();
    // string fifoPath = "/tmp/client" + to_string(pid);
    const string fifoPath = "fifo-client" + to_string(pid);
    cout << fifoPath << endl;
    if (mkfifo(fifoPath.c_str(), 0666) == -1) {
        perror("Błąd przy tworzeniu FIFO");
        return 1;
    }
    printInfo(pid);
    while (true) {
        string command;
        cout << "\n-> Wpisz komendę: ";
        getline(cin, command);
        if (command == "exit") break;
        else if (command == "TSoutput") {
            handleTSoutput(pid);
        }
        else if (command == "TSinput") {
            handleTSinputTSread(pid, true);
        }
        else if (command == "TSread") {
            handleTSinputTSread(pid, false);
        }
        else if (command == "sd") {
            handleServerShutDown(pid);
        } else {
            cout << "Niepoprawna komenda!"<< endl;
        }
    }
    if (remove(fifoPath.c_str()) != 0) {
        perror("Błąd przy usuwaniu FIFO");
        return 1;
    }
    cout << "Zamykanie klienta PID:" << pid << endl;
    return 0;
}