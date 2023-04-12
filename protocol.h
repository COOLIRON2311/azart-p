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

#define franges 3
struct FRs
{
    uint32_t lower_freq[franges];
    uint32_t upper_freq[franges];
};

struct Header
{
    Mode mode; // режим работы

    // --------------------
    // TETRA TMO (стр. 147)
    // --------------------
    uint8_t net;
    uint32_t gssi; // групповой идентификатор (0 - 16777215)
    bool speech_mask; // маскирование речи
    uint32_t keys[8];

    // --------------------
    // TETRA DMO (стр. 148)
    // --------------------
    uint16_t mcc; // код страны
    uint16_t mnc; // код сети
    // gssi

    bool pprch;
    bool retr;

    ///0: chp_dmo, 1: chp_retr, 2: chp_prd, 3: prm_net, 4: prd_net
    //FRs frs[5];

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
        mcc = 0;
        mnc = 0;
        dual_freq = false;
        recv = 0;
        send = 0;
        freq = 0;
        ctcss = 0.0;
        freq_band = false;
        for(int i = 0; i < 8; i++){
            keys[i] = 0;
        }
        pprch = false;
        retr = false;
        /*
        for(int j = 0; j < 5; j++){
            for(int i = 0; i < franges; i++){
                frs[j].lower_freq[i] = 0;
                frs[j].upper_freq[i] = 0;
            }
        }
*/
        net = 0;
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
