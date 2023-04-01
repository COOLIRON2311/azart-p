#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <QtGlobal>
#include <QString>
#include <QDebug>

enum class Mode : unsigned char {
    None,
    TETRA_DMO,
    TETRA_TMO,
    VPD,
    AM25,
    CHM25,
    CHM50,
    OBP,
    FM,
};

QDebug& operator<<(QDebug& d, const Mode& m);

struct Header
{
    Mode mode; // режим работы

    // --------------------
    // TETRA TMO (стр. 147)
    // --------------------
    uint32_t gssi; // групповой идентификатор (0 - 16777215)
    bool speech_mask; // маскирование речи

    // --------------------
    // TETRA DMO (стр. 148)
    // --------------------
    uint32_t mcc; // код страны
    uint32_t mnc; // код сети
    // gssi

    // -----------------------------
    // АМ25(50), ЧМ25(50) (стр. 150)
    // -----------------------------
    bool dual_freq; // двухчастотный
    uint32_t recv; // частота приема в двухчастотном режиме
    uint32_t send; // частота передачи в двухчастотном режиме
    uint32_t freq; // частота приема и передачи в одночастотном режиме
    unsigned char ctcss; // TODO: тут всего 40 стандартных частот, надо проверить на настоящей станции

    // --------------
    // ОБП (стр. 152)
    // --------------
    bool freq_band; // верхняя или нижняя рабочая полоса частот
    // freq - рабочая полоса частот

    inline void clear()
    {
        mode = Mode::None;
        gssi = 0;
        speech_mask = false;
        mcc = false;
        mnc = false;
        dual_freq = false;
        recv = 0;
        send = 0;
        freq = 0;
        ctcss = 0.0;
        freq_band = false;
    }

//    operator QString() const
//    {
//        QString r;
//        r.sprintf("m: %d; gssi: %d; sm: %d; mcc: %d; mnc: %d; 2f: %d; r: %d; s: %d; f: %d; ctcss: %d; fb: %d"
//, mode, gssi, speech_mask, mcc, mnc, dual_freq, recv, send, freq, ctcss, freq_band);
//        return r;
//    }

};

QDebug& operator<<(QDebug& d, const Header& h);
#endif // PROTOCOL_H
