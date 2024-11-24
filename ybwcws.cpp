#include <iostream>
#include <limits>
#include <string>
#include <cstdlib> // For std::atoi
#include "Backend/Board.h"
#include "Backend/Type/Square.h"
#include "SimplifiedMoveList.h"
#include "Backend/Type/Color.h"
#include "Engine.h"
#include <omp.h>
#include <fstream> // For file I/O
#include <iomanip> // For formatting output
#include <queue>

constexpr int maxDepth = 25;

// Function to convert a Square enum to its string representation (e.g., E2 -> "e2")
std::string squareToString(Square square) {
    return std::string(1, File(square)) + std::string(1, Rank(square));
}

// Function to display usage instructions
void printUsage(const std::string &programName) {
    std::cerr << "Usage: " << programName << " <depth>\n";
    std::cerr << "  <depth> : Positive integer specifying the search depth.\n";
    std::cerr << "Example:\n";
    std::cerr << "  " << programName << " 4\n";
}

// Function to display the algorithm options list
void displayAlgorithmOptions() {
    std::cout << "Choose Search Algorithm:\n";
    std::cout << "1. Young Brothers Wait Concept (YBWC)\n";
    std::cout << "2. Principal Variation Search (PVS)\n";
    std::cout << "3. Testing function\n";
    std::cout << "4. YBWC with work stealing (YBWCWS)\n";
    std::cout << "Enter your choice (1, 2, 3, or 4): ";
}

int main(int argc, char* argv[]) {
    // Check if the depth argument is provided
    if (argc != 2) {
        std::cerr << "Error: Incorrect number of arguments.\n";
        printUsage(argv[0]);
        return 1;
    }

    // Parse the depth from the first command-line argument
    int depth = std::atoi(argv[1]);

    // Validate the depth
    if (depth <= 0) {
        std::cerr << "Invalid depth: " << depth << ". Depth must be a positive integer.\n";
        printUsage(argv[0]);
        return 1;
    }

    // Display algorithm options and get user choice
    int algorithmChoice = 0;
    while (true) {
        displayAlgorithmOptions();
        std::cin >> algorithmChoice;

        // Check for input failure (e.g., non-integer input)
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cerr << "Invalid input. Please enter 1, 2, 3, or 4.\n";
            continue;
        }

        if (algorithmChoice >= 1 && algorithmChoice <= 4) {
            break; // Valid choice
        } else {
            std::cerr << "Invalid choice: " << algorithmChoice << ". Please enter 1, 2, 3, or 4.\n";
        }
    }

    // Map the choice to algorithm name
    std::string algorithmName;
    switch (algorithmChoice) {
        case 1:
            algorithmName = "Young Brothers Wait Concept (YBWC)";
            break;
        case 2:
            algorithmName = "Principal Variation Search (PVS)";
            break;
        case 3:
            algorithmName = "All algorithms";
            break;
        case 4:
            algorithmName = "Young Brothers Wait Concept with Work Stealing (YBWCWS)";
            break;
        default:
            // This case should never occur due to the earlier validation
            algorithmName = "Unknown Algorithm";
            break;
    }

    std::cout << "Starting " << algorithmName << " with depth: " << depth << "\n";

    // Initialize the chess board with the standard starting position
    StockDory::Board chessBoard;

    Engine engine;

    // Determine which color is to move
    Color currentPlayer = chessBoard.ColorToMove();

    // Define a pair to hold the result (best move sequence and its score)
    std::pair<std::array<Move, maxDepth>, float> result;
    double tstart = 0.0, tend = 0.0, ttaken;

    // Execute the appropriate search algorithm based on the user's choice and current player
    if (algorithmChoice == 1) { // YBWC
        if (currentPlayer == White) {
            // Perform YBWC for White
            tstart = omp_get_wtime();
            result = engine.YBWC<White, maxDepth>(
                chessBoard,
                -50000,
                50000,
                depth
            );
            tend = omp_get_wtime();
            ttaken = tend - tstart;
            printf("Time taken for main part: %f\n", ttaken);
            // Check if there is at least one move in the sequence
            if (!result.first.empty()) {
                Move bestMove = result.first.front();
                std::cout << "White's Best Move (YBWC): "
                          << squareToString(bestMove.From()) << " to "
                          << squareToString(bestMove.To())
                          << " with score " << result.second << "\n";

                // Print the entire sequence of moves (best line)
                std::cout << "Best Line: ";
                for (const Move &move : result.first) {
                    std::cout << squareToString(move.From()) << " to "
                              << squareToString(move.To()) << ", ";
                }
                std::cout << "\n";
            } else {
                std::cout << "No moves available for White.\n";
            }
        } else if (currentPlayer == Black) {
            // Perform YBWC for Black
            tstart = omp_get_wtime();
            result = engine.YBWC<Black, maxDepth>(
                chessBoard,
                -50000,
                50000,
                depth
            );
            tend = omp_get_wtime();
            ttaken = tend - tstart;
            printf("Time taken for main part: %f\n", ttaken);
            // Check if there is at least one move in the sequence
            if (!result.first.empty()) {
                Move bestMove = result.first.front();
                std::cout << "Black's Best Move (YBWC): "
                          << squareToString(bestMove.From()) << " to "
                          << squareToString(bestMove.To())
                          << " with score " << result.second << "\n";

                // Print the entire sequence of moves (best line)
                std::cout << "Best Line: ";
                for (const Move &move : result.first) {
                    std::cout << squareToString(move.From()) << " to "
                              << squareToString(move.To()) << ", ";
                }
                std::cout << "\n";
            } else {
                std::cout << "No moves available for Black.\n";
            }
        }
    } else if (algorithmChoice == 2) { // PVS
        // Similar logic as above for PVS (omitted for brevity)
    } else if (algorithmChoice == 4) { // YBWCWS
        if (currentPlayer == White) {
            // Perform YBWCWS for White
            tstart = omp_get_wtime();
            result = engine.YBWCWS<White, maxDepth>(
                chessBoard,
                -50000,
                50000,
                depth
            );
            tend = omp_get_wtime();
            ttaken = tend - tstart;
            printf("Time taken for main part (YBWCWS): %f\n", ttaken);
            // Check if there is at least one move in the sequence
            if (!result.first.empty()) {
                Move bestMove = result.first.front();
                std::cout << "White's Best Move (YBWCWS): "
                          << squareToString(bestMove.From()) << " to "
                          << squareToString(bestMove.To())
                          << " with score " << result.second << "\n";

                // Print the entire sequence of moves (best line)
                std::cout << "Best Line: ";
                for (const Move &move : result.first) {
                    std::cout << squareToString(move.From()) << " to "
                              << squareToString(move.To()) << ", ";
                }
                std::cout << "\n";
            } else {
                std::cout << "No moves available for White.\n";
            }
        } else if (currentPlayer == Black) {
            // Perform YBWCWS for Black
            tstart = omp_get_wtime();
            result = engine.YBWCWS<Black, maxDepth>(
                chessBoard,
                -50000,
                50000,
                depth
            );
            tend = omp_get_wtime();
            ttaken = tend - tstart;
            printf("Time taken for main part (YBWCWS): %f\n", ttaken);
            // Check if there is at least one move in the sequence
            if (!result.first.empty()) {
                Move bestMove = result.first.front();
                std::cout << "Black's Best Move (YBWCWS): "
                          << squareToString(bestMove.From()) << " to "
                          << squareToString(bestMove.To())
                          << " with score " << result.second << "\n";

                // Print the entire sequence of moves (best line)
                std::cout << "Best Line: ";
                for (const Move &move : result.first) {
                    std::cout << squareToString(move.From()) << " to "
                              << squareToString(move.To()) << ", ";
                }
                std::cout << "\n";
            } else {
                std::cout << "No moves available for Black.\n";
            }
        }
    }

    return 0;
}