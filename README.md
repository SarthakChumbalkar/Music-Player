# 🎵 Music Player Search System

A console-based Music Player Search System developed in C++ as a **Data Structures and Algorithms (DSA) micro project**. It allows users to search songs using metadata and keywords in lyrics, sort the music library, and open local audio files.

## 📌 Project Overview

The system maintains a music library containing song titles, artists, albums, release years, and filenames. Users can search the library using Linear Search and Binary Search. Bubble Sort is implemented manually to arrange songs alphabetically without using built-in sorting functions.

## ✨ Features

- **Display All Songs:** View all songs stored in the library.
- **Search by Song Title:** Find songs using full or partial titles.
- **Search by Artist:** Find songs using an artist's name.
- **Search by Album:** Find songs belonging to an album.
- **Lyrics Keyword Search:** Search words and phrases inside lyrics text files.
- **Play a Song:** Open a local MP3 using the default Windows audio application.
- **Manual Bubble Sort:** Sort songs alphabetically by title.
- **Binary Search:** Find a song using its exact title.
- **Text Normalization:** Ignore capitalization, spaces, and punctuation during searches.
- **File Handling:** Load metadata and lyrics from text files.

## 🧠 DSA Concepts Used

| Concept | Application |
|---|---|
| Structure (`struct`) | Stores song details in one record |
| Vector | Stores the music library dynamically |
| Linear Search | Searches titles, artists, albums, and lyrics |
| Bubble Sort | Manually sorts song titles alphabetically |
| Binary Search | Searches for an exact title in sorted data |
| File Handling | Reads song metadata and lyrics from files |
| String Processing | Normalizes text for easier searching |

**Note:** Sorting is implemented manually using Bubble Sort. The program does not use `std::sort()` or the `<algorithm>` library.

## 🛠️ Technologies Used

- C++
- Standard Template Library (STL) containers and strings
- GCC/G++ compiler
- Visual Studio Code
- Text files for metadata and lyrics

## 📂 Project Structure

```text
Music-Player/
│
├── main.cpp
├── songs.txt
├── music/
│   ├── alag_aasmaan.mp3
│   ├── arz_kiya_hai.mp3
│   ├── badtameez_dil.mp3
│   └── ...
│
└── lyrics/
    ├── alag_aasmaan.txt
    ├── arz_kiya_hai.txt
    ├── badtameez_dil.txt
    └── ...
```

## ⚙️ Setup and Installation

### 1. Clone the repository

```bash
git clone <YOUR_REPOSITORY_URL>
cd Music-Player
```

Replace `<YOUR_REPOSITORY_URL>` with your GitHub repository URL.

### 2. Install a C++ compiler

Install GCC/G++ and ensure that the `g++` command is available in your terminal.

Verify the installation:

```bash
g++ --version
```

### 3. Prepare the music library

- Keep `songs.txt` in the project root.
- Place MP3 files inside the `music/` folder.
- Place the corresponding lyrics text files inside the `lyrics/` folder.
- Ensure the filenames match the entries in `songs.txt`.

The database uses this format:

```text
ID|Title|Artist|Album|Year|AudioFilename|LyricsFilename
```

Example:

```text
1|Alag Aasmaan|Anuv Jain|Alag Aasmaan - Single|2020|alag_aasmaan.mp3|alag_aasmaan.txt
```

### 4. Compile and run

Open PowerShell or the VS Code terminal in the project directory.

Compile:

```powershell
g++ main.cpp -std=c++17 -o temp_music.exe
```

Run:

```powershell
.\temp_music.exe
```

The current implementation uses Windows commands for clearing the terminal and opening audio files, so it is designed for Windows.

## 🔍 Searching and Sorting Algorithms

### 1. Linear Search

Linear Search checks each song record one by one. It is used for searching by title, artist, and album. Lyrics searching also checks the lyrics files one by one.

**Time complexity:** `O(n)` comparisons for metadata searches, excluding string-comparison costs.

### 2. Bubble Sort

Bubble Sort compares adjacent song titles and swaps them when they are in the wrong alphabetical order. It is implemented using nested loops in the `bubbleSort()` function.

**Time complexity:**
- Best case: `O(n)` when the list is already sorted.
- Average case: `O(n²)`.
- Worst case: `O(n²)`.

### 3. Binary Search

Binary Search finds a song by exact title after arranging a copy of the music library alphabetically using the manual Bubble Sort implementation.

It repeatedly checks the middle element and eliminates half of the remaining search range.

**Search complexity:** `O(log n)` comparisons after sorting.

**Important:** Since this implementation sorts a copy before each Binary Search, the complete operation, including sorting, takes `O(n²)` time in the worst case. The `O(log n)` complexity describes the search phase alone.

## 🔤 Text Normalization

The `normalize()` function converts letters to lowercase and removes spaces and non-alphanumeric characters before comparing text.

For example, the following inputs normalize to the same text:

```text
Phir Se Ud Chala
phir...se ud chala
PHIR-SE-UD-CHALA
```

This makes searching less sensitive to capitalization and punctuation.

## 🎧 How It Works

1. Loads song metadata from `songs.txt`.
2. Displays a console menu.
3. Performs the selected search or sorting operation.
4. Reads lyrics files when searching lyrics keywords.
5. Displays matching song details.
6. Opens the corresponding audio file when playback is selected.

## 🚀 Future Improvements

- Add a linked-list-based playlist.
- Implement a queue for upcoming songs.
- Add song insertion and deletion.
- Compare Linear Search and Binary Search performance.
- Add playlist management and playback controls.

## ⚠️ Notes

- This is a console-based project, not a graphical music player.
- Audio playback opens the MP3 file in the default Windows audio application.
- The music library and lyrics are stored locally.
- The current version does not play audio directly inside the console.
- Share only music and lyrics files that you have permission to distribute.

## 👨‍💻 Author

Developed as a **Data Structures and Algorithms (DSA) Micro Project** using C++.