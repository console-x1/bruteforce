#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>

const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?-+=#@€$:;/\\.,$éèà_ ";
std::string targetPassword;
long long attempts = 0;

std::string indexToString(const std::vector<int> &indexes)
{
    std::string result;
    result.reserve(indexes.size());
    for (int i : indexes)
        result += charset[i];
    return result;
}

bool compare(const std::vector<int>& indexes, const std::string& password) {
    if (indexes.size() != password.size()) return false;
    for (size_t i = 0; i < indexes.size(); ++i)
        if (charset[indexes[i]] != password[i])
            return false;
    return true;
}

void increment(std::vector<int> &indexes, int charsetSize)
{
    int i = indexes.size() - 1;
    while (i >= 0)
    {
        if (indexes[i] + 1 < charsetSize)
        {
            indexes[i]++;
            return;
        }
        else
        {
            indexes[i] = 0;
            i--;
        }
    }
    indexes.insert(indexes.begin(), 0);
}

int main()
{
    while (true)
    {
        std::cout << "Entrez le mot de passe a trouver (ou 'exit' pour quitter) : ";
        std::getline(std::cin, targetPassword);

        if (targetPassword == "exit")
        {
            std::cout << "Programme termine." << std::endl;
            break;
        }

        attempts = 0;

        std::vector<int> attempt(1, 0);
        int charsetSize = charset.size();

        long long totalCombinations = 1;
        for (size_t i = 0; i < targetPassword.length(); ++i)
        {
            totalCombinations *= charsetSize;
        }
        std::cout << "[Start] Combinaison maximum a tester : " << totalCombinations << std::endl;
        

        auto start = std::chrono::high_resolution_clock::now();

        while (true)
        {
            attempts++;

            if (compare(attempt, targetPassword))
            {
                auto end = std::chrono::high_resolution_clock::now();
                double elapsed = std::chrono::duration<double>(end - start).count();

                std::cout << "\n\033[31m>>>>> PASSWORD FOUND <<<<<\033[0m" << std::endl;
                std::cout << "\033[34mPassword:\033[0m \033[44m" << indexToString(attempt) << "\033[0m" << std::endl;
                std::cout << "\033[2mAttempts: " << attempts << "\033[0m" << std::endl;
                std::cout << "\033[34mTime: " << elapsed << " seconds\033[0m" << std::endl;

                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }

            increment(attempt, charsetSize);
        }

        std::cout << "\n";
    }

    return 0;
}
