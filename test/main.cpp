/*
*------------------------------------------------------------------------
*main.cpp
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and its documentation, to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the software, and
* to permit persons to whom the software is furnished to do so, subject to
* the following conditions:
*
* 1. The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the software.
*
* 2.THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.*
*
* Shunguang Wu, All Right Reserved, 02/23/2025
*-------------------------------------------------------------------------
*/

#include <iostream>
#include <chrono>
#include "appLog/test/AppLogUsageExample.hpp"

int test_single_thread_access(int argc, char* argv[]);
int test_multi_thread_access(int argc, char* argv[]);

int main(int argc, char* argv[])
{
  //setup log
#ifdef _WIN32
  const std::string logger_file_dir = "c:/temp";
#else
  const std::string logger_file_dir = "~/temp/";
#endif

  const std::string logger_file_path = logger_file_dir + "/tmp-log.txt";
  const std::string logger_name = "MyApp";
  const int logger_level = 0;
  uint64_t file_size_MB = 1;
  uint64_t num_files = 5;
  bool enableConsoleSink = false;

  setupAppLog(logger_file_path, logger_name, logger_level, file_size_MB, num_files, enableConsoleSink);

  test_single_thread_access(argc, argv);
  test_multi_thread_access(argc, argv);

  APP_TRACE("====end log=========" );

  return 0;
}


int test_single_thread_access(int argc, char* argv[])
{
  AppLogUsageExample x;
  for (int i = 0; i < 60000; i++) {
    x.logSomething();
  }
  return  0;
}


