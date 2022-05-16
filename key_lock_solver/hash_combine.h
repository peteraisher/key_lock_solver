//
//  hash_combine.h
//  key_lock_solver
//
//  Created by Peter Aisher on 13.05.2022.
//

#ifndef hash_combine_h
#define hash_combine_h

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

#endif /* hash_combine_h */
