/*
 * UTPrivateAccessMethod.h
 *
 *  Created on: Mar 28, 2013
 *      Author: Arkadiusz Netczuk
 *
 *      Based on:
 *      	http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html
 */

#ifndef UT_PRIVATE_ACCESS_METHOD_H_
#define UT_PRIVATE_ACCESS_METHOD_H_


template<typename Tag>
struct result {
  /* export it ... */
  typedef typename Tag::type type;
  static type ptr;
};
template<typename Tag>
typename result<Tag>::type result<Tag>::ptr;

template<typename Tag, typename Tag::type p>
struct rob : result<Tag> {
  /* fill it ... */
  struct filler {
    filler() { result<Tag>::ptr = p; }
  };
  static filler filler_obj;
};

template<typename Tag, typename Tag::type p>
typename rob<Tag, p>::filler rob<Tag, p>::filler_obj;

//-----------------------------------------------------------------

//class A {
//private:
//	bool hiddenMethod() const {
//		std::cout << "proof!" << std::endl;
//		return true;
//	}
//};

//struct FriendA_invoker {
//	typedef bool (FriendA::* type)() const;
//};
//template class rob<FriendA_invoker, &FriendA::hiddenMethod> ;

//-----------------------------------------------------------------

#define REGISTER_INVOKER(className, returnType, type1)						\
		struct BOOST_JOIN(className, _invoker) {							\
			typedef returnType (className::* type)(type1);					\
		};

#define REGISTER_INVOKER_STRUCT_NAMED_CONST(invokerName, className, returnType)			\
		struct invokerName {															\
			typedef returnType (className::* type)() const;								\
		};

#define REGISTER_INVOKER_STRUCT_NAMED_CONST1(invokerName, className, returnType, type1)			\
		struct invokerName {																	\
			typedef returnType (className::* type)(type1) const;								\
		};

#define REGISTER_INVOKER_STRUCT_NAMED_CONST2(invokerName, className, returnType, type1, type2)		\
		struct invokerName {																		\
			typedef returnType (className::* type)(type1, type2) const;								\
		};

#define REGISTER_INVOKER_STRUCT_CONST(className, returnType)								\
		REGISTER_INVOKER_STRUCT_NAMED_CONST( BOOST_JOIN(className, _invoker), returnType)

#define REGISTER_INVOKER_STRUCT_CONST1(className, returnType, type1)								\
		REGISTER_INVOKER_STRUCT_NAMED_CONST1( BOOST_JOIN(className, _invoker), returnType, type1)

#define REGISTER_ROB(invokerName, className, method)			\
		template class rob<invokerName, &className::method> ;

#define REGISTER_INVOKER_NAMED_CONST(invokerName, className, method, returnType)			\
		REGISTER_INVOKER_STRUCT_NAMED_CONST(invokerName, className, returnType)				\
		REGISTER_ROB(invokerName, className, method)

#define REGISTER_INVOKER_NAMED_CONST1(invokerName, className, method, returnType, type1)	\
		REGISTER_INVOKER_STRUCT_NAMED_CONST1(invokerName, className, returnType, type1)		\
		REGISTER_ROB(invokerName, className, method)

#define REGISTER_INVOKER_CONST(className, method, returnType)											\
		REGISTER_INVOKER_NAMED_CONST(BOOST_JOIN(className, _invoker), className, method, returnType)

#define REGISTER_INVOKER_CONST1(className, method, returnType, type1)											\
		REGISTER_INVOKER_NAMED_CONST1(BOOST_JOIN(className, _invoker), className, method, returnType, type1)

#define CALL_METHOD_INVOKER(invokerId, object)							\
		(object.* result<invokerId>::ptr)

#endif /* UT_PRIVATE_ACCESS_METHOD_H_ */
