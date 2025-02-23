/*
*------------------------------------------------------------------------
*test_multi_thread_access.cpp
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
#include <chrono>         // std::chrono::seconds
#include <thread>
#include "appLog/include/AppLog.hpp"

// Define five different functions.
void func1() 
{
  for (int i = 0; i < 1000; i++) {
    APP_LOG(1, "func1: Logging cnt=%d", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void func2() {
  for (int i = 0; i < 1000; i++) {
    APP_LOG(2, "func1: Logging cnt=%d", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}

void func3() {
  for (int i = 0; i < 1000; i++) {
    APP_LOG(1, "func3: Logging cnt=%d", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
}

void func4() {
  for (int i = 0; i < 1000; i++) {
    APP_LOG(1, "func4: Logging cnt=%d", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(4));
  }
}

void func5() {
  for (int i = 0; i < 1000; i++) {
    APP_LOG(1, "func5: Logging cnt=%d", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}

// Simulate multithreaded access to the singleton
int test_multi_thread_access(int argc, char* argv[])
{
  // Create threads that execute the corresponding functions.
  std::thread t1(func1);
  std::thread t2(func2);
  std::thread t3(func3);
  std::thread t4(func4);
  std::thread t5(func5);

  // Wait for all threads to finish execution.
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();

  return 0;
}
