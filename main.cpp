
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <cctype>
#include <cstdlib>
#include <windows.h>
#include <shellapi.h>

using namespace std;
namespace fs = std::filesystem;

const int MAX_SONGS = 100;

struct Song {
    int id, year;
    string title, artist, album, audio, lyrics;
};

Song songs[MAX_SONGS];
int songCount = 0;

// Singly linked list for the playback queue.
struct QueueNode {
    Song song;
    QueueNode* next;
};

QueueNode* front = nullptr;
QueueNode* rear = nullptr;

string normalize(const string& text) {
    string result;

    for (unsigned char ch : text) {
        if (isalnum(ch))
            result += static_cast<char>(tolower(ch));
    }

    return result;
}

void clearScreen() {
    system("cls");
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void loadSongs() {
    ifstream file("songs.txt");

    if (!file) {
        cout << "Error: songs.txt could not be opened.\n";
        return;
    }

    string line;

    while (getline(file, line) && songCount < MAX_SONGS) {
        string data[7];
        stringstream ss(line);

        for (int i = 0; i < 7; i++)
            getline(ss, data[i], '|');

        if (data[0].empty() || data[4].empty())
            continue;

        try {
            songs[songCount].id = stoi(data[0]);
            songs[songCount].title = data[1];
            songs[songCount].artist = data[2];
            songs[songCount].album = data[3];
            songs[songCount].year = stoi(data[4]);
            songs[songCount].audio = data[5];
            songs[songCount].lyrics = data[6];
            songCount++;
        }
        catch (...) {
            cout << "Invalid song record skipped: " << line << '\n';
        }
    }

    file.close();
}

void displaySong(const Song& song) {
    cout << "\nID: " << song.id;
    cout << "\nTitle: " << song.title;
    cout << "\nArtist: " << song.artist;
    cout << "\nAlbum: " << song.album;
    cout << "\nYear: " << song.year;
    cout << "\n-----------------------------\n";
}

void displayAll() {
    if (songCount == 0) {
        cout << "\nNo songs found.\n";
        return;
    }

    for (int i = 0; i < songCount; i++)
        displaySong(songs[i]);
}

Song* findSongById(int id) {
    for (int i = 0; i < songCount; i++) {
        if (songs[i].id == id)
            return &songs[i];
    }

    return nullptr;
}

// Linear search by title, artist, or album.
void linearSearch(const string& key, int type) {
    string searchKey = normalize(key);

    if (searchKey.empty()) {
        cout << "\nPlease enter a search keyword.\n";
        return;
    }

    bool found = false;

    for (int i = 0; i < songCount; i++) {
        string value;

        if (type == 1)
            value = songs[i].title;
        else if (type == 2)
            value = songs[i].artist;
        else
            value = songs[i].album;

        if (normalize(value).find(searchKey) != string::npos) {
            displaySong(songs[i]);
            found = true;
        }
    }

    if (!found)
        cout << "\nNo matching song found.\n";
}

// Search lyrics files for a keyword.
void searchLyrics(const string& key) {
    string searchKey = normalize(key);

    if (searchKey.empty()) {
        cout << "\nPlease enter a lyrics keyword.\n";
        return;
    }

    bool found = false;

    for (int i = 0; i < songCount; i++) {
        if (songs[i].lyrics.empty())
            continue;

        string path = "lyrics/" + songs[i].lyrics;
        ifstream file(path);

        if (!file)
            continue;

        string line;
        string content;

        while (getline(file, line))
            content += line + " ";

        file.close();

        if (normalize(content).find(searchKey) != string::npos) {
            displaySong(songs[i]);
            found = true;
        }
    }

    if (!found)
        cout << "\nNo song found with that lyrics keyword.\n";
}

// Open a song using the Windows default application.
bool playAudio(const string& path) {
    if (!fs::exists(path)) {
        cout << "\nAudio file not found: "
             << fs::absolute(path) << '\n';

        return true; // Skip missing files.
    }

    string fullPath = fs::absolute(path).string();

    SHELLEXECUTEINFOA info{};
    info.cbSize = sizeof(info);
    info.fMask = SEE_MASK_NOCLOSEPROCESS;
    info.lpVerb = "open";
    info.lpFile = fullPath.c_str();
    info.nShow = SW_SHOWNORMAL;

    if (!ShellExecuteExA(&info)) {
        DWORD errorCode = GetLastError();

        cout << "\nUnable to open song in the default media player.\n";
        cout << "File: " << fullPath << '\n';
        cout << "Windows error: " << errorCode << '\n';

        return true; // Skip and try the next queued song.
    }

    cout << "\nOpened in default media player: " << path << '\n';

    // Some media apps do not provide a process handle.
    if (info.hProcess == nullptr) {
        cout << "Windows did not provide a media-player process handle.\n";
        cout << "After closing the media player, press Enter here to continue.\n";
        cin.get();
        return true;
    }

    DWORD waitResult = WaitForSingleObject(info.hProcess, INFINITE);
    CloseHandle(info.hProcess);

    if (waitResult == WAIT_FAILED) {
        DWORD errorCode = GetLastError();

        cout << "\nCould not wait for the media player to close.\n";
        cout << "Windows error: " << errorCode << '\n';

        return false;
    }

    return true;
}

void playSong(const string& key) {
    string searchKey = normalize(key);

    if (searchKey.empty()) {
        cout << "\nPlease enter a song title.\n";
        return;
    }

    for (int i = 0; i < songCount; i++) {
        if (normalize(songs[i].title).find(searchKey)
            != string::npos) {
            playAudio("music/" + songs[i].audio);
            return;
        }
    }

    cout << "\nSong not found.\n";
}

// Add a song to the end of the linked-list queue.
void enqueue(const Song& song) {
    QueueNode* newNode = new QueueNode{song, nullptr};

    if (rear == nullptr) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
}

// Remove the first node.
bool dequeue(Song& song) {
    if (front == nullptr)
        return false;

    QueueNode* temp = front;
    song = temp->song;
    front = front->next;

    if (front == nullptr)
        rear = nullptr;

    delete temp;
    return true;
}

void clearQueue() {
    Song temp;

    while (dequeue(temp)) {
    }
}

void displayQueue() {
    if (front == nullptr) {
        cout << "\nQueue is empty.\n";
        return;
    }

    QueueNode* current = front;
    int position = 1;

    cout << "\n--- Playback Queue ---\n";

    while (current != nullptr) {
        cout << position++ << ". "
             << current->song.title << " - "
             << current->song.artist << '\n';

        current = current->next;
    }
}

// Open each song in sequence, moving forward through the list.
void playQueue() {
    QueueNode* current = front;

    while (current != nullptr) {
        cout << "\nNow opening: " << current->song.title << '\n';

        bool canContinue =
            playAudio("music/" + current->song.audio);

        if (!canContinue)
            break;

        current = current->next;
    }

    clearQueue();
    cout << "\nQueue playback finished.\n";
}

void createQueueAndPlay() {
    clearQueue();
    displayAll();

    if (songCount == 0)
        return;

    cout << "\nEnter song IDs separated by spaces: ";

    string input;
    getline(cin, input);

    stringstream ss(input);
    int id;
    bool added = false;

    while (ss >> id) {
        Song* song = findSongById(id);

        if (song != nullptr) {
            enqueue(*song);
            added = true;
        }
        else {
            cout << "Invalid song ID: " << id << '\n';
        }
    }

    if (!added) {
        cout << "\nNo valid songs added to queue.\n";
        return;
    }

    displayQueue();
    playQueue();
}

// Bubble sort by normalized song title.
void bubbleSort(Song list[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (normalize(list[j].title) >
                normalize(list[j + 1].title)) {
                Song temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

void sortSongs() {
    bubbleSort(songs, songCount);

    cout << "\nSongs sorted by title.\n";
    displayAll();
}

// Binary search for an exact normalized title.
void binarySearch(const string& key) {
    if (songCount == 0) {
        cout << "\nNo songs available.\n";
        return;
    }

    string searchKey = normalize(key);

    if (searchKey.empty()) {
        cout << "\nPlease enter a song title.\n";
        return;
    }

    Song sorted[MAX_SONGS];

    for (int i = 0; i < songCount; i++)
        sorted[i] = songs[i];

    bubbleSort(sorted, songCount);

    int low = 0;
    int high = songCount - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        string title = normalize(sorted[mid].title);

        if (title == searchKey) {
            cout << "\nSong found:\n";
            displaySong(sorted[mid]);
            return;
        }
        else if (title < searchKey) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    cout << "\nExact song title not found.\n";
}

int main() {
    loadSongs();

    int choice = 0;

    do {
        clearScreen();

        cout << "===== MUSIC PLAYER SEARCH SYSTEM =====\n";
        cout << "1. Display All Songs\n";
        cout << "2. Search by Song Title\n";
        cout << "3. Search by Artist\n";
        cout << "4. Search by Album\n";
        cout << "5. Search by Lyrics Keyword\n";
        cout << "6. Play a Song\n";
        cout << "7. Create Queue and Play Songs\n";
        cout << "8. Sort Songs by Title\n";
        cout << "9. Binary Search by Exact Title\n";
        cout << "10. Exit\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid input.\n";
            pauseScreen();
            continue;
        }

        cin.ignore(10000, '\n');

        string key;

        switch (choice) {
            case 1:
                displayAll();
                pauseScreen();
                break;

            case 2:
                cout << "Enter song title: ";
                getline(cin, key);
                linearSearch(key, 1);
                pauseScreen();
                break;

            case 3:
                cout << "Enter artist name: ";
                getline(cin, key);
                linearSearch(key, 2);
                pauseScreen();
                break;

            case 4:
                cout << "Enter album name: ";
                getline(cin, key);
                linearSearch(key, 3);
                pauseScreen();
                break;

            case 5:
                cout << "Enter lyrics keyword: ";
                getline(cin, key);
                searchLyrics(key);
                pauseScreen();
                break;

            case 6:
                cout << "Enter song title: ";
                getline(cin, key);
                playSong(key);
                pauseScreen();
                break;

            case 7:
                createQueueAndPlay();
                pauseScreen();
                break;

            case 8:
                sortSongs();
                pauseScreen();
                break;

            case 9:
                cout << "Enter exact song title: ";
                getline(cin, key);
                binarySearch(key);
                pauseScreen();
                break;

            case 10:
                clearQueue();
                cout << "\nExiting Music Player.\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
                pauseScreen();
        }

    } while (choice != 10);

    return 0;
}
