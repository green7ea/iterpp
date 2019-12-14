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

#include "../include/iterpp.hpp"

#include <cstdio>
#include <chrono>

static bool is_prime(int n)
{
  return !range(2, n / 2)
    .find_first(
      [&](int div) -> bool
      {
        return (n % div) == 0;
      });
}

static bool is_prime_for(int n)
{
  const int max = n / 2;
  for (int i = 2; i < max; ++i)
  {
    if ((n % i) == 0)
    {
      return false;
    }
  }

  return true;
}

static long int bench_for(int input_num)
{
  bool res = is_prime(input_num);
  std::vector<long int> durations;

  for (int i = 0; i < 1000; ++i)
  {
    const auto start = std::chrono::high_resolution_clock::now();
    const bool res2 = is_prime_for(input_num);
    const auto end = std::chrono::high_resolution_clock::now();

    if (res != res2)
    {
      fprintf(stderr, "Bad result\n");
      exit(1);
    }

    durations.push_back((end - start).count());
  }

  return iter(durations.begin(), durations.end()).sum() / 1000l;
}

static long int bench_iter(int input_num)
{
  bool res = is_prime_for(input_num);
  std::vector<long int> durations;

  for (int i = 0; i < 1000; ++i)
  {
    const auto start = std::chrono::high_resolution_clock::now();
    const bool res2 = is_prime(input_num);
    const auto end = std::chrono::high_resolution_clock::now();

    if (res != res2)
    {
      fprintf(stderr, "Bad result\n");
      exit(1);
    }

    durations.push_back((end - start).count());
  }

  return iter(durations.begin(), durations.end()).sum() / 1000l;
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr,
            "Please provide the max range as an argument.\n"
            "This is to prevent aggressive optimizations.\n");
    return 1;
  }

  const int input_num = atoi(argv[1]);

  const auto duration_for = bench_for(input_num);
  const auto duration_iter = bench_iter(input_num);

  printf("| test  |     iter |      for | perf |\n");
  printf("|-------+----------+----------|------|\n");
  printf("| prime | %8li | %8li | %3li%% |\n",
         duration_iter,
         duration_for,
         (duration_for * 100) / duration_iter);
  return 0;
}
