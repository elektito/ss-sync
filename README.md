# How to Build

Simply run make:

    $ make

# How to Run the Server

Pass an `mkv` file containing an H.265 video as the only argument to
`ss-sync`:

    $ ./ss-sync ~/sample_data/1_cam01.mkv

# How to Watch the Video

You can use any video supporting RTSP. For example, to use the
command-line version of `VLC` run:

    $ cvlc rtsp://localhost:8554/test

# Checking the Timestamps

Use `tcpdump` or `wireshark` to capture the packets while being
played:

    $ sudo tcpdump -i lo -w foo.pcap

Then filter for RTP packets containing the header extensions in
wireshark: `rtp.ext eq 1`. Under RTP headers, you will see the
extension headers with the type `0xabac`. The first header is the
`seconds` portion of the timestamp, while the second header contains
the `nanoseconds`.
