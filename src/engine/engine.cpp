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
        pieces[0] = 42; // pawn
        pieces[1] = 42; // knight
        pieces[2] = 42; // bishop
        pieces[3] = 42; // rock
        pieces[4] = 42; // queen
        pieces[5] = 42; // king
        color[0] = 42;  // white
        color[1] = 42;  // black
    }
    public:
        Engine(){
            resetBoard();
        }
        ~Engine() = default;
        
    INLINE void generateMoves(bool turn){

    }
    
    INLINE bool isCheck(bool turn){

    }

    INLINE bool isCheckMate(bool turn){

    }

    INLINE bool isStaleMate(bool turn){

    }

    INLINE void movePiece(int from, int to, bool turn, int figure){
        assert(figure < 6); assert(from < 64); assert(to < 64); assert(from != to);
        pieces[figure] ^= (1 << from) | (1 << to);
        color[turn] ^= (1 << from) | (1 << to);
    }

    

};