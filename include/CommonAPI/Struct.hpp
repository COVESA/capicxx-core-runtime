// Copyright (C) 2014-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.hpp> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_STRUCT_HPP_
#define COMMONAPI_STRUCT_HPP_

#include <iostream>
#include <tuple>

namespace CommonAPI {

typedef uint32_t Serial;

template<class _Derived>
class InputStream;

template<class _Derived>
class OutputStream;

template<class _Derived>
class TypeOutputStream;

template<int, class, class, class>
struct StructReader;

template<
	int _Index, class _Input,
    template<class...> class _V, class... _Values,
    template<class...> class _D, class... _Depls>
struct StructReader<_Index, _Input, _V<_Values...>, _D<_Depls...>> {
	void operator()(InputStream<_Input> &_input,
					_V<_Values...> &_values,
					const _D<_Depls...> *_depls) {
		StructReader<_Index-1, _Input, _V<_Values...>, _D<_Depls...>>{}(_input, _values, _depls);
		_input.template readValue<>(std::get<_Index>(_values.values_),
									(_depls ? std::get<_Index>(_depls->values_) : nullptr));
	}
};

template<
	int _Index, class _Input,
	template<class...> class _V, class... _Values,
	class _D>
struct StructReader<_Index, _Input, _V<_Values...>, _D> {
	void operator()(InputStream<_Input> &_input,
					_V<_Values...> &_values,
					const _D *_depls) {
		StructReader<_Index-1, _Input, _V<_Values...>, _D>{}(_input, _values, _depls);
		_input.template readValue<_D>(std::get<_Index>(_values.values_));
	}
};

template<class _Input,
	template<class...> class _V, class... _Values,
	template<class...> class _D, class... _Depls>
struct StructReader<0, _Input, _V<_Values...>, _D<_Depls...>> {
	void operator()(InputStream<_Input> &_input,
					_V<_Values...> &_values,
					const _D<_Depls...> *_depls) {
		_input.template readValue<>(std::get<0>(_values.values_),
									(_depls ? std::get<0>(_depls->values_) : nullptr));
	}
};

template<class _Input,
	template<class...> class _V, class... _Values,
	class _D>
struct StructReader<0, _Input, _V<_Values...>, _D> {
	void operator()(InputStream<_Input> &_input,
					_V<_Values...> &_values,
					const _D *_depls) {
		_input.template readValue<_D>(std::get<0>(_values.values_));
	}
};


template< int, class, class, class >
struct StructWriter;

template<
	int _Index, class _Output,
    template<class ...> class _V, class... _Values,
    template <class...> class _D, class... _Depls>
struct StructWriter<_Index, _Output, _V<_Values...>, _D<_Depls...>> {
	void operator()(OutputStream<_Output> &_output,
					const _V<_Values...> &_values,
					const _D<_Depls...> *_depls) {
		StructWriter<_Index-1, _Output, _V<_Values...>, _D<_Depls...>>{}(_output, _values, _depls);
		_output.template writeValue<>(std::get<_Index>(_values.values_),
									  (_depls ? std::get<_Index>(_depls->values_) : nullptr));
	}
};

template<
	int _Index, class _Output,
	template<class...> class _V, class... _Values,
	class _D>
struct StructWriter<_Index, _Output, _V<_Values...>, _D> {
	void operator()(OutputStream<_Output> &_output,
					const _V<_Values...> &_values,
					const _D *_depls) {
		StructWriter<_Index-1, _Output, _V<_Values...>, _D>{}(_output, _values, _depls);
		_output.template writeValue<_D>(std::get<_Index>(_values.values_));
	}
};

template<class _Output,
	template<class...> class _V, class... _Values,
	template<class...> class _D, class... _Depls>
struct StructWriter<0, _Output, _V<_Values...>, _D<_Depls...>> {
	void operator()(OutputStream<_Output> &_output,
					const _V<_Values...> &_values,
					const _D<_Depls...> *_depls) {
		_output.template writeValue<>(std::get<0>(_values.values_),
									  (_depls ? std::get<0>(_depls->values_) : nullptr));
	}
};

template<class _Output,
	template<class...> class _V, class... _Values,
	class _D>
struct StructWriter<0, _Output, _V<_Values...>, _D> {
	void operator()(OutputStream<_Output> &_output,
					const _V<_Values...> &_values,
					const _D *_depls) {
		_output.template writeValue<_D>(std::get<0>(_values.values_));
	}
};

template<int, class, class>
struct StructTypeWriter;

template<int _Index, class _TypeOutput,
	template<class...> class _V, class... _Values>
struct StructTypeWriter<_Index, _TypeOutput, _V<_Values...>> {
	void operator()(TypeOutputStream<_TypeOutput> &_output,
					const _V<_Values...> &_values) {
		StructTypeWriter<_Index-1, _TypeOutput, _V<_Values...>>{}(_output, _values);
#ifdef WIN32
		_output.writeType(std::get<_Index>(_values.values_));
#else
		_output.template writeType(std::get<_Index>(_values.values_));
#endif
	}
};

template<class _TypeOutput,
	template<class...> class _V, class... _Values>
struct StructTypeWriter<0, _TypeOutput, _V<_Values...>> {
	void operator()(TypeOutputStream<_TypeOutput> &_output,
					const _V<_Values...> &_values) {
#ifdef WIN32
		_output.writeType(std::get<0>(_values.values_));
#else
		_output.template writeType(std::get<0>(_values.values_));
#endif
	}
};

// Structures are mapped to a (generated) struct which inherits from CommonAPI::Struct.
// CommonAPI::Struct holds the structured data in a tuple. The generated class provides
// getter- and setter-methods for the structure members.
template<typename... _Types>
struct Struct {
	std::tuple<_Types...> values_;
};

// Polymorphic structs are mapped to an interface that is derived from the base class
// PolymorphicStruct and contain their parameter in a Struct.
struct PolymorphicStruct {
	virtual const Serial getSerial() const = 0;
};

} // namespace CommonAPI

#endif // COMMONAPI_STRUCT_HPP_
