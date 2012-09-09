#ifndef QSCRIPTING_H
#define QSCRIPTING_H

#include <QObject>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScriptEngine>


class QScriptWindow;
class CProcessor;


/** \brief Scripting object for emulated processor.
    \sa QEmulator */
class QEmulatorProcessor : public QObject
{
    Q_OBJECT
    /** \brief Get the name of the processor: "CPU" or "PPU", short form for getName(). */
    Q_PROPERTY(QString name READ getName)
    /** \brief Get the processor stack register value, short form for getSP(). */
    Q_PROPERTY(ushort sp READ getSP)
    /** \brief Get the processor PC register value, short form for getPC(). */
    Q_PROPERTY(ushort pc READ getPC)
    /** \brief Get the processor status word value, short form for getPSW(). */
    Q_PROPERTY(ushort psw READ getPSW)
    /** \brief Get the processor HALT/USER mode flag, short form for isHalt(). */
    Q_PROPERTY(bool halt READ isHalt)
public:
    QEmulatorProcessor(QScriptEngine* engine, CProcessor* processor);

public slots:
    /** \brief Get the name of the processor: "CPU" or "PPU". */
    QString getName();

    /** \brief Get the processor register value.
        \param regno 0..7 */
    ushort getReg(int regno);
    /** \brief Get the processor register value, short form for getReg().
        \param regno 0..7 */
    ushort r(int regno) { return getReg(regno); }
    /** \brief Get the processor stack register value. */
    ushort getSP() { return getReg(6); }
    /** \brief Get the processor PC register value. */
    ushort getPC() { return getReg(7); }
    /** \brief Get the processor status word value. */
    ushort getPSW();
    /** \brief Get the processor HALT/USER mode flag. */
    bool isHalt();

    /** \brief Read word from the processor memory.
        \param addr memory address */
    ushort readWord(ushort addr);
    /** \brief Read byte from the processor memory.
        \param addr memory address */
    uchar readByte(ushort addr);

    /** \brief Disassemble one instruction at the given address.
        \param addr memory address
        \return Array of four: { address, instruction, arguments, instruction length }. */
    QScriptValue disassemble(ushort addr);

    //TODO: setReg(regno, value); setPC(value); setSP(value); setPSW(value);
    //TODO: writeWord(addr, value);
    //TODO: stepInto(); stepOver();

private:
    QScriptEngine* m_engine;
    CProcessor* m_processor;
};


/** \brief Scripting object for the emulator. */
class QEmulator : public QObject
{
    Q_OBJECT
    /** \brief Get emulator uptime, in seconds, short for for getUptime(). */
    Q_PROPERTY(float uptime READ getUptime)
    /** \brief Get CPU object, short form for getCPU(). */
    Q_PROPERTY(QObject* cpu READ getCPU)
    /** \brief Get PPU object, short form for getPPU(). */
    Q_PROPERTY(QObject* ppu READ getPPU)
public:
    QEmulator(QScriptWindow * window);

public slots:
    /** \brief Resets the emulator. */
    void reset();
    /** \brief Runs the emulator for the given number of frames. One second is 25 frames. */
    bool run(int frames);
    /** \brief Get emulator uptime, in seconds. */
    float getUptime();

    /** \brief Sets CPU breakpoint address for the next run() call. */
    void setCPUBreakpoint(quint16 address);
    /** \brief Sets PPU breakpoint address for the next run() call. */
    void setPPUBreakpoint(quint16 address);
    /** \brief Check if the emulator stopped on a breakpoint. */
    bool isBreakpoint();
    /** \brief Save screenshot to PNG file. */

    void saveScreenshot(const QString& filename);
    /** \brief Load the cartridge image file. */

    /** \brief Attach the cartridge.
        \param slot cartridge slot 1..2 */
    bool attachCartridge(int slot, const QString& filename);
    /** \brief Empty the cartridge slot.
        \param slot cartridge slot 1..2 */
    void detachCartridge(int slot);
    /** \brief Attach the floppy image file -- insert the disk.
        \param slot floppy slot 1..4 */
    bool attachFloppy(int slot, const QString& filename);
    /** \brief Detach the floppy image file -- remove the disk.
        \param slot floppy slot 1..4 */
    void detachFloppy(int slot);
    /** \brief Attach the hard drive image file.
        \param slot hard drive slot 1..2 */
    bool attachHard(int slot, const QString& filename);
    /** \brief Detach the hard drive image file.
        \param slot hard drive slot 1..2 */
    void detachHard(int slot);

    /** \brief Get CPU object.
        \return QProcessor object. */
    QObject* getCPU() { return &m_cpu; }
    /** \brief Get PPU object.
        \return QProcessor object. */
    QObject* getPPU() { return &m_ppu; }

    /** \brief Press the key (by scan code), wait timeout frames, release the key, wait 3 frames.
        \param ukncscan UKNC scan code */
    void keyScan(uchar ukncscan, int timeout = 3);
    /** \brief Press SHIFT, press the key, wait timeout frames, release the key, release SHIFT, wait 3 frames.
        \param ukncscan UKNC scan code */
    void keyScanShift(uchar ukncscan, int timeout = 3);
    /** \brief Type the key sequence. */
    void keyString(QString str);

    /** \brief Save state image to the file. */
    void saveState(const QString& filename);
    /** \brief Restore state image from the file. */
    void loadState(const QString& filename);

    //TODO: Change screen modes, enableSound()
    //TODO: Print message to debug console

private:
    void keyChar(char ch, int timeout = 3);

private:
    QScriptWindow * m_window;
    QEmulatorProcessor m_cpu;
    QEmulatorProcessor m_ppu;
};


/** \brief Dialog class used to run a script. */
class QScriptWindow : public QDialog
{
    Q_OBJECT

public:
    QScriptWindow(QWidget * parent = 0);
    ~QScriptWindow();

public:
    void runScript(const QString & script);
    bool isAborted() const { return m_aborted; }
    QScriptEngine* getEngine() { return &m_engine; }

public slots:
    void reject();

private slots:
    void cancelButtonPressed();

private:
    QVBoxLayout m_vlayout;
    QLabel m_static;
    QPushButton m_cancelButton;
    QScriptEngine m_engine;
    bool m_aborted;
    QEmulator * m_emulator;
};


#endif //QSCRIPTING_H
