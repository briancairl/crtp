/**
 * @copyright 2019 CRTP
 * @author Brian Cairl
 */

// GTest
#include <gtest/gtest.h>

// CRTP
#include <crtp/crtp.h>


/**
 * @brief Base
 */
template<typename DerivedT>
class TestInterface
{
public:
  inline void do_thing() const
  {
    CRTP_INDIRECT_M(do_wrapped_thing)();
  }

  constexpr int do_other_thing(int i)
  {
    return CRTP_INDIRECT_M(do_other_thing)(i);
  }

private:
  IMPLEMENT_CRTP_BASE_CLASS(TestInterface, DerivedT);
};


/**
 * @brief Base -> Derived1
 */
template<typename DerivedT>
class TestWrappedInterface : public TestInterface<TestWrappedInterface<DerivedT>>
{
private:
  void CRTP_OVERRIDE_M(do_wrapped_thing)() const
  {
    CRTP_INDIRECT_M(do_wrapped_thing)();
  }

  constexpr int CRTP_OVERRIDE_M(do_other_thing)(int i)
  {
    return i + i + i;
  }

  IMPLEMENT_CRTP_BASE_CLASS(TestWrappedInterface, DerivedT);
  IMPLEMENT_CRTP_DERIVED_CLASS(TestInterface, TestWrappedInterface);
};


/**
 * @brief Base -> Derived1 -> Derived2
 */
class TestDerived : public TestWrappedInterface<TestDerived>
{
public:
  TestDerived() : TestWrappedInterfaceType{} {};

private:
  void CRTP_OVERRIDE_M(do_wrapped_thing)() const
  {
    // does nothing, but compiles
  }

  IMPLEMENT_CRTP_DERIVED_CLASS(TestWrappedInterface, TestDerived);
};


// TEST(CRTP, ConstructBase)
// {
//   TestInterface<TestWrappedInterface<TestDerived>> test;  // won't compile
// }


// TEST(CRTP, ConstructDerived1)
// {
//   TestWrappedInterface<TestDerived> test;  // won't compile
// }


TEST(CRTP, OverrideConstQualifiedMethod)
{
  TestDerived test;

  test.do_thing();  // compiles
}


TEST(CRTP, OverrideMethodWithArgsReturnType)
{
  TestDerived test;

  ASSERT_EQ(test.do_other_thing(1), 3);
}


template<typename DerivedT>
static void doInterfaceWork(const TestInterface<DerivedT>& interface)
{
  interface.do_thing();
}


TEST(CRTP, NonMemberCallOnBase)
{
  TestDerived test;

  doInterfaceWork(test);
}
