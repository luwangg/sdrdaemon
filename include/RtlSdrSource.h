///////////////////////////////////////////////////////////////////////////////////
// SDRdaemon - send I/Q samples read from a SDR device over the network via UDP. //
//                                                                               //
// Copyright (C) 2015 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef SOFTFM_RTLSDRSOURCE_H
#define SOFTFM_RTLSDRSOURCE_H

#include <cstdint>
#include <string>
#include <vector>
#include <thread>

#include "DeviceSource.h"

class RtlSdrSource : public DeviceSource
{
public:

    /** Open RTL-SDR device. */
    RtlSdrSource(int dev_index);

    /** Close RTL-SDR device. */
    virtual ~RtlSdrSource();

    /** Return sample size in bits */
    virtual std::uint32_t get_sample_bits() { return 8; }

    /** Return current sample frequency in Hz. */
    virtual std::uint32_t get_sample_rate();

    /** Return device current center frequency in Hz. */
    virtual std::uint32_t get_frequency();

    /** Print current parameters specific to device type */
    virtual void print_specific_parms();

    virtual bool start(DataBuffer<IQSample>* samples, std::atomic_bool *stop_flag);
    virtual bool stop();

    /** Return true if the device is OK, return false if there is an error. */
    virtual operator bool() const
    {
        return m_dev && m_error.empty();
    }

    /** Return a list of supported devices. */
    static void get_device_names(std::vector<std::string>& devices);

private:

    /** Configure RTL-SDR tuner from a list of key=values */
    virtual bool configure(parsekv::pairs_type& m);

    /**
     * Configure RTL-SDR tuner and prepare for streaming.
     *
     * changeFlags  :: horrible hack to notify which fields have changed
     * sample_rate  :: desired sample rate in Hz.
     * frequency    :: desired center frequency in Hz.
     * ppm          :: LO PPM correction.
     * tuner_gain   :: desired tuner gain in 0.1 dB, or INT_MIN for auto-gain.
     *
     * Return true for success, false if an error occurred.
     */
    bool configure(std::uint32_t changeFlags,
    		       std::uint32_t sample_rate,
                   std::uint32_t frequency,
				   std::int32_t  ppm,
                   int tuner_gain,
                   int agcmode=0);

    /** Return a list of supported tuner gain settings in units of 0.1 dB. */
    std::vector<int> get_tuner_gains();

    /** Return current tuner gain in units of 0.1 dB. */
    int get_tuner_gain();

    /**
     * Fetch a bunch of samples from the device.
     *
     * This function must be called regularly to maintain streaming.
     * Return true for success, false if an error occurred.
     */
    static bool get_samples(IQSampleVector *samples);
    static void rtlsdrCallback(unsigned char *buf, uint32_t len, void *ctx);
    static void run();
    static void readerThreadEntryPoint();

    struct rtlsdr_dev * m_dev;
    std::vector<int>    m_gains;
    std::string         m_gainsStr;
    bool                m_confAgc;
    std::thread         *m_thread;
    static RtlSdrSource *m_this;
};

#endif
