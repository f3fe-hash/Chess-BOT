#include "transposition_table.h"

TranspositionTable_t* NewTranspositionTable() {
    TranspositionTable_t* tt = calloc(1, sizeof(TranspositionTable_t));
    return tt;
}

void DeleteTranspositionTable(TranspositionTable_t* tt) {
    free(tt);
}

bool TTContains(TranspositionTable_t* tt, uint64_t hash) {
    // If the bloom filter fails, it is DEFINETELY not in the tt.
    if ((tt->bloom & hash) != hash)
        return false;

    int bucket_idx = hash % NUM_BUCKETS;
    TranspositionTableEntry_t* bucket = tt->buckets[bucket_idx];
    
    for (int i = 0; i < ENTRIES_PER_BUCKET; i++) {
        if (bucket[i].hash == hash)
            return true; // We found an entry with the same hash. It is in the tt.
    }

    // Not in the tt.
    return false;
}

TranspositionTableEntry_t GetTTEntry(TranspositionTable_t* tt, uint64_t hash, bool* found)
{
    if (found != NULL)
        *found = false;

    if (tt == NULL)
        return (TranspositionTableEntry_t){};

    // If the bloom filter fails, it is DEFINETELY not in the tt.
    if ((tt->bloom & hash) != hash) {
        if (found != NULL)
            *found = false;

        // Return an empty entry.
        return (TranspositionTableEntry_t){};
    }

    int bucket_idx = hash % NUM_BUCKETS;
    TranspositionTableEntry_t* bucket = tt->buckets[bucket_idx];
    
    for (int i = 0; i < ENTRIES_PER_BUCKET; i++) {
        if (bucket[i].hash == hash) {
            // We found an entry with the same hash. It is in the tt, so return it.
        if (found != NULL)
            *found = true;
        return bucket[i];
        }
    }

    // It was not found. Return an empty entry.
    return (TranspositionTableEntry_t){};
}

void StoreTTEntry(TranspositionTable_t* tt, uint64_t hash, uint8_t depth, int eval_cp, TranspositionTableBound_t bound) {
    TranspositionTableEntry_t entry = {
        .eval_cp = eval_cp,
        .bound = bound,
        .depth = depth,
        .hash = hash
    };

    int bucket_idx = hash % NUM_BUCKETS;
    TranspositionTableEntry_t* bucket = tt->buckets[bucket_idx];

    for (int i = 0; i < ENTRIES_PER_BUCKET; i++) {
        TranspositionTableEntry_t* tt_entry = &bucket[i];
        if (tt_entry->bound == TTBOUND_NONE) {
            if (depth > tt_entry->depth) {
                *tt_entry = entry; // Write it.
                break;
            }
        }
    }

    tt->bloom |= hash;
}
