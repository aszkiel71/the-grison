#include <iostream>
#include <cstdint>
#include <cassert>

#define INLINE __attribute__((always_inline))inline
#define NOINLINE __attribute__((noinline))
#define LIKELY(x)   (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect(!!(x), 0))

typedef uint64_t u64;

class Engine {
    private:
        u64 pieces[6];
        u64 color[2];
        void resetBoard(){
        pieces[0] = 0x00FF00000000FF00; // pawn
        pieces[1] = 0x4200000000000042; // knight
        pieces[2] = 0x2400000000000024; // bishop
        pieces[3] = 0x8100000000000081; // rock
        pieces[4] = 0x1000000000000010; // queen
        pieces[5] = 0x0800000000000008; // king
        color[0] = 0x000000000000FFFF;  // white
        color[1] = 0xFFFF000000000000;  // black
        bool castling_rights[4];
        uint8_t en_passant;
        u64 knights_moves[64];
        u64 king_moves[64];
        u64 pawn_moves[2][64];
 
        }

    public:
        Engine(){
            resetBoard();
        }
        
        
    INLINE void generateMoves(bool turn){

    }
    
    INLINE bool isCheck(bool turn){
        return 0;
    }

    INLINE bool isCheckMate(bool turn){
        return 0;
    }

    INLINE bool isStaleMate(bool turn){
        return 0;
    }

    INLINE void movePiece(int from, int to, bool turn, int figure){
        assert(figure < 6); assert(from < 64); assert(to < 64); assert(from != to);
        std::cout << figure << "\n";
        u64 from_mask = 1ULL << from;
        u64 to_mask = 1ULL << to;
        bool is_capture = to_mask & color[!turn];
        if(is_capture){
            color[!turn] &= ~to_mask;
            for(int i = 0; i < 6; i++){
                if(pieces[i] & to_mask){
                    pieces[i] &= ~to_mask;
                    break;
                }
            }
        }
        pieces[figure] &= ~from_mask;
        pieces[figure] |= to_mask;
        color[turn] &= ~from_mask;
        color[turn] |= to_mask;
    }

    void displayBoard() {
            std::cout << "\n  +-----------------+\n";

            for (int rank = 7; rank >= 0; rank--) {
                std::cout << rank + 1 << " | ";
                for (int file = 0; file < 8; file++) {
                    int square = rank * 8 + file;
                    uint64_t square_mask = 1ULL << square;
                    char piece_char = '.';

                    if (square_mask & (color[0] | color[1])) {
                        if (square_mask & color[0]) {
                            if (square_mask & pieces[0]) piece_char = 'P';
                            else if (square_mask & pieces[1]) piece_char = 'N';
                            else if (square_mask & pieces[2]) piece_char = 'B';
                            else if (square_mask & pieces[3]) piece_char = 'R';
                            else if (square_mask & pieces[4]) piece_char = 'Q';
                            else if (square_mask & pieces[5]) piece_char = 'K';
                        }
                        else {
                            if (square_mask & pieces[0]) piece_char = 'p';
                            else if (square_mask & pieces[1]) piece_char = 'n';
                            else if (square_mask & pieces[2]) piece_char = 'b';
                            else if (square_mask & pieces[3]) piece_char = 'r';
                            else if (square_mask & pieces[4]) piece_char = 'q';
                            else if (square_mask & pieces[5]) piece_char = 'k';
                        }
                    }
                    std::cout << piece_char << " ";
                }
                std::cout << "| " << rank + 1 << "\n";
            }
            
            std::cout << "  +-----------------+\n";
            std::cout << "    a b c d e f g h\n\n";
        }

};


int main(){
    Engine e;
    e.displayBoard();
    int f, t, p; std::cin >> f >> t >> p;
    e.movePiece(f, t, 0, p);
    e.displayBoard();
}