// Copyright (C) 2014-2015 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#error "Only <CommonAPI/CommonAPI.h> can be included directly, this file may disappear or change contents."
#endif

#ifndef COMMONAPI_SERIALIZABLEARGUMENTS_HPP_
#define COMMONAPI_SERIALIZABLEARGUMENTS_HPP_

#include <CommonAPI/InputStream.hpp>
#include <CommonAPI/OutputStream.hpp>

namespace CommonAPI {

template<class _In, class _Out, typename... _Arguments>
struct SerializableArguments;

template<class _In, class _Out>
struct SerializableArguments<_In, _Out> {
    static bool serialize(OutputStream<_Out> &_output) {
        return true;
    }

    static bool deserialize(InputStream<_In> &_input) {
        return true;
    }
};

template<class _In, class _Out, typename _ArgumentType>
struct SerializableArguments<_In, _Out, _ArgumentType> {
    static bool serialize(OutputStream<_Out> &_output, const _ArgumentType &_argument) {
        _output << _argument;
        return !_output.hasError();
    }

    static bool deserialize(InputStream<_In> &_input, _ArgumentType &_argument) {
        _input >> _argument;
        return !_input.hasError();
    }
};

template <class _In, class _Out, typename _ArgumentType, typename ... _Rest>
struct SerializableArguments<_In, _Out, _ArgumentType, _Rest...> {
    static bool serialize(OutputStream<_Out> &_output, const _ArgumentType &_argument, const _Rest&... _rest) {
        _output << _argument;
        return !_output.hasError() ?
        			SerializableArguments<_In, _Out, _Rest...>::serialize(_output, _rest...) : false;
    }

    static bool deserialize(InputStream<_In> &_input, _ArgumentType &_argument, _Rest&... _rest) {
        _input >> _argument;
        return !_input.hasError() ?
        			SerializableArguments<_In, _Out, _Rest...>::deserialize(_input, _rest...) : false;
    }
};

} // namespace CommonAPI

#endif // COMMONAPI_SERIALIZABLE_ARGUMENTS_HPP_
