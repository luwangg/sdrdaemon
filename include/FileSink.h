///////////////////////////////////////////////////////////////////////////////////
// SDRdaemon - receive I/Q samples over the network via UDP and write to a       //
//             SDR device .                                                      //
//                                                                               //
// Copyright (C) 2017 Edouard Griffiths, F4EXB                                   //
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

#ifndef INCLUDE_FILESINK_H_
#define INCLUDE_FILESINK_H_

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "DeviceSink.h"

class FileSink : public DeviceSink
{
public:

    //static const int default_block_length = 65536;

    /** Open File Sink device. */
    FileSink();

    /** Close File Sink device. */
    virtual ~FileSink();

    /** Return sample size in bits */
    virtual std::uint32_t get_device_sample_bits() { return 16; }

    /** Return current sample frequency in Hz. */
    virtual std::uint32_t get_sample_rate();

    /** Return device current center frequency in Hz. */
    virtual std::uint32_t get_frequency();

    /** Print current parameters specific to device type */
    virtual void print_specific_parms();

    virtual bool start(DataBuffer<IQSample> *buf, std::atomic_bool *stop_flag);
    virtual bool stop();

    /** Return true if the device is OK, return false if there is an error. */
    virtual operator bool() const
    {
        return true;
    }

    /** Return a list of supported devices. */
    static void get_device_names(std::vector<std::string>& devices);

private:
    /** Configure HackRF tuner from a list of key=value pairs */
    virtual bool configure(parsekv::pairs_type& m);

    /**
     * Configure HackRF tuner and prepare for streaming.
     *
     * changeFlags  :: horrible hack to notify which fields have changed
     * sample_rate  :: desired sample rate in Hz.
     * frequency    :: desired center frequency in Hz.
     * ext_amp      :: extra amplifier engaged
     * vga_gain     :: desired VGA gain:
     *
     * Return true for success, false if an error occurred.
     */
    bool configure(uint32_t changeFlags,
                   uint32_t sample_rate,
                   uint64_t frequency
    );

    void closeAndOpen();
    static void run(std::atomic_bool *stop_flag);

    uint32_t m_sampleRate;
    uint64_t m_frequency;
    std::string m_filename;
    std::ofstream m_ofstream;
    uint32_t m_delayUS;
    bool m_running;
    std::thread *m_thread;
    static FileSink *m_this;
    std::string m_vgainsStr;
    std::string m_bwfiltStr;
    IQSampleVector m_iqSamples;
    uint32_t m_iqSamplesIndex;
};

#endif /* INCLUDE_HACKRFDEVICESINK_H_ */
