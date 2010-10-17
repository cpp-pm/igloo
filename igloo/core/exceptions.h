
//          Copyright Joakim Karlsson & Kim Gräsman 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef IGLOO_EXCEPTIONS_H
#define IGLOO_EXCEPTIONS_H

#include <igloo/core/assert.h>

namespace igloo {
   
  template <typename ExceptionType>
  class ExceptionStorage
  {
  public:
    static std::auto_ptr<ExceptionType>& last_exception()
    {
      static std::auto_ptr<ExceptionType> last;
      return last;
    }
    
    void compiler_thinks_i_am_unused() {}
    
    ~ExceptionStorage()
    {
      last_exception().reset(NULL);
    }
  };
    
  template <typename ExceptionType>
  inline ExceptionType& LastException()
  {
    if(ExceptionStorage<ExceptionType>::last_exception().get() == NULL)
    {
      Assert::Failure("No exception was stored");
    }
    
    return *(ExceptionStorage<ExceptionType>::last_exception().get());
  }  
}

#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)

#define AssertThrows(EXCEPTION_TYPE, METHOD) \
ExceptionStorage<EXCEPTION_TYPE> CONCAT(storage_, __LINE__); CONCAT(storage_, __LINE__).compiler_thinks_i_am_unused(); \
{ \
  bool wrong_exception = false; \
  bool no_exception = false; \
  try \
  { \
    METHOD; \
    no_exception = true; \
  } \
  catch (const EXCEPTION_TYPE& e) \
  { \
    ExceptionStorage<EXCEPTION_TYPE>::last_exception() = std::auto_ptr<EXCEPTION_TYPE>(new EXCEPTION_TYPE(e)); \
  } \
  catch(...) \
  { \
    wrong_exception = true; \
  } \
  if(no_exception) \
  { \
    Assert::Failure("No exception was thrown"); \
  } \
  if(wrong_exception) \
  { \
    Assert::Failure("Wrong exception was thrown"); \
  } \
}

#endif


