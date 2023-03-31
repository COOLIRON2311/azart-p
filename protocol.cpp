#include "protocol.h"

QDebug& operator<<(QDebug& d, const Mode& m)
{
    switch (m) {
    case Mode::None:
        d << "Нет";
        break;
    case Mode::TETRA_DMO:
        d << "DMO";
        break;
    case Mode::TETRA_TMO:
        d << "TMO";
        break;
    case Mode::VPD:
        d << "ВПД";
        break;
    case Mode::AM25:
        d << "АМ25";
        break;
    case Mode::CHM25:
        d << "ЧМ25";
        break;
    case Mode::CHM50:
        d << "ЧМ50";
        break;
    case Mode::OBP:
        d << "ОБП";
        break;
    case Mode::FM:
        d << "ФМ";
        break;
    }
    return d;
}

QDebug& operator<<(QDebug& d, const Header& h)
{
    d << "{m:" << h.mode << ";";
    d << "gssi:" << h.gssi << ";";
    d << "s_m:" << h.speech_mask << ";";
    d << "mcc:" << h.mcc << ";";
    d << "mnc:" << h.mnc << ";";
    d << "2f:" << h.dual_freq << ";";
    d << "r:" << h.recv << ";";
    d << "s:" << h.send<< ";";
    d << "f:" << h.freq << ";";
    d << "ctcss:" << h.ctcss << ";";
    d << "fb:" << h.freq_band << "}";
    return d;
}
