/**
 * KFR (http://kfrlib.com)
 * Copyright (C) 2016  D Levin
 * See LICENSE.txt for details
 */

// library_version()
#include <kfr/version.hpp>

// print(), format()
#include <kfr/cometa/string.hpp>

#include <kfr/math.hpp>

// simpleimpulse()
#include <kfr/dsp/oscillators.hpp>

// biquad*
#include <kfr/dsp/biquad.hpp>

// plot_save()
#include <kfr/io/python_plot.hpp>

using namespace kfr;
using namespace kfr::native;

int main(int argc, char** argv)
{
    println(library_version());

    using namespace native;
    const std::string options = "phaseresp=True";

    univector<double, 128> output;
    {
        biquad_params<double> bq[] = { biquad_notch(0.1, 0.5), biquad_notch(0.2, 0.5), biquad_notch(0.3, 0.5),
                                       biquad_notch(0.4, 0.5) };
        output = biquad(bq, simpleimpulse());
    }
    plot_save("biquad_notch", output, options);

    {
        biquad_params<double> bq[] = { biquad_lowpass(0.2, 0.9) };
        output                     = biquad(bq, simpleimpulse());
    }
    plot_save("biquad_lowpass", output, options);

    {
        biquad_params<double> bq[] = { biquad_highpass(0.3, 0.1) };
        output                     = biquad(bq, simpleimpulse());
    }
    plot_save("biquad_highpass", output, options);

    {
        biquad_params<double> bq[] = { biquad_peak(0.3, 0.5, +9.0) };
        output                     = biquad(bq, simpleimpulse());
    }
    plot_save("biquad_peak", output, options);

    {
        biquad_params<double> bq[] = { biquad_peak(0.3, 3.0, -2.0) };
        output                     = biquad(bq, simpleimpulse());
    }
    plot_save("biquad_peak2", output, options);

    {
        biquad_params<double> bq[] = { biquad_lowshelf(0.3, -1.0) };
        output                     = biquad(bq, simpleimpulse());
    }
    plot_save("biquad_lowshelf", output, options);

    {
        biquad_params<double> bq[] = { biquad_highshelf(0.3, +9.0) };
        output                     = biquad(bq, simpleimpulse());
    }
    plot_save("biquad_highshelf", output, options);

    {
        biquad_params<double> bq[] = { biquad_bandpass(0.25, 0.2) };
        output                     = biquad(bq, simpleimpulse());
    }
    plot_save("biquad_bandpass", output, options);

    return 0;
}
