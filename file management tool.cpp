#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FileManager {
public:
    static void createFile(const std::string& filename) {
        std::ofstream file(filename);
        if (file) {
            std::cout << "File " << filename << " created successfully.\n";
        } else {
            std::cerr << "Error creating file " << filename << "!\n";
        }
    }

    static void writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (file) {
            file << content;
            std::cout << "Content written to " << filename << " successfully.\n";
        } else {
            std::cerr << "Error writing to file " << filename << "!\n";
        }
    }

    static void readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file) {
            std::string line;
            while (getline(file, line)) {
                std::cout << line << "\n";
            }
        } else {
            std::cerr << "Error reading file " << filename << "!\n";
        }
    }

    static void deleteFile(const std::string& filename) {
        if (fs::remove(filename)) {
            std::cout << "File " << filename << " deleted successfully.\n";
        } else {
            std::cerr << "Error deleting file " << filename << "!\n";
        }
    }
};

int main() {
    int choice;
    std::string filename, content;
    do {
        std::cout << "\nFile Management Tool\n";
        std::cout << "1. Create File\n";
        std::cout << "2. Write to File\n";
        std::cout << "3. Read File\n";
        std::cout << "4. Delete File\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); 

        switch (choice) {
            case 1:
                std::cout << "Enter filename: ";
                std::getline(std::cin, filename);
                FileManager::createFile(filename);
                break;
            case 2:
                std::cout << "Enter filename: ";
                std::getline(std::cin, filename);
                std::cout << "Enter content: ";
                std::getline(std::cin, content);
                FileManager::writeFile(filename, content);
                break;
            case 3:
                std::cout << "Enter filename: ";
                std::getline(std::cin, filename);
                FileManager::readFile(filename);
                break;
            case 4:
                std::cout << "Enter filename: ";
                std::getline(std::cin, filename);
                FileManager::deleteFile(filename);
                break;
            case 5:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
