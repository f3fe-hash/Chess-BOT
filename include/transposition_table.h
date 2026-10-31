#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum TranspositionTableBound_t
{
    // Default. The entry is invalid.
    TTBOUND_NONE,

    // The stored evaluation gives an EXACT evaluation
    TTBOUND_EXACT,

    // The stored evaluation gives a lower bound on the evaluation.
    TTBOUND_LOWER,

    // The stored evaluation gives an upper bound on the evaluation.
    TTBOUND_UPPER
} TranspositionTableBound_t;

typedef struct TranspositionTableEntry_t
{
    // Centipawn evaluation
    int eval_cp;
    TranspositionTableBound_t bound;

    uint8_t depth;
    uint64_t hash; // Hash of the position.
} TranspositionTableEntry_t;

const int NUM_BUCKETS = 1024;
const int ENTRIES_PER_BUCKET = 8;

typedef struct TranspositionTable_t {
    TranspositionTableEntry_t buckets[ENTRIES_PER_BUCKET][NUM_BUCKETS];

    // A basic bloom filter to easily check if a hash is present.
    // Only really works in the first few positions. This is an EXTREMELY simple and fast bloom filter.
    uint64_t bloom;
} TranspositionTable_t;

// Create a new transposition table.
TranspositionTable_t* NewTranspositionTable();

// Delete (deallocate) a transposition table.
void DeleteTranspositionTable(TranspositionTable_t* tt);

// Find if the TT contains a hash entry. If you use using the "GetTTEntry" function, it already returns if an entry is found (the found parameter), so avoid the extra over-head using the function when using GetTTEntry.
bool TTContains(TranspositionTable_t* tt, uint64_t hash);

// Get a TT entry from the TT.
// TODO: Should optimze this to return a pointer, therefore not creating an extra copy.
TranspositionTableEntry_t GetTTEntry(TranspositionTable_t* tt, uint64_t hash, bool* found);

// Store a TT evaluation / bound.
void StoreTTEntry(TranspositionTable_t* tt, uint64_t hash, uint8_t depth, int eval_cp, TranspositionTableBound_t bound);