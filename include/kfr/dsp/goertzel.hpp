/**
 * Copyright (C) 2016 D Levin (http://www.kfrlib.com)
 * This file is part of KFR
 *
 * KFR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KFR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KFR.
 *
 * If GPL is not suitable for your project, you must purchase a commercial license to use KFR.
 * Buying a commercial license is mandatory as soon as you develop commercial activities without
 * disclosing the source code of your own applications.
 * See http://www.kfrlib.com for details.
 */
#pragma once

#include "../base/complex.hpp"
#include "../base/sin_cos.hpp"
#include "../base/vec.hpp"
#include "../expressions/basic.hpp"

namespace kfr
{
namespace internal
{

template <cpu_t c = cpu_t::native, cpu_t cc = c>
struct in_goertzel : in_sin_cos<cc>
{
private:
    using in_sin_cos<cc>::sin;
    using in_sin_cos<cc>::cos;

public:
    template <typename T>
    struct expression_goertzel : output_expression
    {
        expression_goertzel(complex<T>& result, identity<T> omega)
            : result(result), omega(omega), coeff(2 * cos(omega)), q0(), q1(), q2()
        {
        }
        ~expression_goertzel()
        {
            result.real(q1 - q2 * cos(omega));
            result.imag(q2 * sin(omega));
        }
        template <typename U, size_t N>
        KFR_INLINE void operator()(coutput_t, size_t index, vec<U, N> x)
        {
            vec<T, N> in = cast<T>(x);
            KFR_LOOP_UNROLL
            for (size_t i = 0; i < N; i++)
            {
                q0 = coeff * q1 - q2 + in[i];
                q2 = q1;
                q1 = q0;
            }
        }
        complex<T>& result;
        const T omega;
        const T coeff;
        T q0;
        T q1;
        T q2;
    };

    template <typename T, size_t width>
    struct expression_parallel_goertzel : output_expression
    {
        expression_parallel_goertzel(complex<T> result[], vec<T, width> omega)
            : result(result), omega(omega), coeff(cos(omega)), q0(), q1(), q2()
        {
        }
        ~expression_parallel_goertzel()
        {
            const vec<T, width> re = q1 - q2 * cos(omega);
            const vec<T, width> im = q2 * sin(omega);
            for (size_t i = 0; i < width; i++)
            {
                result[i].real(re[i]);
                result[i].imag(im[i]);
            }
        }
        template <typename U, size_t N>
        KFR_INLINE void operator()(coutput_t, size_t index, vec<U, N> x)
        {
            const vec<T, N> in = cast<T>(x);
            KFR_LOOP_UNROLL
            for (size_t i = 0; i < N; i++)
            {
                q0 = coeff * q1 - q2 + in[i];
                q2 = q1;
                q1 = q0;
            }
        }
        complex<T> result[];
        const vec<T, width> omega;
        const vec<T, width> coeff;
        vec<T, width> q0;
        vec<T, width> q1;
        vec<T, width> q2;
    };

    template <typename T>
    KFR_SINTRIN expression_goertzel<T> goertzel(complex<T>& result, identity<T> omega)
    {
        return expression_goertzel<T>(result, omega);
    }

    template <typename T, size_t width>
    KFR_SINTRIN expression_parallel_goertzel<T, width> goertzel(complex<T> (&result)[width],
                                                                const T (&omega)[width])
    {
        return expression_parallel_goertzel<T, width>(result, read<width>(omega));
    }
};
}
}
