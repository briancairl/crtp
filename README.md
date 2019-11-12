# CRTP

This is a ridiculously simple library that contains a few macros.

I end up copying them into every new library I made, anyway, so I figured I would just make one library for the sake of consistency.

Consider the following example use case, which creates class templates which follow the chain <code>Base -> Derived1 -> Derived2</code>

  ```c++
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
  ```