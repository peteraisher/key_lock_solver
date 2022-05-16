//  Copyright (c) 2022 Peter Aisher
//
//  hash_combine.h
//  key_lock_solver
//
//  Created by Peter Aisher on 13.05.2022.
//

#ifndef KEY_LOCK_SOLVER_UTIL_HASH_COMBINE_H_
#define KEY_LOCK_SOLVER_UTIL_HASH_COMBINE_H_

template <class T>
inline void hash_combine(std::size_t* seed, const T& v) {
    std::hash<T> hasher;
    *seed ^= hasher(v) + 0x9e3779b9 + ((*seed) << 6) + ((*seed) >> 2);
}

#endif  // KEY_LOCK_SOLVER_UTIL_HASH_COMBINE_H_
