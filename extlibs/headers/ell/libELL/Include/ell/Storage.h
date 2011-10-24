// This file is part of Ell library.
//
// Ell library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Ell library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Ell library.  If not, see <http://www.gnu.org/licenses/>.

#ifndef INCLUDED_ELL_STORAGE_H
#define INCLUDED_ELL_STORAGE_H

#include <vector>
#include <list>
#include <set>

namespace ell
{
    /// Storage class is used to propagate output data of matched values
    /// through recursive 'parse' calls.
    template <typename T>
    struct Storage
    {
        Storage() : value() { }
        T value;
        typedef Storage<void> Unit;
    };

    template <>
    struct Storage<void>
    {
        typedef Storage<void> Unit;
        void enqueue(const Unit &) { }
        void clear() { }
    };

    template <typename C, typename T>
    struct ContainerStorage
    {
        C value;
        typedef Storage<T> Unit;
        void enqueue(const Unit & v) { value.insert(value.end(), v.value); }
        void clear() { value.clear(); }
    };

#   define D(container)                                                                \
    template <typename T>                                                              \
    struct Storage<std::container<T> > : public ContainerStorage<std::container<T>, T> \
    { };                                                                               \

    D(vector)
    D(set)
    D(list)

#   undef D

    template <typename V1, typename V2>
    void assign(Storage<V1> & v1, const Storage<V2> & v2)
    {
        v1.value = v2.value;
    }

    template <typename V1>
    void assign(Storage<V1> & v1, const Storage<void> &)
    {
        v1.clear();
    }

    template <typename V2>
    void assign(Storage<void> &, const Storage<V2> &)
    { }

    inline void assign(Storage<void> &, const Storage<void> &)
    { }
}

#endif // INCLUDED_ELL_STORAGE_H
