/*
 * Copyright 2019 Emmanuel Boudreault
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ITERPP_HPP
#define ITERPP_HPP

#include <vector>
#include <optional>
#include <functional>
#include <string>

template <typename T>
struct RangeIter
{
  T start;
  T end;
  T inc;

  __attribute__((always_inline))
  std::optional<T>
  next()
  {
    if (start < end)
    {
      return start++;
    }

    return std::nullopt;
  }
};

template <typename Iterator>
struct CppIter
{
  using T = typename Iterator::value_type;

  Iterator current;
  Iterator end;

  __attribute__((always_inline))
  std::optional<T>
  next()
  {
    if (current != end)
    {
      return *current++;
    }

    return std::nullopt;
  }
};

template <typename T, typename Iterator>
struct FilterIter
{
  Iterator iter;
  std::function<bool (T)> func;

  __attribute__((always_inline))
  std::optional<T> next()
  {
    for (std::optional<T> item = iter.next(); item; item = iter.next())
    {
      if (func(*item))
      {
        return item;
      }
    }

    return std::nullopt;
  }
};

template <typename In, typename Out, typename Iterator>
struct MapIter
{
  Iterator iter;
  std::function<Out (In)> func;

  __attribute__((always_inline))
  std::optional<Out> next()
  {
    const std::optional<In> item = iter.next();

    return item ? std::optional<Out>{ func(*item) } : std::nullopt;
  }
};

template <typename T, typename Iterator>
struct MetaIter
{
  Iterator iter;

  __attribute__((always_inline))
  std::optional<T>
  next()
  {
    return iter.next();
  }

  __attribute__((always_inline))
  MetaIter<T, FilterIter<T, Iterator>>
  filter(const std::function<bool (T)> &filter_func)
  {
    return MetaIter<T, FilterIter<T, Iterator>> {
      FilterIter<T, Iterator> { iter, filter_func }
    };
  }

  template <typename Out>
  __attribute__((always_inline))
  MetaIter<Out, MapIter<T, Out, Iterator>>
  map(const std::function<Out (T)> &map_func)
  {
    return MetaIter<Out, MapIter<T, Out, Iterator>> {
      MapIter<T, Out, Iterator> { iter, map_func }
    };
  }

  __attribute__((always_inline))
  std::optional<T>
  find_first(const std::function<bool (T)> &func)
  {
    for (std::optional<T> item = iter.next(); item; item = iter.next())
    {
      if (func(*item))
      {
        return item;
      }
    }

    return std::nullopt;
  }

  __attribute__((always_inline))
  T
  sum(T start = 0)
  {
    for (std::optional<T> item = iter.next(); item; item = iter.next())
    {
      start += *item;
    }

    return start;
  }

  __attribute__((always_inline))
  std::string
  join(const std::string &sep = ", ")
  {
    std::string str = "";

    const std::optional<T> first = iter.next();
    if (first)
    {
      str.append(*first);
    }
    for (std::optional<T> item = iter.next(); item; item = iter.next())
    {
      str.append(sep);
      str.append(*item);
    }

    return str;
  }
};

template <typename Iterator>
__attribute__((always_inline))
static MetaIter<typename Iterator::value_type, CppIter<Iterator>>
iter(const Iterator &begin, const Iterator &end)
{
  return MetaIter<typename Iterator::value_type, CppIter<Iterator>> {
    CppIter<Iterator> { begin, end }
  };
}

__attribute__((always_inline))
static MetaIter<int, RangeIter<int>>
range(int start, int stop, int inc = 1)
{
  return MetaIter<int, RangeIter<int>> {
    RangeIter<int> { start, stop, inc }
  };
}

#endif // ITERPP_HPP
