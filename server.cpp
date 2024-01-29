#include <iostream>
#include <csignal>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>

#include "Tuple.h"
#include "TupleTemplate.h"

using namespace std;

vector<Tuple> tuples;

void handleTSoutput(string data) {
    size_t pos = data.find('~');
    string pid = data.substr(0, pos);
    cout << "pid: " << pid << endl;
    data.erase(0, pos+1);

    Tuple tuple = Tuple(data);
    tuples.push_back(tuple);
    cout << "Dodano krotkę: " << tuple.toString() << endl;
}

int searchTuple(TupleTemplate tupleTemplate) {
    int n_elements = tupleTemplate.elements.size();
    for (int i=0; i<tuples.size(); i++) {
        Tuple tuple = tuples[i];
        if (tuple.elements.size() == n_elements) {
            int j;
            for (j=0; j<n_elements; j++) {
                auto element = tuple.elements[j];
                if (auto *str = get_if<string>(&element)) {
                    if (tupleTemplate.elements[j] != "str") break;
                } else if (auto *integer = get_if<int>(&element)) {
                    if (tupleTemplate.elements[j] != "int") break;
                } else if (auto *floating = get_if<float>(&element)) {
                    if (tupleTemplate.elements[j] != "fl") break;
                }
            }
            if (j == n_elements) return i;
        }
    }
    return -1;
}

void handleTSinputTSread(string data, bool isInput) {
    size_t pos = data.find('~');
    string pid = data.substr(0, pos);
    cout << "pid: " << pid << endl;
    data.erase(0, pos+1);

    TupleTemplate tupleTemplate = TupleTemplate(data);
    cout << "Szukanie krotki: " << tupleTemplate.toString() << endl;

    const string clientPath = "fifo-client" + pid;
    int fifo_fd = open(clientPath.c_str(), O_WRONLY);
    if ( fifo_fd == -1) {
        perror("Błąd przy otwieraniu potoku klienta");
        return;
    }
    string message;
    int index = searchTuple(tupleTemplate);
    if (index == -1) {
        cout << "Nie znaleziono krotki" << endl;
        message = "0~Nie znaleziono pasującej krotki";
        write(fifo_fd, message.c_str(), message.length());
    } else {
        Tuple tuple = tuples[index];
        cout << "Zwroócono krotkę: " << tuple.toString() << endl;
        message = "1~" + tuple.toString();
        write(fifo_fd, message.c_str(), message.length());
        if (isInput) tuples.erase(tuples.begin() + index);
    }
    close(fifo_fd);
}

int main() {
    string serverPath = "fifo-server";
    if (mkfifo(serverPath.c_str(), 0666) == -1) {
        perror("Błąd przy tworzeniu FIFO");
        return 1;
    }
    int server_fd = open(serverPath.c_str(), O_RDONLY | O_NONBLOCK);
    if ( server_fd == -1) {
        perror("Błąd przy otwieraniu potoku");
        return 1;
    }
    cout << "Serwer uruchomiony" << endl;

    while (true) {
        char buffer[256];
        ssize_t bytesRead = read(server_fd, buffer, sizeof(buffer) - 1);
        if (bytesRead == -1 || bytesRead == 0) {
            continue;
        } else {
            buffer[bytesRead] = '\0';
            string data(buffer, bytesRead);
            cout << "\n---klient--->  " << data <<endl;
            char command = data[0];
            string noCommandData = data.substr(2,bytesRead);
            if (command == '4') {
                cout << "Zamykanie serwera..." << endl;
                break;
            }
            else if (command == '1') {
                handleTSoutput(noCommandData);
            }
            else if (command == '2') {
                handleTSinputTSread(noCommandData, true);
            }
            else if (command == '3') {
                handleTSinputTSread(noCommandData, false);
            }
            else {
                perror("Nieznana komenda");
                break;
            }
        }
    }
    close(server_fd);
    if (remove(serverPath.c_str()) != 0) {
        perror("Błąd przy usuwaniu FIFO");
        return 1;
    }
    tuples.clear();
    cout << "Zamknięto serwer" << endl;
    return 0;
}