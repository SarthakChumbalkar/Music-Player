#include <iostream>    // for input and output
#include <fstream>     // for file operations
#include <sstream>     // for reading database fields
#include <vector>      // for storing songs
#include <string>      // for handling text
#include <filesystem>  // for file paths
#include <cctype>      // for character handling
#include <cstdlib>     // for system commands

using namespace std;
namespace fs = std::filesystem;

// Store details of each song
struct Song {
    int id, year;
    string title, artist, album, audio, lyrics;
};

vector<Song> songs;

// Remove spaces and special characters for searching
string normalize(string text) {
    string result;

    for (unsigned char c : text) {
        if (isalnum(c))
            result += static_cast<char>(tolower(c));
    }

    return result;
}

// Clear the terminal screen
void clearScreen() {
    system("cls");
}

// Wait before returning to the menu
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

// Load song details from songs.txt
void loadSongs() {
    ifstream file("songs.txt");
    string line;

    if (!file) {
        cout << "Cannot open songs.txt\n";
        return;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> data;
        string field;

        while (getline(ss, field, '|'))
            data.push_back(field);

        if (data.size() != 7)
            continue;

        try {
            Song s;

            s.id = stoi(data[0]);
            s.title = data[1];
            s.artist = data[2];
            s.album = data[3];
            s.year = stoi(data[4]);
            s.audio = data[5];
            s.lyrics = data[6];

            songs.push_back(s);
        }
        catch (...) {
            cout << "Skipping invalid song data.\n";
        }
    }
}

// Display details of one song
void displaySong(const Song& s) {
    cout << "\n------------------------------\n";
    cout << "ID     : " << s.id << '\n';
    cout << "Title  : " << s.title << '\n';
    cout << "Artist : " << s.artist << '\n';
    cout << "Album  : " << s.album << '\n';
    cout << "Year   : " << s.year << '\n';
    cout << "------------------------------\n";
}

// Display all songs
void displayAll() {
    for (const Song& s : songs)
        displaySong(s);
}

// Linear Search by title, artist, or album
void linearSearch(string key, int type) {
    key = normalize(key);
    bool found = false;

    if (key.empty()) {
        cout << "Enter a valid keyword.\n";
        return;
    }

    for (const Song& s : songs) {
        string value;

        if (type == 1)
            value = s.title;
        else if (type == 2)
            value = s.artist;
        else
            value = s.album;

        if (normalize(value).find(key) != string::npos) {
            displaySong(s);
            found = true;
        }
    }

    if (!found)
        cout << "No matching song found.\n";
}

// Linear Search through lyrics files
void searchLyrics(string key) {
    key = normalize(key);
    bool found = false;

    if (key.empty()) {
        cout << "Enter a valid keyword.\n";
        return;
    }

    for (const Song& s : songs) {
        ifstream file(fs::path("lyrics") / s.lyrics);
        string line, text;

        if (!file)
            continue;

        while (getline(file, line))
            text += line;

        // Ignore spaces and punctuation in lyrics
        if (normalize(text).find(key) != string::npos) {
            displaySong(s);
            found = true;
        }
    }

    if (!found)
        cout << "No matching lyrics found.\n";
}

// Open a song in the default Windows music player
void playSong(string key) {
    key = normalize(key);

    if (key.empty()) {
        cout << "Enter a valid song title.\n";
        return;
    }

    for (const Song& s : songs) {
        if (normalize(s.title).find(key) != string::npos) {
            fs::path path = fs::path("music") / s.audio;

            if (!fs::exists(path)) {
                cout << "Music file not found: "
                     << path.string() << '\n';
                return;
            }

            string command = "start \"\" \"" + path.string() + "\"";
            system(command.c_str());

            cout << "Opened: " << s.title << '\n';
            return;
        }
    }

    cout << "Song not found.\n";
}

// Bubble Sort: arrange songs alphabetically by title
void bubbleSort(vector<Song>& list) {
    int n = static_cast<int>(list.size());

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (normalize(list[j].title) >
                normalize(list[j + 1].title)) {

                Song temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;

                swapped = true;
            }
        }

        // Stop if the list is already sorted
        if (!swapped)
            break;
    }
}

// Sort songs using our own Bubble Sort
void sortSongs() {
    bubbleSort(songs);

    cout << "Songs sorted by title.\n";
    displayAll();
}

// Binary Search for an exact song title
void binarySearch(string key) {
    key = normalize(key);

    if (key.empty()) {
        cout << "Enter a valid song title.\n";
        return;
    }

    // Sort a copy before performing Binary Search
    vector<Song> sorted = songs;
    bubbleSort(sorted);

    int low = 0;
    int high = static_cast<int>(sorted.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        string title = normalize(sorted[mid].title);

        if (title == key) {
            displaySong(sorted[mid]);
            return;
        }
        else if (title < key) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    cout << "Song not found.\n";
}

// Main menu
int main() {
    loadSongs();

    if (songs.empty()) {
        cout << "No songs loaded. Check songs.txt.\n";
        return 0;
    }

    string choice, key;

    do {
        clearScreen();

        cout << "====================================\n";
        cout << "       MUSIC PLAYER SEARCH SYSTEM\n";
        cout << "====================================\n";
        cout << "Total Songs: " << songs.size() << '\n';
        cout << "------------------------------------\n";
        cout << "1. Display All Songs\n";
        cout << "2. Search by Song Title\n";
        cout << "3. Search by Artist\n";
        cout << "4. Search by Album\n";
        cout << "5. Search by Lyrics Keyword\n";
        cout << "6. Play a Song\n";
        cout << "7. Sort Songs by Title\n";
        cout << "8. Binary Search by Exact Title\n";
        cout << "9. Exit\n";
        cout << "====================================\n";
        cout << "Enter choice: ";

        getline(cin, choice);

        if (choice == "9")
            break;

        clearScreen();

        if (choice == "1") {
            displayAll();
        }
        else if (choice == "2") {
            cout << "Enter song title: ";
            getline(cin, key);
            linearSearch(key, 1);
        }
        else if (choice == "3") {
            cout << "Enter artist name: ";
            getline(cin, key);
            linearSearch(key, 2);
        }
        else if (choice == "4") {
            cout << "Enter album name: ";
            getline(cin, key);
            linearSearch(key, 3);
        }
        else if (choice == "5") {
            cout << "Enter lyrics keyword: ";
            getline(cin, key);
            searchLyrics(key);
        }
        else if (choice == "6") {
            cout << "Enter song title: ";
            getline(cin, key);
            playSong(key);
        }
        else if (choice == "7") {
            sortSongs();
        }
        else if (choice == "8") {
            cout << "Enter exact song title: ";
            getline(cin, key);
            binarySearch(key);
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }

        pauseScreen();

    } while (true);

    clearScreen();
    cout << "Thank you for using Music Player!\n";

    return 0;
}