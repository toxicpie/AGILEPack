
#ifndef BOOST_MPL_AUX_ITER_FOLD_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_ITER_FOLD_IMPL_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: iter_fold_impl.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include "boost_mod/mpl/next_prior.hpp"
#   include "boost_mod/mpl/apply.hpp"
#   include "boost_mod/mpl/aux_/config/ctps.hpp"
#   if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#       include "boost_mod/mpl/if.hpp"
#       include "boost_mod/type_traits/is_same.hpp"
#   endif
#endif

#include "boost_mod/mpl/aux_/config/use_preprocessed.hpp"

#if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER iter_fold_impl.hpp
#   include "boost_mod/mpl/aux_/include_preprocessed.hpp"

#else

#   define AUX778076_FOLD_IMPL_OP(iter) iter
#   define AUX778076_FOLD_IMPL_NAME_PREFIX iter_fold
#   include "boost_mod/mpl/aux_/fold_impl_body.hpp"

#endif // BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // BOOST_MPL_AUX_ITER_FOLD_IMPL_HPP_INCLUDED