/*
 * UTPrivateAccess.h
 *
 *  Created on: Mar 28, 2013
 *      Author: Arkadiusz Netczuk
 *
 *      Based on:
 *      	http://bloglitb.blogspot.com/2011/12/access-to-private-members-safer.html
 *
 */

#ifndef UT_PRIVATE_ACCESS_H_
#define UT_PRIVATE_ACCESS_H_


template<typename Tag, typename Tag::type M>
struct Rob {
	friend typename Tag::type get(Tag) {
		return M;
	}
};


//-------------#######################################################


#define REGISTER_ROB(invokerName, className, member)			\
		template struct Rob<invokerName, &className::member> ;

#define CALL_INVOKER(invokerId, object)							\
		(a.*get(invokerId()))

#define CALL_METHOD_INVOKER(invokerId, object)					\
		CALL_INVOKER(invokerId, object)

#define CALL_FIELD_INVOKER(invokerId, object)					\
		CALL_INVOKER(invokerId, object)

//-------------#######################################################


#define REGISTER_FIELD_INVOKER_STRUCT_NAMED(invokerName, className, returnType)			\
		struct invokerName {															\
			typedef returnType className::*type;										\
			friend type get(invokerName);												\
		};

#define REGISTER_FIELD_INVOKER_STRUCT_NAMED_CONST(invokerName, className, returnType)			\
		struct invokerName {															\
			typedef const returnType className::*type;										\
			friend type get(invokerName);												\
		};

#define REGISTER_FIELD_INVOKER_NAMED(invokerName, className, field, returnType)			\
		REGISTER_FIELD_INVOKER_STRUCT_NAMED(invokerName, className, returnType)				\
		REGISTER_ROB(invokerName, className, field)

#define REGISTER_FIELD_INVOKER_NAMED_CONST(invokerName, className, field, returnType)			\
		REGISTER_FIELD_INVOKER_STRUCT_NAMED_CONST(invokerName, className, returnType)				\
		REGISTER_ROB(invokerName, className, field)


//-------------#######################################################


#define REGISTER_INVOKER_STRUCT_NAMED(invokerName, className, returnType)				\
		struct invokerName {															\
			typedef returnType (className::*type)();									\
			friend type get(invokerName);												\
		};

#define REGISTER_INVOKER_STRUCT_NAMED1(invokerName, className, returnType, type1)		\
		struct invokerName {															\
			typedef returnType (className::*type)(type1);								\
			friend type get(invokerName);												\
		};

#define REGISTER_INVOKER_STRUCT_NAMED_CONST(invokerName, className, returnType)			\
		struct invokerName {															\
			typedef returnType (className::*type)() const;								\
			friend type get(invokerName);												\
		};

#define REGISTER_INVOKER_STRUCT_NAMED_CONST1(invokerName, className, returnType, type1)	\
		struct invokerName {															\
			typedef returnType (className::*type)(type1) const;							\
			friend type get(invokerName);												\
		};

#define REGISTER_INVOKER_NAMED(invokerName, className, method, returnType)				\
		REGISTER_INVOKER_STRUCT_NAMED(invokerName, className, returnType)				\
		REGISTER_ROB(invokerName, className, method)

#define REGISTER_INVOKER_NAMED1(invokerName, className, method, returnType, type1)			\
		REGISTER_INVOKER_STRUCT_NAMED1(invokerName, className, returnType, type1)			\
		REGISTER_ROB(invokerName, className, method)

#define REGISTER_INVOKER_NAMED_CONST(invokerName, className, method, returnType)			\
		REGISTER_INVOKER_STRUCT_NAMED_CONST(invokerName, className, returnType)				\
		REGISTER_ROB(invokerName, className, method)

#define REGISTER_INVOKER_NAMED_CONST1(invokerName, className, method, returnType, type1)	\
		REGISTER_INVOKER_STRUCT_NAMED_CONST1(invokerName, className, returnType, type1)		\
		REGISTER_ROB(invokerName, className, method)

#endif /* UT_PRIVATE_ACCESS_H_ */
