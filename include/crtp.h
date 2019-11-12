/**
 * @copyright 2019 CRTP
 * @author Brian Cairl
 *
 * @file crtp.h
 */

#ifndef CRTP_CRTP_H
#define CRTP_CRTP_H

/**
 * @brief Macro used to implement CRTP base class  template
 *
 *        Implements derived class pointer casting messages and makes base
 *        class impossible to instance
 *
 * @param BaseTmpl  CRTP-base template
 * @param DerivedT  CRTP-derived type
 */
#define IMPLEMENT_CRTP_BASE_CLASS(BaseTmpl, DerivedT) \
protected: \
  BaseTmpl() = default;\
\
private: \
  constexpr DerivedT* derived() \
  {\
    return static_cast<DerivedT*>(this); \
  }\
  \
  constexpr const DerivedT* derived() const \
  {\
    return static_cast<const DerivedT*>(this); \
  }


/**
 * @brief Macro used to implement CRTP derived class template
 *
 *        - Makes CRTP base a friend of dervived class
 *        - Declares member variable alias of <code>BaseTmpl<DerivedT></code>
 *
 * @param BaseTmpl  CRTP-base template
 * @param DerivedT  CRTP-derived type
 */
#define IMPLEMENT_CRTP_DERIVED_CLASS(BaseTmpl, DerivedT) \
using BaseTmpl##Type = BaseTmpl<DerivedT>; \
\
friend BaseTmpl##Type;


/**
 * @brief Macro used to reference a method implemented in a CRTP derived class template
 * @param method  method name
 */
#define CRTP_OVERRIDE_M(__method_name) __method_name##_impl


/**
 * @brief Macro used to call a method implemented in a CRTP derived class from CRTP base template
 * @param __method_name  method name
 */
#define CRTP_INDIRECT_M(__method_name) this->derived()->CRTP_OVERRIDE_M(__method_name)


#endif // CRTP_CRTP_H