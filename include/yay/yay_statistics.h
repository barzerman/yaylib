/*============================================================================
The MIT License (MIT)

Copyright (c) 2014 Andre Yanpolsky, Max Eronin, Georg Rudoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
============================================================================*/

#pragma once

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/covariance.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/kurtosis.hpp>
#include <boost/accumulators/statistics/skewness.hpp>
#include <boost/accumulators/statistics/variates/covariate.hpp>

#include <boost/accumulators/statistics/weighted_variance.hpp>
#include <boost/accumulators/statistics/weighted_covariance.hpp>
#include <boost/accumulators/statistics/weighted_density.hpp>
#include <boost/accumulators/statistics/weighted_extended_p_square.hpp>
#include <boost/accumulators/statistics/weighted_kurtosis.hpp>
#include <boost/accumulators/statistics/weighted_mean.hpp>
#include <boost/accumulators/statistics/weighted_median.hpp>
#include <boost/accumulators/statistics/weighted_moment.hpp>

#include <boost/version.hpp>
#if BOOST_VERSION >= 105000
#include <boost/accumulators/statistics/weighted_p_square_cumul_dist.hpp>
#else
#include <boost/accumulators/statistics/weighted_p_square_cumulative_distribution.hpp>
#endif

#include <boost/accumulators/statistics/weighted_p_square_quantile.hpp>
#include <boost/accumulators/statistics/weighted_peaks_over_threshold.hpp>
#include <boost/accumulators/statistics/weighted_skewness.hpp>
#include <boost/accumulators/statistics/weighted_sum.hpp>

#include <boost/math/special_functions/fpclassify.hpp>

namespace yay {
//// wrapper of boost accumulators

typedef boost::accumulators::accumulator_set<
    double, 
    boost::accumulators::stats<
        boost::accumulators::tag::weighted_mean,
        boost::accumulators::tag::weighted_skewness,
        boost::accumulators::tag::weighted_variance,
        boost::accumulators::tag::weighted_kurtosis
    >,
    double
> double_standardized_moments_weighted;

typedef boost::accumulators::accumulator_set<
    double, 
    boost::accumulators::stats<
        boost::accumulators::tag::mean,
        boost::accumulators::tag::skewness,
        boost::accumulators::tag::variance,
        boost::accumulators::tag::kurtosis
    >
> double_standardized_moments;

}
