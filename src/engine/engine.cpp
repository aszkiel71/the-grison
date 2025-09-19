#include <iostream>
#include <cstdint>
#include <cassert>
#include <vector>
#define INLINE __attribute__((always_inline))inline
#define NOINLINE __attribute__((noinline))
#define LIKELY(x)   (__builtin_expect(!!(x), 1))
#define UNLIKELY(x) (__builtin_expect(!!(x), 0))

typedef uint64_t u64;
using std::vector;

struct Move{
    int from, to;
    int piece, captured_piece = -1;
    bool is_capture = 0; 
    bool en_passant = 0;
    bool is_castling = 0;
    int promotionPiece = -1;
};


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
        bool turn = 0; // white
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
        
    
    INLINE void displayMove(int from, int to, int prom = -1){
        char fromRow = 'a' + (from % 8);
        char fromRank = '1' + (from / 8);
        char toRow = 'a' + (to % 8);
        char toRank = '1' + (to / 8);
        std::cout << fromRow << fromRank << toRow << toRank;
        if(prom != -1){
            if(prom == 4) std::cout << "q";
            else if(prom == 3) std::cout << "r";
            else if(prom == 2) std::cout << "b";
            else if(prom == 1) std::cout << "n";
        }
        std::cout << "\n";
    }


    INLINE void genWhitePawn(vector<Move>& moves){
        u64 w_Pawns = pieces[0] & color[0];

        u64 single_push = (w_Pawns << 8) & ~(color[0] & color[1]) & ~(0xFF00000000000000ULL); // without promotion
        while(single_push){
            int to_sq = __builtin_ctzll(single_push);
            int from_sq = to_sq - 8;
            Move newMove;
            newMove.from = from_sq;
            newMove.to = to_sq;
            newMove.piece = 0; // pawn
            moves.push_back(newMove);
            single_push &= single_push - 1; 
        }
        u64 promo = (w_Pawns << 8) & ~(color[0] | color[1]) & (0xFF00000000000000ULL);
        while(promo){
            int to_sq = __builtin_ctzll(promo);
            int from_sq = to_sq - 8;
            moves.push_back({from_sq, to_sq, 0, -1, false, false, false, 4});
            moves.push_back({from_sq, to_sq, 0, -1, false, false, false, 3});
            moves.push_back({from_sq, to_sq, 0, -1, false, false, false, 2});
            moves.push_back({from_sq, to_sq, 0, -1, false, false, false, 1});
            promo &= promo - 1;
        }

        u64 double_push = (((w_Pawns & 0x000000000000FF00ULL) << 8) & ~(color[0] | color[1])) << 8 & ~(color[0] | color[1]);
        while(double_push){
            int to_sq = __builtin_ctzll(double_push);
            int from_sq = to_sq - 16;
            Move newMove;
            newMove.from = from_sq;
            newMove.to = to_sq;
            newMove.piece = 0;
            moves.push_back(newMove);
            double_push &= double_push - 1;
        }

        // en passant kiedys zaimplementuje XD

        u64 black_pieces = color[1];
        u64 left_capt = (w_Pawns << 7) & black_pieces;
        while(left_capt){
            int to_sq = __builtin_ctzll(left_capt);
            int from_sq = to_sq - 7;
            Move newMove;
            newMove.from = from_sq;
            newMove.to = to_sq;
            newMove.piece = 0;
            newMove.is_capture = true;
            for(int i = 0; i <= 5; i++){
                if(pieces[i] & (1ULL << to_sq)){
                    newMove.captured_piece = i;
                    break;
                }
            }
            moves.push_back(newMove);
            left_capt &= left_capt - 1;
        }
        u64 right_capt = (w_Pawns << 9) & black_pieces;
        while(right_capt){
            int to_sq = __builtin_ctzll(right_capt);
            int from_sq = to_sq - 9;
            Move newMove;
            newMove.from = from_sq;
            newMove.to = to_sq;
            newMove.is_capture = true;
            for(int i = 0; i <= 5; i++){
                if(pieces[i] & 1ULL << to_sq){
                    newMove.captured_piece = i;
                    break;
                }
            }
            moves.push_back(newMove);
            right_capt &= right_capt - 1;
        }

    }

    INLINE void genBlackPawn(){

    }

    INLINE void genRock(bool t){

    }

    INLINE void genKnight(bool t){

    }

    INLINE void genBishop(bool t){

    }

    INLINE void genQueen(bool t){
        
    }


    INLINE vector<Move> generateMoves(bool t){
        vector<Move> moves;
        genWhitePawn(moves);
        return moves;
    }


    INLINE bool isCheck(bool t){
        return 0;
    }

    INLINE bool isCheckMate(bool t){
        return 0;
    }

    INLINE bool isStaleMate(bool turn){
        return 0;
    }

    INLINE void movePiece(int from, int to, bool t, int figure){
        assert(figure < 6); assert(from < 64); assert(to < 64); assert(from != to);
        //std::cout << figure << "\n";
        u64 from_mask = 1ULL << from;
        u64 to_mask = 1ULL << to;
        bool is_capture = to_mask & color[!t];
        if(is_capture){
            color[!t] &= ~to_mask;
            for(int i = 0; i < 6; i++){
                if(pieces[i] & to_mask){
                    pieces[i] &= ~to_mask;
                    break;
                }
            }
        }
        pieces[figure] &= ~from_mask;
        pieces[figure] |= to_mask;
        color[t] &= ~from_mask;
        color[t] |= to_mask;
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
    vector<Move> mv = e.generateMoves(0);
    int i = 0;
    for(auto k : mv){
        std::cout << ++i << " : ";
        e.displayMove(k.from, k.to);
    }
}