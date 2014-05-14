#ifndef VIDEOFORMAT_HPP
#define VIDEOFORMAT_HPP

#include "stdafx.hpp"
extern "C" {
#include <video/mp_image.h>
}

class VideoFormat {
public:
    typedef mp_imgfmt Type;
    VideoFormat(const mp_image_params &params, const mp_imgfmt_desc &desc)
        : d(new Data(params, desc)) { }
    VideoFormat(): d(new Data) { }
    auto operator == (const VideoFormat &rhs) const -> bool
        { return isSame(d->params, rhs.d->params); }
    auto operator != (const VideoFormat &rhs) const -> bool
        { return !operator == (rhs); }
    auto params() const -> const mp_image_params& { return d->params; }
    auto isEmpty() const -> bool;
    auto width() const -> int { return d->params.w; }
    auto height() const -> int { return d->params.h; }
    auto size() const -> QSize { return QSize(d->params.w, d->params.h); }
    auto displaySize() const -> QSize
        { return QSize(d->params.d_w, d->params.d_h); }
    auto bitrate(double fps) const -> double
        { return fps*d->params.w*d->params.h*d->bpp;}
    auto name() const -> QString
        { return QString::fromLatin1(mp_imgfmt_to_name(d->params.imgfmt)); }
    auto colorspace() const -> mp_csp { return d->params.colorspace; }
    auto range() const -> mp_csp_levels { return d->params.colorlevels; }
    auto chroma() const -> mp_chroma_location
        { return d->params.chroma_location; }
private:
    struct Data : public QSharedData {
        Data() { }
        Data(const mp_image_params &params, const mp_imgfmt_desc &desc);
        int bpp = 0;
        mp_image_params params{
            IMGFMT_NONE, 0, 0, 0, 0, // w, h, d_w, d_h
            MP_CSP_BT_709,    MP_CSP_LEVELS_PC,
            MP_CHROMA_CENTER, MP_CSP_LEVELS_PC, 0
        };
    };
    static auto isSame(const mp_image_params &p1,
                       const mp_image_params &p2) -> bool {
        return p1.imgfmt == p2.imgfmt
               && p1.w == p2.w && p1.h == p2.h
               && p1.d_w == p2.d_w && p1.d_h == p2.d_h
               && p1.colorspace == p2.colorspace
               && p1.colorlevels == p2.colorlevels
               && p1.chroma_location == p2.chroma_location;
    }
    QSharedDataPointer<Data> d;
};

inline auto VideoFormat::isEmpty() const -> bool
{
    return d->params.w <= 0 || d->params.h <= 0
           || d->params.imgfmt == IMGFMT_NONE;
}

#endif // VIDEOFORMAT_HPP