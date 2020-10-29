#ifndef STREAMSINK_H
#define STREAMSINK_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <streambuf>
#include <iostream>
#include <string>

class StreamSink : public QObject, public std::basic_streambuf<char>
{
    Q_OBJECT
public:
    explicit StreamSink(std::ostream& oStream)
        : _oStream(oStream)
        , _pOldStreamBuf(oStream.rdbuf())
    {
        oStream.rdbuf(this);
    }
    ~StreamSink()
    {
        if(!_sString.empty())
        {
            emit toLogs(QString::fromStdString(_sString));
        }

        _oStream.rdbuf(_pOldStreamBuf);
    }

signals:
    void toLogs(const QString& sMessage);

protected:
    virtual int_type overflow(int_type iCharacterToStore) override
    {
        _sString += std::to_string(iCharacterToStore);
        if (iCharacterToStore == '\n')
        {
            emit toLogs(QString::fromStdString(_sString));
            _sString.clear();
        }
        return iCharacterToStore;
    }

    virtual std::streamsize xsputn(const char* const p, const std::streamsize n) override
    {
        _sString.append(p, p + n);

        long pos = 0;
        while (pos != static_cast<long>(std::string::npos))
        {
            pos = static_cast<long>(_sString.find('\n'));
            if (pos != static_cast<long>(std::string::npos))
            {
                std::string tmp(_sString);
                emit toLogs(QString::fromStdString(tmp));
                _sString.clear();
            }
        }

        return n;
    }

private:
    StreamSink(const StreamSink&) = delete;
    StreamSink& operator=(const StreamSink&) = delete;

private:
    std::ostream& _oStream;
    std::streambuf* const _pOldStreamBuf = nullptr;
    std::string _sString;
};

#endif // STREAMSINK_H
